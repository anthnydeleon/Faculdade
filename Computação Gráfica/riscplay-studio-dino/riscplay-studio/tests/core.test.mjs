import assert from 'node:assert/strict';
import fs from 'node:fs';
import {Riscplay,decodePackage,loaderPackage,CPU_HZ} from '../dist/core.js';
import {APU} from '../dist/audio.js';
const root=new URL('../',import.meta.url);
const read=p=>fs.readFileSync(new URL(p,root));
let traced=0;
for(const name of ['isa','arithmetic']) {
  const machine=new Riscplay({firmware:new Uint8Array(read('tests/fixtures/'+name+'.bin'))});
  for(const line of read('tests/fixtures/'+name+'.trace').toString().trim().split('\n')) {
    const [pc,instruction,rd,value]=line.trim().split(/\s+/).map(x=>parseInt(x,16));
    assert.equal(machine.pc,pc,name+' PC');assert.equal(machine.readWord(pc)>>>0,instruction,name+' instruction');machine.step();
    if(rd)assert.equal(machine.r[rd]>>>0,value,name+' rd');assert.equal(machine.r[0],0);traced++;
  }
}
for(const name of ['illegal_mul','illegal_div','illegal_custom0','misaligned_load','misaligned_jump']){
  const machine=new Riscplay({firmware:new Uint8Array(read('tests/fixtures/'+name+'.bin'))});
  assert.throws(()=>{for(let k=0;k<20;k++)machine.step();});assert.ok(machine.fault);
}
assert.throws(()=>decodePackage([{type:0,value:'garbage'}]));
assert.throws(()=>decodePackage([{type:0,value:'13000000'},{type:1,addr:'200000',value:[1]}]));
const machine=new Riscplay({firmware:new Uint8Array([0x13,0,0,0])});
machine.buttons=(1<<14)|(1<<5);assert.equal((~machine.readWord(12)>>>16)&65535,machine.buttons);
machine.sdram[0x100]=0x12345678;machine.ppu(0x100<<7,4<<2);assert.deepEqual(Array.from(machine.sprite.slice(0,4)),[0x12,0x34,0x56,0x78]);
machine.sdram[0x200]=0xff334455;machine.ppu(0x10000000|(0x200<<7),0);assert.equal(machine.palette[0],0xff334455);
machine.sprite[0]=0;machine.spriteLength=1;machine.ppu(0x20000000|(0xa080<<7)|(1<<3),0x10200000);assert.equal(machine.sdram[0xa080]&0xffffff,0x334455);
const apu=new APU();apu.write(0x200,1,0);apu.write(0x204,Math.floor(37125000/440),0);apu.until(7425000);const samples=apu.take();
assert.equal(samples.length,4800);let crossings=0;for(let j=1;j<samples.length;j++)if(samples[j]>=0&&samples[j-1]<0)crossings++;
assert.ok(crossings>=43&&crossings<=45,'440 Hz pulse');assert.ok(samples.every(v=>Math.abs(v)<=1));
const silent=new APU();silent.until(742500);assert.ok(silent.take().every(v=>v===0));
for(const name of ['flappybird','hollow','mandelbrot','starter']){
  const original=JSON.parse(read('dist/examples/'+name+'.json')).package;
  const exported=loaderPackage(original),restored=decodePackage(JSON.parse(JSON.stringify(exported)));
  const before=decodePackage(original);assert.deepEqual(restored.firmware,before.firmware);assert.deepEqual(restored.assets,before.assets);
  for(const e of exported){assert.equal(typeof e.value,'string');assert.ok(e.comp_uuid);assert.ok(e.addr);assert.ok(e.addr_mem);}
}
for(const name of ['flappybird','hollow','mandelbrot','starter']){
  const original=JSON.parse(read('dist/examples/'+name+'.json')),rebuilt=JSON.parse(read('reports/rebuilt-'+name+'.json'));
  const a=new Riscplay(decodePackage(original.package)),b=new Riscplay(decodePackage(rebuilt));const frames=name==='mandelbrot'?2:5;
  for(const m of [a,b])while(m.frames<frames&&m.cycles<CPU_HZ*20)m.runUntil(m.cycles+CPU_HZ/60);
  assert.equal(a.frames,b.frames,name+' frames');assert.ok(a.frames>=frames);assert.deepEqual(a.framebuffer(),b.framebuffer(),name+' rebuilt framebuffer');
}
let inputResults=[];
for(const name of ['flappybird','hollow']){
  const pkg=JSON.parse(read('dist/examples/'+name+'.json')).package,audio=new APU(),game=new Riscplay(decodePackage(pkg),audio);
  let nonzero=0;
  while(game.cycles<CPU_HZ*3){const ms=game.cycles/CPU_HZ*1000;game.buttons=ms>=250&&ms<330||ms>=550&&ms<630?0x4000:ms>=800?0x20|((ms%350<50)?0x4000:0):0;game.runUntil(game.cycles+CPU_HZ/120);audio.until(game.cycles*2.4);for(const v of audio.take())if(v!==0)nonzero++;}
  assert.ok(game.buttonReads>0);assert.ok(game.audioWrites>40);assert.ok(nonzero>0);assert.ok(game.frames>10);
  inputResults.push({game:name,buttonReads:game.buttonReads,audioWrites:game.audioWrites,nonzeroSamples:nonzero,frames:game.frames});
}
const report={ok:true,rtlRetirementsCompared:traced,faultCases:5,rebuiltGames:4,loaderRoundtrips:4,pulse440HzCrossings:crossings,inputResults};
fs.writeFileSync(new URL('reports/core.json',root),JSON.stringify(report,null,2));console.log(report);
