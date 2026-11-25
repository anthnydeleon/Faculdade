#ifndef HOLLOW_RISCPLAY_GAME_H
#define HOLLOW_RISCPLAY_GAME_H

#include <stdint.h>

enum { INPUT_LEFT=1, INPUT_RIGHT=2, INPUT_UP=4, INPUT_DOWN=8,
       INPUT_JUMP=16, INPUT_ATTACK=32, INPUT_START=64 };
enum { GAME_MENU, GAME_PLAY, GAME_DEAD };
enum { POSE_IDLE, POSE_RUN, POSE_JUMP, POSE_FALL, POSE_ATTACK,
       POSE_HEAL, POSE_SIT };
enum { ATTACK_RIGHT, ATTACK_LEFT, ATTACK_UP, ATTACK_DOWN };
enum { TILE_EMPTY, TILE_SOLID, TILE_SPIKE, TILE_BENCH };
enum { INTERACT_NONE, INTERACT_BENCH, INTERACT_ELDERBUG, INTERACT_HORNET };
enum { ROOM_COUNT=13, WORLD_WIDTH=256, WORLD_HEIGHT=240, MAX_HP=3, MAX_SOUL=60 };

typedef struct { int x,y,w,h; } Rect;
typedef struct { uint8_t x,y; const char *name; } RoomInfo;
typedef struct {
    int x,y,dir,hp,stun;
    int room,anim;
} Enemy;

typedef struct {
    /* Top-left of a 16x16 metasprite. Velocities/remainders use 1/16 pixel. */
    int x,y,vx,vy,subx,suby;
    int room,hp,soul,facing,grounded;
    uint32_t frame;
    int mode,pose,attack_dir;
    int attack_timer,attack_cooldown,invuln,heal_timer;
    int sitting,dialogue,dialogue_line,paused,interaction;
    uint16_t previous_input;
    Enemy enemies[2];
    int jump_hold,jump_cooldown,attack_hit_mask;
    int safe_x,safe_y;
    /* One-frame events for optional platform sound effects. */
    unsigned events;
} Game;

enum { EVENT_JUMP=1, EVENT_ATTACK=2, EVENT_HIT=4, EVENT_DAMAGE=8,
       EVENT_HEAL=16, EVENT_INTERACT=32, EVENT_ROOM=64, EVENT_DEATH=128 };

extern const RoomInfo rooms[ROOM_COUNT];
void game_init(Game *game);
void game_tick(Game *game,uint16_t input);
/* Tile coordinates are 0..31,0..29; invalid coordinates return TILE_EMPTY. */
int game_tile(int room,int tx,int ty);
int game_room_at(int x,int y);
/* Returns 1 during the damaging portion of the current swing, otherwise 0. */
int game_attack_rect(const Game *game,Rect *rect);

#endif
