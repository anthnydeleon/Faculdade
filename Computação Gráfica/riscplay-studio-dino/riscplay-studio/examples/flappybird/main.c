#include "sprites.h"
#include "console.h"
#include <stdint.h>
#include "tilemap1.h"
#include "tilemap2.h"
#include "riscplay_audio.h"
#define __MIN_ADDR_IN_SDRAM_TO_LOAD_SPRITES__ 0x26280 //((320*128)+128)+((320*180)*2)
#define PLAYER_X 50                      // player X position in pixels
#define PLAYER_INIT_Y 10                 // Initial Y position in pixels
#define PLAYER_MIN_Y 0                 // player upper limit
#define PLAYER_MAX_Y 148                 // player bottom limit
#define GRAVITY 4                        // Gravity applied to the player
#define SUBPIXELS 16 
#define MAX_GRAVITY 80
#define JUMP_SPEED -34

int player_y = 0;                     // player y position
int player_y_sub = 0;                 // player y position (subpixel)
int player_y_vel = 0;                 // player y velocity in subpixels
bool player_alive = false;

int32_t riscv_abs_c(int32_t val);
void delay(volatile unsigned long count);
void initialize_player();
void update_player();
void handle_player_input();
void apply_player_physics();
int apply_subpixel_movement(int *position, int *subpixel, int velocity);
void handle_player_movement();
void player_jump();

/* RiscPlay Audio v2: sequenciador nao bloqueante.
   Um quadro do jogo equivale aproximadamente a 16,67 ms (60 fps).
   O andamento fica condicionado a essa cadencia; substituir por relogio
   real em ms se a taxa de quadros nao for estavel. */
static uint8_t audio_frame_remainder = 0;
static uint8_t score_chime_stage = 0;
static uint16_t score_chime_ms = 0;
static uint8_t game_over_pending = 0;
static uint16_t game_over_wait_ms = 0;

#define NOTE(a,b,c,n,u) {{a,b,c},n,u}
static const rp_note_t menu_notes[] = {
    NOTE(523,262,131,0,2), NOTE(659,330,165,0,2),
    NOTE(784,392,196,0,2), NOTE(659,330,165,0,2),
    NOTE(587,294,147,0,2), NOTE(698,349,175,0,2),
    NOTE(880,440,220,0,2), NOTE(0,0,0,0,2)
};
static const rp_pattern_t menu_patterns[] = {{menu_notes,8}};
static const uint8_t menu_order[] = {0};
static const rp_song_t menu_song = {menu_patterns,1,menu_order,1,145,1};

static const rp_note_t over_notes[] = {
    NOTE(523,262,131,0,2), NOTE(392,196,98,0,2),
    NOTE(330,165,82,0,2), NOTE(262,131,65,0,4),
    NOTE(0,0,0,0,2)
};
static const rp_pattern_t over_patterns[] = {{over_notes,5}};
static const uint8_t over_order[] = {0};
static const rp_song_t over_song = {over_patterns,1,over_order,1,115,0};

static void audio_reset_effects(void) {
    score_chime_stage=0; score_chime_ms=0;
    game_over_pending=0; game_over_wait_ms=0;
    riscplay_audio_sfx_stop();
}
static void audio_enter_menu(void) {
    audio_reset_effects();
    riscplay_audio_play_song(&menu_song);
    riscplay_audio_update_ms(1);
}
static void audio_start_game(void) {
    audio_reset_effects();
    riscplay_audio_stop();
}
static void audio_jump(void) {
    /* Salto: canal Pulse 2, prioridade temporaria sobre musica. */
    riscplay_audio_sfx_tone(740,95,10);
}
static void audio_score(void) {
    /* Dois tons curtos, disparados em quadros diferentes. */
    score_chime_stage=1;
    score_chime_ms=65;
    riscplay_audio_sfx_tone(988,65,11);
}
static void audio_collision(void) {
    /* Ruido de impacto seguido de melodia descendente de game over. */
    riscplay_audio_stop();
    audio_reset_effects();
    riscplay_audio_sfx_noise(2,180,12);
    game_over_pending=1;
    game_over_wait_ms=200;
}
static void audio_tick(uint32_t elapsed_ms) {
    riscplay_audio_update_ms(elapsed_ms);
    if (score_chime_stage==1) {
        if (elapsed_ms>=score_chime_ms) {
            score_chime_stage=0;
            score_chime_ms=0;
            riscplay_audio_sfx_tone(1319,95,11);
        } else score_chime_ms-=(uint16_t)elapsed_ms;
    }
    if (game_over_pending) {
        if (elapsed_ms>=game_over_wait_ms) {
            game_over_pending=0;
            game_over_wait_ms=0;
            riscplay_audio_sfx_stop();
            riscplay_audio_play_song(&over_song);
            riscplay_audio_update_ms(1);
        } else game_over_wait_ms-=(uint16_t)elapsed_ms;
    }
}
static void audio_tick_frame(void) {
    /* Sequencia exata de 16,17,17 ms => media de 50/3 ms. */
    uint32_t ms=16;
    if (++audio_frame_remainder==3) audio_frame_remainder=0;
    else ms=17;
    audio_tick(ms);
}

