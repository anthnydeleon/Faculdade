import {EditorView,basicSetup} from 'codemirror';
import {EditorState} from '@codemirror/state';
import {keymap} from '@codemirror/view';
import {indentWithTab} from '@codemirror/commands';
import {HighlightStyle,syntaxHighlighting} from '@codemirror/language';
import {tags} from '@lezer/highlight';
import {c} from '@fazelstudio/codemirror-lang-c';
import {Screen} from './renderer.js';
import {decodePackage,loaderPackage} from './core.js';

const $=id=>document.getElementById(id);
const colorTheme=HighlightStyle.define([{tag:tags.keyword,color:'#dca9e8'},{tag:[tags.number,tags.bool],color:'#edc88d'},{tag:[tags.string,tags.character],color:'#a8d78a'},{tag:tags.comment,color:'#718d85',fontStyle:'italic'},{tag:tags.typeName,color:'#81d7d4'},{tag:tags.function(tags.variableName),color:'#e7d7a0'},{tag:tags.meta,color:'#8fbfdc'}]);
let project=null,currentFile='main.c',editor=null,revision=0,buildTimer=0,saveTimer=0,loadingEditor=false;
let building=false,buildAgain=false,compiledRevision=0,paused=false,generation=0,busy=false,ready=false;
let audioContext=null,audioGain=null,audioOn=false,audioTime=0,audioSources=new Set();
let keyboard=0,touch=0,lastButtons=-1,token=new URLSearchParams(location.hash.slice(1)).get('key')||sessionStorage.getItem('rp-key')||'';
if(token){sessionStorage.setItem('rp-key',token);history.replaceState(null,'',location.pathname);}
const screen=new Screen($('screen'));$('renderer').textContent=screen.mode;
const worker=new Worker(new URL('./worker.js',import.meta.url),{type:'module'});
const db=new Promise((resolve,reject)=>{const request=indexedDB.open('riscplay-studio',1);request.onupgradeneeded=()=>request.result.createObjectStore('projects');request.onsuccess=()=>resolve(request.result);request.onerror=()=>reject(request.error);});
async function stored(id){try{return await new Promise(async(resolve,reject)=>{const r=(await db).transaction('projects').objectStore('projects').get(id);r.onsuccess=()=>resolve(r.result);r.onerror=()=>reject(r.error);});}catch{return null;}}
async function persist(){try{const d=await db;await new Promise((resolve,reject)=>{const tx=d.transaction('projects','readwrite');tx.objectStore('projects').put(project,project.id);tx.oncomplete=resolve;tx.onerror=()=>reject(tx.error);});$('saved').textContent='Salvo neste navegador';}catch{$('saved').textContent='Use Salvar projeto para guardar uma cópia';}}
function autosave(){clearTimeout(saveTimer);$('saved').textContent='Salvando…';saveTimer=setTimeout(persist,500);}
function toast(message){$('toast').textContent=message;$('toast').classList.remove('hidden');clearTimeout(toast.timer);toast.timer=setTimeout(()=>$('toast').classList.add('hidden'),4000);}
function diagnostic(message,type='',details=''){const box=$('diagnostics');box.className='diagnostics '+type;box.replaceChildren(document.createTextNode(message));if(details){const pre=document.createElement('pre');pre.textContent=details;box.append(pre);}}
function download(name,data,type='application/json'){const a=document.createElement('a'),url=URL.createObjectURL(new Blob([data],{type}));a.href=url;a.download=name;a.click();setTimeout(()=>URL.revokeObjectURL(url),3000);}
async function api(path,body){const res=await fetch(path,{method:'POST',headers:{'Content-Type':'application/json','Authorization':'Bearer '+token},body:JSON.stringify(body)});const result=await res.json();if(!result.ok)throw Error(result.error||'A operação não foi concluída.');return result;}

