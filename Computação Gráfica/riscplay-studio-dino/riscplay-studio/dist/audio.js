// Sampled model of the supplied four-channel APU. Registers and ADSR follow RTL.
// The eight-cycle mixer pipeline and HDMI serialization are not simulated.
export class APU {
  constructor(rate=48000) {
    this.rate=rate;this.t=0;this.sampleIndex=1;this.samples=[];
    this.enable=0;this.gates=0;this.adsr=0;this.vol=[15,15,15,15];
    this.period=[84374,112499,5272,5000];this.duty=[2,2];this.count=[0,0,0,0];this.phase=[0,0,0];
    this.lfsr=1;this.short=0;this.cfg=[0,0,0,0];this.level=[0,0,0,0];this.state=[0,0,0,0];this.elapsed=[0,0,0,0];this.envClock=0;
  }
  envelope() {
    for(let ch=0;ch<4;ch++) {
      const cfg=this.cfg[ch],state=this.state[ch],sustain=(cfg>>>8)&15;
      if(state===1) {
        if(this.elapsed[ch]++>=(cfg&15)){this.elapsed[ch]=0;if(this.level[ch]<15)this.level[ch]++;if(this.level[ch]>=15)this.state[ch]=2;}
      } else if(state===2) {
        if(this.level[ch]<=sustain){this.level[ch]=sustain;this.elapsed[ch]=0;this.state[ch]=3;}
        else if(this.elapsed[ch]++>=((cfg>>>4)&15)){this.elapsed[ch]=0;this.level[ch]--;}
      } else if(state===3)this.level[ch]=sustain;
      else if(state===4) {
        if(this.level[ch]===0){this.state[ch]=0;this.elapsed[ch]=0;}
        else if(this.elapsed[ch]++>=(cfg>>>12)){this.elapsed[ch]=0;this.level[ch]--;}
      } else {this.level[ch]=0;this.elapsed[ch]=0;}
    }
  }
  advance(t) {
    t=Math.floor(t);let dt=t-this.t;if(dt<=0)return;
    this.envClock+=dt;while(this.envClock>=74250){this.envelope();this.envClock-=74250;}
    for(let ch=0;ch<4;ch++) {
      const period=(ch<2?this.period[ch]>>>2:this.period[ch])+1;
      const n=Math.floor((this.count[ch]+dt)/period);this.count[ch]=(this.count[ch]+dt)%period;
      if(ch<3)this.phase[ch]=(this.phase[ch]+n)&(ch===2?31:7);
      else for(let k=0;k<n;k++)this.lfsr=(((this.lfsr^(this.lfsr>>>(this.short?6:1)))&1)<<14)|(this.lfsr>>>1);
    }
    this.t=t;
  }
  sample() {
    let sum=0;
    for(let ch=0;ch<4;ch++) {
      if(!(this.enable&(1<<ch)))continue;
      const gain=(this.adsr&(1<<ch))?this.level[ch]:15;
      let amp;
      if(ch<2)amp=this.phase[ch]<[1,2,4,6][this.duty[ch]]?2800:-2800;
      else if(ch===2){const p=this.phase[2],level=p<16?p:31-p;amp=(level-8)*300;}
      else amp=(this.lfsr&1)?-2200:2200;
      sum+=Math.trunc(Math.trunc(amp*this.vol[ch]/15)*gain/15);
    }
    return sum/32768;
  }
  until(t) {
    while(this.sampleIndex*74250000/this.rate<=t) {
      this.advance(this.sampleIndex*74250000/this.rate);this.samples.push(this.sample());this.sampleIndex++;
    }
    this.advance(t);
  }
  write(a,v,t) {
    this.until(t);
    if(a===0x200)this.enable=v&15;
    else if(a>=0x204&&a<=0x20c)this.period[(a-0x204)/4]=v;
    else if(a>=0x210&&a<=0x218)this.vol[(a-0x210)/4]=v&15;
    else if(a===0x21c||a===0x220)this.duty[(a-0x21c)/4]=v&3;
    else if(a===0x224)this.period[3]=v;
    else if(a===0x228)this.vol[3]=v&15;
    else if(a===0x22c){this.short=v&1;if(v&2){this.lfsr=1;this.count[3]=0;}}
    else if(a>=0x230&&a<=0x23c)this.cfg[(a-0x230)/4]=v&65535;
    else if(a===0x240){
      for(let ch=0;ch<4;ch++) {
        if((v&(1<<ch))&&!(this.gates&(1<<ch))){this.level[ch]=0;this.elapsed[ch]=0;this.state[ch]=1;}
        else if(!(v&(1<<ch))&&(this.gates&(1<<ch))){this.elapsed[ch]=0;this.state[ch]=4;}
      }
      this.gates=v&15;
    } else if(a===0x244)this.adsr=v&15;
  }
  take() {const data=Float32Array.from(this.samples);this.samples.length=0;return data;}
}