// Pontuacao: um ponto por coluna de obstaculo ultrapassada.
// Cada coluna pode conter varios tiles e/f/g, mas vale apenas um ponto.
#define SCORE_DIGITS 5
#define SCORE_Y 6
#define SCORE_X ((320 - SCORE_DIGITS * 8) / 2)
static uint32_t score = 0;
static uint32_t best_score = 0;
static uint32_t scored_columns = 0;

static uint32_t score_digit_address(uint32_t digit) {
    static const uint32_t addresses[10] = {
        zero_adr_spr, um_adr_spr, dois_adr_spr, tres_adr_spr,
        quatro_adr_spr, cinco_adr_spr, seis_adr_spr,
        sete_adr_spr, oito_adr_spr, nove_adr_spr
    };
    return addresses[digit];
}

static void draw_score(uint32_t framebuffer) {
    uint32_t value = score % 100000u;
    uint32_t divisor = 10000u;
    for (int i = 0; i < SCORE_DIGITS; ++i) {
        uint32_t digit = value / divisor;
        value %= divisor;
        divisor /= 10u;
        uint32_t address = score_digit_address(digit);
        load_pallet_from_sdram(address);
        copy_sprite_from_sdram_to_buffer(address + 0x100u, 8u * 8u, 0);
        // ID 3: HUD separado do jogador (ID 2) e dos obstaculos (ID 1).
        render_sprite(3, framebuffer, 8, 8, SCORE_X + i * 8, SCORE_Y, 0, 0);
    }
}

// Telas completas: tres colunas de 90 pixels e uma de 50 pixels.
// Cada linha tem 90 pixels, totalizando 320x180.
// O jogo opera a 60 quadros/s; 600 quadros correspondem a 10 s.
#define GAME_OVER_TIMEOUT_FRAMES 600u
typedef enum { GAME_LOGO, GAME_MENU, GAME_PLAYING, GAME_OVER } game_state_t;
static const uint32_t logo_tiles[8] = {
    lg11_adr_spr, lg12_adr_spr, lg13_adr_spr, lg14_adr_spr,
    lg21_adr_spr, lg22_adr_spr, lg23_adr_spr, lg24_adr_spr
};
static const uint32_t menu_tiles[8] = {
    me11_adr_spr, me12_adr_spr, me13_adr_spr, me14_adr_spr,
    me21_adr_spr, me22_adr_spr, me23_adr_spr, me24_adr_spr
};
static const uint32_t over_tiles[8] = {
    go11_adr_spr, go12_adr_spr, go13_adr_spr, go14_adr_spr,
    go21_adr_spr, go22_adr_spr, go23_adr_spr, go24_adr_spr
};

static void draw_full_screen(uint32_t framebuffer, const uint32_t *tiles) {
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 4; ++col) {
            uint32_t address = tiles[row * 4 + col];
            uint32_t width = col == 3 ? 50u : 90u;
            load_pallet_from_sdram(address);
            copy_sprite_from_sdram_to_buffer(address + 0x100u, width * 90u, 0);
            render_sprite(4, framebuffer, width, 90u,
                          col * 90, row * 90, 0, 0);
        }
    }
}

static void draw_final_score(uint32_t framebuffer) {
    uint32_t value = score % 100000u;
    uint32_t divisor = 10000u;
    for (int i = 0; i < SCORE_DIGITS; ++i) {
        uint32_t digit = value / divisor;
        value %= divisor;
        divisor /= 10u;
        uint32_t address = score_digit_address(digit);
        load_pallet_from_sdram(address);
        copy_sprite_from_sdram_to_buffer(address + 0x100u, 8u * 8u, 0);
        render_sprite(3, framebuffer, 8, 8,
                      ((320 - SCORE_DIGITS * 8) / 2 + i * 8)+70,
                      ((180 - 8) / 2)+3, 0, 0);
    }
}

