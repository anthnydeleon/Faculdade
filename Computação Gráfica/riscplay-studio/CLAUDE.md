# Riscplay Studio: contexto para criar jogos

Trabalho de Computação Gráfica: criar jogos simples em C para o console **Riscplay**.
A CPU é a DRV32, com RV32I a 30,9375 MHz. O jogo roda no navegador por um emulador em JavaScript.
O FlappyBird (`examples/flappybird`) é a referência original.
O Dino (`examples/dino`) é o jogo já feito e o **modelo a copiar** para jogos novos.

## Comandos

```powershell
start_windows.cmd                  # Studio em http://localhost:8090/#key=... (editor + compilação live)
python examples/dino/build.py      # gera imagens, converte, compila e atualiza Studio + Player
python tools/riscplay_emu.py dist/examples/dino.json --frames 300 --press 30:x --shot 100,250 --out <pasta>
```

- **GCC:** está em `C:\Users\antho\toolchains\xpack-riscv-none-elf-gcc-15.2.0-1`. A função `find_compiler()` do `server.py` procura lá.
- **Python e Pillow:** estão instalados.
- **Node.js:** não está instalado, então `tests/*.mjs` não rodam. Use `tools/riscplay_emu.py`, um port da CPU e da PPU de `dist/core.js` validado com as telas do FlappyBird e do Dino.
- **Sem git:** o projeto não está no git. O original está em `../riscplay-studio.tar.gz`; use-o para comparar com `diff`.

## Estrutura

| Caminho | Papel |
|---|---|
| `server.py` | Serve `dist/`, compila C (`/api/build`) e converte imagens (`/api/assets`) |
| `asset_manager.py` | PNG para SDRAM: 256 palavras de paleta + pixels, 4 por palavra, MSB primeiro |
| `sdk/` | `console.h` (PPU e controle), `riscplay_audio.c/.h`, `startup.S`, `linker.ld` |
| `dist/` | Studio: `app.js` (interface), `core.js` (CPU e PPU), `audio.js` (APU), `worker.js`, `examples/*.json` |
| `examples/<jogo>/` | Fontes legíveis de cada jogo |
| `build_player.py` | Gera `Riscplay_Player.html`, um player offline com os jogos embutidos |
| `tools/riscplay_emu.py` | Emulador headless para testes |

Um `dist/examples/<id>.json` tem `{id, title, files, package}`.
`package` é a lista do loader da FPGA.
O bloco com `type:0` é o firmware em hex, e cada bloco com `type:1` é um asset na SDRAM com `addr` em hex de palavras.

## Hardware (o que importa para o código)

**Memória e CPU**
- **RAM:** 32 KiB em `0x80000000`, com 4 KiB reservados para a pilha. O Dino usa cerca de 10 KB.
- **Sem MUL/DIV de hardware:** o `libgcc` resolve `*`, `/` e `%`, porém mais devagar.
- **Sem ponto flutuante na prática:** use ponto fixo. O Dino usa 1/16 de pixel.
- **Sem libc:** se o linker reclamar de `memset` ou `memcpy`, defina-os como em `examples/hollow/platform.c`.
- **`main`:** pode ser `int main(void)` com um laço infinito.

**MMIO**
| Endereço | Uso |
|---|---|
| `0x00000000` | Tempo em centésimos de segundo; use `get_time()`, que já inverte o endianness |
| `0x00000028` | Tempo em milissegundos |
| `0x0000000C` | Controle; use `get_ps2_controller_state()` + `BUTTON_*_PRESSED` |
| `0x10000000` | Registrador de colisão da PPU |
| `0x200`–`0x244` | APU; use `riscplay_audio` em vez de acessar direto |

**Botões e teclado**
| Bit | Botão | Teclado |
|---|---|---|
| 4 | Cima | Seta para cima |
| 5 | Direita | Seta para direita |
| 6 | Baixo | Seta para baixo |
| 7 | Esquerda | Seta para esquerda |
| 14 | X | Espaço, Z ou Enter |
| 13 | Círculo | X |
| 15 | Quadrado | A |
| 12 | Triângulo | S |

**PPU e SDRAM**
- **Endereços:** são palavras de 32 bits, não bytes. A tela tem 320×180.
- **Framebuffers:** ficam em `0xA080` e `0x18180` (57.600 palavras cada). A SDRAM fica livre a partir de `0x26280`.
  O Dino ocupa de `0x26280` a `0x280BF`. Assets importados pelo Studio começam em `0x100000`.
- **Laço de cada quadro:** este padrão é obrigatório.
  ```c
  fb = framebuffers[buffer];
  /* limpa o fundo e desenha os sprites */
  ppu_finished_all_operations();   // espera a PPU
  set_frame_buffer(fb, 0);         // mostra o quadro e sincroniza a 60 Hz
  buffer ^= 1;
  ```
- **Desenhar um sprite:** `load_pallet_from_sdram(PAL)`, depois `copy_sprite_from_sdram_to_buffer(PIX, w*h, 0)`, depois `render_sprite(id, fb, w, h, x, y, flipx, flipy)`.
  A PPU tem **um único buffer de sprite**. O render usa o tamanho da última cópia.
  Carregar paleta é caro, então guarde qual sprite está carregado. Veja `draw()` no Dino.
