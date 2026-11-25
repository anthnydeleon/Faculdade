/*
 * DINO RISC - versao do "jogo do dinossauro" do Google Chrome para o
 * Riscplay (CPU DRV32 / RV32I, tela 320x180, PPU de sprites e APU de 4 canais).
 *
 * Controles:  ESPACO / Z / ENTER / seta para cima  -> pular
 *             seta para baixo                     -> abaixar (ou cair mais rapido no ar)
 *
 * Organizacao do codigo:
 *   1. Constantes do jogo
 *   2. Sprites (tabela de imagens geradas em dino_assets.h)
 *   3. Som (musicas e efeitos com a biblioteca riscplay_audio)
 *   4. Estado do jogo (dinossauro, obstaculos, nuvens, placar)
 *   5. Logica (fisica, geracao de obstaculos, colisao)
 *   6. Desenho de cada quadro
 *   7. main(): laco principal a 60 quadros por segundo
 */
#include "console.h"
#include "riscplay_audio.h"
#include "dino_assets.h"

/* ------------------------------------------------------------------ 1 */
#define SCREEN_W 320
#define SCREEN_H 180
#define BG_COLOR 0xFFF7F7F7u     /* ARGB; alfa 0xFF marca "fundo vazio" para a PPU */
#define GROUND_Y 150             /* linha do chao: os pes do dino ficam em y = 149 */

#define SUB 16                   /* subpixels por pixel (ponto fixo) */
#define DINO_X 24
#define GRAVITY 5                /* subpixels/quadro^2 */
#define JUMP_VELOCITY 80         /* altura maxima ~40 px */
#define DROP_VELOCITY 30         /* soltar o botao corta o pulo */
#define MIN_JUMP_HEIGHT (20 * SUB)
#define FAST_FALL 12             /* gravidade extra segurando "baixo" no ar */

#define SPEED_START 48           /* 3,0 px por quadro */
#define SPEED_MAX 104            /* 6,5 px por quadro */
#define SPEED_STEP_FRAMES 64     /* +1/16 px a cada 64 quadros */
#define SCORE_PIXELS 16          /* 1 ponto a cada 16 px percorridos */

#define MAX_OBSTACLES 3
#define MAX_CLOUDS 3
#define PTERO_MIN_SCORE 250      /* pterodatilos so aparecem depois disso */
#define BIG_CACTUS_MIN_SCORE 300 /* o trio de cactos grandes tambem */
#define GAME_OVER_DELAY 30       /* quadros antes de aceitar reinicio */

/* Identificadores de sprite na PPU (o hardware separa 4 grupos: 0..3). */
#define ID_SCENE 0
#define ID_OBSTACLE 1
#define ID_DINO 2
#define ID_HUD 3

/* Framebuffers duplos na SDRAM (enderecos em palavras de 32 bits). */
static const uint32_t framebuffers[2] = {0xA080u, 0x18180u};
static uint32_t fb;              /* buffer onde o quadro atual e desenhado */

/* ------------------------------------------------------------------ 2 */
typedef struct { uint32_t palette, pixels, w, h; } Sprite;
#define SPR(N) { N##_PALETTE_ADDR, N##_ADDR, N##_WIDTH, N##_HEIGHT }

enum {
    S_DINO_STAND, S_DINO_RUN1, S_DINO_RUN2, S_DINO_DEAD, S_DINO_DUCK1, S_DINO_DUCK2,
    S_CACTUS_S1, S_CACTUS_S2, S_CACTUS_L1, S_CACTUS_L3, S_PTERO1, S_PTERO2,
    S_CLOUD, S_GROUND, S_RESTART, S_TXT_HI, S_TXT_GAMEOVER, S_TXT_TITLE, S_TXT_PRESS,
    S_COUNT
};
static const Sprite sprites[S_COUNT] = {
    SPR(DINO_STAND), SPR(DINO_RUN1), SPR(DINO_RUN2), SPR(DINO_DEAD),
    SPR(DINO_DUCK1), SPR(DINO_DUCK2),
    SPR(CACTUS_S1), SPR(CACTUS_S2), SPR(CACTUS_L1), SPR(CACTUS_L3),
    SPR(PTERO1), SPR(PTERO2),
    SPR(CLOUD), SPR(GROUND), SPR(RESTART),
    SPR(TXT_HI), SPR(TXT_GAMEOVER), SPR(TXT_TITLE), SPR(TXT_PRESS),
};