static void update_score(int previous_scroll, int current_scroll) {
    // Detecta a passagem da borda direita de cada coluna de obstaculos
    // pela borda esquerda do passaro. Evita pontuacao duplicada.
    for (int col = 0; col < ___TILEMAP_SIZEX__; ++col) {
        int obstacle = 0;
        for (int row = 0; row < ___TILEMAP_SIZEY__ - 1; ++row) {
            char tile = __tilemap__[row][col];
            if (tile == 'e' || tile == 'f' || tile == 'g') {
                obstacle = 1;
                break;
            }
        }
        if (!obstacle) continue;
        int crossing = (col + 1) * ___TILE_SIZE__ - PLAYER_X;
        if (crossing > previous_scroll && crossing <= current_scroll) {
            uint32_t bit = (col < 32) ? (1u << col) : 0u;
            // Colunas de obstaculos atualmente presentes cabem em 32 bits.
            if (col < 32 && !(scored_columns & bit)) {
                scored_columns |= bit;
                ++score;
                if (score > best_score) best_score = score;
                audio_score();
            }
        }
    }
}

int32_t riscv_abs_c(int32_t val) {
    int32_t mask = val >> 31; // Deslocamento aritmético (preserva o sinal)
    return (val ^ mask) - mask;
}

void delay(volatile unsigned long count){
    while (count > 0) {
        count--;
    }
}

void initialize_player() {
   player_y = PLAYER_INIT_Y;          // Set player’s initial Y position
   player_alive = true;
   player_y_sub = 0;                 // player y position (subpixel)
   player_y_vel = 0;                 // player y velocity in subpixels
}

// Updates all aspects of the player’s movement each frame.
void update_player() {
  apply_player_physics();    // Apply gravity and movement physics
  handle_player_movement();  // Updates player movement using subpixel calculations
}

// Handles input from the controller.
void handle_player_input() {
  uint32_t controller_data=get_ps2_controller_state();
    if(BUTTON_X_PRESSED(controller_data) && (player_alive)){
      player_jump();
    }  
}

int apply_subpixel_movement(int *position, int *subpixel, int velocity) {
    // Takes a position pointer, subpixel pointer, and velocity.
    int pixel_movement = 0;

    *subpixel += velocity;  // Accumulates velocity into subpixels.

    // Convert accumulated subpixels into full pixels
    while (riscv_abs_c(*subpixel) >= SUBPIXELS) {  
        if (*subpixel > 0) {      
            pixel_movement++;
            *subpixel -= SUBPIXELS;
        } else {                
            pixel_movement--;
            *subpixel += SUBPIXELS;
        }
    }

    *position += pixel_movement;  // Update main position with pixel movement
    return pixel_movement;        // Return how many pixels were moved
}


// Applies gravity to the player's velocity.
void apply_player_physics() {
  player_y_vel += GRAVITY;  // Apply gravity (Increases downward velocity each frame.)
  // Caps fall speed to `MAX_GRAVITY` to prevent excessive falling speed.
  if (player_y_vel > MAX_GRAVITY) player_y_vel = MAX_GRAVITY;
}

// Makes the player jump.
void player_jump() {
   player_y_vel = JUMP_SPEED;    // Sets `player_y_vel` to a negative value (JUMP_SPEED) to move upward.
}

// Handles vertical movement of the player using subpixels.
void handle_player_movement() {
  int new_y = player_y;
  apply_subpixel_movement(&new_y, &player_y_sub, player_y_vel);
  // Prevent the player from moving outside the screen bounds
  if (new_y < PLAYER_MIN_Y) new_y = PLAYER_MIN_Y + 1;   // Prevent moving too high
  if (new_y > PLAYER_MAX_Y) new_y = PLAYER_MAX_Y - 1; // Prevent moving too low
  player_y = new_y;  // Update player position
}

