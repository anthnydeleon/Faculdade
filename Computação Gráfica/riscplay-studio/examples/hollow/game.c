#include "game.h"
#include "generated/world_data.h"

const RoomInfo rooms[ROOM_COUNT] = {
    {0,0,"DIRTMOUTH"}, {1,0,"ESTRADA"}, {2,0,"ESPINHOS"},
    {3,0,"PASSAGEM"}, {4,0,"POCO"}, {2,1,"REFUGIO"},
    {3,1,"ABISMO"}, {4,1,"ENCRUZILHADA"}, {5,1,"GALERIA"},
    {6,1,"CAVERNAS"}, {7,1,"TRAVESSIA"}, {8,1,"SANTUARIO"},
    {2,2,"HORNET"}
};

static int abs_i(int x) { return x<0?-x:x; }
static int clamp(int x,int lo,int hi) { return x<lo?lo:(x>hi?hi:x); }
static int overlaps(Rect a,Rect b) {
    return a.x<b.x+b.w && a.x+a.w>b.x && a.y<b.y+b.h && a.y+a.h>b.y;
}

int game_room_at(int x,int y) {
    int i;
    for (i=0;i<ROOM_COUNT;i++) if (rooms[i].x==x && rooms[i].y==y) return i;
    return -1;
}

int game_tile(int room,int tx,int ty) {
    int p;
    if (room<0 || room>=ROOM_COUNT || tx<0 || tx>=32 || ty<0 || ty>=30) return TILE_EMPTY;
    p=ty*32+tx;
    return (world_collision[room][p>>2]>>((p&3)*2))&3;
}

/* The narrow collision box preserves the original Knight's ledge clearance. */
static int body_has(int room,int x,int y,int kind) {
    int tx,ty,left=x+5,right=x+11,top=y+4,bottom=y+15;
    if (right<0 || left>=256 || bottom<0 || top>=240) return 0;
    left=clamp(left,0,255)>>3; right=clamp(right,0,255)>>3;
    top=clamp(top,0,239)>>3; bottom=clamp(bottom,0,239)>>3;
    for (ty=top;ty<=bottom;ty++)
        for (tx=left;tx<=right;tx++)
            if (game_tile(room,tx,ty)==kind) return 1;
    return 0;
}

static int support_has(int room,int x,int y,int kind) {
    int tx,left=x+5,right=x+11,ty=y+16;
    if (right<0 || left>=256 || ty<0 || ty>=240) return 0;
    left=clamp(left,0,255)>>3; right=clamp(right,0,255)>>3; ty>>=3;
    for (tx=left;tx<=right;tx++) if (game_tile(room,tx,ty)==kind) return 1;
    return 0;
}

static void safe_position(Game *g) {
    int x,y,best=0x7fffffff,bx=g->x,by=g->y;
    if (support_has(g->room,g->x,g->y,TILE_SOLID) &&
        !support_has(g->room,g->x,g->y,TILE_SPIKE) &&
        !body_has(g->room,g->x,g->y,TILE_SOLID) &&
        !body_has(g->room,g->x,g->y,TILE_SPIKE)) {
        g->safe_x=g->x; g->safe_y=g->y;
        return;
    }
    /* A reachable floor within the current room, even if the entry was airborne. */
    for (y=0;y<=216;y+=8) for (x=1;x<=239;x+=4) {
        int d=abs_i(x-g->x)+abs_i(y-g->y);
        if (d<best && support_has(g->room,x,y,TILE_SOLID) &&
            !support_has(g->room,x,y,TILE_SPIKE) &&
            !body_has(g->room,x,y,TILE_SOLID) && !body_has(g->room,x,y,TILE_SPIKE)) {
            bx=x; by=y; best=d;
        }
    }
    g->safe_x=bx; g->safe_y=by;
}

void game_init(Game *g) {
    Game clean={0};
    *g=clean;
    g->x=16; g->y=2; g->hp=MAX_HP; g->facing=1;
    g->mode=GAME_MENU;
    g->safe_x=16; g->safe_y=176;
    g->enemies[0].x=120; g->enemies[0].y=183; g->enemies[0].room=1;
    g->enemies[1].x=120; g->enemies[1].y=183; g->enemies[1].room=8;
    g->enemies[0].dir=g->enemies[1].dir=1;
    g->enemies[0].hp=g->enemies[1].hp=60;
}

int game_attack_rect(const Game *g,Rect *r) {
    if (!g || !r || g->mode!=GAME_PLAY || g->attack_timer<=0 || g->attack_timer>12) return 0;
    r->x=g->x; r->y=g->y; r->w=r->h=16;
    switch (g->attack_dir) {
    case ATTACK_LEFT: r->x-=13; break;
    case ATTACK_UP: r->y-=16; break;
    case ATTACK_DOWN: r->y+=16; break;
    default: r->x+=13; break;
    }
    return 1;
}