function editorState(text){return EditorState.create({doc:text,extensions:[basicSetup,c(),syntaxHighlighting(colorTheme),keymap.of([indentWithTab,{key:'Ctrl-Enter',run:()=>{compile();return true;}},{key:'Mod-s',run:()=>{saveProject();return true;}}]),EditorView.lineWrapping,EditorView.updateListener.of(update=>{
  const at=update.state.selection.main.head,line=update.state.doc.lineAt(at);$('cursor').textContent='Ln '+line.number+', Col '+(at-line.from+1);
  if(update.docChanged&&!loadingEditor&&project&&currentFile){project.files[currentFile]=update.state.doc.toString();changed();}
})]});}
function showFile(name){currentFile=name;loadingEditor=true;if(!editor)editor=new EditorView({state:editorState(project.files[name]||''),parent:$('editor')});else editor.setState(editorState(project.files[name]||''));loadingEditor=false;$('files').value=name;}
function fileList(){const sel=$('files');sel.replaceChildren();for(const name of Object.keys(project.files).filter(n=>/\.(c|h)$/.test(n)).sort((a,b)=>(a==='main.c'?-1:b==='main.c'?1:a.localeCompare(b)))){const opt=document.createElement('option');opt.value=name;opt.textContent=name;sel.append(opt);}if(!(currentFile in project.files))currentFile=Object.keys(project.files).find(n=>n.endsWith('.c'))||Object.keys(project.files)[0]||'';showFile(currentFile);}
function changed(){revision++;project.dirty=true;$('export-prog').disabled=true;diagnostic('Alteração pendente.','busy');autosave();clearTimeout(buildTimer);if($('live').checked)buildTimer=setTimeout(compile,600);}
function combinedPackage(hex){const original=project.package.find(e=>+e.type===0)||{};const firmware={...original,comp_uuid:original.comp_uuid||'7d020b92-9304-5b43-bdb7-633ec4a3c3f1',type:0,addr:'20000000',addr_mem:'20000000',value:hex};return [firmware,...project.baseAssets,...project.assetBlocks];}
async function compile(){
  clearTimeout(buildTimer);if(!project)return;
  if(building){buildAgain=true;return;}
  building=true;const rev=revision,owner=project;diagnostic('Compilando C para DRV32…','busy');$('build').disabled=true;
  try{
    const files=Object.fromEntries(Object.entries(project.files).filter(([n])=>/\.(c|h|S)$/.test(n)));
    const result=await api('/api/build',{files});
    if(rev!==revision||owner!==project){buildAgain=true;return;}
    project.package=combinedPackage(result.hex);project.dirty=false;compiledRevision=revision;startGame();
    diagnostic('Compilação concluída · '+result.size.toLocaleString('pt-BR')+' bytes · jogo reiniciado.', '',result.log);
    $('export-prog').disabled=false;autosave();
  }catch(e){if(rev===revision&&owner===project)diagnostic('Corrija o código. A última versão continua em execução.','error',e.message);}
  finally{building=false;$('build').disabled=false;if(buildAgain){buildAgain=false;if($('live').checked)buildTimer=setTimeout(compile,100);}}
}
function silence(){for(const s of audioSources){try{s.stop();}catch{}}audioSources.clear();audioTime=0;}
function startGame(){if(!project.package?.length)return;decodePackage(project.package);generation++;ready=false;busy=false;paused=false;keyboard=touch=0;lastButtons=-1;silence();worker.postMessage({type:'load',generation,package:project.package});$('pause').textContent='Ⅱ Pausar';$('run-dot').style.background='var(--accent)';}
async function selectProject(id,restore=true){
  clearTimeout(buildTimer);if(project)await persist();
  const prior=restore?await stored(id):null;
  if(prior)project=prior;
  else{
    const response=await fetch('examples/'+id+'.json');if(!response.ok)throw Error('Exemplo indisponível.');const ex=await response.json();
    project={format:'riscplay-studio',version:1,id:ex.id,title:ex.title,files:Object.fromEntries(Object.entries(ex.files).filter(([n])=>/\.(c|h)$/.test(n))),package:ex.package,baseAssets:ex.package.filter(e=>+e.type===1),images:[],assetBlocks:[],assetEntries:[],header:''};
  }
  revision++;compiledRevision=revision;currentFile='main.c';fileList();drawAssets();startGame();$('export-prog').disabled=false;
  diagnostic(prior?'Projeto restaurado deste navegador.':'Exemplo pronto. Edite o C para experimentar.');localStorage.setItem('rp-last',id);
  if(project.dirty){$('export-prog').disabled=true;compiledRevision=-1;if($('live').checked)buildTimer=setTimeout(compile,600);}
}
worker.onmessage=({data:d})=>{
  if(d.generation!==undefined&&d.generation!==generation)return;
  if(d.type==='ready'){ready=true;busy=false;lastButtons=-1;}
  else if(d.type==='idle')busy=false;
  else if(d.type==='frame'){
    busy=false;screen.draw(d.pixels);queueAudio(d.audio);
    $('speed').textContent='Tempo virtual: '+(d.stats.ms/1000).toFixed(2).replace('.',',')+' s';
    $('frame-stat').textContent=d.stats.frames+' quadros apresentados';
    $('perf').textContent=d.stats.workMs.toFixed(1)+' ms / passo';
  }else if(d.type==='error'){busy=false;ready=false;diagnostic('A execução parou.','error',d.message);$('run-dot').style.background='var(--danger)';silence();}
};
async function enableSound(){
  try{
    if(!audioContext){audioContext=new (window.AudioContext||window.webkitAudioContext)({sampleRate:48000});audioGain=audioContext.createGain();audioGain.gain.value=.85;audioGain.connect(audioContext.destination);}
    await audioContext.resume();audioOn=!audioOn;if(!audioOn)silence();$('sound').textContent=audioOn?'♪ Som ligado':'♪ Ativar som';$('sound').classList.toggle('primary',audioOn);
  }catch(e){toast('Não foi possível iniciar o áudio: '+e.message);}
}
function queueAudio(samples){
  if(!audioOn||paused||!audioContext||audioContext.state!=='running'||!samples.length)return;
  if(audioTime>audioContext.currentTime+.35)silence();
  const buffer=audioContext.createBuffer(1,samples.length,48000);buffer.copyToChannel(samples,0);const source=audioContext.createBufferSource();source.buffer=buffer;source.connect(audioGain);
  audioTime=Math.max(audioTime,audioContext.currentTime+.035);source.start(audioTime);audioTime+=buffer.duration;audioSources.add(source);source.onended=()=>audioSources.delete(source);
}
function togglePause(){paused=!paused;worker.postMessage({type:'pause',paused});$('pause').textContent=paused?'▶ Continuar':'Ⅱ Pausar';$('run-dot').style.background=paused?'var(--amber)':'var(--accent)';if(paused)silence();}
const keyMap={ArrowUp:4,KeyW:4,ArrowRight:5,ArrowDown:6,ArrowLeft:7,Space:14,KeyZ:14,Enter:14,KeyX:13,KeyA:15,KeyS:12,KeyQ:10,KeyE:11,ShiftLeft:8,ShiftRight:9};
function typing(el){return el?.closest?.('.cm-editor,input,textarea,select,[contenteditable="true"]');}
addEventListener('keydown',e=>{if(typing(e.target)||e.ctrlKey||e.metaKey||e.altKey)return;const bit=keyMap[e.code];if(bit!==undefined){e.preventDefault();keyboard|=1<<bit;}});
addEventListener('keyup',e=>{const bit=keyMap[e.code];if(bit!==undefined){keyboard&=~(1<<bit);if(!typing(e.target))e.preventDefault();}});
addEventListener('blur',()=>{keyboard=touch=0;lastButtons=-1;worker.postMessage({type:'buttons',mask:0});});
document.addEventListener('focusin',e=>{if(typing(e.target))keyboard=0;});
document.addEventListener('visibilitychange',()=>{keyboard=touch=0;if(document.hidden){worker.postMessage({type:'buttons',mask:0});silence();}});
function gamepadMask(){
  let mask=0,pads=[];try{pads=navigator.getGamepads?.()||[];}catch{}
  const pad=Array.from(pads).find(Boolean);if(!pad)return mask;
  $('gamepad-status').textContent='Conectado: '+pad.id;
  const map={0:14,1:13,2:15,3:12,4:10,5:11,6:8,7:9,8:0,9:3,12:4,13:6,14:7,15:5};
  for(const [button,bit] of Object.entries(map))if(pad.buttons[button]?.pressed)mask|=1<<bit;
  if(pad.axes[0]<-.35)mask|=1<<7;if(pad.axes[0]>.35)mask|=1<<5;if(pad.axes[1]<-.35)mask|=1<<4;if(pad.axes[1]>.35)mask|=1<<6;
  return mask;
}
let nextTick=0;
function animation(t){
  const mask=keyboard|touch|gamepadMask();if(mask!==lastButtons){lastButtons=mask;worker.postMessage({type:'buttons',mask});}
  if(t>=nextTick&&!busy&&ready&&!paused&&!document.hidden){busy=true;worker.postMessage({type:'tick'});nextTick=Math.max(nextTick+1000/60,t-1000/60);}
  if(t-nextTick>100)nextTick=t;requestAnimationFrame(animation);
}requestAnimationFrame(animation);
function createPad(target){
  const dpad=document.createElement('div');dpad.className='dpad';const face=document.createElement('div');face.className='facepad';
  for(const [label,bit,container] of [['↑',4,dpad],['←',7,dpad],['→',5,dpad],['↓',6,dpad],['□',15,face],['△',12,face],['×',14,face],['○',13,face]]){
    const b=document.createElement('button');b.textContent=label;b.setAttribute('aria-label','Controle '+label);b.dataset.bit=bit;
    b.onpointerdown=e=>{e.preventDefault();b.setPointerCapture(e.pointerId);touch|=1<<bit;b.classList.add('pressed');};
    const up=()=>{touch&=~(1<<bit);b.classList.remove('pressed');};b.onpointerup=up;b.onpointercancel=up;b.onlostpointercapture=up;container.append(b);
  }
  const hint=document.createElement('span');hint.className='touch-caption';hint.textContent='Controles na tela';target.append(dpad,hint,face);
}createPad($('desktop-pad'));createPad($('mobile-pad'));

