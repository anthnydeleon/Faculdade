#include "game.h"
#include "platform.h"
#include "render.h"
#include "menu_audio.h"

static Game game;
static uint16_t input_buttons(uint16_t ps2) {
    uint16_t buttons = 0;
    if (ps2 & (1u<<7)) buttons |= INPUT_LEFT;
    if (ps2 & (1u<<5)) buttons |= INPUT_RIGHT;
    if (ps2 & (1u<<4)) buttons |= INPUT_UP;
    if (ps2 & (1u<<6)) buttons |= INPUT_DOWN;
    if (ps2 & (1u<<14)) buttons |= INPUT_JUMP;
    if (ps2 & (1u<<15)) buttons |= INPUT_ATTACK;
    if (ps2 & ((1u<<12) | (1u<<13))) buttons |= INPUT_START;
    return buttons;
}
int main(void) {
    int old_hp, old_soul, old_attack, old_room, old_grounded;
    static const unsigned notes[8] = {220, 0, 330, 0, 294, 0, 196, 0};
    platform_init();
    game_init(&game);
    for (;;) {
        old_hp = game.hp;
        old_soul = game.soul;
        old_attack = game.attack_timer;
        old_room = game.room;
        old_grounded = game.grounded;
        game_tick(&game, input_buttons(platform_input()));
        menu_audio_tick(game.mode == GAME_MENU);
        if (game.mode != GAME_MENU) {
        platform_audio_tick();
        if (game.hp < old_hp) platform_tone(110, 12);
        else if (game.hp > old_hp) platform_tone(880, 10);
        else if (game.soul > old_soul) platform_tone(988, 5);
        else if (game.attack_timer > old_attack) platform_tone(440, 3);
        else if (old_grounded && !game.grounded && game.vy < 0) platform_tone(660, 4);
        else if (old_room != game.room) platform_tone(330, 8);
        else if ((game.frame % 90u) == 0 && !game.paused)
            platform_tone(notes[(game.frame/90u)%8u], 18);
        }
        render_game(&game);
    }
}
