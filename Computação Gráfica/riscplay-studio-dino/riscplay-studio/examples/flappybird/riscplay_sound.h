#ifndef RISCPLAY_SOUND_H
#define RISCPLAY_SOUND_H
#include <stdint.h>
#define RP_AUDIO_CLK_HZ 74250000u
#define RP_AUDIO_ENABLE (*(volatile uint32_t *)(uintptr_t)0x00000200u)
#define RP_AUDIO_PULSE1 (*(volatile uint32_t *)(uintptr_t)0x00000204u)
#define RP_AUDIO_PULSE2 (*(volatile uint32_t *)(uintptr_t)0x00000208u)
#define RP_AUDIO_TRIANGLE (*(volatile uint32_t *)(uintptr_t)0x0000020Cu)
// Square waves toggle every (period+1) cycles, hence the factor 2.
static inline uint32_t rp_square_period(uint32_t hz) {
    if (!hz) return 0xffffffffu;
    uint64_t n = (uint64_t)RP_AUDIO_CLK_HZ / (2u * (uint64_t)hz);
    return (uint32_t)(n ? n - 1u : 0u);
}
// Triangle oscillator advances one of 32 phase steps per period.
static inline uint32_t rp_triangle_period(uint32_t hz) {
    if (!hz) return 0xffffffffu;
    uint64_t n = (uint64_t)RP_AUDIO_CLK_HZ / (32u * (uint64_t)hz);
    return (uint32_t)(n ? n - 1u : 0u);
}
static inline void rp_audio_off(void) { RP_AUDIO_ENABLE = 0; }
static inline void rp_audio_pulse1(uint32_t hz) { RP_AUDIO_PULSE1 = rp_square_period(hz); }
static inline void rp_audio_pulse2(uint32_t hz) { RP_AUDIO_PULSE2 = rp_square_period(hz); }
static inline void rp_audio_triangle(uint32_t hz) { RP_AUDIO_TRIANGLE = rp_triangle_period(hz); }
static inline void rp_audio_channels(uint32_t mask) { RP_AUDIO_ENABLE = mask & 7u; }
#endif
