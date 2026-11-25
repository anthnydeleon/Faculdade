/* Native Riscplay backend. Command layout follows first_game/console.h.
 * SDRAM addresses in PPU commands are 32-bit WORD addresses, not CPU bytes.
 */
#include "platform.h"
#include "generated/assets.h"
#include <stddef.h>

#define SCREEN_W 320
#define SCREEN_H 180

/* Low-address peripherals are real Riscplay registers. Explicit loads/stores
 * avoid treating these addresses as objects in the C null-pointer page. */
static uint32_t mmio_read(uint32_t address)
{
    uint32_t value;
    __asm__ volatile("lw %0, 0(%1)" : "=r"(value) : "r"(address) : "memory");
    return value;
}

static void mmio_write(uint32_t address, uint32_t value)
{
    __asm__ volatile("sw %1, 0(%0)" : : "r"(address), "r"(value) : "memory");
}

static const uint32_t framebuffers[2] = {0xA080u, 0x18180u};
static unsigned backbuffer;
static unsigned cached_asset;
static unsigned tone_left;

static void ppu_command(uint32_t rs1, uint32_t rs2)
{
    __asm__ volatile(".insn r 0x2B, 0x0, 0x00, x0, %0, %1"
                     : : "r"(rs1), "r"(rs2) : "memory");
}

static void ppu_finish(void)
{
    ppu_command(0x50000000u, 0);
    __asm__ volatile(".insn r 0x2B, 0x1, 0x00, x0, x0, x0"
                     : : : "memory");
}

static void ppu_select(uint32_t framebuffer)
{
    uint32_t offset = 0;
    __asm__ volatile(".insn r 0x2B, 0x0, 0x01, x0, %0, %1"
                     : : "r"(framebuffer), "r"(offset) : "memory");
}

void platform_init(void)
{
    backbuffer = 0;
    cached_asset = (unsigned)-1;
    tone_left = 0;
    mmio_write(0x200u, 0);       /* Silence all audio channels. */
    mmio_write(0x240u, 0);
    mmio_write(0x244u, 0);       /* Simple sustained pulse, without ADSR. */
    mmio_write(0x214u, 9);
    mmio_write(0x220u, 1);
    ppu_command(0x40000000u, 0);
    ppu_command(0x30000000u | (framebuffers[0] << 7), 0xFF000000u);
    ppu_command(0x30000000u | (framebuffers[1] << 7), 0xFF000000u);
    ppu_finish();
    ppu_select(framebuffers[1]);
}

uint16_t platform_input(void)
{
    return (uint16_t)((~mmio_read(0xCu) >> 16) & 0xFFFFu);
}

void platform_begin(void)
{
    /* Dark blue-black ARGB; no NES palette conversion is needed here. */
    ppu_command(0x30000000u | (framebuffers[backbuffer] << 7), 0xFF080C18u);
    cached_asset = (unsigned)-1;
}

void platform_draw(unsigned asset_id, int x, int y, int flipx, int flipy)
{
    const Asset *a;
    uint32_t dest, rs1, rs2;
    if (asset_id >= ASSET_COUNT) return;
    a = &assets[asset_id];
    if (!a->width || !a->height || a->width > 127 || a->height > 127) return;
    if (x >= SCREEN_W || y >= SCREEN_H || x + a->width <= 0 || y + a->height <= 0) return;
    if (cached_asset != asset_id) {
        ppu_command(0x10000000u | (a->palette << 7), 0);
        ppu_command(a->pixels << 7, ((uint32_t)a->width * a->height) << 2);
        cached_asset = asset_id;
    }
    /* Signed coordinates and origin formula match the supplied clipping API. */
    dest = (uint32_t)((int32_t)framebuffers[backbuffer] + y * SCREEN_W + x);
    rs1 = 0x20000000u | (dest << 7) | (2u << 3) | ((a->width & 0x70u) >> 4);
    rs2 = ((uint32_t)(a->width & 15u) << 28)
        | ((uint32_t)a->height << 21)
        | (((uint32_t)x & 0x3FFu) << 11)
        | (((uint32_t)y & 0x1FFu) << 2)
        | ((uint32_t)!!flipx << 1) | (uint32_t)!!flipy;
    ppu_command(rs1, rs2);
}

void platform_present(void)
{
    ppu_finish();
    ppu_select(framebuffers[backbuffer]);
    backbuffer ^= 1u;
}

void platform_tone(unsigned hz, unsigned frames)
{
    if (!hz || !frames) {
        tone_left = 0;
        mmio_write(0x200u, 0);
        mmio_write(0x240u, 0);
        return;
    }
    if (hz > 20000u) hz = 20000u;
    mmio_write(0x240u, 0);
    mmio_write(0x208u, 37125000u / hz);
    mmio_write(0x200u, 2);
    mmio_write(0x240u, 2);
    tone_left = frames;
}

void platform_audio_tick(void)
{
    if (tone_left && --tone_left == 0) {
        mmio_write(0x240u, 0);
        mmio_write(0x200u, 0);
    }
}

/* Freestanding compiler support: there is no libc or operating system. */
void *memset(void *dest, int value, size_t n)
{
    unsigned char *p = dest;
    while (n--) *p++ = (unsigned char)value;
    return dest;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) *d++ = *s++;
    return dest;
}