- **Limites de sprite:** cada sprite tem no máximo **127×127**. Telas cheias são feitas de pedaços, como os blocos 90×90 do Flappy.
- **Cor 255:** o índice de cor 255 é transparente.
- **Cópia parcial:** começa em `PIX + n` palavras, então o recorte precisa ser múltiplo de 4 pixels.
  O Dino usa isso para os dígitos empilhados em células de 6×8, com 12 palavras e uma paleta só.
- **Recorte na tela:** `x` negativo e sprites saindo pela direita são recortados. Evite `y` negativo.
- **Cor de fundo:** `set_backgound()` recebe um índice da **paleta NES**. Para uma cor RGB exata, envie o comando tipo 3 com ARGB e alfa `0xFF`. Veja `clear_screen()` no Dino.
- **Colisão por hardware:** só existem **4 grupos** (`id & 3`). Ela é assimétrica: pixel transparente de um sprite conta contra o outro.
  Para colisão justa, prefira **caixas retangulares em software**, como `dino_hits_obstacle()`.
  A colisão de hardware serve para casos simples, como `check_collision(2,1)` no Flappy.

**Áudio (riscplay_audio v2)**
- **Canais:** 2 pulse, 1 triângulo e 1 de ruído.
- **Música:** use `rp_note_t {{hz_p1, hz_p2, hz_tri}, ruido, unidades_de_16avos}`, depois `rp_pattern_t` e `rp_song_t {..., bpm, loop}`, e toque com `riscplay_audio_play_song`.
- **Atualização:** chame `riscplay_audio_update_ms(ms)` uma vez por quadro, alternando 16, 17 e 17 ms, que dá a média exata de 1/60 s.
- **Efeitos:** `riscplay_audio_sfx_tone` usa o pulse 2 e `riscplay_audio_sfx_noise` usa o ruído. Deixe o pulse 2 fora da música.
- **Ordem importa:** `play_song` e `stop` cancelam o efeito em curso. Troque a música antes de disparar o efeito.
- **Volume:** `riscplay_audio_set_volume(canal, 0..15)` deve ser chamado depois de `play_song`.

## Receita para um jogo novo

1. Copie `examples/dino/` para `examples/<novo>/`.
2. Desenhe as imagens em `tools/make_assets.py`. As grades usam `#` para cinza, `w` para branco, `l` para cinza-claro e `.` para transparente; adicione cores no dict `COLORS`. Também dá para salvar PNGs prontos em `assets/`.
3. No `build.py`, ajuste `ASSETS` (a ordem fixa os endereços), `SDRAM_BASE`, `SOURCES`, os ids e títulos (`'dino'`/`'Dino'`) e as macros do header.
4. Escreva `main.c` reaproveitando do Dino `draw()`, `draw_number()`, `audio_tick()`, a máquina de estados TITLE, PLAYING e GAME_OVER e o RNG xorshift.
5. Registre o jogo. O `build.py` já cuida do `dist/examples/index.json`. Também é preciso registrar à mão:
   - em `dist/index.html`, uma `<option>` no `<select id="project">`;
   - em `dist/app.js`, a lista `['dino','flappybird',...]` perto de `localStorage.getItem('rp-last')`;
   - em `build_player.py`, a lista `for name in [...]` e a `<option>` do `<select id="game">`;
   - em `dist/player.js`, `let pkg=GAMES.<padrão>`, só se for o jogo padrão.
6. Rode `python examples/<novo>/build.py` e teste com `tools/riscplay_emu.py`, vendo os PNGs.

## Testes

- **Telas:** `tools/riscplay_emu.py` com `--press` e `--shot`, depois abra os PNGs. Confira também se não há exceção de instrução inválida ou desalinhada.
- **Robô que lê a RAM:** compile o ELF com as mesmas flags de `server.compile_files`, sem o build do servidor, para pegar endereços com `riscv-none-elf-nm`. Com isso, um script lê o estado do jogo, por exemplo `obstacles`, `speed` e `score`, e joga sozinho. Antes, confira se o `.bin` sai idêntico ao firmware do pacote.
- **Cuidados:** o emulador Python não mede desempenho e não toca som. Confirme no navegador.

## Armadilhas já encontradas

- **Cache do Studio:** o Studio salva cada projeto no IndexedDB. Depois de rodar `build.py`, ele pode abrir a **cópia antiga** ("Projeto restaurado deste navegador").
  Para ver a versão nova, limpe os dados do site, abra numa aba anônima ou use o `Riscplay_Player.html`.
- **Quebras de linha:** `dist/index.html` e `server.py` têm finais de linha próprios. Edite em modo binário (`open(...,'rb')`) para não converter o arquivo inteiro.
- **Avisos do build:** o `SyntaxWarning` de `build_player.py` e os avisos `-Warray-bounds` do SDK já vinham no projeto original. São inofensivos.
- **Guarda do header:** `asset_manager` gera headers com a guarda `RISCPLAY_ASSETS_H`. O `build.py` a renomeia para não colidir com `generated_assets.h` do Studio.
- **Largura do texto:** com a fonte 5×7 + 1 de espaço, um texto de 1 linha cabe em no máximo 21 caracteres (127 px).
- **Primeira tecla:** a primeira tecla só funciona depois de clicar na tela do jogo. O som precisa do botão "Ativar som".
