#include "render.h"
#include "platform.h"
#include "generated/assets.h"

/* The original rooms stay 256x240. A 256x160 window follows the player
   vertically, with 32-pixel decorative margins and a separate 20-pixel HUD. */
static void draw(unsigned id, int x, int y) {
    platform_draw(id, x, y, 0, 0);
}
static int text_width(const char *s) {
    int n = 0;
    while (*s++) ++n;
    return n * 8;
}
static void text(int x, int y, const char *s) {
    while (*s) {
        unsigned c = (unsigned char)*s++;
        if (c >= 'a' && c <= 'z') c -= 'a' - 'A';
        if (c >= 'A' && c <= 'Z') draw(ASSET_ART_A + c - 'A', x, y);
        else if (c >= 32 && c <= 90) draw(ASSET_FONT_FIRST + c - 32, x, y);
        x += 8;
    }
}
static void center(int y, const char *s) { text((320 - text_width(s))/2, y, s); }
static void panel(int y) {
    int x;
    for (x = 0; x < 320; x += 80) draw(ASSET_PANEL, x, y);
}
static void background(int menu) {
    int row, col;
    for (row = 0; row < 2; ++row)
        for (col = 0; col < 4; ++col)
            draw((menu ? ASSET_MENU_BG_0 : ASSET_BG_0) + row*4 + col, col*80, row*90);
}
static int camera_y(const Game *g) {
    int c = g->y - 94;
    if (c < 0) c = 0;
    if (c > 80) c = 80;
    return (c / 8) * 8;
}
static void title(const Game *g) {
    int row, col;
    (void)g;
    for (row = 0; row < 2; ++row)
        for (col = 0; col < 4; ++col)
            draw(ASSET_ART_TITULO11 + row*4 + col, col*90, row*90);
    center(165, "APERTE X PARA INICIAR");
}
static void hud(const Game *g) {
    int i;
    for (i = 0; i < 4; ++i) draw(ASSET_HUD, i*80, 0);
    for (i = 0; i < 3; ++i)
        draw(i < g->hp ? ASSET_MASK_FULL : ASSET_MASK_EMPTY, 8+i*13, 4);
    text(54, 2, "ALMA");
    for (i = 0; i < 6; ++i)
        draw(g->soul >= (i+1)*10 ? ASSET_METER_FULL : ASSET_METER_EMPTY, 54+i*5, 11);
    text(100, 6, rooms[g->room].name);
    text(274, 6, "RISC");
}
static void world(const Game *g) {
    int cy = camera_y(g);
    int row, col, kind, sx, sy, i;
    const RoomInfo *room = &rooms[g->room];
    /* Decorations are visual; only the imported collision map is solid. */
    draw(ASSET_DECOR_ARCH, 42, 20+112-cy);
    draw(ASSET_DECOR_CRYSTAL, 248, 20+155-cy);
    for (row = cy/8; row < (cy+160)/8; ++row) {
        for (col = 0; col < 32; ++col) {
            kind = game_tile(g->room, col, row);
            if (!kind) continue;
            sx = 32 + col*8;
            sy = 20 + row*8-cy;
            if (kind == 2) draw(ASSET_TILE_SPIKE, sx, sy);
            else if (kind == 1)
                draw(game_tile(g->room, col, row-1) == 1 ? ASSET_TILE_ROCK : ASSET_TILE_TOP, sx, sy);
        }
    }
    /* Bench collision cells in the original are interaction markers. */
    for (row = 0; row < 30; ++row) {
        for (col = 0; col < 32; ++col) {
            if (game_tile(g->room, col, row) == 3 &&
                game_tile(g->room, col-1, row) != 3 &&
                game_tile(g->room, col, row-1) != 3) {
                sy = 20+row*8-cy;
                if (sy >= 20 && sy <= 164) draw(ASSET_BENCH, 32+col*8, sy);
            }
        }
    }
    /* The supplied third archive has no Elderbug/Hornet artwork. These
       traveler portraits replace their appearance and retain interactions. */
    if (room->x == 0 && room->y == 0)
        draw(ASSET_NPC_0, 32+70, 20+191-28-cy);
    if (room->x == 2 && room->y == 2)
        draw(ASSET_NPC_1, 32+102, 20+191-28-cy);
    for (i = 0; i < 2; ++i) {
        const Enemy *e = &g->enemies[i];
        int enemy_room = i == 0 ? 1 : 8;
        if (g->room != enemy_room || e->hp <= 0) continue;
        if (e->stun && ((g->frame/3u)&1u)) continue;
        platform_draw(ASSET_ENEMY_0 + ((g->frame/8u)&1u),
                      32+e->x-4, 20+e->y-8-cy, e->dir > 0, 0);
    }
    if (!g->invuln || !((g->frame/5u)&1u)) {
        unsigned sprite = ASSET_KNIGHT_IDLE_0;
        switch (g->pose) {
        case POSE_RUN: sprite = ASSET_KNIGHT_RUN_0 + (g->frame/5u)%4u; break;
        case POSE_JUMP: sprite = ASSET_KNIGHT_JUMP_0 + (g->frame/8u)%2u; break;
        case POSE_FALL: sprite = ASSET_KNIGHT_FALL_0 + (g->frame/8u)%2u; break;
        case POSE_ATTACK: sprite = ASSET_KNIGHT_SLASH_0 + (g->frame/6u)%3u; break;
        case POSE_HEAL: sprite = ASSET_KNIGHT_FOCUS_0 + (g->frame/14u)%3u; break;
        case POSE_SIT: sprite = ASSET_KNIGHT_SIT_0; break;
        default: sprite += (g->frame/24u)%4u; break;
        }
        platform_draw(sprite, 32+g->x-4, 20+g->y+16-30-cy, g->facing > 0, 0);
    }
    if (g->attack_timer > 0) {
        Rect strike;
        if (game_attack_rect(g, &strike)) {
            unsigned sprite = ASSET_SLASH_SIDE;
            int flip = g->attack_dir == ATTACK_RIGHT;
            int x = 32+strike.x-4, y = 20+strike.y-cy-2;
            if (g->attack_dir == ATTACK_UP) sprite = ASSET_SLASH_UP;
            if (g->attack_dir == ATTACK_DOWN) sprite = ASSET_SLASH_DOWN;
            platform_draw(sprite, x, y, flip, 0);
        }
    }
    /* Close the view with the HUD after world sprites have been clipped. */
    hud(g);
    if (g->interaction && !g->sitting && !g->dialogue && g->mode == GAME_PLAY) {
        text(32+g->x-4, 20+g->y-24-cy, "CIMA");
    }
    if (g->sitting) {
        panel(140);
        center(146, "DESCANSE. SUAS MASCARAS FORAM CURADAS.");
        center(163, "X: LEVANTAR");
    } else if (g->dialogue) {
        static const char *const elder[] = {
            "BEM-VINDO AS RUINAS, VIAJANTE.",
            "OS ANTIGOS CAMINHOS AINDA ESTAO AQUI.",
            "A ESPADA RECOLHE ALMA DOS INIMIGOS.",
            "CADA ACERTO CONCEDE DEZ DE ALMA.",
            "TRINTA DE ALMA RESTAURAM UMA MASCARA.",
            "FIQUE PARADO E SEGURE BAIXO PARA CURAR.",
            "CIMA PERMITE DESCANSAR NOS BANCOS.",
            "ESPINHOS E CRIATURAS GUARDAM AS SALAS.",
            "SIGA ATE O POCO A LESTE. BOA VIAGEM."
        };
        static const char *const traveler[] = {
            "VOCE ENCONTROU O CORACAO DAS RUINAS.",
            "POUCOS ALCANCAM ESTE LUGAR.",
            "SUA JORNADA CHEGA AO FIM AQUI.",
            "NA PROXIMA, EXPLORE O CAMINHO A LESTE.",
            "OBRIGADO PELA VISITA, VIAJANTE."
        };
        int line = g->dialogue_line;
        if (line < 0 || line >= (room->y == 2 ? 5 : 9)) line = 0;
        panel(140);
        center(146, room->y == 2 ? traveler[line] : elder[line]);
        center(164, room->y == 2 && line == 4 ? "X: VOLTAR AO MENU" : "X: CONTINUAR");
    }
    if (g->paused) {
        panel(70);
        center(80, "PAUSADO");
        center(96, "TRIANGULO: CONTINUAR");
    }
}
void render_game(const Game *g) {
    platform_begin();
    background(g->mode == GAME_MENU);
    if (g->mode == GAME_MENU) title(g);
    else {
        world(g);
        if (g->mode == GAME_DEAD) {
            panel(70);
            center(79, "O VAZIO CHAMA");
            center(96, "X OU TRIANGULO: VOLTAR AO MENU");
        }
    }
    platform_present();
}
