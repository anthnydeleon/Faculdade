#include "riscplay_audio.h"
#include <stddef.h>
#define REG(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define ENABLE REG(0x200u)
#define SQ1_PERIOD REG(0x204u)
#define SQ2_PERIOD REG(0x208u)
#define TRI_PERIOD REG(0x20Cu)
#define VOL1 REG(0x210u)
#define VOL2 REG(0x214u)
#define VOL3 REG(0x218u)
#define DUTY1 REG(0x21Cu)
#define DUTY2 REG(0x220u)
#define NOISE_PERIOD REG(0x224u)
#define NOISE_VOL REG(0x228u)
#define NOISE_MODE REG(0x22Cu)
#define ADSR1 REG(0x230u)
#define ADSR2 REG(0x234u)
#define ADSR3 REG(0x238u)
#define ADSR4 REG(0x23Cu)
#define GATE REG(0x240u)
#define ADSR_ENABLE REG(0x244u)

static uint8_t active, gates, playing, mode, loop_v1, left_v1;
static const rp_music_step_t *steps_v1;
static uint32_t count_v1, index_v1;
static const rp_song_t *song;
static uint16_t order_idx, note_idx;
static uint32_t step_remaining_us, remainder_us;
static uint32_t last_ms;
static uint8_t time_valid;
static uint16_t music_hz[3];
static uint8_t music_noise;
static uint8_t sfx_channel=255u, sfx_volume_saved;
static uint32_t sfx_remaining_us;

