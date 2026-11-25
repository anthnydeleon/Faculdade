"""Gera as imagens PNG do Dino a partir de desenhos em texto.

Cada caractere vira um pixel:
  '.' ou ' '  transparente
  '#'         cinza escuro (cor principal do jogo)
  'w'         branco (olho, preenchimento da nuvem)
  'l'         cinza claro (contorno da nuvem)

Uso:  python examples/dino/tools/make_assets.py [pasta_saida]
"""
import sys
from pathlib import Path
from PIL import Image

COLORS = {
    '#': (83, 83, 83, 255),
    'w': (247, 247, 247, 255),
    'l': (200, 200, 200, 255),
}


def sprite(rows, width=None):
    width = width or max(len(r) for r in rows)
    im = Image.new('RGBA', (width, len(rows)), (0, 0, 0, 0))
    for y, row in enumerate(rows):
        for x, ch in enumerate(row.ljust(width, '.')):
            if ch in COLORS:
                im.putpixel((x, y), COLORS[ch])
    return im


# ---------------------------------------------------------------- dinossauro
# 22 x 23. Cabeca, corpo e cauda sao iguais em todos os quadros;
# so as pernas (ultimas linhas) mudam.
DINO_TOP = [
    "            ########",
    "           ##########",
    "           #w########",
    "           ##########",
    "           ##########",
    "           ##########",
    "           #####",
    "           ########",
    "#         #####",
    "#        ######",
    "##      #######",
    "###    ##########",
    "####  ######### #",
    "###############",
    " #############",
    "  ############",
    "   ##########",
    "    ########",
]
DINO_DEAD_TOP = [r.replace('w', '.') for r in DINO_TOP]
DINO_DEAD_TOP[1] = "           #...######"
DINO_DEAD_TOP[2] = "           ##.#######"
DINO_DEAD_TOP[3] = "           #...######"
LEGS_STAND = [
    "     ###  ##",
    "     ##    #",
    "     #     #",
    "     #     #",
    "     ##    ##",
]
LEGS_RUN1 = [
    "     ###  ##",
    "     ##    #",
    "     #     ##",
    "     ##",
    "",
]
LEGS_RUN2 = [
    "     ###  ##",
    "     ###   #",
    "      ##   #",
    "           ##",
    "",
]

# Agachado: 29 x 14, cabeca baixa e corpo esticado.
DUCK_TOP = [
    "#                  ########",
    "##      #######   ##w#######",
    "###  #########################",
    "##############################",
    " ###########################",
    "  ######################",
    "   ##################",
    "    ############  ##",
    "     ##########",
]
DUCK_LEGS1 = [
    "      ##   ##",
    "      #     ##",
    "      ##",
    "",
    "",
]
DUCK_LEGS2 = [
    "      ##   ##",
    "      ##    #",
    "            ##",
    "",
    "",
]

# ------------------------------------------------------------------ cactos
CACTUS_SMALL = [
    "     ##",
    "    ####",
    "    ####",
    "    ####",
    " #  ####",
    "##  ####",
    "##  ####  #",
    "##  #### ##",
    "##  #### ##",
    "##  #### ##",
    "##  #### ##",
    "######## ##",
    " ##########",
    "    ######",
    "    ####",
    "    ####",
    "    ####",
    "    ####",
    "    ####",
    "    ####",
    "    ####",
    "    ####",
    "    ####",
]
CACTUS_LARGE = [
    "       ###",
    "      #####",
    "      #####",
    "      #####",
    "      #####",
    "  ##  #####",
    " #### #####",
    " #### #####  ##",
    " #### ##### ####",
    " #### ##### ####",
    " #### ##### ####",
    " #### ##### ####",
    " #### ##### ####",
    " #### ##### ####",
    " #### ##### ####",
    " #### ##### ####",
    " ########## ####",
    "  ######### ####",
    "   #############",
    "      ########",
    "      #####",
    "      #####",
    "      #####",
    "      #####",
    "      #####",
    "      #####",
    "      #####",
    "      #####",
    "      #####",
    "      #####",
    "      #####",
    "      #####",
]


