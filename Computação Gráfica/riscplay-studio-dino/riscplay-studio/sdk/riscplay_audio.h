#ifndef RISCPLAY_AUDIO_H
#define RISCPLAY_AUDIO_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

enum { RP_PULSE1=0, RP_PULSE2=1, RP_TRIANGLE=2, RP_NOISE=3 };
/* API v1: ticks correspond to calls to riscplay_audio_update(). */
typedef struct {
    uint16_t frequency_hz[3];
    uint8_t noise;
    uint8_t ticks;
} rp_music_step_t;

void riscplay_audio_init(void);
void riscplay_audio_set_volume(uint8_t channel, uint8_t volume);
void riscplay_audio_set_duty(uint8_t pulse_channel, uint8_t duty);
void riscplay_audio_set_adsr(uint8_t channel,uint8_t a,uint8_t d,uint8_t s,uint8_t r);
void riscplay_audio_note_on(uint8_t channel,uint16_t hz);
void riscplay_audio_note_off(uint8_t channel);
void riscplay_audio_noise(uint8_t type);
void riscplay_audio_all_off(void);
void riscplay_audio_play(const rp_music_step_t *steps,uint32_t count,uint8_t loop);
void riscplay_audio_stop(void);
void riscplay_audio_update(void);
uint8_t riscplay_audio_is_playing(void);

/* v2: a step lasts units sixteenth notes (1=one 16th, 4=quarter).
   BPM is quarter notes per minute. Frequencies are Hz, 0 means rest. */
typedef struct {
    uint16_t frequency_hz[3];
    uint8_t noise;
    uint8_t units;
} rp_note_t;
typedef struct {
    const rp_note_t *notes;
    uint16_t count;
} rp_pattern_t;
/* Pattern indices form a song order. The arrays must remain valid during play. */
typedef struct {
    const rp_pattern_t *patterns;
    uint16_t pattern_count;
    const uint8_t *order;
    uint16_t order_count;
    uint16_t bpm;
    uint8_t loop;
} rp_song_t;

/* Start/advance time-based playback. Do not call v1 update() while using v2. */
void riscplay_audio_play_song(const rp_song_t *song);
void riscplay_audio_update_ms(uint32_t elapsed_ms);
/* Optional absolute clock API, handles 32-bit millisecond counter wraparound. */
void riscplay_audio_update_time(uint32_t now_ms);
/* Short effect uses pulse 2 or noise; steals channel until duration expires.
   An SFX on the same channel replaces the previous SFX. */
void riscplay_audio_sfx_tone(uint16_t hz,uint16_t duration_ms,uint8_t volume);
void riscplay_audio_sfx_noise(uint8_t type,uint16_t duration_ms,uint8_t volume);
void riscplay_audio_sfx_stop(void);
uint8_t riscplay_audio_sfx_active(void);
#ifdef __cplusplus
}
#endif
#endif
