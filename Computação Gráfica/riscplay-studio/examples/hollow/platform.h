#ifndef RISCPLAY_PLATFORM_H
#define RISCPLAY_PLATFORM_H

#include <stdint.h>

enum {
    RP_BTN_UP = 1u << 4, RP_BTN_RIGHT = 1u << 5,
    RP_BTN_DOWN = 1u << 6, RP_BTN_LEFT = 1u << 7,
    RP_BTN_L2 = 1u << 8, RP_BTN_R2 = 1u << 9,
    RP_BTN_L1 = 1u << 10, RP_BTN_R1 = 1u << 11,
    RP_BTN_TRIANGLE = 1u << 12, RP_BTN_CIRCLE = 1u << 13,
    RP_BTN_X = 1u << 14, RP_BTN_SQUARE = 1u << 15
};

void platform_init(void);
uint16_t platform_input(void);
void platform_begin(void);
void platform_draw(unsigned asset_id, int x, int y, int flipx, int flipy);
void platform_present(void);
void platform_tone(unsigned hz, unsigned frames);
void platform_audio_tick(void);

#endif
