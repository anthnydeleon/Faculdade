// Riscplay functional model. RV32I + custom-1 from drv32_riscplay_cpu.sv.
// The bus handshake is scheduled against the /12 CPU and /5 IO clock edges.
// SDRAM/PPU service time is an approximation, not gate-level timing simulation.
export const CPU_HZ = 30937500, IO_HZ = 74250000, W = 320, H = 180;
export const swap32 = x => ((x>>>24)|((x>>>8)&0xff00)|((x<<8)&0xff0000)|(x<<24))>>>0;
const sx=(x,n)=>(x<<(32-n))>>(32-n);
const hex=x=>(x>>>0).toString(16).padStart(8,'0');

export function decodePackage(entries) {
  if (!Array.isArray(entries)) throw Error('O pacote deve conter uma lista .json.prog.');
  let firmware=null; const assets=[];
  for (const e of entries) {
    if (+e.type===0) {
      if(firmware) throw Error('O pacote contém mais de um firmware.');
      if(typeof e.value!=='string'||!/^([0-9a-f]{2})+$/i.test(e.value)||e.value.length>65536)
        throw Error('Firmware inválido ou maior que os 32 KiB da RAM.');
      firmware=Uint8Array.from(e.value.match(/../g),x=>parseInt(x,16));
    } else if (+e.type===1) {
      const address=typeof e.addr==='number'?e.addr:parseInt(e.addr,16);
      const words=typeof e.value==='string'?JSON.parse(e.value):e.value;
      if(!Number.isInteger(address)||address<0||!Array.isArray(words)||address+words.length>0x200000||
        words.some(v=>!Number.isInteger(v)||v<0||v>0xffffffff)) throw Error('Bloco SDRAM inválido.');
      assets.push({address,words});
    } else throw Error('Tipo de bloco não suportado: '+e.type);
  }
  if(!firmware) throw Error('O pacote não contém firmware.');
  return {firmware,assets};
}

export function loaderPackage(entries) {
  decodePackage(entries); // Check the bytes and the 21-bit SDRAM address space before export.
  return entries.map((e,i)=>({
    ...e,
    comp_uuid:e.comp_uuid||'00000000-0000-4000-8000-'+(i+1).toString(16).padStart(12,'0'),
    type:+e.type,
    addr:typeof e.addr==='number'?e.addr.toString(16).toUpperCase():e.addr||'20000000',
    addr_mem:e.addr_mem||(typeof e.addr==='number'?e.addr.toString(16).toUpperCase():e.addr)||'20000000',
    value:+e.type===0?e.value:(typeof e.value==='string'?e.value:JSON.stringify(e.value))
  }));
}