/* A PPU tem um unico buffer de sprite. Carregar paleta + pixels custa
   tempo, entao lembramos qual imagem ja esta carregada e so recarregamos
   quando outra for pedida. Varios desenhos seguidos da mesma imagem
   (chao, nuvens) aproveitam a mesma carga. */
static int loaded_sprite = -1;

static void clear_screen(void)
{
    uint32_t rs1 = 0x30000000u | (fb << 7);
    uint32_t rs2 = BG_COLOR;
    asm volatile (".insn r 0x2B, 0x0, 0x00, x0, %0, %1" : : "r"(rs1), "r"(rs2) : "memory");
}

static void draw(int s, int x, int y, int id)
{
    const Sprite *p = &sprites[s];
    if (x >= SCREEN_W || y >= SCREEN_H || x + (int)p->w <= 0 || y + (int)p->h <= 0) return;
    if (loaded_sprite != s) {
        load_pallet_from_sdram(p->palette);
        copy_sprite_from_sdram_to_buffer(p->pixels, p->w * p->h, 0);
        loaded_sprite = s;
    }
    render_sprite(id, fb, p->w, p->h, x, y, 0, 0);
}

/* Os 10 digitos estao empilhados numa unica imagem (6x80). Cada digito
   ocupa 6x8 = 48 pixels = 12 palavras, entao basta copiar a partir de
   DIGITS_ADDR + 12*d. A paleta e carregada uma so vez para todos. */
#define DIGIT_W 6
#define DIGIT_H 8
#define DIGIT_WORDS 12

static void draw_number(uint32_t value, int digits, int x, int y)
{
    uint32_t divisor = 1;
    for (int i = 1; i < digits; i++) divisor *= 10u;
    load_pallet_from_sdram(DIGITS_PALETTE_ADDR);
    for (int i = 0; i < digits; i++) {
        uint32_t d = (value / divisor) % 10u;
        divisor /= 10u;
        copy_sprite_from_sdram_to_buffer(DIGITS_ADDR + d * DIGIT_WORDS, DIGIT_W * DIGIT_H, 0);
        render_sprite(ID_HUD, fb, DIGIT_W, DIGIT_H, x + i * DIGIT_W, y, 0, 0);
    }
    loaded_sprite = -1;  /* o buffer da PPU agora contem um digito */
}

/* ------------------------------------------------------------------ 3 */
#define NOTE(a, b, c, units) {{a, b, c}, 0, units}

/* Tela inicial: melodia alegre no pulse 1 e baixo no triangulo. */
static const rp_note_t title_notes[] = {
    NOTE(523, 0, 131, 2), NOTE(659, 0, 131, 2), NOTE(784, 0, 98, 2), NOTE(659, 0, 98, 2),
    NOTE(698, 0, 87, 2),  NOTE(880, 0, 87, 2),  NOTE(784, 0, 98, 2), NOTE(0, 0, 98, 2),
    NOTE(659, 0, 131, 2), NOTE(587, 0, 131, 2), NOTE(523, 0, 110, 2), NOTE(587, 0, 110, 2),
    NOTE(659, 0, 87, 2),  NOTE(587, 0, 98, 2),  NOTE(523, 0, 131, 4),
};
static const rp_pattern_t title_patterns[] = {{title_notes, 15}};
static const uint8_t title_order[] = {0};
static const rp_song_t title_song = {title_patterns, 1, title_order, 1, 140, 1};

/* Durante a corrida: trilha leve. O pulse 2 fica livre para os efeitos. */
static const rp_note_t run_a[] = {
    NOTE(659, 0, 110, 2), NOTE(0, 0, 110, 2), NOTE(784, 0, 110, 2), NOTE(0, 0, 110, 2),
    NOTE(880, 0, 87, 2),  NOTE(784, 0, 87, 2), NOTE(659, 0, 98, 2), NOTE(0, 0, 98, 2),
};
static const rp_note_t run_b[] = {
    NOTE(523, 0, 131, 2), NOTE(0, 0, 131, 2), NOTE(587, 0, 131, 2), NOTE(0, 0, 131, 2),
    NOTE(659, 0, 98, 2),  NOTE(587, 0, 98, 2), NOTE(523, 0, 82, 2), NOTE(0, 0, 82, 2),
};
static const rp_pattern_t run_patterns[] = {{run_a, 8}, {run_b, 8}};
static const uint8_t run_order[] = {0, 0, 1, 1};
static const rp_song_t run_song = {run_patterns, 2, run_order, 4, 150, 1};

