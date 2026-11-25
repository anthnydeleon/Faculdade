# Dino RISC

Versão do jogo do dinossauro do Google Chrome para o Riscplay (CPU DRV32, RV32I).
O dinossauro corre sozinho; o jogador pula cactos e pterodáctilos. A velocidade
aumenta com o tempo e o placar conta a distância percorrida.

## Como jogar

- **Sem instalar nada:** abra `Riscplay_Player.html` (na raiz do projeto). O Dino já vem selecionado.
- **No Studio:** execute `start_windows.cmd`, abra o endereço exibido e escolha **Dino** em PROJETO.
  Ali dá para editar `main.c` e ver o jogo recompilar.

Clique em **Ativar som** para ouvir a música e os efeitos.

| Tecla | Ação |
|---|---|
| Espaço, Z, Enter ou seta para cima | Pular (segurar = pulo mais alto) |
| Seta para baixo | Abaixar; no ar, cai mais rápido |

## O que o jogo usa do hardware

- **Gráficos (PPU):** 20 imagens convertidas para a SDRAM, cada uma com sua paleta.
  Framebuffer duplo em `0xA080` e `0x18180`; o quadro é desenhado num buffer enquanto o outro é exibido.
- **Imagens compartilhando paleta:** os dígitos 0–9 ficam empilhados numa só imagem.
  O código copia apenas o trecho do dígito (12 palavras), sem recarregar a paleta.
- **Cache de sprite:** só recarrega paleta e pixels quando a imagem muda.
  O chão é desenhado 6 vezes por quadro com uma única carga.
- **Som (APU):** música no pulse 1 e triângulo; efeitos de pulo e pontuação no pulse 2;
  ruído na batida, seguido de uma melodia de fim de jogo.
- **Ponto fixo:** posição e velocidade usam 1/16 de pixel, porque não há ponto flutuante.
- **Colisão:** caixas retangulares desenhadas sobre cada imagem, como no jogo original.

## Arquivos

| Arquivo | Conteúdo |
|---|---|
| `main.c` | O jogo inteiro, dividido em 7 seções comentadas |
| `dino_assets.h` | Endereços e tamanhos das imagens na SDRAM (gerado) |
| `assets/*.png` | As imagens (geradas) |
| `tools/make_assets.py` | Os desenhos em texto que viram PNG |
| `build.py` | Gera imagens, converte, compila e atualiza Studio e Player |
| `dino.json.prog` | Pacote completo para o loader da FPGA (gerado) |
| `console.h`, `riscplay_audio.*` | Cópias do SDK |

## Alterar o jogo

- **Regras e física:** constantes no topo de `main.c`, por exemplo `JUMP_VELOCITY`, `GRAVITY` e `SPEED_MAX`.
- **Desenhos:** edite as grades de caracteres em `tools/make_assets.py`.
- Depois de mudar desenhos ou o C fora do Studio, rode:

```
python examples/dino/build.py
```

O build precisa do GCC RISC-V. O servidor o procura em `%USERPROFILE%\toolchains\xpack-riscv-none-elf-gcc-15.2.0-1`.

As imagens são pixel art própria, inspirada no jogo original do Chrome.