export class Riscplay {
  constructor({firmware,assets=[]}, audio=null) {
    if(!firmware?.length||firmware.length>32768) throw Error('Firmware fora do limite de 32 KiB.');
    this.ram=new Uint8Array(32768); this.ram.set(firmware); this.ramView=new DataView(this.ram.buffer);
    this.sdram=new Uint32Array(0x200000); this.sprite=new Uint8Array(16384); this.palette=new Uint32Array(256);
    for(const a of assets) this.sdram.set(a.words,a.address);
    this.r=new Int32Array(32); this.pc=0x80000000; this.cycles=0; this.instructions=0;
    this.buttons=0; this.collision=0; this.fb=0; this.offset=0; this.frames=0;
    this.commands=0; this.audioWrites=0; this.buttonReads=0; this.audio=audio;
    this.spriteLength=0; this.queue=[]; this.ppuEnd=0; this.fault=null;
  }
  fail(message,i=0) { this.fault={message,pc:hex(this.pc),instruction:hex(i)}; throw Error(message+' @ 0x'+hex(this.pc)); }
  // CPU clock index at which the sequencer observes a held IO acknowledgement.
  busAck(c, wait=4) {
    const t=12+24*c;
    const firstIO=5+10*(Math.floor((t-5)/10)+1);
    const ack=firstIO+10*(2+wait);
    return Math.floor((ack-12)/24)+3;
  }
  readWord(a) {
    a>>>=0;
    if(a>=0x80000000) return this.ramView.getInt32(a&0x7ffc,true);
    if(a>=0x10000000&&a<0x18000000) return swap32(this.collision)|0;
    if(a>=0x08000000&&a<0x10000000) return this.sprite[(a>>>2)&16383]<<24;
    if(a===0) return swap32(Math.floor(this.cycles/(CPU_HZ/100)))|0;
    if(a===0x28) return Math.floor(this.cycles/(CPU_HZ/1000))|0;
    if(a===0x0c) {this.buttonReads++;return (~this.buttons<<16)|0xffff;}
    return 0;
  }
  writeWord(a,v) {
    a>>>=0;
    if(a>=0x80000000) this.ramView.setInt32(a&0x7ffc,v,true);
    else if(a>=0x08000000&&a<0x10000000) this.sprite[(a>>>2)&16383]=v&255;
    else if(a>=0x200&&a<=0x244) {this.audioWrites++;this.audio?.write(a,v>>>0,this.cycles*IO_HZ/CPU_HZ);}
  }
  flushPPU() {
    while(this.queue.length&&this.queue[0].end<=this.cycles) {
      const q=this.queue.shift();this.ppu(q.a,q.b);
    }
  }
  submit(a,b) {
    this.flushPPU();
    if(this.queue.length>=64) {this.cycles=Math.max(this.cycles,this.queue[0].end);this.flushPPU();}
    const type=a>>>28, n=(b>>>2)&65535, w=((a&7)<<4)|(b>>>28);
    const ioCost=type===0?80+12*n:type===1?2200:type===2?100+8*this.spriteLength+32*Math.ceil(this.spriteLength/(w||1)):type===3?210000:12;
    this.ppuEnd=Math.max(this.cycles,this.ppuEnd)+Math.ceil(ioCost*CPU_HZ/IO_HZ);
    this.queue.push({a:a>>>0,b:b>>>0,end:this.ppuEnd});this.commands++;
  }
  ppu(a,b) {
    const type=a>>>28, base=(a>>>7)&0x1fffff;
    if(type===0) {
      const n=(b>>>2)&65535,dest=(b>>>18)&16383;this.spriteLength=n;
      for(let j=0;j<n;j++) this.sprite[(dest+j)&16383]=(this.sdram[(base+(j>>>2))&0x1fffff]>>>(24-8*(j&3)))&255;
    } else if(type===1) {
      for(let j=0;j<256;j++) this.palette[j]=this.sdram[(base+j)&0x1fffff];
    } else if(type===2) {
      const w=((a&7)<<4)|(b>>>28),h=(b>>>21)&127,id=(a>>>3)&15;
      if(!w||!h||!this.spriteLength) return;
      const x=sx((b>>>11)&1023,10),y=sx((b>>>2)&511,9),ix=!!(b&2),iy=!!(b&1);
      // Mirror pointer updates of the supplied PPU, including its row-end behavior.
      let p=iy?w*h-w:ix?w-1:0;
      for(let j=0;j<this.spriteLength;j++) {
        const col=j%w,row=Math.floor(j/w),xx=sx((x+col)&1023,10),yy=sx((y+row)&511,9);
        const adr=(base+row*320+col)&0x1fffff,old=this.sdram[adr],index=this.sprite[p&16383];
        if(xx>=0&&xx<W&&yy>=0&&yy<H) {
          const oldid=(old>>>24)&15;
          if((old>>>24)!==255&&oldid!==id) {
            this.collision|=(1<<(((id&3)*4+(oldid&3))+(index===255?16:0)));
            this.collision|=(1<<(((oldid&3)*4+(id&3))+((old&0x10000000)?16:0)));
          }
          this.sdram[adr]=((index===255?0x90000000:0x80000000)|(id<<24)|(index===255?old&0xffffff:this.palette[index]&0xffffff))>>>0;
        }
        if(col+1<w||(!ix&&!iy)) p+=ix?-1:1;
        else if(iy) p+=1-2*w;
        else if(ix) p=j;
      }
    } else if(type===3) {
      if(base+57600<=this.sdram.length) this.sdram.fill(b,base,base+57600);
      else for(let j=0;j<57600;j++)this.sdram[(base+j)&0x1fffff]=b;
    } else if(type===4) this.collision=0;
  }
  step() {
    this.flushPPU();
    if(this.pc&3) this.fail('PC desalinhado');
    const i=this.readWord(this.pc),op=i&127,rd=(i>>>7)&31,f3=(i>>>12)&7,s1=(i>>>15)&31,s2=(i>>>20)&31,f7=i>>>25;
    const a=this.r[s1],b=this.r[s2],imm=i>>20;
    let next=(this.pc+4)>>>0,v=0,write=true;
    this.cycles=this.busAck(this.cycles)+1; // FETCH_WAIT -> EXECUTE
    switch(op) {
      case 0x37:v=i&0xfffff000;break;
      case 0x17:v=this.pc+(i&0xfffff000);break;
      case 0x6f:v=next;next=(this.pc+sx(((i>>>31)<<20)|(((i>>>12)&255)<<12)|(((i>>>20)&1)<<11)|(((i>>>21)&1023)<<1),21))>>>0;break;
      case 0x67:if(f3) this.fail('JALR inválido',i);v=next;next=((a+imm)&~1)>>>0;break;
      case 0x63:{
        write=false;let take;
        if(f3===0)take=a===b;else if(f3===1)take=a!==b;else if(f3===4)take=a<b;else if(f3===5)take=a>=b;
        else if(f3===6)take=(a>>>0)<(b>>>0);else if(f3===7)take=(a>>>0)>=(b>>>0);else this.fail('Branch inválido',i);
        if(take)next=(this.pc+sx(((i>>>31)<<12)|(((i>>>7)&1)<<11)|(((i>>>25)&63)<<5)|(((i>>>8)&15)<<1),13))>>>0;
        break;
      }
      case 0x13:
        if(f3===0)v=a+imm;else if(f3===2)v=+(a<imm);else if(f3===3)v=+((a>>>0)<(imm>>>0));
        else if(f3===4)v=a^imm;else if(f3===6)v=a|imm;else if(f3===7)v=a&imm;
        else if(f3===1&&f7===0)v=a<<s2;else if(f3===5&&(f7===0||f7===32))v=f7===32?a>>s2:a>>>s2;
        else this.fail('Shift imediato inválido',i);break;
      case 0x33:
        if(f7!==0&&!(f7===32&&(f3===0||f3===5)))this.fail('Instrução não suportada pelo DRV32 (RV32I, sem M)',i);
        if(f3===0)v=f7===32?a-b:a+b;else if(f3===1)v=a<<(b&31);else if(f3===2)v=+(a<b);else if(f3===3)v=+((a>>>0)<(b>>>0));
        else if(f3===4)v=a^b;else if(f3===5)v=f7===32?a>>(b&31):a>>>(b&31);else if(f3===6)v=a|b;else v=a&b;
        break;
      case 0x03:{
        if(![0,1,2,4,5].includes(f3))this.fail('Load inválido',i);
        const address=(a+imm)>>>0,size=1<<(f3&3);
        if(address&(size-1))this.fail('Load desalinhado',i);
        this.cycles=this.busAck(this.cycles)+1;this.flushPPU();
        const word=this.readWord(address&~3)>>>((address&3)*8);
        v=f3===0?sx(word&255,8):f3===1?sx(word&65535,16):f3===4?word&255:f3===5?word&65535:word;
        break;
      }
      case 0x23:{
        write=false;if(f3>2)this.fail('Store inválido',i);
        const address=(a+sx(((i>>>25)<<5)|rd,12))>>>0,size=1<<f3;
        if(address&(size-1))this.fail('Store desalinhado',i);
        this.cycles=this.busAck(this.cycles)+1;this.flushPPU();
        const shift=(address&3)*8,mask=f3===0?255:f3===1?65535:-1;
        const value=f3===2?b:(this.readWord(address&~3)&~(mask<<shift))|((b&mask)<<shift);
        this.writeWord(address&~3,value);this.cycles=this.busAck(this.cycles)+1;break;
      }
      case 0x2b:
        write=false;
        if(f7===1&&f3===0) {
          const send=this.busAck(this.cycles,1);
          this.cycles=(Math.floor(send/(CPU_HZ/60))+1)*(CPU_HZ/60)+3;
          this.flushPPU();this.fb=a&0x1fffff;this.offset=b&0x1fffff;this.frames++;this.onFrame?.(this);
        } else if(f7===0&&f3===0) {this.submit(a,b);this.cycles=this.busAck(this.cycles)+1;}
        else if(f7===0&&(f3===1||f3===2)) {this.cycles=this.busAck(Math.max(this.cycles,Math.ceil(this.ppuEnd)),1)+1;this.flushPPU();}
        else this.fail('Instrução PPU inválida',i);
        break;
      case 0x0f:write=false;if(f3>1)this.fail('Fence inválido',i);break;
      default:this.fail('Instrução inválida no DRV32',i);
    }
    if(next&3)this.fail('Destino de salto desalinhado',i);
    if(write&&rd)this.r[rd]=v;
    this.r[0]=0;this.pc=next;this.cycles++;this.instructions++;
  }
  runUntil(target,maxInstructions=2000000) {
    let count=0;while(this.cycles<target&&count++<maxInstructions)this.step();
    this.flushPPU();return count;
  }
  framebuffer() {
    const out=new Uint8Array(W*H*4);
    for(let j=0;j<W*H;j++) {
      const p=this.sdram[(this.fb+j+this.offset)&0x1fffff],k=j*4;
      out[k]=p>>>16;out[k+1]=p>>>8;out[k+2]=p;out[k+3]=255;
    }
    return out;
  }
}
