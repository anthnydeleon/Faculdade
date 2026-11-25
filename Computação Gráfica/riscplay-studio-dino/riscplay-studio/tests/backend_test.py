import base64,io,json,sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
import server
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]

class BackendTests(unittest.TestCase):
    def test_build_all_and_roundtrip(self):
        result=[]
        for name in ['flappybird','hollow','mandelbrot','starter']:
            ex=json.loads((ROOT/'dist/examples'/f'{name}.json').read_text(encoding='utf-8'))
            files={k:v for k,v in ex['files'].items() if k.endswith(('.c','.h','.S'))}
            b=server.compile_files(files,server.find_compiler());self.assertTrue(b['ok'],b.get('error'))
            original=next(e for e in ex['package'] if e['type']==0)
            pkg=[dict(original,value=b['hex']),*[e for e in ex['package'] if e['type']==1]]
            raw=json.dumps(pkg);decoded=json.loads(raw)
            self.assertEqual(bytes.fromhex(decoded[0]['value']),bytes.fromhex(b['hex']))
            self.assertEqual(decoded[1:],ex['package'][1:])
            (ROOT/'reports'/f'rebuilt-{name}.json').write_text(raw,encoding='utf-8')
            result.append({'game':name,'ok':True,'size':b['size'],'matches_original_binary':b['hex']==original['value'],'loader_roundtrip':True})
        (ROOT/'reports/builds.json').write_text(json.dumps(result,indent=2),encoding='utf-8')

    def test_compile_failure(self):
        b=server.compile_files({'main.c':'int main(void) { invalid syntax! }'},server.find_compiler())
        self.assertFalse(b['ok']);self.assertIn('main.c',b['error'])

    def test_paths_and_capacity(self):
        with self.assertRaises(ValueError):server.compile_files({'../main.c':'int main(){}'},server.find_compiler())
        with self.assertRaises(ValueError):server.compile_files({'C:/main.c':'int main(){}'},server.find_compiler())
        im=Image.new('RGBA',(4,4),(255,80,40,255));im.putpixel((0,0),(0,0,0,0));out=io.BytesIO();im.save(out,format='PNG')
        data=base64.b64encode(out.getvalue()).decode();req={'images':[{'name':'hero.png','data':data}],'base':0x100000}
        result=server.convert_images(req);entry=result['entries'][0];block=result['blocks'][0];words=json.loads(block['value'])
        self.assertEqual(entry['pixels_addr'],0x100100);self.assertEqual(words[256]>>24,255)
        self.assertEqual(words[0],0xff5028);self.assertEqual(block['addr'],block['addr_mem']);self.assertIsInstance(block['comp_uuid'],str)
        self.assertIn('HERO_ADDR 0x100100u',result['header']);self.assertEqual(json.loads(block['pixels'])[0][0],255)
        (ROOT/'tests/tiny.png').write_bytes(out.getvalue())
        with self.assertRaises(ValueError):server.convert_images(dict(req,base=0x1fffff))
        with self.assertRaises(ValueError):server.convert_images(dict(req,images=req['images']*2))

if __name__=='__main__':unittest.main()
