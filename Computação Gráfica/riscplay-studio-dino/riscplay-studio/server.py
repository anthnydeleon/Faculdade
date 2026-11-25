#!/usr/bin/env python3
"""Riscplay Studio: one local origin for the browser app and RV32I GCC."""
import argparse,base64,json,os,re,secrets,shutil,socket,subprocess,tempfile,threading,time,uuid
from pathlib import Path
from http.server import SimpleHTTPRequestHandler,ThreadingHTTPServer
from urllib.parse import urlsplit

ROOT=Path(__file__).resolve().parent
PREFIX=None
TOKEN=secrets.token_urlsafe(32)
BUILD_LOCK=threading.Lock()
MAX_REQUEST=24*1024*1024
NAME=re.compile(r'^[A-Za-z0-9_./-]+\.(?:c|h|S)$')

def find_compiler(explicit=None):
    candidates=[explicit,os.environ.get('CROSS_COMPILE')]
    candidates += [str(p/'bin/riscv-none-elf-') for p in (ROOT/'toolchain', ROOT.parent/'toolchains/xpack-riscv-none-elf-gcc-15.2.0-1', Path.home()/'toolchains/xpack-riscv-none-elf-gcc-15.2.0-1')]
    candidates += ['riscv-none-elf-','riscv64-unknown-elf-','riscv32-unknown-elf-']
    for p in candidates:
        if p and shutil.which(p+'gcc'+('.exe' if os.name=='nt' else '')):return p
    return None

def compile_files(files,prefix=None):
    prefix=prefix or PREFIX
    if not prefix:raise ValueError('GCC RISC-V não encontrado. Defina CROSS_COMPILE ou use --cross-prefix. Consulte README.md.')
    if not isinstance(files,dict) or not files or len(files)>120:raise ValueError('Projeto inválido: limite de 120 fontes.')
    for name,content in files.items():
        if not isinstance(name,str) or not NAME.fullmatch(name) or name.startswith('/') or '..' in name.split('/') or not isinstance(content,str):raise ValueError('Nome de arquivo inválido: '+str(name))
        if len(content)>2*1024*1024:raise ValueError('Fonte muito grande: '+name)
    if not any(n.endswith('.c') for n in files):raise ValueError('Adicione pelo menos um fonte C.')
    with tempfile.TemporaryDirectory(prefix='riscplay-studio-') as tmp:
        root=Path(tmp)
        # Trusted linker/startup are fixed; user source stays in this build directory.
        for p in (ROOT/'sdk').iterdir():shutil.copy2(p,root/p.name)
        for name,content in files.items():
            if name=='startup.S':continue
            path=root/name;path.parent.mkdir(parents=True,exist_ok=True);path.write_text(content,encoding='utf-8')
        sources=sorted(name for name in files if name.endswith('.c'))
        if 'riscplay_audio.c' not in sources:sources.append('riscplay_audio.c')
        arch=['-march=rv32i','-mabi=ilp32','-mno-relax']
        cmd=[prefix+'gcc',*arch,'-Os','-std=gnu99','-ffreestanding','-fno-builtin','-fno-delete-null-pointer-checks',
             '-fno-jump-tables','-fno-pic','-fno-pie','-fno-stack-protector','-msmall-data-limit=0',
             '-ffunction-sections','-fdata-sections','-fno-unwind-tables','-fno-asynchronous-unwind-tables',
             '-fno-tree-loop-distribute-patterns','-I.','-nostdlib','-nostartfiles','-static',
             '-Wl,--no-relax,--gc-sections','-T','linker.ld','startup.S',*sources,'-lgcc','-o','firmware.elf']
        result=subprocess.run(cmd,cwd=root,capture_output=True,text=True,encoding='utf-8',errors='replace',timeout=30)
        log=(result.stdout+result.stderr).replace(str(root),'projeto')[-16000:]
        if result.returncode:return {'ok':False,'error':log}
        subprocess.run([prefix+'objcopy','-O','binary','firmware.elf','firmware.bin'],cwd=root,check=True,capture_output=True,timeout=10)
        code=(root/'firmware.bin').read_bytes()
        if len(code)>0x7000:raise ValueError('O programa invade a reserva de 4 KiB da pilha.')
        return {'ok':True,'hex':code.hex(),'size':len(code),'log':log,'arch':'RV32I / ILP32'}

