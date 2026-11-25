"""Port em Python de dist/core.js (CPU RV32I + PPU do Riscplay) para testes sem navegador.
Os comandos da PPU executam na hora (sem modelo de tempo): serve para checar
logica, desenho e falhas, nao desempenho. Audio: so registra as escritas na APU."""
import json, sys
from PIL import Image

W, H = 320, 180
M32 = 0xFFFFFFFF


def sx(x, n):
    x &= (1 << n) - 1
    return x - (1 << n) if x >> (n - 1) else x


def s32(x):
    x &= M32
    return x - (1 << 32) if x & 0x80000000 else x


def swap32(x):
    return int.from_bytes((x & M32).to_bytes(4, 'little'), 'big')


class Emu:
    def __init__(self, package):
        fw = next(e for e in package if int(e['type']) == 0)
        self.ram = bytearray(32768)
        code = bytes.fromhex(fw['value'])
        self.ram[:len(code)] = code
        self.sdram = [0] * 0x200000
        for e in package:
            if int(e['type']) == 1:
                a = int(e['addr'], 16)
                v = json.loads(e['value']) if isinstance(e['value'], str) else e['value']
                self.sdram[a:a + len(v)] = v
        self.sprite = bytearray(16384)
        self.palette = [0] * 256
        self.r = [0] * 32
        self.pc = 0x80000000
        self.buttons = 0
        self.collision = 0
        self.fb = 0
        self.frames = 0
        self.instr = 0
        self.sprite_len = 0
        self.audio_writes = []
        self.ppu_cmds = 0

    def rw(self, a):
        a &= M32
        if a >= 0x80000000:
            o = a & 0x7ffc
            return int.from_bytes(self.ram[o:o + 4], 'little')
        if 0x10000000 <= a < 0x18000000:
            return swap32(self.collision)
        if 0x08000000 <= a < 0x10000000:
            return self.sprite[(a >> 2) & 16383] << 24
        if a == 0:
            return swap32(self.instr // 1000)
        if a == 0x0c:
            return ((~self.buttons << 16) | 0xffff) & M32
        return 0

    def ww(self, a, v):
        a &= M32
        v &= M32
        if a >= 0x80000000:
            o = a & 0x7ffc
            self.ram[o:o + 4] = v.to_bytes(4, 'little')
        elif 0x08000000 <= a < 0x10000000:
            self.sprite[(a >> 2) & 16383] = v & 255
        elif 0x200 <= a <= 0x244:
            self.audio_writes.append((self.frames, a, v))

    def ppu(self, a, b):
        self.ppu_cmds += 1
        t = a >> 28
        base = (a >> 7) & 0x1fffff
        sd = self.sdram
        if t == 0:
            n = (b >> 2) & 65535
            dest = (b >> 18) & 16383
            self.sprite_len = n
            for j in range(n):
                self.sprite[(dest + j) & 16383] = (sd[(base + (j >> 2)) & 0x1fffff] >> (24 - 8 * (j & 3))) & 255
        elif t == 1:
            for j in range(256):
                self.palette[j] = sd[(base + j) & 0x1fffff]
        elif t == 2:
            w = ((a & 7) << 4) | (b >> 28)
            h = (b >> 21) & 127
            sid = (a >> 3) & 15
            if not w or not h or not self.sprite_len:
                return
            x = sx((b >> 11) & 1023, 10)
            y = sx((b >> 2) & 511, 9)
            ix = bool(b & 2)
            iy = bool(b & 1)
            p = w * h - w if iy else (w - 1 if ix else 0)
            for j in range(self.sprite_len):
                col = j % w
                row = j // w
                xx = sx((x + col) & 1023, 10)
                yy = sx((y + row) & 511, 9)
                adr = (base + row * 320 + col) & 0x1fffff
                old = sd[adr]
                idx = self.sprite[p & 16383]
                if 0 <= xx < W and 0 <= yy < H:
                    oldid = (old >> 24) & 15
                    if (old >> 24) != 255 and oldid != sid:
                        self.collision |= 1 << (((sid & 3) * 4 + (oldid & 3)) + (16 if idx == 255 else 0))
                        self.collision |= 1 << (((oldid & 3) * 4 + (sid & 3)) + (16 if old & 0x10000000 else 0))
                    sd[adr] = ((0x90000000 if idx == 255 else 0x80000000) | (sid << 24) |
                               ((old & 0xffffff) if idx == 255 else (self.palette[idx] & 0xffffff))) & M32
                if col + 1 < w or (not ix and not iy):
                    p += -1 if ix else 1
                elif iy:
                    p += 1 - 2 * w
                elif ix:
                    p = j
        elif t == 3:
            for j in range(57600):
                sd[(base + j) & 0x1fffff] = b
        elif t == 4:
            self.collision = 0

    def step(self):
        pc = self.pc
        i = self.rw(pc)
        op = i & 127
        rd = (i >> 7) & 31
        f3 = (i >> 12) & 7
        r = self.r
        a = r[(i >> 15) & 31]
        b = r[(i >> 20) & 31]
        s2 = (i >> 20) & 31
        f7 = i >> 25
        imm = sx(i >> 20, 12)
        nxt = (pc + 4) & M32
        v = 0
        wr = True
        if op == 0x37:
            v = s32(i & 0xfffff000)
        elif op == 0x17:
            v = s32(pc + (i & 0xfffff000))
        elif op == 0x6f:
            v = s32(nxt)
            off = sx(((i >> 31) << 20) | (((i >> 12) & 255) << 12) | (((i >> 20) & 1) << 11) | (((i >> 21) & 1023) << 1), 21)
            nxt = (pc + off) & M32
        elif op == 0x67:
            v = s32(nxt)
            nxt = ((a + imm) & ~1) & M32
        elif op == 0x63:
            wr = False
            ua, ub = a & M32, b & M32
            take = [a == b, a != b, None, None, a < b, a >= b, ua < ub, ua >= ub][f3]
            if take is None:
                raise RuntimeError('bad branch')
            if take:
                off = sx(((i >> 31) << 12) | (((i >> 7) & 1) << 11) | (((i >> 25) & 63) << 5) | (((i >> 8) & 15) << 1), 13)
                nxt = (pc + off) & M32
        elif op == 0x13:
            if f3 == 0: v = a + imm
            elif f3 == 2: v = int(a < imm)
            elif f3 == 3: v = int((a & M32) < (imm & M32))
            elif f3 == 4: v = a ^ imm
            elif f3 == 6: v = a | imm
            elif f3 == 7: v = a & imm
            elif f3 == 1: v = a << s2
            elif f3 == 5: v = (a >> s2) if f7 == 32 else ((a & M32) >> s2)
            v = s32(v)
        elif op == 0x33:
            if f7 not in (0, 32):
                raise RuntimeError('M extension used at %x' % pc)
            sh = b & 31
            if f3 == 0: v = a - b if f7 == 32 else a + b
            elif f3 == 1: v = a << sh
            elif f3 == 2: v = int(a < b)
            elif f3 == 3: v = int((a & M32) < (b & M32))
            elif f3 == 4: v = a ^ b
            elif f3 == 5: v = (a >> sh) if f7 == 32 else ((a & M32) >> sh)
            elif f3 == 6: v = a | b
            else: v = a & b
            v = s32(v)
        elif op == 0x03:
            addr = (a + imm) & M32
            size = 1 << (f3 & 3)
            if addr & (size - 1):
                raise RuntimeError('misaligned load %x' % pc)
            word = self.rw(addr & ~3) >> ((addr & 3) * 8)
            if f3 == 0: v = sx(word & 255, 8)
            elif f3 == 1: v = sx(word & 65535, 16)
            elif f3 == 4: v = word & 255
            elif f3 == 5: v = word & 65535
            else: v = s32(word)
        elif op == 0x23:
            wr = False
            addr = (a + sx(((i >> 25) << 5) | rd, 12)) & M32
            size = 1 << f3
            if addr & (size - 1):
                raise RuntimeError('misaligned store %x' % pc)
            shift = (addr & 3) * 8
            mask = [255, 65535, M32][f3]
            val = b if f3 == 2 else (self.rw(addr & ~3) & ~(mask << shift)) | ((b & mask) << shift)
            self.ww(addr & ~3, val)
        elif op == 0x2b:
            wr = False
            if f7 == 1 and f3 == 0:
                self.fb = a & 0x1fffff
                self.frames += 1
            elif f7 == 0 and f3 == 0:
                self.ppu(a & M32, b & M32)
            elif f7 == 0 and f3 in (1, 2):
                pass
            else:
                raise RuntimeError('bad ppu instr')
        elif op == 0x0f:
            wr = False
        else:
            raise RuntimeError('illegal instruction %08x at %08x' % (i, pc))
        if wr and rd:
            r[rd] = v
        self.pc = nxt
        self.instr += 1

    def run_frame(self, limit=3_000_000):
        f = self.frames
        n = 0
        while self.frames == f:
            self.step()
            n += 1
            if n > limit:
                raise RuntimeError('frame did not finish')
        return n

    def image(self):
        im = Image.new('RGB', (W, H))
        px = []
        for j in range(W * H):
            p = self.sdram[(self.fb + j) & 0x1fffff]
            px.append(((p >> 16) & 255, (p >> 8) & 255, p & 255))
        im.putdata(px)
        return im


BUTTONS = {'up': 4, 'right': 5, 'down': 6, 'left': 7, 'x': 14, 'circle': 13, 'square': 15, 'triangle': 12}


def main():
    import argparse, os
    ap = argparse.ArgumentParser(description='Roda um jogo Riscplay sem navegador e salva telas em PNG.')
    ap.add_argument('package', help='dist/examples/<jogo>.json ou um .json.prog')
    ap.add_argument('--frames', type=int, default=300)
    ap.add_argument('--press', default='30:x', help='quadro:botao[,quadro:botao...] (segura 3 quadros)')
    ap.add_argument('--shot', default='', help='quadros a salvar, ex.: 10,100,250')
    ap.add_argument('--out', default='.', help='pasta das imagens')
    a = ap.parse_args()
    data = json.load(open(a.package, encoding='utf-8'))
    e = Emu(data['package'] if isinstance(data, dict) else data)
    presses = {}
    for item in filter(None, a.press.split(',')):
        f, b = item.split(':')
        for k in range(3):
            presses[int(f) + k] = presses.get(int(f) + k, 0) | (1 << BUTTONS[b])
    shots = {int(s) for s in a.shot.split(',') if s}
    worst = 0
    for f in range(a.frames):
        e.buttons = presses.get(f, 0)
        worst = max(worst, e.run_frame())
        if f in shots:
            path = os.path.join(a.out, 'frame_%d.png' % f)
            e.image().resize((640, 360), Image.NEAREST).save(path)
            print('salvo', path)
    print('quadros', e.frames, '| instrucoes', e.instr, '| pior quadro', worst,
          'instr | comandos PPU', e.ppu_cmds, '| escritas APU', len(e.audio_writes))


if __name__ == '__main__':
    main()
