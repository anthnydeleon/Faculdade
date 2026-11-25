#include "console.h"
#include <stdint.h>
#define __MIN_ADDR_IN_SDRAM_TO_LOAD_SPRITES__ 0x26280 //((320*128)+128)+((320*180)*2)
#define MAXITERATE 16 //numero maximo de iteracoes
#define NORM_BITS 13 //fator de normalizacao dos reais para inteiros
#define NORM ((long) 1 << NORM_BITS) //fator de 13 bits de normalizacao

uint32_t tile_sprite_to_sdram[128];
uint32_t tile_sprites_ids[128];
int buffer=0;
uint32_t addr_buffer[2] = {0x00a080, 0x018180};
int32_t frames=0;

int iterate(long real0, long imag0) {
   long realq, imagq, real, imag;
   int i;
   real = real0;
   imag = imag0;
   for (i = 0; i < MAXITERATE; i++) {
     realq = (real * real) >> NORM_BITS; //elevando ao quadrado...
     imagq = (imag * imag) >> NORM_BITS; //...o ultimo elemento.
     if ((realq + imagq) > 4 * NORM)
        break;   //(se o valor exceder o limiar, sair do funcao)
     imag = ((real * imag) >> (NORM_BITS - 1)) + imag0; //adicionando...
     real = realq - imagq + real0; //...seu numero inicial
   }
   return i;
}

// Plano complexo: largura 3.5, altura 1.96875 (proporcao 16:9).
// Centro horizontal em -0.5 e vertical em 0.
// Coordenadas em ponto fixo Q13 para evitar ponto flutuante no RISC-V.
#define REAL_MIN (-18432L)  // -2.25 * 8192
#define REAL_MAX ( 10240L)  //  1.25 * 8192
#define IMAG_MIN ( -8064L)  // -0.984375 * 8192
#define IMAG_MAX (  8064L)  //  0.984375 * 8192
#define BLOCK_SIZE 2

void mand_calc(long realmin, long imagmin, long realmax, long imagmax,
               long hres, long vres)
{
    long x, y;
    // Calcula cada amostra diretamente, evitando acumulacao de arredondamentos.
    for (y = 0; y < vres; ++y) {
        long imag0 = imagmax - (y * (imagmax - imagmin)) / vres;
        for (x = 0; x < hres; ++x) {
            long real0 = realmin + (x * (realmax - realmin)) / hres;
            if (iterate(real0, imag0) >= MAXITERATE) {
                render_sprite(1, addr_buffer[buffer], BLOCK_SIZE, BLOCK_SIZE,
                              (int32_t)(x * BLOCK_SIZE),
                              (int32_t)(y * BLOCK_SIZE), 0, 0);
            }
        }
    }
}

void main(void)
{
    const int width = 320, height = 180;
    ppu_finished_all_operations();
    load_pallet_from_sdram(0xa0000);
    set_backgound(addr_buffer[buffer], 0x0E);

    // O sprite 2x2 e carregado uma unica vez no buffer da PPU.
    copy_sprite_from_sdram_to_buffer(0xa0100, BLOCK_SIZE * BLOCK_SIZE, 0x0);
    // Select a fully cleared framebuffer before the long software calculation.
    // Otherwise scanout continues at SDRAM address zero until completion.
    ppu_finished_all_operations();
    set_frame_buffer(addr_buffer[buffer], 0);

    mand_calc(REAL_MIN, IMAG_MIN, REAL_MAX, IMAG_MAX,
              width / BLOCK_SIZE, height / BLOCK_SIZE);

    ppu_finished_all_operations();
    frames++;
    set_frame_buffer(addr_buffer[buffer], 0);
    buffer = (buffer + 1) % 2;
    while (1) { }
}