def convert_images(req):
    from asset_manager import convert
    images=[]
    for item in req.get('images',[]):
        data=base64.b64decode(item['data'],validate=True)
        if len(data)>4*1024*1024:raise ValueError('Imagem acima de 4 MiB.')
        images.append({'name':item['name'],'data':data})
    binary,header,entries=convert(images,base=int(req.get('base',0x100000)))
    import struct
    words=[(a,v) for a,v in struct.iter_unpack('<II',binary)]
    blocks=[];cursor=0
    for e in entries:
        values=[v for _,v in words[cursor:cursor+e['words']]];cursor+=e['words']
        address=format(e['palette_addr'],'06X')
        indices=[(values[256+(i//4)]>>(24-8*(i%4)))&255 for i in range(e['width']*e['height'])]
        pixels=[indices[i:i+e['width']] for i in range(0,len(indices),e['width'])]
        palette=[{'r':(v>>16)&255,'g':(v>>8)&255,'b':v&255} for v in values[:256]]
        blocks.append({'comp_uuid':str(uuid.uuid5(uuid.NAMESPACE_URL,'riscplay-studio/'+e['name'])),
                       'type':1,'addr':address,'addr_mem':address,'value':json.dumps(values,separators=(',',':')),
                       'pixels':json.dumps(pixels,separators=(',',':')),'palette':json.dumps(palette,separators=(',',':'))})
    return {'ok':True,'header':header,'entries':entries,'blocks':blocks}

class Handler(SimpleHTTPRequestHandler):
    def __init__(self,*args,**kwargs):super().__init__(*args,directory=str(ROOT/'dist'),**kwargs)
    def log_message(self,fmt,*args):
        # Avoid printing source code or authorization values.
        if self.command!='GET':print(self.command,self.path.split('?')[0],flush=True)
    def end_headers(self):
        self.send_header('Cache-Control','no-store');self.send_header('X-Content-Type-Options','nosniff');super().end_headers()
    def reply(self,code,payload):
        data=json.dumps(payload,ensure_ascii=False).encode('utf-8')
        self.send_response(code);self.send_header('Content-Type','application/json; charset=utf-8');self.send_header('Content-Length',str(len(data)));self.end_headers();self.wfile.write(data)
    def do_GET(self):
        if urlsplit(self.path).path=='/api/status':return self.reply(200,{'ok':True,'compiler':bool(PREFIX),'arch':'RV32I','cpu_hz':30937500})
        return super().do_GET()
    def do_POST(self):
        origin=self.headers.get('Origin')
        if self.headers.get('Authorization')!='Bearer '+TOKEN or (origin and urlsplit(origin).netloc!=self.headers.get('Host')):
            return self.reply(403,{'ok':False,'error':'Abra o endereço completo exibido pelo servidor, incluindo a chave após #.'})
        try:
            n=int(self.headers.get('Content-Length','0'))
            if not 0<n<=MAX_REQUEST:raise ValueError('Pedido vazio ou acima de 24 MiB.')
            req=json.loads(self.rfile.read(n))
            with BUILD_LOCK:
                if self.path=='/api/build':result=compile_files(req.get('files'))
                elif self.path=='/api/assets':result=convert_images(req)
                else:return self.reply(404,{'ok':False,'error':'Rota inexistente.'})
            self.reply(200 if result['ok'] else 422,result)
        except ImportError:self.reply(503,{'ok':False,'error':'Instale Pillow: python -m pip install -r requirements.txt'})
        except (ValueError,TypeError,KeyError,OSError,subprocess.SubprocessError) as e:self.reply(400,{'ok':False,'error':str(e)})

def main():
    global PREFIX
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--port',type=int,default=8090);p.add_argument('--lan',action='store_true',help='Permitir acesso de celular na mesma rede');p.add_argument('--cross-prefix');args=p.parse_args()
    PREFIX=find_compiler(args.cross_prefix)
    server=ThreadingHTTPServer(('0.0.0.0' if args.lan else '127.0.0.1',args.port),Handler)
    print('Riscplay Studio | DRV32 30,9375 MHz | GCC '+('disponível' if PREFIX else 'não encontrado'),flush=True)
    print(f'PC: http://localhost:{args.port}/#key={TOKEN}',flush=True)
    if args.lan:
        for ip in sorted(set(socket.gethostbyname_ex(socket.gethostname())[2])):
            if not ip.startswith('127.'):print(f'Celular (mesma rede): http://{ip}:{args.port}/#key={TOKEN}',flush=True)
    print('Ctrl+C encerra o servidor.',flush=True)
    try:server.serve_forever()
    except KeyboardInterrupt:pass
    finally:server.server_close()

if __name__=='__main__':main()
