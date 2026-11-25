# Riscplay Studio — DRV32

Ambiente web para criar e executar jogos do Riscplay com a **CPU DRV32 da versão estável de 30,9375 MHz**.

O DRV32 é o processador alvo. **RV32I** é o conjunto de instruções que ele implementa, acrescido das instruções customizadas do Riscplay. A opção `-march=rv32i` impede o compilador de gerar multiplicação/divisão de hardware que não existem nessa versão da CPU; as operações correspondentes são implementadas por software.

## Começar no Windows

1. Extraia o ZIP em uma pasta nova, por exemplo `C:\codex\riscplay-studio`.
2. Execute `start_windows.cmd`.
3. Abra o endereço completo exibido no terminal, incluindo a chave após `#key=`.

O projeto procura automaticamente o compilador já presente em `C:\codex\toolchains\xpack-riscv-none-elf-gcc-15.2.0-1`. Também procura `toolchain/bin` dentro do projeto, a variável `CROSS_COMPILE` e os compiladores disponíveis no PATH. O ZIP não inclui o GCC nem o Python.

Em outro computador, instale Python 3.10 ou superior, Pillow e uma distribuição RISC-V GCC com multilib RV32I/ILP32:

```powershell
python -m pip install -r requirements.txt
python server.py --cross-prefix 'C:\ferramentas\xpack-riscv-none-elf-gcc\bin\riscv-none-elf-'
```

O prefixo termina em `-`, sem `gcc.exe`. A distribuição usada nos testes foi xPack GCC 15.2.0-1. O arquivo README fornecido com ela aponta para <https://xpack.github.io/riscv-none-elf-gcc/releases/>.

## Linux e celular

```sh
python3 -m pip install -r requirements.txt
python3 server.py --lan --cross-prefix riscv64-unknown-elf-
```

Use o prefixo do seu compilador instalado. O servidor e o compilador ficam no PC. No celular conectado à mesma rede, abra o endereço `Celular (mesma rede)` exibido no terminal, incluindo sua chave. O firewall do PC precisa permitir a conexão escolhida pelo usuário.

`python server.py` escuta somente no próprio PC. A opção `--lan`, usada pelos dois lançadores incluídos, permite acessar pela rede local. A porta padrão é 8090 e pode ser alterada com `--port`. A chave é renovada a cada inicialização; reabra o novo endereço ao reiniciar o servidor.

O navegador executa CPU, gráficos e áudio. O PC só participa ao compilar C ou converter imagens. Não é necessário manter o 8bitworkshop funcionando, e não há dependências de CDN.

## Player sem instalação

Abra **`Riscplay_Player.html`** diretamente no navegador. É um único arquivo com FlappyBird, Hollow, Mandelbrot e um pequeno exemplo interativo. Ele também abre pacotes `.json.prog` e oferece teclado, gamepad, toque, WebGL e áudio.

Esse player é para execução. A edição C com recompilação automática e o gerador de assets estão no Studio, acessado pelo servidor.

## Código C live

- Escolha FlappyBird, Hollow, Mandelbrot ou Novo projeto.
- Edite `main.c` ou outro arquivo pelo seletor do editor.
- Com **LIVE** ligado, uma pausa de aproximadamente 600 ms na digitação inicia o GCC. A duração total inclui o tempo de compilação.
- Uma compilação válida reinicia o jogo com o novo firmware. O estado da partida não é preservado.
- Erros aparecem abaixo do editor; o último binário válido continua executando.
- **Ctrl+Enter** compila imediatamente. **Ctrl+S** exporta o projeto completo.
- O projeto é salvo automaticamente no armazenamento do navegador; use **Salvar projeto** para uma cópia transportável com fontes e assets. Arquivos de projeto usam `.riscplay.json`.
- **Abrir** aceita um projeto completo, um pacote do loader ou arquivos `.c`/`.h`. Pacotes do loader não contêm os fontes C: importe os fontes correspondentes se desejar editá-los.

A memória de programa tem 32 KiB, com 4 KiB reservados para a pilha. O linker verifica a imagem incluindo BSS. `startup.S` e `linker.ld` são os arquivos de suporte controlados pelo servidor em `sdk/`.

## Assets gráficos

Em **Assets**, importe PNG, JPEG, WebP ou BMP. A conversão preserva alfa como transparência binária, com índice 255 reservado pela PPU. Imagens com muitas cores são quantizadas para até 255 cores visíveis. Não há editor de pintura.

Imagens maiores que 127 × 127 são divididas em partes de no máximo 127 × 127, sem redimensionamento. Cada parte recebe macros com as dimensões, posição dentro da imagem original e endereços. O limite de importação é 1024 × 1024 por imagem, 4 MiB por arquivo e 128 partes no projeto.

O Studio cria **`generated_assets.h`**, também disponível em **Baixar .h**:

```c
#include "console.h"
#include "generated_assets.h"

/* Para uma imagem chamada personagem.png: */
load_pallet_from_sdram(PERSONAGEM_PALETTE_ADDR);
copy_sprite_from_sdram_to_buffer(PERSONAGEM_ADDR,
    PERSONAGEM_WIDTH * PERSONAGEM_HEIGHT, 0);
render_sprite(1, framebuffer, PERSONAGEM_WIDTH, PERSONAGEM_HEIGHT,
    x, y, 0, 0);
```