/* Fim de jogo: frase descendente, toca uma vez. */
static const rp_note_t over_notes[] = {
    NOTE(523, 0, 131, 2), NOTE(392, 0, 98, 2), NOTE(330, 0, 82, 2), NOTE(262, 0, 65, 4),
    NOTE(0, 0, 0, 2),
};
static const rp_pattern_t over_patterns[] = {{over_notes, 5}};
static const uint8_t over_order[] = {0};
static const rp_song_t over_song = {over_patterns, 1, over_order, 1, 120, 0};

static uint8_t audio_remainder;
static uint8_t chime_pending;        /* segunda nota do "ponto" */
static uint16_t chime_ms;
static uint8_t over_pending;         /* musica de fim apos o ruido da batida */
static uint16_t over_ms;

static void music(const rp_song_t *song, uint8_t melody_vol, uint8_t bass_vol)
{
    chime_pending = over_pending = 0;
    riscplay_audio_play_song(song);
    riscplay_audio_set_volume(RP_PULSE1, melody_vol);
    riscplay_audio_set_volume(RP_TRIANGLE, bass_vol);
    riscplay_audio_update_ms(1);
}

static void sfx_jump(void)  { riscplay_audio_sfx_tone(620, 70, 10); }

static void sfx_point(void)
{
    riscplay_audio_sfx_tone(988, 70, 11);
    chime_pending = 1;
    chime_ms = 70;
}

static void sfx_hit(void)
{
    riscplay_audio_stop();
    chime_pending = 0;
    riscplay_audio_sfx_noise(2, 180, 12);
    over_pending = 1;
    over_ms = 220;
}

/* Chamado uma vez por quadro. 16,17,17 ms -> media exata de 1/60 s. */
static void audio_tick(void)
{
    uint32_t ms = 17;
    if (++audio_remainder == 3) { audio_remainder = 0; ms = 16; }
    riscplay_audio_update_ms(ms);
    if (chime_pending) {
        if (chime_ms <= ms) { chime_pending = 0; riscplay_audio_sfx_tone(1319, 110, 11); }
        else chime_ms -= (uint16_t)ms;
    }
    if (over_pending) {
        if (over_ms <= ms) {
            over_pending = 0;
            riscplay_audio_sfx_stop();
            riscplay_audio_play_song(&over_song);
            riscplay_audio_set_volume(RP_PULSE1, 11);
            riscplay_audio_set_volume(RP_TRIANGLE, 11);
            riscplay_audio_update_ms(1);
        } else over_ms -= (uint16_t)ms;
    }
}

/* ------------------------------------------------------------------ 4 */
typedef enum { STATE_TITLE, STATE_PLAYING, STATE_GAME_OVER } GameState;

typedef enum { K_CACTUS_S1, K_CACTUS_S2, K_CACTUS_L1, K_CACTUS_L3, K_PTERO, K_COUNT } Kind;

typedef struct { int8_t x, y, w, h; } Box;     /* caixa de colisao relativa ao sprite */

typedef struct {
    int active;
    Kind kind;
    int x, y;
} Obstacle;

typedef struct { int x, y; } Cloud;

static GameState state;
static uint32_t frame;

static int dino_height;          /* altura acima do chao, em subpixels (0 = no chao) */
static int dino_vy;              /* velocidade vertical (positiva = subindo) */
static int dino_ducking;
static int jumping;

static int speed;                /* subpixels por quadro */
static int scroll_sub;           /* fracao acumulada do deslocamento */
static int ground_offset;        /* 0..63, desloca o chao repetido */
static int cloud_sub;
static int distance;             /* pixels percorridos desde o ultimo ponto */
static int next_spawn;           /* pixels ate o proximo obstaculo */
static uint32_t score, best;     /* best = recorde das partidas anteriores */
static int flash_frames;         /* placar pisca a cada 100 pontos */
static int over_frames;

