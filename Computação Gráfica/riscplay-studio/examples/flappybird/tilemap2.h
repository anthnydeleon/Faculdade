#ifndef  ___TILEMAP2___
#define  ___TILEMAP2___
#include <stdint.h>
#define  ___TILE2_SIZE__ 28
#define  ___TILEMAP2_SIZEX__ 37
#define  ___TILEMAP2_SIZEX_PLUS_1__ ___TILEMAP2_SIZEX__+1
#define  ___TILEMAP2_SIZEY__ 7

static const char *__tilemap2__[] = {
  "                                     ",
  "                                     ",
  "                                     ",
  "                                     ",
  "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
  "ccccccccccccccccccccccccccccccccccccc",
  "                                     "
};

void define_tile2_addrs(uint32_t *tile_sprite_to_sdram){
  for(int i=0;i<128;i++){
    tile_sprite_to_sdram[i]=UINT32_MAX;
  }
  tile_sprite_to_sdram['b']=b_adr_spr+0x100;
  tile_sprite_to_sdram['c']=c_adr_spr+0x100;  
 

}
void define_tile2_sprites_ids(uint32_t *tile_sprits_ids){
  for(int i=0;i<128;i++){
    tile_sprits_ids[i]=0;
  }


}
#endif