def side_by_side(*parts, gap=1):
    height = max(len(p) for p in parts)
    widths = [max(len(r) for r in p) for p in parts]
    rows = []
    for y in range(height):
        line = ''
        for p, w in zip(parts, widths):
            pad = height - len(p)                      # alinha pela base
            src = p[y - pad] if y >= pad else ''
            line += src.ljust(w, '.') + '.' * gap
        rows.append(line.rstrip('.'))
    return rows


CACTUS_SMALL_B = [r[::-1] for r in [row.ljust(11) for row in CACTUS_SMALL]]

# ------------------------------------------------------------- pterodatilo
# 23 x 17, voando para a esquerda.
PTERO_UP = [
    "        #",
    "        ##",
    "        ###",
    "        ####",
    "    #   #####",
    "   ##   ######",
    "  ###   #######",
    " ##w##  ########",
    "######   ########",
    "      ##################",
    "       ###############",
    "        ###########",
    "         #########",
    "",
    "",
    "",
    "",
]
PTERO_DOWN = [
    "",
    "",
    "",
    "",
    "    #",
    "   ##",
    "  ###",
    " ##w##",
    "######  #########",
    "      ##################",
    "       ###############",
    "        ##########",
    "        #######",
    "        #####",
    "        ####",
    "        ###",
    "        ##",
]

# ------------------------------------------------------------ cenario e HUD
CLOUD = [
    "            llllll",
    "          ll.wwww.ll",
    "    lllll.wwwwwwwwwl",
    "   lwwwwwwwwwwwwwwwwlll",
    "  llwwwwwwwwwwwwwwwwwwwl",
    "llwwwwwwwwwwwwwwwwwwwwwwll",
    "llllllllllllllllllllllllll",
]
CLOUD = [r.replace('.', 'w') for r in CLOUD]

GROUND = [
    "                  ##                          ###               ",
    "################################################################",
    "   #          ##              #       #              ##         ",
    "        #                 ##              #     #             # ",
    "  ##                #             ##                 #          ",
]

RESTART = [
    "  ##########",
    " ##        ##",
    "##   #      ##",
    "#   ##       #",
    "#  ######    #",
    "#   ##       #",
    "#    #       #",
    "#           ##",
    "##         ##",
    " ##########",
]

# Fonte 5x7 para textos e numeros.
FONT = {
    'A': ["01110", "10001", "10001", "11111", "10001", "10001", "10001"],
    'C': ["01110", "10001", "10000", "10000", "10000", "10001", "01110"],
    'D': ["11110", "10001", "10001", "10001", "10001", "10001", "11110"],
    'E': ["11111", "10000", "10000", "11110", "10000", "10000", "11111"],
    'G': ["01110", "10001", "10000", "10111", "10001", "10001", "01111"],
    'H': ["10001", "10001", "10001", "11111", "10001", "10001", "10001"],
    'I': ["01110", "00100", "00100", "00100", "00100", "00100", "01110"],
    'J': ["00111", "00010", "00010", "00010", "00010", "10010", "01100"],
    'K': ["10001", "10010", "10100", "11000", "10100", "10010", "10001"],
    'M': ["10001", "11011", "10101", "10101", "10001", "10001", "10001"],
    'N': ["10001", "11001", "10101", "10011", "10001", "10001", "10001"],
    'O': ["01110", "10001", "10001", "10001", "10001", "10001", "01110"],
    'P': ["11110", "10001", "10001", "11110", "10000", "10000", "10000"],
    'R': ["11110", "10001", "10001", "11110", "10100", "10010", "10001"],
    'S': ["01111", "10000", "10000", "01110", "00001", "00001", "11110"],
    'T': ["11111", "00100", "00100", "00100", "00100", "00100", "00100"],
    'U': ["10001", "10001", "10001", "10001", "10001", "10001", "01110"],
    'V': ["10001", "10001", "10001", "10001", "10001", "01010", "00100"],
    'X': ["10001", "10001", "01010", "00100", "01010", "10001", "10001"],
    '0': ["01110", "10011", "10101", "10101", "10101", "11001", "01110"],
    '1': ["00100", "01100", "00100", "00100", "00100", "00100", "01110"],
    '2': ["01110", "10001", "00001", "00010", "00100", "01000", "11111"],
    '3': ["11111", "00010", "00100", "00010", "00001", "10001", "01110"],
    '4': ["00010", "00110", "01010", "10010", "11111", "00010", "00010"],
    '5': ["11111", "10000", "11110", "00001", "00001", "10001", "01110"],
    '6': ["00110", "01000", "10000", "11110", "10001", "10001", "01110"],
    '7': ["11111", "00001", "00010", "00100", "01000", "01000", "01000"],
    '8': ["01110", "10001", "10001", "01110", "10001", "10001", "01110"],
    '9': ["01110", "10001", "10001", "01111", "00001", "00010", "01100"],
    ':': ["00000", "00100", "00000", "00000", "00000", "00100", "00000"],
}