static Obstacle obstacles[MAX_OBSTACLES];
static Cloud clouds[MAX_CLOUDS];

/* Gerador pseudoaleatorio xorshift32: so usa deslocamentos e XOR. */
static uint32_t rng = 0x1234567u;
static uint32_t random_u32(void)
{
    rng ^= rng << 13;
    rng ^= rng >> 17;
    rng ^= rng << 5;
    return rng;
}
static int random_range(int n) { return (int)(random_u32() % (uint32_t)n); }

/* ------------------------------------------------------------------ 5 */
/* Caixas de colisao desenhadas a mao sobre cada imagem. Usar varias caixas
   pequenas, como o jogo original faz, evita colisoes "no vazio" entre os
   bracos do cacto ou atras da cabeca do dinossauro. */
static const Box dino_boxes[] = {{12, 1, 8, 6}, {5, 8, 10, 9}, {6, 17, 6, 5}};
static const Box duck_boxes[] = {{19, 1, 9, 4}, {3, 2, 22, 6}, {6, 9, 7, 3}};

static const Box box_s1[] = {{4, 0, 4, 23}, {0, 4, 4, 8}, {8, 6, 3, 7}};
static const Box box_s2[] = {{4, 0, 4, 23}, {0, 4, 4, 8}, {8, 6, 3, 7},
                             {15, 0, 4, 23}, {19, 4, 4, 8}, {12, 6, 3, 7}};
static const Box box_l1[] = {{6, 0, 5, 32}, {1, 5, 5, 13}, {11, 7, 5, 12}};
static const Box box_l3[] = {{6, 0, 5, 32}, {1, 5, 5, 13}, {11, 7, 5, 12},
                             {21, 9, 4, 23}, {17, 13, 4, 8}, {25, 15, 3, 7},
                             {35, 0, 5, 32}, {30, 5, 5, 13}, {40, 7, 5, 12}};
static const Box box_ptero_up[] = {{1, 5, 5, 4}, {6, 8, 17, 4}, {9, 2, 6, 6}};
static const Box box_ptero_down[] = {{1, 5, 5, 4}, {6, 8, 17, 4}, {9, 11, 6, 5}};

static const int kind_sprite[K_COUNT] = {S_CACTUS_S1, S_CACTUS_S2, S_CACTUS_L1, S_CACTUS_L3, S_PTERO1};

/* Alturas do pterodatilo: baixo (pular), medio (abaixar), alto (passa por baixo). */
static const int ptero_heights[3] = {131, 120, 100};

static int ptero_frame(void) { return (frame >> 3) & 1u; }

static int obstacle_sprite(const Obstacle *o)
{
    if (o->kind == K_PTERO) return ptero_frame() ? S_PTERO2 : S_PTERO1;
    return kind_sprite[o->kind];
}

static int obstacle_boxes(const Obstacle *o, const Box **boxes)
{
    switch (o->kind) {
    case K_CACTUS_S1: *boxes = box_s1; return 3;
    case K_CACTUS_S2: *boxes = box_s2; return 6;
    case K_CACTUS_L1: *boxes = box_l1; return 3;
    case K_CACTUS_L3: *boxes = box_l3; return 9;
    default:
        *boxes = ptero_frame() ? box_ptero_down : box_ptero_up;
        return 3;
    }
}

static int dino_sprite(void)
{
    if (state == STATE_GAME_OVER) return S_DINO_DEAD;
    if (state == STATE_TITLE || dino_height > 0) return S_DINO_STAND;
    if (dino_ducking) return ((frame >> 2) & 1u) ? S_DINO_DUCK2 : S_DINO_DUCK1;
    return ((frame >> 2) & 1u) ? S_DINO_RUN2 : S_DINO_RUN1;
}

static int dino_top(void)
{
    int s = dino_sprite();
    return GROUND_Y - (int)sprites[s].h - dino_height / SUB;
}

static int overlap(int ax, int ay, const Box *a, int bx, int by, const Box *b)
{
    int l1 = ax + a->x, t1 = ay + a->y, l2 = bx + b->x, t2 = by + b->y;
    return l1 < l2 + b->w && l2 < l1 + a->w && t1 < t2 + b->h && t2 < t1 + a->h;
}

