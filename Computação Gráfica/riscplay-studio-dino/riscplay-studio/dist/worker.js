import {Riscplay,decodePackage,CPU_HZ,IO_HZ} from './core.js';
import {APU} from './audio.js';
let vm=null,apu=null,running=false,target=0,generation=0;
onmessage=({data:d})=>{
  try{
    if(d.type==='load') {
      generation=d.generation;apu=new APU();vm=new Riscplay(decodePackage(d.package),apu);running=true;target=0;
      postMessage({type:'ready',generation});
    } else if(d.type==='buttons'&&vm)vm.buttons=d.mask;
    else if(d.type==='pause')running=!d.paused;
    else if(d.type==='tick'&&vm&&running) {
      const start=performance.now();target+=CPU_HZ/60;
      vm.runUntil(target);apu.until(vm.cycles*IO_HZ/CPU_HZ);
      const pixels=vm.framebuffer(),audio=apu.take();
      postMessage({type:'frame',generation,pixels,audio,stats:{ms:vm.cycles/CPU_HZ*1000,frames:vm.frames,instructions:vm.instructions,pc:vm.pc,commands:vm.commands,workMs:performance.now()-start}},[pixels.buffer,audio.buffer]);
    } else if(d.type==='tick')postMessage({type:'idle'});
  }catch(e){running=false;postMessage({type:'error',generation,message:e.message,fault:vm?.fault});}
};