static uint32_t period(uint8_t ch,uint16_t hz) {
    return hz ? ((ch==RP_TRIANGLE ? 2320312u : 37125000u)/(uint32_t)hz) : 0u;
}
static void commit(void) { ENABLE=active; GATE=gates; }
static void hard_off(uint8_t ch) {
    uint8_t bit=(uint8_t)(1u<<ch);
    gates&=(uint8_t)~bit; active&=(uint8_t)~bit; commit();
}
static void start_tone(uint8_t ch,uint16_t hz) {
    if(ch>2u) return;
    if(!hz) { hard_off(ch); return; }
    uint8_t bit=(uint8_t)(1u<<ch);
    gates&=(uint8_t)~bit; commit();
    if(ch==0) SQ1_PERIOD=period(ch,hz);
    else if(ch==1) SQ2_PERIOD=period(ch,hz);
    else TRI_PERIOD=period(ch,hz);
    active|=bit; gates|=bit; commit();
}
static void start_noise(uint8_t type) {
    if(!type) { hard_off(RP_NOISE); return; }
    gates&=(uint8_t)~8u; commit();
    NOISE_PERIOD=(type==1u)?12000u:1800u;
    NOISE_MODE=2u;
    active|=8u; gates|=8u; commit();
}
void riscplay_audio_set_volume(uint8_t ch,uint8_t vol) {
    if(vol>15u) vol=15u;
    switch(ch) { case 0: VOL1=vol; break; case 1: VOL2=vol; break;
                 case 2: VOL3=vol; break; case 3: NOISE_VOL=vol; break; }
}
void riscplay_audio_set_duty(uint8_t ch,uint8_t duty) {
    if(ch==0) DUTY1=duty&3u;
    else if(ch==1) DUTY2=duty&3u;
}
void riscplay_audio_set_adsr(uint8_t ch,uint8_t a,uint8_t d,uint8_t s,uint8_t r) {
    uint32_t v=(a&15u)|((uint32_t)(d&15u)<<4)|((uint32_t)(s&15u)<<8)|((uint32_t)(r&15u)<<12);
    switch(ch) { case 0: ADSR1=v; break; case 1: ADSR2=v; break;
                 case 2: ADSR3=v; break; case 3: ADSR4=v; break; }
}
void riscplay_audio_init(void) {
    active=gates=playing=mode=loop_v1=left_v1=0;
    steps_v1=NULL; count_v1=index_v1=0; song=NULL;
    order_idx=note_idx=0; step_remaining_us=remainder_us=0;
    last_ms=0; time_valid=0; sfx_channel=255; sfx_remaining_us=0;
    music_hz[0]=music_hz[1]=music_hz[2]=0; music_noise=0;
    ENABLE=0; GATE=0; ADSR_ENABLE=0;
    VOL1=11; VOL2=7; VOL3=11; NOISE_VOL=7;
    DUTY1=2; DUTY2=1; NOISE_PERIOD=5000; NOISE_MODE=2;
    ADSR1=0x1922u; ADSR2=0x1732u; ADSR3=0x1932u; ADSR4=0x0010u;
    ADSR_ENABLE=15;
}
void riscplay_audio_note_on(uint8_t ch,uint16_t hz) { start_tone(ch,hz); }
void riscplay_audio_note_off(uint8_t ch) { if(ch<4u) { gates&=(uint8_t)~(1u<<ch); commit(); } }
void riscplay_audio_noise(uint8_t type) { start_noise(type); }
void riscplay_audio_all_off(void) { gates=active=0; commit(); }
static void clear_music(void) {
    music_hz[0]=music_hz[1]=music_hz[2]=0; music_noise=0;
}
static void apply_music(void) {
    uint8_t ch;
    for(ch=0;ch<3u;ch++) {
        if(sfx_channel==ch) continue;
        if(music_hz[ch]) start_tone(ch,music_hz[ch]);
        else hard_off(ch);
    }
    if(sfx_channel!=RP_NOISE) start_noise(music_noise);
}
void riscplay_audio_sfx_stop(void) {
    uint8_t old=sfx_channel;
    if(old==255u) return;
    sfx_channel=255u; sfx_remaining_us=0;
    hard_off(old);
    if(old==RP_PULSE2) {
        VOL2=sfx_volume_saved;
        if(playing && music_hz[1]) start_tone(RP_PULSE2,music_hz[1]);
    } else if(old==RP_NOISE) {
        NOISE_VOL=sfx_volume_saved;
        if(playing && music_noise) start_noise(music_noise);
    }
}
void riscplay_audio_stop(void) {
    playing=0; mode=0; song=NULL; steps_v1=NULL; left_v1=0;
    clear_music(); riscplay_audio_sfx_stop(); riscplay_audio_all_off();
}
void riscplay_audio_play(const rp_music_step_t *s,uint32_t count,uint8_t loop) {
    riscplay_audio_stop();
    steps_v1=s; count_v1=count; index_v1=0; loop_v1=!!loop;
    playing=(s!=NULL && count!=0); mode=1;
}
uint8_t riscplay_audio_is_playing(void) { return playing; }
void riscplay_audio_update(void) {
    if(!playing || mode!=1) return;
    if(left_v1 && --left_v1) return;
    riscplay_audio_all_off();
    if(index_v1>=count_v1) {
        if(loop_v1) index_v1=0;
        else { playing=0; clear_music(); return; }
    }
    const rp_music_step_t *s=&steps_v1[index_v1++];
    music_hz[0]=s->frequency_hz[0]; music_hz[1]=s->frequency_hz[1];
    music_hz[2]=s->frequency_hz[2]; music_noise=s->noise;
    apply_music(); left_v1=s->ticks?s->ticks:1;
}
void riscplay_audio_play_song(const rp_song_t *s) {
    riscplay_audio_stop();
    if(!s || !s->patterns || !s->order || !s->pattern_count || !s->order_count || !s->bpm) return;
    song=s; mode=2; playing=1; order_idx=note_idx=0;
    step_remaining_us=0; remainder_us=0; time_valid=0;
}
static uint8_t next_note(void) {
    uint32_t attempts=0;
    while(attempts++ < (uint32_t)song->order_count+1u) {
        if(order_idx>=song->order_count) {
            if(song->loop) order_idx=0;
            else return 0;
        }
        uint8_t pidx=song->order[order_idx];
        if(pidx>=song->pattern_count) return 0;
        const rp_pattern_t *p=&song->patterns[pidx];
        if(!p->notes || !p->count || note_idx>=p->count) {
            order_idx++; note_idx=0; continue;
        }
        const rp_note_t *n=&p->notes[note_idx++];
        music_hz[0]=n->frequency_hz[0]; music_hz[1]=n->frequency_hz[1];
        music_hz[2]=n->frequency_hz[2]; music_noise=n->noise;
        apply_music();
        uint32_t units=n->units?n->units:1u;
        /* microseconds per sixteenth, 64-bit to avoid overflow */
        uint64_t us=((uint64_t)15000000u*units)/(uint64_t)song->bpm;
        step_remaining_us=(us>UINT32_MAX)?UINT32_MAX:(uint32_t)us;
        if(!step_remaining_us) step_remaining_us=1;
        return 1;
    }
    return 0;
}
void riscplay_audio_sfx_tone(uint16_t hz,uint16_t duration_ms,uint8_t vol) {
    if(!hz || !duration_ms) return;
    riscplay_audio_sfx_stop();
    sfx_channel=RP_PULSE2; sfx_remaining_us=(uint32_t)duration_ms*1000u;
    sfx_volume_saved=(uint8_t)VOL2;
    riscplay_audio_set_volume(RP_PULSE2,vol);
    start_tone(RP_PULSE2,hz);
}
void riscplay_audio_sfx_noise(uint8_t type,uint16_t duration_ms,uint8_t vol) {
    if(!type || !duration_ms) return;
    riscplay_audio_sfx_stop();
    sfx_channel=RP_NOISE; sfx_remaining_us=(uint32_t)duration_ms*1000u;
    sfx_volume_saved=(uint8_t)NOISE_VOL;
    riscplay_audio_set_volume(RP_NOISE,vol);
    start_noise(type);
}
uint8_t riscplay_audio_sfx_active(void) { return sfx_channel!=255u; }
void riscplay_audio_update_ms(uint32_t elapsed_ms) {
    /* Avoid integer overflow and process elapsed time in bounded chunks. */
    while(elapsed_ms) {
        uint32_t chunk=elapsed_ms>1000u?1000u:elapsed_ms;
        elapsed_ms-=chunk;
        uint32_t remaining=chunk*1000u;
        while(remaining) {
            if(playing && mode==2 && !step_remaining_us) {
                if(!next_note()) { playing=0; clear_music(); apply_music(); }
            }
            uint32_t slice=remaining;
            if(playing && mode==2 && step_remaining_us<slice) slice=step_remaining_us;
            if(sfx_channel!=255u && sfx_remaining_us<slice) slice=sfx_remaining_us;
            if(!slice) break;
            if(playing && mode==2) step_remaining_us-=slice;
            if(sfx_channel!=255u) {
                sfx_remaining_us-=slice;
                if(!sfx_remaining_us) riscplay_audio_sfx_stop();
            }
            remaining-=slice;
        }
    }
}
void riscplay_audio_update_time(uint32_t now_ms) {
    if(!time_valid) { last_ms=now_ms; time_valid=1; riscplay_audio_update_ms(0); return; }
    uint32_t elapsed=now_ms-last_ms; last_ms=now_ms;
    riscplay_audio_update_ms(elapsed);
}
