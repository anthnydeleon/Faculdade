#ifndef  ___TILEMAP___
#define  ___TILEMAP___
#include <stdint.h>
#define  ___TILE_SIZE__ 28
#define  ___TILEMAP_SIZEX__ 37
#define  ___TILEMAP_SIZEX_PLUS_1__ ___TILEMAP_SIZEX__+1
#define  ___TILEMAP_SIZEY__ 7

static const char *__tilemap__[] = {
  "         g       g     e         g   ",
  "         e       g               e   ",
  "                 e                   ",
  "                       f             ",
  "         f             g         f   ",
  "         g       f     g         g   ",
  "ddddddddddddddddddddddddddddddddddddd"
};

void define_tile_addrs(uint32_t *tile_sprite_to_sdram){
  for(int i=0;i<128;i++){
    tile_sprite_to_sdram[i]=UINT32_MAX;
  }

  tile_sprite_to_sdram['d']=d_adr_spr+0x100; 
  tile_sprite_to_sdram['e']=e_adr_spr+0x100; 
  tile_sprite_to_sdram['f']=f_adr_spr+0x100; 
  tile_sprite_to_sdram['g']=g_adr_spr+0x100;  


}
void define_tile_sprites_ids(uint32_t *tile_sprits_ids){
  for(int i=0;i<128;i++){
    tile_sprits_ids[i]=0;
  }
  
  tile_sprits_ids['e']=1;
  tile_sprits_ids['f']=1;
  tile_sprits_ids['g']=1;

}
#endif