def text(s, scale=1, spacing=1):
    """Texto em uma linha. Cada letra ocupa 5 colunas + espacamento."""
    cell = 5 + spacing
    width = len(s) * cell - spacing
    rows = []
    for y in range(7):
        line = ''
        for ch in s:
            glyph = FONT.get(ch, ["00000"] * 7)[y]
            line += glyph.replace('1', '#').replace('0', '.') + '.' * spacing
        rows.append(line[:width])
    if scale > 1:
        rows = [''.join(c * scale for c in r) for r in rows for _ in range(scale)]
    return rows


def digit_strip():
    """Digitos 0-9 empilhados: cada um ocupa 6 x 8 = 48 pixels = 12 palavras.
    Por ficarem na mesma imagem, compartilham a paleta e o codigo C copia
    so o trecho do digito desejado (deslocamento de DIGIT_WORDS palavras)."""
    rows = []
    for d in '0123456789':
        rows += [r + '.' for r in text(d)] + ['......']
    return rows


def build(out):
    out.mkdir(parents=True, exist_ok=True)
    images = {
        'dino_stand': sprite(DINO_TOP + LEGS_STAND, 22),
        'dino_run1': sprite(DINO_TOP + LEGS_RUN1, 22),
        'dino_run2': sprite(DINO_TOP + LEGS_RUN2, 22),
        'dino_dead': sprite(DINO_DEAD_TOP + LEGS_STAND, 22),
        'dino_duck1': sprite(DUCK_TOP + DUCK_LEGS1, 30),
        'dino_duck2': sprite(DUCK_TOP + DUCK_LEGS2, 30),
        'cactus_s1': sprite(CACTUS_SMALL),
        'cactus_s2': sprite(side_by_side(CACTUS_SMALL, CACTUS_SMALL_B)),
        'cactus_l1': sprite(CACTUS_LARGE),
        'cactus_l3': sprite(side_by_side(CACTUS_LARGE, CACTUS_SMALL, CACTUS_LARGE)),
        'ptero1': sprite(PTERO_UP, 24),
        'ptero2': sprite(PTERO_DOWN, 24),
        'cloud': sprite(CLOUD),
        'ground': sprite(GROUND, 64),
        'restart': sprite(RESTART),
        'digits': sprite(digit_strip(), 6),
        'txt_hi': sprite(text('HI')),
        'txt_gameover': sprite(text('G A M E  O V E R')),
        'txt_title': sprite(text('DINO RISC', scale=2, spacing=1)),
        'txt_press': sprite(text('ESPACO PARA JOGAR')),
    }
    for name, im in images.items():
        assert im.width <= 127 and im.height <= 127, name
        im.save(out / (name + '.png'))
    return images


def preview(images, path, scale=4):
    """Folha de conferencia ampliada (nao faz parte do jogo)."""
    pad = 6
    w = max(im.width for im in images.values()) * scale + pad * 2
    h = sum(im.height * scale + pad for im in images.values()) + pad
    sheet = Image.new('RGBA', (w, h), (247, 247, 247, 255))
    y = pad
    for im in images.values():
        sheet.alpha_composite(im.resize((im.width * scale, im.height * scale), Image.NEAREST), (pad, y))
        y += im.height * scale + pad
    sheet.save(path)


if __name__ == '__main__':
    here = Path(__file__).resolve().parent.parent
    out = Path(sys.argv[1]) if len(sys.argv) > 1 else here / 'assets'
    imgs = build(out)
    if len(sys.argv) > 2:
        preview(imgs, sys.argv[2])
    print(len(imgs), 'imagens em', out)