void main(void)
{
    uint32_t tile_sprite_to_sdram[128];
    uint32_t tile_sprites_ids[128];
    define_tile_addrs(tile_sprite_to_sdram);
    define_tile_sprites_ids(tile_sprites_ids);
    uint32_t tile2_sprite_to_sdram[128];
    uint32_t tile2_sprites_ids[128];
    define_tile2_addrs(tile2_sprite_to_sdram);
    define_tile2_sprites_ids(tile2_sprites_ids);
    int buffer=0;
    int posiy_bird=PLAYER_INIT_Y;
    int addr_buffer[2];
    addr_buffer[0] = 0xa080;//((320*128)+128)
    addr_buffer[1] = 0x18180;//((320*128)+128)+(320*180)
    int posix_tilemap=0;
    int posix_tilemap2=0;
    int tilemap2_half_step=0;
    int32_t time_when_collision_happen=-1;
    clear_collision();
    ppu_finished_all_operations();
    int32_t frames=0;
    initialize_player();
    game_state_t state = GAME_LOGO;
    uint32_t game_over_frames = 0;
    uint32_t previous_x = 0;
    riscplay_audio_init();
    riscplay_audio_all_off();
    while(1) {
        audio_tick_frame();
        uint32_t x_pressed = BUTTON_X_PRESSED(get_ps2_controller_state());
        uint32_t x_just_pressed = x_pressed && !previous_x;
        previous_x = x_pressed;
        if (state != GAME_PLAYING) {
            set_backgound(addr_buffer[buffer], 0x0E);
            if (state == GAME_LOGO) {
                draw_full_screen(addr_buffer[buffer], logo_tiles);
            } else if (state == GAME_MENU) {
                draw_full_screen(addr_buffer[buffer], menu_tiles);
            } else {
                draw_full_screen(addr_buffer[buffer], over_tiles);
                draw_final_score(addr_buffer[buffer]);
            }
            ppu_finished_all_operations();
            set_frame_buffer(addr_buffer[buffer], 0);
            if (state == GAME_LOGO && x_just_pressed) {
                state = GAME_MENU;
                audio_enter_menu();
            } else if (state == GAME_MENU && x_just_pressed) {
                score = 0;
                scored_columns = 0;
                posix_tilemap = 0;
                posix_tilemap2 = 0;
                tilemap2_half_step = 0;
                initialize_player();
                clear_collision();
                audio_start_game();
                state = GAME_PLAYING;
            } else if (state == GAME_OVER) {
                if (x_just_pressed) {
                    score = 0;
                    scored_columns = 0;
                    posix_tilemap = 0;
                    posix_tilemap2 = 0;
                    tilemap2_half_step = 0;
                    initialize_player();
                    clear_collision();
                    audio_start_game();
                    state = GAME_PLAYING;
                } else if (++game_over_frames >= GAME_OVER_TIMEOUT_FRAMES) {
                    state = GAME_LOGO;
                    game_over_frames = 0;
                    riscplay_audio_stop();
                    audio_reset_effects();
                }
            }
            buffer ^= 1;
            continue;
        }
        set_backgound(addr_buffer[buffer],0x0E); 
        // Camada distante: desenhada primeiro, com metade da velocidade.
        print_tilemap(__tilemap2__,tile2_sprite_to_sdram,tile2_sprites_ids,
                      addr_buffer[buffer],posix_tilemap2,10,
                      ___TILEMAP2_SIZEX__,___TILEMAP2_SIZEY__,___TILE2_SIZE__);
        // Camada proxima: velocidade original e prioridade visual.
        print_tilemap(__tilemap__,tile_sprite_to_sdram,tile_sprites_ids,addr_buffer[buffer],posix_tilemap, 10, ___TILEMAP_SIZEX__, ___TILEMAP_SIZEY__, ___TILE_SIZE__ );             
        
        /* Som apenas na borda de pressionamento, evitando retrigger
           continuo enquanto o botao permanece apertado. */
        if (x_just_pressed && player_alive) audio_jump();
        handle_player_input();     // Reads and Process controller input
        update_player();     
        
        load_pallet_from_sdram(bird_adr_spr);
        copy_sprite_from_sdram_to_buffer(bird_adr_spr+0x100,16*12,0x0);
        render_sprite(2,addr_buffer[buffer], 16,12,PLAYER_X,player_y,0,0);        
        draw_score(addr_buffer[buffer]);
        ppu_finished_all_operations();          
        
        
        if(check_collision(2,1)) {
            time_when_collision_happen = get_time();
            audio_collision();
            state = GAME_OVER;
            game_over_frames = 0;
        }
        clear_collision();
        frames++;        
        set_frame_buffer(addr_buffer[buffer],0);
        if (state == GAME_PLAYING) update_score(posix_tilemap, posix_tilemap + 1);
        posix_tilemap=posix_tilemap+1;
        // Um pixel a cada dois quadros: 50% da velocidade da camada 1.
        tilemap2_half_step++;
        if(tilemap2_half_step==2) {
            tilemap2_half_step=0;
            posix_tilemap2++;
        }
        if(posix_tilemap>671) {
            posix_tilemap=0;
            scored_columns=0;
            posix_tilemap2=0;
            tilemap2_half_step=0;
        }
        buffer++;
        if(buffer==2) buffer=0;       
  }
}