static void damage(Game *g,int spike) {
    if (g->invuln) return;
    g->hp--; g->events|=EVENT_DAMAGE; g->invuln=60;
    g->heal_timer=g->sitting=g->attack_timer=0;
    if (g->hp<=0) {
        g->hp=0; g->mode=GAME_DEAD; g->events|=EVENT_DEATH;
        return;
    }
    if (spike) {
        g->x=g->safe_x; g->y=g->safe_y;
        g->vx=g->vy=g->subx=g->suby=g->jump_hold=0;
        g->grounded=support_has(g->room,g->x,g->y,TILE_SOLID);
    }
}

static void transition(Game *g) {
    int rx=rooms[g->room].x,ry=rooms[g->room].y,n=-1;
    int nx=g->x,ny=g->y,up=0;
    if (g->x<=0) { n=game_room_at(rx-1,ry); nx=239; }
    else if (g->x>=240) { n=game_room_at(rx+1,ry); nx=1; }
    else if (g->y<=0) { n=game_room_at(rx,ry-1); ny=214; up=1; }
    else if (g->y>=224) { n=game_room_at(rx,ry+1); ny=4; }
    if (n>=0 && !body_has(n,nx,ny,TILE_SOLID)) {
        g->room=n; g->x=nx; g->y=ny; g->subx=g->suby=0;
        g->grounded=g->sitting=g->dialogue=g->interaction=0;
        if (up) { g->vy=-96; g->jump_hold=40; }
        safe_position(g);
        g->events|=EVENT_ROOM;
    } else {
        if (g->x<1) { g->x=1; g->vx=g->subx=0; }
        if (g->x>239) { g->x=239; g->vx=g->subx=0; }
        if (g->y<1) { g->y=1; if (g->vy<0) g->vy=g->suby=0; }
        if (g->y>223) { g->y=223; if (g->vy>0) g->vy=g->suby=0; }
    }
}

static void move_player(Game *g) {
    int dx,dy,s;
    g->subx+=g->vx; dx=g->subx/16; g->subx-=dx*16;
    g->suby+=g->vy; dy=g->suby/16; g->suby-=dy*16;
    s=dx<0?-1:1;
    while (dx) {
        if (body_has(g->room,g->x+s,g->y,TILE_SOLID)) { g->vx=g->subx=0; break; }
        g->x+=s; dx-=s;
    }
    s=dy<0?-1:1;
    g->grounded=0;
    while (dy) {
        if (body_has(g->room,g->x,g->y+s,TILE_SOLID)) {
            if (s>0) { g->grounded=1; g->jump_cooldown=8; }
            g->vy=g->suby=0; break;
        }
        g->y+=s; dy-=s;
    }
    if (g->vy>=0 && support_has(g->room,g->x,g->y,TILE_SOLID)) {
        g->grounded=1; g->vy=g->suby=0;
    }
    transition(g);
    if (body_has(g->room,g->x,g->y,TILE_SPIKE) || support_has(g->room,g->x,g->y,TILE_SPIKE)) damage(g,1);
    else if (g->grounded && !g->invuln) { g->safe_x=g->x; g->safe_y=g->y; }
}

static void update_interaction(Game *g) {
    Rect p={g->x,g->y,16,16};
    Rect elder={72,167,16,24},hornet={102,167,16,24};
    g->interaction=INTERACT_NONE;
    if (!g->grounded) return;
    if (g->room==0 && overlaps(p,elder)) g->interaction=INTERACT_ELDERBUG;
    else if (g->room==12 && overlaps(p,hornet)) g->interaction=INTERACT_HORNET;
    else if (body_has(g->room,g->x,g->y,TILE_BENCH)) g->interaction=INTERACT_BENCH;
}

static void update_enemies(Game *g) {
    int i;
    Rect strike;
    int active=game_attack_rect(g,&strike);
    for (i=0;i<2;i++) {
        Enemy *e=&g->enemies[i];
        Rect er;
        Rect player={g->x+2,g->y,6,16};
        if (e->hp<=0 || e->room!=g->room) continue;
        if (e->stun) e->stun--;
        else {
            e->x+=e->dir;
            if (e->x<=0) { e->x=0; e->dir=1; }
            if (e->x>=240) { e->x=240; e->dir=-1; }
            e->anim++;
        }
        er.x=e->x; er.y=e->y; er.w=16; er.h=8;
        if (active && !(g->attack_hit_mask&(1<<i)) && overlaps(strike,er)) {
            e->hp-=20; e->stun=30; g->attack_hit_mask|=1<<i;
            g->soul=clamp(g->soul+10,0,MAX_SOUL); g->events|=EVENT_HIT;
        }
        er.w=14; er.h=7;
        if (e->hp>0 && overlaps(player,er)) damage(g,0);
    }
}

