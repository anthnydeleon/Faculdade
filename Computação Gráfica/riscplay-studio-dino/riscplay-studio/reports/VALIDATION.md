# Validação — Riscplay Studio 0.1

Referência: projeto local Riscplay DRV32 estável, CPU 30,9375 MHz, IO/vídeo 74,25 MHz.

| Verificação | Resultado |
|---|---|
| Comparação de PC, instrução e registrador de destino com traces RTL | 73.882 instruções, sem divergências |
| MUL, DIV, custom inválida, load e salto desalinhados | 5 casos pararam com falha, como esperado |
| FlappyBird | 5 primeiras imagens idênticas às referências RTL |
| Hollow | 5 primeiras imagens idênticas às referências RTL |
| Mandelbrot | 2 imagens idênticas, incluindo o fractal completo |
| Mandelbrot completo | 10,167 segundos virtuais; 44.419.000 instruções até a apresentação |
| GCC dos três jogos e do starter | 4 compilações válidas; imagens recompiladas iguais às originais |
| Binários de FlappyBird e Mandelbrot recompilados | Idênticos byte a byte aos pacotes originais |
| Binário de Hollow recompilado | Layout diferente pela ordem de link; execução gráfica comparada com sucesso |
| Round-trip de pacotes do loader | Firmware e palavras SDRAM preservados nos 4 exemplos |
| Controle e áudio de FlappyBird | 321 leituras de controle, 110 escritas APU, 41.948 amostras não nulas |
| Controle e áudio de Hollow | 65 leituras de controle, 55 escritas APU, 49.978 amostras não nulas |
| Pulse de 440 Hz | 43 cruzamentos positivos em janela de 100 ms; amplitude dentro dos limites |
| Editor no navegador | Recompilação por alteração, erro de C e recuperação testados |
| Assets no navegador | Importação PNG, header, exportação FPGA e reimportação testados |
| Responsividade | 1440 px e 390 × 844 px; sem overflow horizontal móvel |
| Player independente | Abriu via `file://` sem servidor e apresentou os jogos |

Os testes de navegador usaram Edge headless, com requisições atendidas por arquivos locais em memória e o compilador GCC real. Não substituem um teste real da rede local, de telefone, de gamepad físico, de saída sonora nos alto-falantes ou do loader ESP32 ligado à FPGA. A interface de ativação do áudio e a geração das amostras PCM foram testadas.

Não houve síntese FPGA, alteração no bitstream ou validação de timing físico nesta etapa. A emulação funcional não modela todos os ciclos de PPU/SDRAM nem interfaces de configuração/SPI.

Resultados detalhados: `core.json`, `games.json`, `builds.json`, `browser.json` e `player.json`.