static int dino_hits_obstacle(void)
{
    const Box *dboxes = dino_ducking && dino_height == 0 ? duck_boxes : dino_boxes;
    int dy = dino_top();
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        const Obstacle *o = &obstacles[i];
        if (!o->active) continue;
        const Box *oboxes;
        int n = obstacle_boxes(o, &oboxes);
        for (int a = 0; a < 3; a++)
            for (int b = 0; b < n; b++)
                if (overlap(DINO_X, dy, &dboxes[a], o->x, o->y, &oboxes[b])) return 1;
    }
    return 0;
}

static void spawn_obstacle(void)
{
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        Obstacle *o = &obstacles[i];
        if (o->active) continue;
        Kind kind;
        if (score >= PTERO_MIN_SCORE && random_range(4) == 0) kind = K_PTERO;
        else if (score < BIG_CACTUS_MIN_SCORE) kind = (Kind)random_range(3);  /* sem o trio grande */
        else kind = (Kind)random_range(4);
        o->active = 1;
        o->kind = kind;
        o->x = SCREEN_W;
        o->y = kind == K_PTERO ? ptero_heights[random_range(3)]
                               : GROUND_Y + 1 - (int)sprites[kind_sprite[kind]].h;
        /* Distancia ate o proximo: cresce com a velocidade para o pulo sempre caber. */
        int min_gap = ((speed * 36) >> 4) + 24;
        next_spawn = (int)sprites[kind_sprite[kind]].w + min_gap + random_range((min_gap >> 1) + 1);
        return;
    }
    next_spawn = 20;  /* todos ocupados: tenta de novo em breve */
}

static void reset_world(void)
{
    dino_height = dino_vy = dino_ducking = jumping = 0;
    speed = SPEED_START;
    scroll_sub = cloud_sub = distance = 0;
    next_spawn = 120;
    score = 0;
    flash_frames = 0;
    for (int i = 0; i < MAX_OBSTACLES; i++) obstacles[i].active = 0;
    for (int i = 0; i < MAX_CLOUDS; i++) {
        clouds[i].x = 40 + i * 110 + random_range(40);
        clouds[i].y = 30 + random_range(60);
    }
}

static void start_jump(void)
{
    dino_vy = JUMP_VELOCITY;
    jumping = 1;
    sfx_jump();
}

static void update_dino(int jump_held, int down_held)
{
    dino_ducking = down_held;
    if (dino_height == 0 && !jumping) {
        if (jump_held && !down_held) start_jump();
        else return;
    }
    /* Soltou o botao cedo: pulo curto (depois de uma altura minima). */
    if (!jump_held && dino_vy > DROP_VELOCITY && dino_height >= MIN_JUMP_HEIGHT)
        dino_vy = DROP_VELOCITY;
    dino_vy -= GRAVITY;
    if (down_held) dino_vy -= FAST_FALL;
    dino_height += dino_vy;
    if (dino_height <= 0) {
        dino_height = 0;
        dino_vy = 0;
        jumping = 0;
    }
}

static void update_world(void)
{
    /* Velocidade em ponto fixo: acumula subpixels e anda so os pixels inteiros. */
    if ((frame % SPEED_STEP_FRAMES) == 0 && speed < SPEED_MAX) speed++;
    scroll_sub += speed;
    int dx = scroll_sub / SUB;
    scroll_sub -= dx * SUB;

    ground_offset = (ground_offset + dx) & 63;

    /* Nuvens andam a 1/4 da velocidade (paralaxe). */
    cloud_sub += speed;
    int cdx = cloud_sub / (SUB * 4);
    cloud_sub -= cdx * SUB * 4;
    for (int i = 0; i < MAX_CLOUDS; i++) {
        clouds[i].x -= cdx;
        if (clouds[i].x < -(int)CLOUD_WIDTH) {
            clouds[i].x = SCREEN_W + random_range(80);
            clouds[i].y = 30 + random_range(60);
        }
    }

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        Obstacle *o = &obstacles[i];
        if (!o->active) continue;
        o->x -= dx;
        if (o->kind == K_PTERO && (frame & 1u)) o->x--;   /* voa um pouco mais rapido */
        if (o->x + (int)sprites[obstacle_sprite(o)].w < 0) o->active = 0;
    }
    next_spawn -= dx;
    if (next_spawn <= 0) spawn_obstacle();

    distance += dx;
    while (distance >= SCORE_PIXELS) {
        distance -= SCORE_PIXELS;
        score++;
        if (score % 100u == 0) {
            sfx_point();
            flash_frames = 48;
        }
    }
    if (flash_frames) flash_frames--;
}

