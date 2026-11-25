#include "menu_audio.h"
#include "riscplay_audio.h"
#include "hollow_nes_music.h"
static int was_active;
static unsigned remainder;
void menu_audio_tick(int active) {
    if (active && !was_active) {
        riscplay_audio_init();
        riscplay_audio_all_off();
        riscplay_audio_play_song(&hollow_menu_song);
        remainder = 0;
    } else if (!active && was_active) {
        riscplay_audio_stop();
        riscplay_audio_all_off();
    }
    was_active = active;
    if (active) {
        unsigned ms = (++remainder == 3) ? 16u : 17u;
        if (remainder == 3) remainder = 0;
        riscplay_audio_update_ms(ms);
    }
}