static void update_pose(Game *g) {
    if (g->sitting) g->pose=POSE_SIT;
    else if (g->heal_timer) g->pose=POSE_HEAL;
    else if (g->attack_timer) g->pose=POSE_ATTACK;
    else if (!g->grounded) g->pose=g->vy<0?POSE_JUMP:POSE_FALL;
    else g->pose=g->vx?POSE_RUN:POSE_IDLE;
}

void game_tick(Game *g,uint16_t input) {
    uint16_t pressed=(uint16_t)(input&~g->previous_input);
    g->previous_input=input; g->frame++; g->events=0;
    if (g->mode==GAME_MENU) {
        if (pressed&(INPUT_START|INPUT_JUMP)) {
            game_init(g); g->mode=GAME_PLAY; g->previous_input=input;
        }
        return;
    }
    if (g->mode==GAME_DEAD) {
        if (pressed&(INPUT_START|INPUT_JUMP)) { game_init(g); g->previous_input=input; }
        return;
    }
    if (pressed&INPUT_START) g->paused=!g->paused;
    if (g->paused) return;
    if (g->invuln) g->invuln--;
    if (g->attack_cooldown) g->attack_cooldown--;
    if (g->attack_timer) g->attack_timer--;
    if (g->jump_cooldown) g->jump_cooldown--;
    if (g->heal_timer) g->heal_timer--;
    if (g->dialogue) {
        if (pressed&INPUT_JUMP) {
            g->dialogue_line++;
            if (g->dialogue_line>=(g->dialogue==1?9:5)) {
                if (g->dialogue==2) {
                    /* The original last room has no climbable exit. Closing
                       its final conversation completes this short journey. */
                    game_init(g);
                    g->previous_input=input;
                    return;
                }
                g->dialogue=0; g->dialogue_line=0;
            }
            g->events|=EVENT_INTERACT;
        }
        return;
    }
    if (g->sitting) {
        if (pressed&INPUT_JUMP) { g->sitting=0; g->jump_cooldown=8; }
        update_pose(g);
        return;
    }
    if (g->heal_timer) {
        g->vx=0;
    } else {
        if (input&INPUT_LEFT) { g->vx=clamp(g->vx-4,-32,32); g->facing=-1; }
        else if (input&INPUT_RIGHT) { g->vx=clamp(g->vx+4,-32,32); g->facing=1; }
        else if (g->vx>0) g->vx=clamp(g->vx-3,0,32);
        else if (g->vx<0) g->vx=clamp(g->vx+3,-32,0);
        if ((input&INPUT_JUMP) && g->grounded && !g->jump_cooldown) {
            g->vy=-96; g->grounded=0; g->jump_hold=40;
            g->events|=EVENT_JUMP;
        }
        if ((input&INPUT_ATTACK) && !g->attack_timer && !g->attack_cooldown) {
            g->attack_timer=24; g->attack_cooldown=36; g->attack_hit_mask=0;
            if (input&INPUT_UP) g->attack_dir=ATTACK_UP;
            else if ((input&INPUT_DOWN) && !g->grounded) g->attack_dir=ATTACK_DOWN;
            else g->attack_dir=g->facing<0?ATTACK_LEFT:ATTACK_RIGHT;
            g->events|=EVENT_ATTACK;
        }
        update_interaction(g);
        if ((pressed&INPUT_UP) && g->grounded && !g->vx && !g->attack_timer && g->interaction) {
            g->events|=EVENT_INTERACT;
            if (g->interaction==INTERACT_BENCH) {
                g->sitting=1; g->hp=MAX_HP;
            } else {
                g->dialogue=g->interaction==INTERACT_ELDERBUG?1:2;
                g->dialogue_line=0;
            }
            update_pose(g); return;
        }
        if ((input&INPUT_DOWN) && g->grounded && !g->vx && !g->attack_timer && g->hp<MAX_HP && g->soul>=30) {
            g->hp++; g->soul-=30; g->heal_timer=84;
            g->events|=EVENT_HEAL;
        }
    }
    if (!(input&INPUT_JUMP)) g->jump_hold=0;
    else if (g->jump_hold) g->jump_hold--;
    if (!g->grounded) g->vy=clamp(g->vy+(g->jump_hold?4:10),-96,96);
    else if (!support_has(g->room,g->x,g->y,TILE_SOLID)) { g->grounded=0; g->vy=10; }
    move_player(g);
    if (g->mode==GAME_PLAY) update_enemies(g);
    update_interaction(g);
    update_pose(g);
}