/* ------------------------------------------------------------------ 6 */
static void draw_scene(void)
{
    for (int i = 0; i < MAX_CLOUDS; i++) draw(S_CLOUD, clouds[i].x, clouds[i].y, ID_SCENE);
    for (int x = -ground_offset; x < SCREEN_W; x += 64) draw(S_GROUND, x, GROUND_Y - 1, ID_SCENE);
    for (int i = 0; i < MAX_OBSTACLES; i++)
        if (obstacles[i].active)
            draw(obstacle_sprite(&obstacles[i]), obstacles[i].x, obstacles[i].y, ID_OBSTACLE);
    draw(dino_sprite(), DINO_X, dino_top(), ID_DINO);
}

static void draw_hud(void)
{
    if (best > 0) {
        draw(S_TXT_HI, 222, 8, ID_HUD);
        draw_number(best, 5, 238, 8);
    }
    /* Durante o "flash" de 100 pontos o placar pisca. */
    if (state != STATE_TITLE && (flash_frames == 0 || ((flash_frames >> 3) & 1u)))
        draw_number(score, 5, 284, 8);
}

static void draw_frame(void)
{
    clear_screen();
    draw_scene();
    draw_hud();
    if (state == STATE_TITLE) {
        draw(S_TXT_TITLE, (SCREEN_W - (int)TXT_TITLE_WIDTH) / 2, 42, ID_HUD);
        if ((frame >> 5) & 1u)
            draw(S_TXT_PRESS, (SCREEN_W - (int)TXT_PRESS_WIDTH) / 2, 72, ID_HUD);
    } else if (state == STATE_GAME_OVER) {
        draw(S_TXT_GAMEOVER, (SCREEN_W - (int)TXT_GAMEOVER_WIDTH) / 2, 60, ID_HUD);
        draw(S_RESTART, (SCREEN_W - (int)RESTART_WIDTH) / 2, 78, ID_HUD);
    }
}

/* ------------------------------------------------------------------ 7 */
int main(void)
{
    int buffer = 0;
    int prev_jump = 1;           /* exige soltar e apertar de novo ao ligar */

    riscplay_audio_init();
    clear_collision();
    reset_world();
    state = STATE_TITLE;
    music(&title_song, 10, 11);

    for (;;) {
        audio_tick();
        uint32_t pad = get_ps2_controller_state();
        int jump_held = BUTTON_X_PRESSED(pad) || BUTTON_UP_PRESSED(pad);
        int down_held = BUTTON_DOWN_PRESSED(pad);
        int jump_pressed = jump_held && !prev_jump;
        prev_jump = jump_held;
        frame++;

        if (state == STATE_TITLE) {
            if (jump_pressed) {
                rng ^= get_time() ^ (frame << 16);   /* semente diferente a cada partida */
                if (!rng) rng = 1;
                reset_world();
                state = STATE_PLAYING;
                music(&run_song, 6, 9);
                start_jump();                        /* como no original: comeca pulando */
            }
        } else if (state == STATE_PLAYING) {
            update_dino(jump_held, down_held);
            update_world();
            if (dino_hits_obstacle()) {
                state = STATE_GAME_OVER;
                over_frames = 0;
                if (score > best) best = score;
                dino_ducking = 0;
                sfx_hit();
            }
        } else {
            if (over_frames < GAME_OVER_DELAY) over_frames++;
            else if (jump_pressed) {
                reset_world();
                state = STATE_PLAYING;
                music(&run_song, 6, 9);
                start_jump();
            }
        }

        fb = framebuffers[buffer];
        draw_frame();
        ppu_finished_all_operations();   /* espera a PPU terminar o quadro */
        set_frame_buffer(fb, 0);         /* mostra o quadro (sincroniza a 60 Hz) */
        buffer ^= 1;
    }
}