Os endereços são **palavras de 32 bits da SDRAM**, como no hardware. Cada asset tem 256 palavras de paleta seguidas dos índices de pixels, quatro por palavra, byte mais significativo primeiro. A alocação começa em `0x100000` ou após o último bloco original, o que for maior, e respeita o limite de `0x200000` palavras da SDRAM. Os blocos originais dos exemplos são preservados.

Adicionar imagens não as desenha automaticamente: use os símbolos gerados no código. Para a apresentação, aguarde `ppu_finished_all_operations()` e chame `set_frame_buffer()` como nos exemplos.

## Exportar para a FPGA pelo loader

Clique em **Exportar FPGA ↓**. O arquivo **`<projeto>.json.prog`** usa o mesmo formato JSON dos pacotes fornecidos com o Riscplay:

| Bloco | Conteúdo |
|---|---|
| `type: 0` | Firmware DRV32 em hexadecimal, `addr` e `addr_mem` = `20000000` |
| `type: 1` | Paletas e pixels na SDRAM, com endereços originais ou gerados |

Cada bloco inclui `comp_uuid`, `type`, `addr`, `addr_mem` e `value`. Nos blocos SDRAM, `value` é uma string contendo o array JSON de palavras, como nos arquivos originais. Os metadados de imagem existentes são preservados, e os novos assets também incluem `pixels` e `palette`.

O arquivo contém **firmware e todos os assets**. Não é necessário gerar um pacote separado de imagens. O firmware executa a partir de `0x80000000`; `20000000` é o endereço em palavras usado pelo loader para a RAM da CPU, não um novo endereço de execução.

A exportação fica desabilitada quando o C ou os assets têm alterações ainda não compiladas, para evitar exportar um binário antigo com fontes novos. Uma compilação válida a libera. Carregue o arquivo pelo procedimento normal do seu loader. A formatação e a reimportação foram testadas; a gravação física via ESP32/SPI ainda deve ser confirmada na sua placa.

## Controles e áudio

| Entrada | Controle Riscplay |
|---|---|
| Setas | Direções |
| Espaço, Enter ou Z | X |
| X | Círculo |
| A / S | Quadrado / triângulo |
| Q / E | L1 / R1 |
| Shift esquerdo / direito | L2 / R2 |
| Gamepad padrão | Direcional, eixos e botões correspondentes |
| Botões na tela | Direcional e ações por toque |

Clique na tela do jogo para usar o teclado. Enquanto o editor ou um campo recebe foco, suas teclas ficam reservadas à edição. Clique em **Ativar som** para liberar o áudio do navegador. O modelo possui dois canais pulse, triângulo, ruído LFSR e envelopes ADSR.

## Fidelidade e limites

O núcleo é uma **emulação funcional do DRV32**, escrita em JavaScript e executada em Web Worker. Ele roda os binários RV32I reais e implementa as instruções customizadas `0x2B`, a RAM, os timers, os controles, a PPU, colisões e os registradores da APU usados pelos jogos fornecidos.

A referência de clock é 30,9375 MHz para a CPU e 74,25 MHz para os periféricos. O sequenciamento da CPU considera as bordas e confirmações entre os domínios. Os tempos de serviço da PPU/SDRAM são aproximados. Não é o Verilog completo compilado para WebAssembly; não serve para verificar metastabilidade, fechamento de timing, SPI, arbitragem física da SDRAM ou serialização HDMI. A interface direta do gerenciador SDRAM pela CPU não é implementada nesta versão; os exemplos acessam gráficos pelas instruções PPU suportadas.

O WebGL apresenta a imagem já calculada pela PPU funcional, mantendo pixels sem interpolação. Há fallback Canvas 2D. O áudio é amostrado a 48 kHz; a latência de oito ciclos do mixer e a serialização HDMI não são reproduzidas. Se o dispositivo não acompanhar a carga, o tempo virtual avança mais devagar que o real. Pausar ou ocultar a aba interrompe o avanço.

O suporte a gamepad depende da API oferecida pelo navegador e do contexto de acesso; teclado e controles de toque estão disponíveis como alternativas. A aparência móvel foi testada em viewport de 390 × 844, não em um telefone físico. Não foi conectado um joystick físico durante a validação.

## Validação incluída

Veja `reports/VALIDATION.md` e os relatórios JSON. Os testes verificaram 73.882 instruções contra traces RTL, falhas de instrução inválida/alinhamento, imagens dos três jogos, recompilação C, áudio, controles, assets e ida e volta dos pacotes do loader.

```sh
python tests/backend_test.py
node tests/core.test.mjs
```

Os testes de navegador são opcionais e precisam de Playwright e um navegador instalado. Configure `PLAYWRIGHT_MODULE` e `BROWSER_PATH` se necessário. Eles atendem requisições com arquivos locais em memória, sem abrir uma porta de servidor. O início real do servidor HTTP não pôde ser validado no ambiente de desenvolvimento porque a abertura de sockets foi bloqueada.

## Organização

- `dist/`: interface, editor, emulador, áudio e exemplos.
- `server.py`, `asset_manager.py`: compilação C e conversão de imagens.
- `sdk/`: headers, áudio C, startup e linker do Riscplay.
- `examples/`: cópias dos fontes dos jogos usados como referência.
- `tests/`: testes de CPU, recompilação, assets e interface.
- `build_player.py`: gera o HTML independente usando os fontes em `dist/`.

Créditos e licenças em `THIRD_PARTY.md`, `LICENSE_8BITWORKSHOP` e `dist/vendor/`.
