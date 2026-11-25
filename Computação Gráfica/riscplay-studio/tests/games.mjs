import fs from 'node:fs';
import {Riscplay,decodePackage,CPU_HZ} from '../dist/core.js';
import {createHash} from 'node:crypto';
const names={flappybird:'FlappyBird',hollow:'Hollow_Riscplay',mandelbrot:'mandelbrot'};
const result=[];
for(const name of process.argv.slice(2).length?process.argv.slice(2):Object.keys(names)) {
  const ex=JSON.parse(fs.readFileSync(new URL('../dist/examples/'+name+'.json',import.meta.url)));
  const vm=new Riscplay(decodePackage(ex.package));const start=performance.now();let reports=[];
  vm.onFrame=m=>{
    const rgba=m.framebuffer(),rgb=Buffer.alloc(320*180*3);
    for(let j=0;j<57600;j++){rgb[j*3]=rgba[j*4];rgb[j*3+1]=rgba[j*4+1];rgb[j*3+2]=rgba[j*4+2];}
    const file=new URL('../reports/'+name+'_'+m.frames+'.ppm',import.meta.url);
    fs.writeFileSync(file,Buffer.concat([Buffer.from('P6\n320 180\n255\n'),rgb]));
    const ref=(process.env.RISCPLAY_RTL_REFERENCE||'../riscplay_drv32/build_drv32/soc_tests')+'/'+names[name]+'_frame_'+m.frames+'.ppm';
    const reference=fs.existsSync(ref)?fs.readFileSync(ref).subarray(-rgb.length):null;
    let different=0;if(reference)for(let j=0;j<rgb.length;j+=3)if(!rgb.subarray(j,j+3).equals(reference.subarray(j,j+3)))different++;
    const info={game:name,frame:m.frames,ms:Math.round(m.cycles/CPU_HZ*1000),instructions:m.instructions,commands:m.commands,audio:m.audioWrites,pixelDifferences:reference?different:null,sha256:createHash('sha256').update(rgb).digest('hex')};reports.push(info);console.log(info);
  };
  try{
    while(vm.frames<(name==='mandelbrot'?2:5)&&vm.cycles<CPU_HZ*40)vm.runUntil(vm.cycles+CPU_HZ/60);
    if(vm.frames<(name==='mandelbrot'?2:5))throw Error('Timeout');
    result.push({name,reports,host_ms:Math.round(performance.now()-start),fault:vm.fault});
  }catch(e){console.error(e);console.error('pc',vm.pc.toString(16),'frames',vm.frames,'cycles',vm.cycles);process.exitCode=1;}
}
fs.writeFileSync(new URL('../reports/games.json',import.meta.url),JSON.stringify(result,null,2));