function displayTab(assets){$('code-panel').classList.toggle('hidden',assets);$('assets-panel').classList.toggle('hidden',!assets);$('code-tab').classList.toggle('active',!assets);$('assets-tab').classList.toggle('active',assets);$('code-tab').setAttribute('aria-selected',!assets);$('assets-tab').setAttribute('aria-selected',assets);if(!assets)editor?.requestMeasure();}
function spriteCanvas(entry){
  if(!entry.pixels||!entry.value)return null;
  try{const rows=typeof entry.pixels==='string'?JSON.parse(entry.pixels):entry.pixels,values=typeof entry.value==='string'?JSON.parse(entry.value):entry.value;
    const canvas=document.createElement('canvas');canvas.width=rows[0].length;canvas.height=rows.length;const ctx=canvas.getContext('2d'),image=ctx.createImageData(canvas.width,canvas.height);
    for(let y=0;y<canvas.height;y++)for(let x=0;x<canvas.width;x++){const j=y*canvas.width+x,index=(values[256+(j>>>2)]>>>(24-8*(j&3)))&255,rgb=values[index],k=j*4;image.data[k]=rgb>>>16;image.data[k+1]=rgb>>>8;image.data[k+2]=rgb;image.data[k+3]=index===255?0:255;}
    ctx.putImageData(image,0,0);return canvas;
  }catch{return null;}
}
function drawAssets(){
  const grid=$('assets-grid');grid.replaceChildren();const originals=project.baseAssets.filter(e=>e.pixels);$('asset-count').textContent='· '+(originals.length+project.images.length);
  $('asset-summary').textContent=project.images.length+' importadas · '+project.baseAssets.length+' blocos originais';
  const card=(name,preview,description,remove)=>{
    const el=document.createElement('div');el.className='asset';const top=document.createElement('div');top.className='asset-preview';if(preview)top.append(preview);const info=document.createElement('div');info.className='asset-info';const title=document.createElement('strong');title.textContent=name;const p=document.createElement('p');p.textContent=description;info.append(title,p);
    if(remove){const b=document.createElement('button');b.textContent='Remover';b.onclick=remove;info.append(b);}el.append(top,info);grid.append(el);
  };
  for(let j=0;j<originals.length;j++){const e=originals[j],canvas=spriteCanvas(e);card('Sprite '+(j+1),canvas,(canvas?canvas.width+' × '+canvas.height+' · ':'')+'0x'+e.addr);}
  for(let j=0;j<project.images.length;j++){
    const im=project.images[j],entry=project.assetEntries[j],image=new Image();image.src='data:image/png;base64,'+im.data;
    card(entry?.name||im.name,image,im.width+' × '+im.height+(entry?' · 0x'+entry.pixels_addr.toString(16).toUpperCase():''),()=>syncImages(project.images.filter((_,n)=>n!==j)).catch(e=>diagnostic('Falha nos assets.','error',e.message)));
  }
  if(!grid.children.length){const p=document.createElement('p');p.className='asset-note';p.textContent='Importe imagens para gerar endereços e includes. Os blocos do jogo já estão incluídos na execução.';grid.append(p);}
}
async function imageFiles(files){
  const staged=[...project.images];
  for(const file of files){
    if(file.size>4*1024*1024)throw Error(file.name+': limite de 4 MiB.');
    const url=URL.createObjectURL(file),im=new Image();
    try{im.src=url;await im.decode();if(im.width>1024||im.height>1024)throw Error(file.name+': limite de 1024 × 1024.');
      for(let y=0;y<im.height;y+=127)for(let x=0;x<im.width;x+=127){
        const canvas=document.createElement('canvas');canvas.width=Math.min(127,im.width-x);canvas.height=Math.min(127,im.height-y);canvas.getContext('2d').drawImage(im,x,y,canvas.width,canvas.height,0,0,canvas.width,canvas.height);
        const stem=file.name.replace(/\.[^.]+$/,'').replace(/[^a-zA-Z0-9_]/g,'_'),suffix=im.width>127||im.height>127?'_'+x+'_'+y:'';
        const name=stem+suffix+'.png';if(staged.some(v=>v.name.toUpperCase()===name.toUpperCase()))throw Error('Nome repetido: '+name);
        staged.push({name,data:canvas.toDataURL('image/png').split(',')[1],width:canvas.width,height:canvas.height,x,y});
      }
    }finally{URL.revokeObjectURL(url);}
  }
  if(staged.length>128)throw Error('Limite de 128 sprites importados.');await syncImages(staged);
}
let assetBusy=false;
async function syncImages(images){
  if(assetBusy)throw Error('Aguarde a conversão em andamento.');assetBusy=true;const owner=project;
  try{diagnostic('Convertendo imagens e gerando endereços…','busy');
    let result={header:'/* Nenhuma imagem importada. */\n',entries:[],blocks:[]};
    if(images.length){let base=0x100000;for(const e of project.baseAssets){const values=typeof e.value==='string'?JSON.parse(e.value):e.value;base=Math.max(base,parseInt(e.addr,16)+values.length);}result=await api('/api/assets',{images,base});}
    if(project!==owner)return;
    let offsets='';for(let j=0;j<result.entries.length;j++){const e=result.entries[j];offsets+='\n#define '+e.name+'_X '+images[j].x+'\n#define '+e.name+'_Y '+images[j].y+'\n';}
    const header=result.header.includes('#endif')?result.header.replace('#endif',offsets+'\n#endif'):result.header;
    project.images=images;project.assetEntries=result.entries;project.assetBlocks=result.blocks;project.header=header;project.files['generated_assets.h']=header;
    fileList();drawAssets();changed();toast('Assets atualizados. Inclua generated_assets.h no seu C.');
  }finally{assetBusy=false;}
}
function saveProject(){if(project){download(project.id+'.riscplay.json',JSON.stringify(project));toast('Projeto completo exportado com fontes e assets.');}}
async function openFiles(files){
  if(files.every(f=>/\.(c|h|S)$/i.test(f.name))){for(const f of files){if(f.size>2*1024*1024)throw Error('Fonte acima de 2 MiB.');project.files[f.name]=await f.text();}fileList();changed();return;}
  if(files.length!==1)throw Error('Selecione um projeto ou pacote por vez.');const f=files[0];if(f.size>24*1024*1024)throw Error('Arquivo acima de 24 MiB.');const data=JSON.parse(await f.text());
  if(Array.isArray(data)){decodePackage(data);await persist();project={format:'riscplay-studio',version:1,id:'importado',title:f.name,files:{},package:data,baseAssets:data.filter(e=>+e.type===1),images:[],assetBlocks:[],assetEntries:[],header:''};diagnostic('Pacote carregado. Para editar, abra também os fontes C e H correspondentes.');}
  else{
    if(data.format!=='riscplay-studio'||data.version!==1||!data.files||!Array.isArray(data.baseAssets)||!Array.isArray(data.images)||!Array.isArray(data.assetBlocks))throw Error('Formato de projeto não reconhecido.');
    if(Object.values(data.files).some(v=>typeof v!=='string'))throw Error('Fontes inválidos.');decodePackage(data.package);await persist();project=data;diagnostic('Projeto importado.');
  }
  revision++;compiledRevision=revision;fileList();drawAssets();startGame();$('export-prog').disabled=false;autosave();
}
$('files').onchange=()=>showFile($('files').value);
$('project').onchange=()=>selectProject($('project').value).catch(e=>diagnostic('Não foi possível abrir o exemplo.','error',e.message));
$('build').onclick=compile;$('live').onchange=()=>{if($('live').checked&&compiledRevision!==revision)compile();};
$('code-tab').onclick=()=>displayTab(false);$('assets-tab').onclick=()=>displayTab(true);
$('pause').onclick=togglePause;$('restart').onclick=startGame;$('sound').onclick=enableSound;
$('fullscreen').onclick=async()=>{try{if(document.fullscreenElement)await document.exitFullscreen();else await $('screen-panel').requestFullscreen();}catch{toast('Tela cheia indisponível neste navegador.');}};
$('save-project').onclick=saveProject;$('export-prog').onclick=()=>download(project.id+'.json.prog',JSON.stringify(loaderPackage(project.package)));
$('import-project').onclick=()=>$('project-input').click();$('project-input').onchange=async e=>{try{await openFiles(Array.from(e.target.files));}catch(err){diagnostic('Falha na importação.','error',err.message);}finally{e.target.value='';}};
$('add-file').onclick=()=>{const name=prompt('Nome do novo arquivo C ou H:','novo.c');if(!name)return;if(!/^[a-zA-Z0-9_-]+\.(c|h)$/.test(name))return toast('Use um nome simples terminado em .c ou .h.');if(name in project.files)return toast('Esse arquivo já existe.');project.files[name]='';currentFile=name;fileList();changed();};
$('add-images').onclick=()=>$('image-input').click();$('image-input').onchange=async e=>{try{await imageFiles(Array.from(e.target.files));}catch(err){diagnostic('Falha na importação das imagens.','error',err.message);}finally{e.target.value='';}};
$('drop').ondragover=e=>{e.preventDefault();$('drop').classList.add('drag');};$('drop').ondragleave=()=>$('drop').classList.remove('drag');$('drop').ondrop=async e=>{e.preventDefault();$('drop').classList.remove('drag');try{await imageFiles(Array.from(e.dataTransfer.files));}catch(err){diagnostic('Falha nos assets.','error',err.message);}};
$('header-download').onclick=()=>{if(!project.header)return toast('Importe uma imagem primeiro.');download('generated_assets.h',project.header,'text/x-c');};
$('help').onclick=()=>$('help-dialog').showModal();$('close-help').onclick=()=>$('help-dialog').close();
addEventListener('beforeunload',()=>{if(project)persist();});
try{const status=await (await fetch('/api/status')).json();$('compiler-state').textContent=status.compiler?' GCC disponível.':' GCC não encontrado no PC.';}catch{$('compiler-state').textContent='Abra pelo servidor Python para editar e compilar.';}
try{const id=localStorage.getItem('rp-last')||'dino';$('project').value=['dino','flappybird','hollow','mandelbrot','starter'].includes(id)?id:'dino';await selectProject($('project').value);}catch(e){diagnostic('Não foi possível iniciar.','error',e.message);}
