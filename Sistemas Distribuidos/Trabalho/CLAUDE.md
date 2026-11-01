# Especificação do Projeto: KV Store Distribuído em Java

> Este arquivo é o contexto completo do trabalho. Leia-o inteiro antes de escrever qualquer código.
> Sugestão: salve-o na raiz da pasta do projeto como `CLAUDE.md` (se usar Claude Code) ou peça ao assistente para lê-lo no início de cada sessão.

---

## 0. Regras de trabalho com o assistente

O aluno tem **zero experiência em Java** e precisa explicar o código no relatório, citando números de linha. Por isso:

1. Trabalhe **por fases pequenas** (Seção 12). Cada fase termina com algo que compila e roda. Não escreva o projeto inteiro de uma vez.
2. Antes de cada trecho de código, explique **o que ele faz e por quê**, em linguagem simples, com analogia quando ajudar.
3. Ao terminar cada fase, diga ao aluno **como compilar, como executar e o que deve aparecer** no console.
4. **Nunca crie classes além de `Servidor`, `Cliente` e `Mensagem`** (Seção 3). Isso custa 3 pontos.
5. **Use apenas Java 8** (Seção 4). Se algo não existir no Java 8, não use.
6. **A única biblioteca externa permitida é o Gson** (JSON). Fora ela, só a JDK.
7. Os textos de `System.out.println` devem ser **idênticos** aos do enunciado (Seção 9). Não adicione prints de depuração no código final.
8. Marque no código, com comentários, cada trecho que implementa uma funcionalidade das Seções 5 e 6 (Seção 11).
9. Se uma decisão não estiver clara, **pergunte ao aluno** em vez de assumir. As dúvidas abertas estão na Seção 14.
10. Se especular ou não tiver certeza, diga isso de forma explícita.

---

---

## 0.1 Estado atual do projeto (atualizado em 29/10/2026)

**Fases concluídas: 1 a 8.** Faltam a Fase 9 (revisão final) e a Fase 10 (vídeo e relatório).

Arquivos na pasta: `Mensagem.java`, `Servidor.java`, `Cliente.java`, `gson-2.10.1.jar`, `CLAUDE.md`, `ajustes.md`, `plano-de-testes-kv-store.md` e o PDF do enunciado. Só os quatro primeiros vão no ZIP (mais o relatório).

O que já funciona e foi testado de verdade (teste de integração automatizado, 3 servidores + clientes, com os logs conferidos):

- Compila limpo, sem avisos com `-Xlint:all`, no JDK 1.8.0_504: com `-encoding windows-1252`, com `-encoding UTF-8` e sem `-encoding`. Os três `.java` são **100% ASCII**, então a saída é idêntica em qualquer encoding que o professor use.
- Servidor: inicialização pelo teclado, detecção de líder, laço `accept()` com uma thread por conexão.
- Registro dos seguidores no líder via `SYNC_REQUEST`, com retry silencioso de 1 segundo. Testado subindo os dois seguidores **antes** do líder: os dois se registraram sozinhos.
- PUT: encaminhamento pelo não líder, gravação no líder, `REPLICATION` em paralelo (uma thread por seguidor), espera de todos os `REPLICATION_OK` com `join()` e `PUT_OK` ao cliente. Testado: 12 GETs sorteados entre os 3 servidores devolveram o mesmo valor e o mesmo ts, sem nenhum `NULL`.
- GET: resposta imediata; `NULL` para chave inexistente com ts 0; `WAIT_FOR_RESPONSE` quando o servidor está atrasado, com thread de espera (`while` + `wait()`) e entrega tardia na porta de escuta do cliente. Testado fim a fim: 4 `WAIT_FOR_RESPONSE` seguidos, depois da sincronização, de 4 respostas tardias entregues.
- Recuperação: o seguidor volta vazio, mescla o estado do líder (só timestamps maiores), chama `notifyAll()` e imprime `Informações recebidas do líder`. Os acentos foram conferidos byte a byte na saída Cp1252 (`e7`, `f5`, `ed`).

**Pendência obrigatória antes da entrega:** existe um bloco marcado `DEMO - REMOVER ANTES DE ENTREGAR` no início de `tratarReplication`, em `Servidor.java`: um `Thread.sleep(10000)` que roda **só no servidor da porta 10099**. Ele serve só para gravar o vídeo — deixa aquele servidor por 10 segundos com a **versão antiga** da chave, enquanto o líder e o outro seguidor já têm a nova, o que torna o `WAIT_FOR_RESPONSE` filmável (ver Seção 8.3). Efeito colateral enquanto o bloco existir: todo PUT leva esses 10 segundos para ser confirmado, porque o líder espera o `REPLICATION_OK` de todos. **Apagar o bloco inteiro e recompilar antes de montar o ZIP.**

**Conformidade com os arquivos da pasta `agentes/`:** o código foi auditado contra `requisitos.md` e `outputs.md`. Todos os 24 pontos de saída (11 no servidor, 12 no cliente, mais nenhum) são ou um texto definido pelo `outputs.md`, ou um prompt de captura de teclado, que o `outputs.md` §1.1 e §2.1 autorizam. Não há `System.err` nem `printStackTrace` em nenhum arquivo.

### Decisões técnicas já travadas (não reabrir sem motivo)

| Decisão | Qual é | Por quê |
|---|---|---|
| Serialização | **Gson 2.10.1** (`.jar` na pasta, vai no ZIP) | O FAQ do próprio PDF recomenda explicitamente o Gson. O `.jar` tem bytecode major 51 (Java 7), compatível com o JDK 8. |
| Acentos no código | Arquivos `.java` com os **literais de saída em ASCII**, usando escapes `\uXXXX` para os acentos obrigatórios (`Meu ts é`, `Informações recebidas do líder`) | O professor compila sem `-encoding`, então o javac usa Cp1252 no Windows. Com `\uXXXX` o acento é resolvido antes da codificação do arquivo importar. Comentários podem ter acento (não afetam a saída). |
| IP do cliente | Tirado do próprio socket: `conexao.getLocalAddress().getHostAddress()` | É exatamente o endereço pelo qual aquele servidor consegue ligar de volta. Mais seguro que `getLocalHost()` (que pode escolher adaptador de VPN) e mais correto que fixar `127.0.0.1`. |
| Campos do servidor | `static` dentro de `Servidor` | Existe um só servidor por processo; evita passar referências entre threads e é mais fácil de explicar no relatório. |
| `meu timestamp` no print do GET | O timestamp que o cliente tinha **antes** daquela resposta (o `Tx` que ele enviou) | Mostra lado a lado o que o cliente já tinha visto e o que o servidor devolveu, que é o ponto da leitura monotônica. |
| Rótulos dos comentários de funcionalidade | `[Enunciado 4a - ...]`, `[Enunciado 5c - ...]` etc., seguindo a numeração do **PDF** (cliente = Seção 4, servidor = Seção 5), escritos **sem acento** | O professor lê o PDF, não este arquivo. Rótulos em ASCII mantêm os `.java` 100% ASCII. |
| Mensagens extras no cliente | **Nenhuma.** Também não há linha em branco no menu. As verificações continuam, mas silenciosas: `if (inicializado)` antes do PUT/GET, `catch (IOException)` vazio na falha de conexão, e `hasNextLine()` antes de ler o menu | `outputs.md` §1.6 proíbe, com todas as letras, mensagem de conexão, de debug e stack trace, e o §1.1 lista o menu sem linha em branco. As verificações em si são necessárias: sem a do INIT, `Random.nextInt(0)` mataria o cliente; sem a de `hasNextLine()`, o fim da entrada imprimiria um stack trace. **Custo aceito:** com um servidor derrubado, 1 de cada 3 operações falha sem nenhuma saída (medido: 2 PUT digitados, 1 `PUT_OK`). |
| GET de chave inexistente com `Tx > 0` | Devolve `NULL` e `ts 0`, **não** `WAIT_FOR_RESPONSE` | `requisitos.md` §8.7 manda seguir o texto literal do enunciado: *"Caso a chave não exista, o value devolvido será NULL"* aparece **antes** dos dois sub-itens, e esses sub-itens falam do "timestamp da key em S", que só existe se a key existir. Reverte a antiga decisão 7 da Seção 14. Documentar no relatório. |
| Texto do `WAIT_FOR_RESPONSE` no cliente | `GET key: [key] WAIT_FOR_RESPONSE do servidor [IP:porta], meu timestamp [ts]` | É a sugestão do `outputs.md` §1.5. Sem `value:` e sem o ts do servidor, porque nenhum valor foi lido — fica visivelmente diferente da resposta normal, que é o que o enunciado pede. No servidor, o `outputs.md` §2.7 manda usar o mesmo formato do §2.6 trocando o valor por `WAIT_FOR_RESPONSE`, e é o que o código faz. |
| INIT repetido | Reescreve os endereços dos servidores e **reaproveita** a porta de escuta e a thread já abertas | Evita vazar `ServerSocket` e thread, e evita resposta tardia impressa duas vezes. Nem o PDF nem este arquivo tratam do caso. |
| Fechamento da conexão no servidor | `conexao.close()` dentro de `finally`, em `AtendeConexao` | Sem isso, uma falha no tratamento (ex.: o não líder não alcança o líder) deixava o socket aberto e **o cliente ficava pendurado para sempre** no `readLine()`. |
| Atraso artificial para a demo | `Thread.sleep(10000)` no início de `tratarReplication`, só na porta 10099, isolado e marcado para remoção | Com o GET de chave inexistente devolvendo `NULL`, o servidor que acabou de voltar já **não** produz mais `WAIT_FOR_RESPONSE`. O único caminho que sobra é chave **existente** com `ts` menor que o `Tx` do cliente, e isso exige atrasar o `REPLICATION` de um seguidor — exatamente a receita da Seção 8.3. |

---

## 1. Resumo do trabalho

Disciplina: Sistemas Distribuídos. Trabalho individual.

Criar um **sistema chave-valor (KV Store) replicado e consistente**, com **3 servidores** e **vários clientes**, usando **TCP**. É uma versão muito simplificada do Zookeeper.

**Conceitos teóricos envolvidos** (úteis para o relatório):

| Conceito | Onde aparece |
|---|---|
| Replicação primário-backup | Só o líder grava. Os outros copiam. |
| Consistência centrada no cliente (leituras monotônicas) | O cliente nunca lê valor mais antigo que o já visto. |
| Relógio lógico (contador) | O timestamp é um contador mantido pelo líder. |
| Comunicação assíncrona orientada a eventos | O WAIT_FOR_RESPONSE. |
| Exclusão mútua | Proteção do `HashMap` com `synchronized`. |
| Recuperação de falha | Servidor não líder volta e copia o estado do líder. |

**Analogia:** três escritórios. A matriz (líder) é a única que altera o caderno oficial. As filiais guardam cópia e respondem consultas. Cada cliente leva consigo um bilhete com a "última versão que já li" de cada chave. Se a filial está atrasada, ela diz "volto a te chamar".

---

## 2. Arquitetura

- **3 servidores**, com IP e porta conhecidos. Portas padrão: `10097`, `10098`, `10099`. IP padrão: `127.0.0.1`.
- **N clientes** (o código não pode estar limitado a 2).
- Um dos servidores é o **líder**, escolhido na inicialização e **fixo**. Não há troca de líder.
- O cliente **não sabe** quem é o líder.
- Qualquer servidor recebe PUT e GET. Só o líder executa o PUT de fato.
- Toda comunicação (cliente-servidor e servidor-servidor) usa **TCP** e transporta um objeto **`Mensagem`**.

```
Cliente --PUT--> Servidor B (não líder) --PUT--> Líder A
                                                  |-- REPLICATION --> B
                                                  |-- REPLICATION --> C
                                                  |<-- REPLICATION_OK -- B, C
Cliente <------------------ PUT_OK (ts) ----------|
```

---

## 3. Estrutura de arquivos (regra rígida)

Somente **3 arquivos**, sem pacote (`package`), na mesma pasta:

```
Servidor.java
Cliente.java
Mensagem.java
```

- Classes de Thread: **somente como classes aninhadas** dentro de `Servidor` ou `Cliente`.
- **Não criar** `enum` em arquivo separado, nem classes auxiliares. Para os tipos de mensagem, usar **constantes `String`** dentro de `Mensagem`.
- **Não enviar** arquivos `.class` na entrega (-2 pontos).
- `Mensagem` deve ter **atributos específicos** (tipo, chave, valor, etc.). Não pode ser só uma `String` com parsing.
- Se não transferir a `Mensagem` pela rede: -3 pontos.

---

## 4. Restrições técnicas

| Item | Regra |
|---|---|
| Compilação | `javac` do **JDK 1.8** (versão do aluno: `1.8.0_504`). |
| Se não compilar | **Nota zero.** |
| Proibido no Java 8 | `var`, `List.of`, `Map.of`, `String.isBlank`, text blocks, `record`. |
| Proibido por regra | `ConcurrentHashMap`, `ArrayBlockingQueue` e similares. |
| Permitido | `HashMap`, `ArrayList`, `HashSet`, `synchronized`, `wait/notifyAll`, `Thread`, `Scanner`, `Socket`, `ServerSocket`. |
| Bibliotecas externas | Somente o **Gson**, recomendado pelo FAQ do enunciado. Nenhuma outra. Confirmar com o professor (Seção 14, item 1). |

### Envio da classe `Mensagem` pela rede

**Decisão adotada:** usar **JSON com a biblioteca Gson** (https://github.com/google/gson), como recomenda o FAQ do enunciado.

- O Gson converte o objeto `Mensagem` em texto JSON (`toJson`) e o texto de volta em objeto `Mensagem` (`fromJson`).
- O programa sempre monta e recebe um objeto `Mensagem` com atributos próprios. Não é parsing manual de `String`.
- Versão sugerida: **Gson 2.10.1** (`gson-2.10.1.jar`). Deve funcionar no Java 8, mas **conferir compilando** com o `javac` 1.8 do aluno.
- Download: https://repo1.maven.org/maven2/com/google/code/gson/gson/2.10.1/gson-2.10.1.jar
- O `.jar` fica **na pasta do projeto** e vai **junto no ZIP** (o PDF diz: "Caso tenha utilizado uma biblioteca permitida pelo professor, envie-a também").
- Compilar e executar com o `.jar` no classpath (Seção 16).
- Pendência: confirmar com o professor o item 1 da Seção 14.

**Padrão de envio e recebimento (uma conexão por requisição, uma mensagem por linha):**

```java
// Criar uma vez por classe (o Gson pode ser reutilizado)
Gson gson = new Gson();

// Enviar
PrintWriter out = new PrintWriter(
        new OutputStreamWriter(socket.getOutputStream(), "UTF-8"), true);
out.println(gson.toJson(mensagem));

// Receber
BufferedReader in = new BufferedReader(
        new InputStreamReader(socket.getInputStream(), "UTF-8"));
String linha = in.readLine();
Mensagem recebida = gson.fromJson(linha, Mensagem.class);
```

**Armadilhas conhecidas:**

1. **Não use `setPrettyPrinting()`.** O JSON compacto cabe em uma linha, e o protocolo depende de uma mensagem por linha. Quebras de linha dentro de um valor são escapadas pelo Gson.
2. Declare os mapas com genéricos completos: `HashMap<String, String>` e `HashMap<String, Long>`. Sem isso, o Gson pode devolver números como `Double`.
3. Atributos `null` não aparecem no JSON e voltam como `null`. O value `"NULL"` do GET é uma `String` normal. Não confunda com `null`.
4. Se `readLine()` devolver `null`, a outra ponta fechou a conexão sem enviar nada.
5. Use `UTF-8` explícito nos dois lados, para acentos não corromperem.
6. Crie um construtor sem argumentos em `Mensagem`, por segurança.
7. `import com.google.gson.Gson;` no topo de `Servidor.java` e `Cliente.java`.
8. Sem o `.jar` no classpath, o erro é `package com.google.gson does not exist`.

---

## 5. Funcionalidades do Cliente

### 5.1 INIT (inicialização)

- Captura do teclado o **IP e a porta dos três servidores**.
- Portas padrão: 10097, 10098, 10099. IP padrão: 127.0.0.1.
- Cria o `ServerSocket` de escuta do cliente (Seção 5.4) e inicia a thread que o atende.
- Guarda os 3 endereços em uma lista.
- O cliente **não conhece o líder**.

### 5.2 PUT

1. Captura do teclado a **key** e o **value**.
2. Escolhe um servidor **aleatório** entre os 3 (`java.util.Random`).
3. Envia `Mensagem` do tipo `PUT` com key, value e informações do cliente (IP e porta de escuta).
4. Espera a resposta `PUT_OK`, que traz o **timestamp**.
5. Atualiza seu `HashMap<String, Long>` de timestamps (chave -> último timestamp visto).
6. Imprime a mensagem da Seção 9.

### 5.3 GET

1. Captura do teclado só a **key**. O timestamp **não** vem do teclado.
2. Busca no seu mapa o último timestamp daquela key (0 se nunca viu).
3. Escolhe um servidor **aleatório**.
4. Envia `Mensagem` do tipo `GET` com key, timestamp do cliente e informações do cliente.
5. Resposta possível:
   - **Valor imediato:** atualiza o timestamp local e imprime.
   - **`WAIT_FOR_RESPONSE`:** imprime a versão diferenciada. A conexão é encerrada. O cliente **não fica bloqueado**. A resposta chega depois, na porta de escuta (Seção 5.4), e então imprime o resultado final.

### 5.4 Recebimento assíncrono

- O cliente mantém um `ServerSocket` em **porta livre** (use `new ServerSocket(0)` e leia a porta com `getLocalPort()`).
- Uma thread aninhada fica em `accept()` e recebe respostas tardias do GET.
- A porta e o IP desse socket vão dentro da `Mensagem` do PUT e do GET, para o servidor saber onde chamar o cliente de volta.

### 5.5 Menu

- Menu interativo com **somente** as opções `INIT`, `PUT` e `GET`.
- Cada cliente tem seus próprios timestamps, **inicializados em zero**.
- O cliente não sai do sistema nem morre.

---

## 6. Funcionalidades do Servidor

### 6.1 Inicialização

Captura do teclado, nesta ordem:

1. IP do próprio servidor
2. Porta do próprio servidor
3. IP do líder
4. Porta do líder

**É líder** se `(meuIP, minhaPorta)` for igual a `(ipLider, portaLider)`.

### 6.2 Atendimento simultâneo (obrigatório com threads)

- Laço `accept()` na thread principal.
- Para cada conexão aceita, cria uma **thread** que trata a requisição.
- O servidor continua aceitando novas conexões enquanto trata outras.

### 6.3 Estruturas de dados do servidor

Cada estrutura deve ser **comentada no código** e **explicada no relatório**.

| Estrutura | Tipo sugerido | Conteúdo |
|---|---|---|
| Tabela de valores | `HashMap<String, String>` | key -> value |
| Tabela de timestamps | `HashMap<String, Long>` | key -> timestamp |
| Contador de timestamp (só líder) | `long` | Último timestamp gerado. Incrementa a cada PUT. |
| Lista de seguidores (só líder) | `HashSet<String>` com `"ip:porta"` | Servidores que se registraram no líder. |
| Objeto de trava | `Object` | Protege tabelas e contador. |

Usar **duas tabelas** evita criar uma classe auxiliar para guardar "valor + timestamp".

### 6.4 Recebe PUT

**Se não é líder:**
- Imprime a mensagem de encaminhamento (Seção 9).
- Abre conexão com o líder, envia o `PUT` (mantendo as informações do cliente original).
- Espera o `PUT_OK` do líder e **repassa ao cliente** na mesma conexão em que o cliente espera.

**Se é líder:**
1. Imprime a mensagem de recebimento do PUT.
2. Dentro de `synchronized`: incrementa o contador, grava key/value e o timestamp nas tabelas (se a key já existir, atualiza value e timestamp).
3. **Fora** do `synchronized`, envia `REPLICATION` (key, value, timestamp) aos outros servidores.
4. Espera `REPLICATION_OK` de **todos** os seguidores.
5. Imprime a mensagem "Enviando PUT_OK..." (Seção 9).
6. Envia `PUT_OK` com o timestamp.

Cuidados:
- **Nunca segure o lock enquanto espera a rede.** Isso trava o sistema.
- Replicar aos dois seguidores em paralelo (duas threads curtas) é melhor que em sequência, mas sequência também é aceitável.
- Se a conexão com um seguidor falhar (ele caiu), **ignore esse seguidor**. Ele recupera tudo quando voltar (Seção 6.8). Assim o PUT não fica preso para sempre.

### 6.5 Recebe REPLICATION (seguidor)

1. Imprime a mensagem de REPLICATION.
2. Dentro de `synchronized`: grava key/value/timestamp.
   - **Proteção sugerida:** só sobrescreve se o timestamp recebido for **maior** que o já guardado. Isso evita que uma replicação atrasada apague uma mais nova.
3. Chama `notifyAll()` (acorda GETs pendentes, Seção 6.6).
4. Responde `REPLICATION_OK` ao líder.

### 6.6 Recebe GET

Cliente `Cx` com timestamp `Tx` pede a key `Kx` ao servidor `S`.

| Situação | Resposta |
|---|---|
| Key não existe em S e `Tx == 0` | Devolve value `NULL`. |
| Key existe e `ts(S) >= Tx` | Devolve value e `ts(S)`. |
| Key não existe em S e `Tx > 0` | Devolve value `NULL` e `ts 0` (leitura literal do enunciado, ver `agentes/requisitos.md` §8.7). |
| Key **existe** em S e `ts(S) < Tx` | Devolve `WAIT_FOR_RESPONSE`, **encerra a conexão** e cria uma thread de espera. |

**Thread de espera (classe aninhada):**
1. Dentro de `synchronized`, usa `while (ts(S) < Tx) lock.wait();`.
2. Quando `REPLICATION` (ou a sincronização) chamar `notifyAll()` e a condição for satisfeita, sai do laço.
3. Abre conexão com o cliente (IP e porta de escuta que vieram na `Mensagem`).
4. Envia a resposta do GET com value e timestamp do servidor.

Regra de ouro: **o cliente nunca pode obter um value anterior ao que já viu.**

Sempre devolva ao cliente, junto com o value, o **timestamp do servidor S** para aquela key.

### 6.7 Recebe REPLICATION_OK (líder)

Quando todos os seguidores responderem, o líder envia `PUT_OK` ao cliente. Pode ser implementado dentro da própria thread do PUT: ela envia as REPLICATION e lê os OK, em vez de receber o OK por outra conexão. Se o aluno preferir separar, tudo bem. O importante é cumprir o comportamento e o print.

### 6.8 Queda e volta de servidor não líder

- O teste é: o professor encerra um servidor não líder (Ctrl+C) e o sobe de novo. Ele volta **sem nenhum dado**.
- Na inicialização, todo servidor **não líder** envia ao líder uma mensagem `SYNC_REQUEST` com seu próprio IP:porta.
- O líder:
  1. Registra o IP:porta na lista de seguidores (isso também resolve a descoberta dos seguidores, Seção 14, item 3).
  2. Responde `SYNC_RESPONSE` com cópia das duas tabelas (dentro de `synchronized`).
- O seguidor mescla os dados (só aceita timestamp maior que o local), chama `notifyAll()` e imprime "Informações recebidas do líder".
- **Se o líder ainda não estiver no ar** na inicialização, o seguidor deve **tentar de novo** a cada 1 segundo, em uma thread de fundo, sem imprimir nada até conseguir.

### 6.9 Concorrência

- O líder pode receber vários PUT ao mesmo tempo (de chaves diferentes). O enunciado garante que **a mesma chave não terá PUT concorrente**.
- Todo acesso às tabelas, ao contador e à lista de seguidores é feito dentro de `synchronized (lock)`.
- Sem proteção: -2 pontos.

---

## 7. Classe `Mensagem`

`public class Mensagem` (não precisa de `Serializable`, o Gson cuida da conversão)

**Tipos (constantes `String`, `public static final`):**

| Constante | Quem envia -> quem recebe | Uso |
|---|---|---|
| `PUT` | cliente -> servidor; servidor -> líder | Pedido de gravação |
| `PUT_OK` | líder -> cliente (direto ou via servidor) | Gravação concluída, com timestamp |
| `GET` | cliente -> servidor | Pedido de leitura |
| `GET_RESPONSE` | servidor -> cliente | Resposta do GET (imediata ou tardia) |
| `WAIT_FOR_RESPONSE` | servidor -> cliente | "Estou atrasado, volto a te chamar" |
| `REPLICATION` | líder -> seguidor | Copiar (key, value, ts) |
| `REPLICATION_OK` | seguidor -> líder | Cópia feita |
| `SYNC_REQUEST` | seguidor -> líder | Registro e pedido do estado completo |
| `SYNC_RESPONSE` | líder -> seguidor | Estado completo |

**Atributos sugeridos (todos específicos, nada de string única):**

| Atributo | Tipo | Uso |
|---|---|---|
| `tipo` | `String` | Uma das constantes acima |
| `chave` | `String` | Key |
| `valor` | `String` | Value |
| `timestamp` | `long` | Timestamp da key |
| `ipCliente`, `portaCliente` | `String`, `int` | Endereço de escuta do cliente |
| `ipServidor`, `portaServidor` | `String`, `int` | Endereço do servidor que originou ou executou a operação |
| `dados` | `HashMap<String,String>` | Só em `SYNC_RESPONSE` |
| `tempos` | `HashMap<String,Long>` | Só em `SYNC_RESPONSE` |

Inclua um construtor sem argumentos, construtor(es) úteis, getters e setters simples.

---

## 8. Fluxos completos (para conferir o código)

### 8.1 PUT via servidor não líder

1. Cliente escolhe B. Envia `PUT(k, v, cliente)`.
2. B (não líder) imprime "Encaminhando PUT...". Envia `PUT` ao líder A.
3. A imprime "Cliente ... PUT...". Gera ts, grava, envia `REPLICATION` a B e C.
4. B e C imprimem "REPLICATION...", gravam, respondem `REPLICATION_OK`.
5. A imprime "Enviando PUT_OK ao Cliente...". Responde `PUT_OK(ts)`.
6. O cliente imprime "PUT_OK key:...".

### 8.2 GET imediato

Cliente envia `GET(k, Tx)`. O servidor tem `ts >= Tx`. Imprime o log do GET e responde `GET_RESPONSE(valor, ts)`. O cliente atualiza o timestamp e imprime.

### 8.3 GET com WAIT_FOR_RESPONSE

1. Servidor S tem `ts < Tx`.
2. S imprime o log do GET (versão WAIT_FOR_RESPONSE) e responde `WAIT_FOR_RESPONSE`. A conexão fecha.
3. O cliente imprime a versão WAIT_FOR_RESPONSE e volta ao menu.
4. Quando S recebe a replicação, a thread de espera acorda, conecta ao cliente e envia `GET_RESPONSE`.
5. O cliente (thread de escuta) atualiza o timestamp e imprime a linha final do GET.

**Como provocar esse caso na demonstração (o cliente não digita timestamp):**
- Adicione um `Thread.sleep(...)` de alguns segundos no tratamento de `REPLICATION` de **um** seguidor, para que ele fique atrasado.
- Sequência: faça o PUT (a key fica com ts novo no líder e no seguidor rápido). Faça GET e confira que algum servidor rápido responde (o cliente guarda o ts novo). Faça GET de novo até cair no seguidor lento, antes de a replicação terminar nele.
- Como o líder só confirma o PUT depois de **todos** os OK, a janela de atraso existe apenas entre a gravação local do líder e o último OK. Planeje a demo para aproveitar essa janela, por exemplo com PUT feito por um cliente e GET por outro cliente que já tenha ts alto. Ver Seção 14, item 4.

---

## 9. Mensagens de console (textos exatos)

O enunciado exige exatamente estas informações, **nem mais nem menos**. Substitua o que está entre colchetes pelos valores reais. O enunciado deixa algumas aspas sem fechar. **Reproduza o texto sem as aspas finais soltas**, a menos que o professor diga o contrário.

### Cliente

| Evento | Texto |
|---|---|
| Receber PUT_OK | `PUT_OK key: [key] value [value] timestamp [timestamp] realizada no servidor [IP:porta]` |
| Receber resposta do GET | `GET key: [key] value: [valor devolvido pelo servidor] obtido do servidor [IP:porta], meu timestamp [timestamp_do_cliente] e do servidor [timestamp_do_servidor]` |
| Receber WAIT_FOR_RESPONSE | `GET key: [key] WAIT_FOR_RESPONSE do servidor [IP:porta], meu timestamp [timestamp_do_cliente]` (sugestão do `agentes/outputs.md` §1.5). |

### Servidor

| Evento | Texto |
|---|---|
| PUT recebido (líder) | `Cliente [IP]:[porta] PUT key:[key] value:[value]` |
| PUT recebido (não líder) | `Encaminhando PUT key:[key] value:[value]` |
| REPLICATION recebido | `REPLICATION key:[key] value:[value] ts:[timestamp]` |
| Líder recebeu todos os REPLICATION_OK | `Enviando PUT_OK ao Cliente [IP]:[porta] da key:[key] ts:[timestamp_do_servidor]` |
| GET recebido | `Cliente [IP]:[porta] GET key:[key] ts:[timestamp]. Meu ts é [timestamp_da_key], portanto devolvendo [valor ou informação]` |
| GET com WAIT_FOR_RESPONSE | Mesmo formato, **diferenciado** (devolvendo `WAIT_FOR_RESPONSE`). |
| Servidor caiu e voltou | `Informações recebidas do líder` |

O menu do cliente (INIT, PUT, GET) e os pedidos de entrada de dados (digite a key etc.) são necessários para o uso e **não contam** como informação extra, mas **não** adicione mais nada além disso.

---

## 10. Teste que o professor fará

1. Compila com `javac` (JDK 8), no CMD do Windows. **Com o Gson, o comando precisa do `-cp`. Confirmar com o professor como ele compila** (Seção 14, item 1).
2. Abre **5 consoles**: 3 servidores e 2 clientes.
3. Sobe os **3 servidores primeiro**, depois os clientes.
4. Faz PUT e GET pelos clientes, em servidores aleatórios.
5. Derruba um servidor não líder e o sobe de novo.
6. O código deve funcionar com **mais de 2 clientes**.

**Roteiro de teste próprio** (rodar antes de entregar):

- [ ] Compila sem warnings graves com `javac *.java` no JDK 8.
- [ ] Os 3 servidores sobem em qualquer ordem.
- [ ] PUT em cada um dos 3 servidores funciona (líder e não líder).
- [ ] PUT na mesma key duas vezes atualiza value e ts.
- [ ] GET de key inexistente devolve `NULL`.
- [ ] GET em servidores diferentes devolve o mesmo valor após o PUT_OK.
- [ ] WAIT_FOR_RESPONSE aparece e a resposta tardia chega ao cliente.
- [ ] Servidor não líder cai, volta e imprime "Informações recebidas do líder". Depois, GET nele devolve os dados.
- [ ] 3 ou mais clientes simultâneos funcionam.
- [ ] Dois PUT de chaves diferentes ao mesmo tempo no líder funcionam.

---

## 11. Regras de comentário no código (-2 pontos se faltar)

1. Antes de cada trecho que implementa uma funcionalidade, um comentário no formato:
   `// [Seção 5.2 - PUT do cliente] envia requisição a servidor aleatório`
   `// [Seção 6.6 - GET no servidor] devolve WAIT_FOR_RESPONSE`
2. **Toda estrutura de dados** criada tem comentário dizendo **para que serve e o que armazena**.
3. Cada classe de Thread aninhada tem comentário explicando seu papel.
4. Os comentários ficam em **português** (ou no idioma que o aluno preferir, de forma consistente).

---

## 12. Fases de desenvolvimento

Cada fase deve terminar com compilação limpa e um teste manual.

| Fase | Entrega | Critério de pronto |
|---|---|---|
| 1 ✅ | `Mensagem.java` completa | Compila. |
| 2 ✅ | TCP básico: `Servidor` aceita, `Cliente` envia uma `Mensagem` em JSON (Gson), o servidor imprime. | Troca de `Mensagem` funciona nos dois sentidos. |
| 3 ✅ | Servidor com threads (uma por conexão) | Dois clientes ao mesmo tempo. |
| 4 ✅ | Tabelas + `synchronized` + GET local (sem replicação) | PUT e GET em um só servidor. |
| 5 ✅ | Inicialização completa: IP/porta próprios e do líder, detecção de líder, registro de seguidores (`SYNC_REQUEST` com retry) | Líder lista 2 seguidores. |
| 6 ✅ | PUT com encaminhamento, REPLICATION, REPLICATION_OK, PUT_OK | PUT por qualquer servidor aparece nos 3. |
| 7 ✅ | GET com timestamp, `WAIT_FOR_RESPONSE`, escuta assíncrona no cliente | Caso WAIT demonstrável. |
| 8 ✅ | Recuperação do servidor não líder (`SYNC_RESPONSE`) | Reinício devolve os dados. |
| 9 | Revisão: prints exatos, comentários, ausência de debug, teste com 3+ clientes | Checklist da Seção 10 completo. |
| 10 | Vídeo e relatório | Seção 13. |

---

## 13. Entrega

**Um único arquivo ZIP**, enviado pelo **SIGAA**, contendo:

```
[SeuRA].pdf          <- relatório
Servidor.java
Cliente.java
Mensagem.java
gson-2.10.1.jar   <- biblioteca permitida (confirmar com o professor)
```

Sem arquivos `.class`. Sem outras classes.

### Relatório `[SeuRA].pdf`: seções obrigatórias

| Seção | Conteúdo |
|---|---|
| I | Nome e RA |
| II | Link do vídeo (máx. **5 minutos**), com permissão de visualização, mostrando a compilação e o funcionamento nas **5 consoles visíveis ao mesmo tempo**. Não enviar o arquivo do vídeo. |
| III | No vídeo: GET normal, GET com `WAIT_FOR_RESPONSE`, e servidor caindo e voltando com recuperação do líder. |
| IV | Para **cada funcionalidade do servidor**: explicação de alto nível, **com as linhas do código**. Explicar cada estrutura de dados (para que serve, o que armazena). |
| V | Idem para **cada funcionalidade do cliente**. |
| VI | Uso de threads, **separando servidor e cliente**, com linhas do código. |
| VII | Proteção de acessos concorrentes, com linhas do código. |
| VIII | Links das fontes em que o código se baseou. |

**Importante:** escreva o relatório **depois** de congelar o código. Qualquer alteração muda os números de linha. Gere a lista de linhas relevantes só no final.

### Penalidades (resumo)

| Falha | Desconto |
|---|---|
| Não compila ou usa biblioteca externa sem aprovação | **Zero** |
| Não transfere a `Mensagem` | -3 |
| Cria classes além das permitidas | -3 |
| Não usa threads | -2 |
| Não protege acessos concorrentes | -2 |
| Não envia relatório | -2 |
| Sem link de vídeo, link indisponível ou vídeo sem o funcionamento pedido | -2 |
| Vídeo sem as 5 consoles visíveis juntas | -2 |
| Envia `.class` ou outros arquivos de código | -2 |
| Sem comentários das Seções 4 e 5 ou sem comentar as estruturas de dados | -2 |
| Plágio, cola ou fraude | **Zero em tudo na disciplina** |

---

## 14. Dúvidas abertas e decisões a confirmar

Decisões tomadas para seguir adiante. Algumas valem uma confirmação com o professor.

| # | Questão | Decisão atual | Confirmar com o professor? |
|---|---|---|---|
| 1 | Gson ou serialização nativa | **Gson 2.10.1. Decidido e já implementado.** O FAQ do PDF (pergunta 1) recomenda o Gson pelo nome e dá o link do repositório, então a biblioteca já está aprovada pelo enunciado. | **Sim, mas só sobre o comando de compilação.** O risco que sobra não é a permissão da biblioteca, é o professor rodar `javac *.java` sem `-cp gson-2.10.1.jar` e não compilar. Perguntar: (a) ele compila com `-cp`? (b) o `.jar` dentro do ZIP é aceito? |
| 2 | Origem do timestamp | **Contador global no líder**, incrementado a cada PUT. | Não é necessário. O enunciado aceita contador. |
| 3 | Como o líder descobre os outros dois servidores, se cada servidor só informa o próprio endereço e o do líder | Seguidores se **registram** no líder via `SYNC_REQUEST` (com retry). | Opcional. |
| 4 | Como mostrar `WAIT_FOR_RESPONSE` na demo | `Thread.sleep` no REPLICATION de um seguidor, com demo planejada para a janela de atraso. | Opcional. Perguntar se um atraso artificial é aceitável. |
| 5 | Texto exato da versão WAIT_FOR_RESPONSE no cliente e no servidor | **Resolvido pelo `agentes/outputs.md`:** no cliente, linha curta do §1.5; no servidor, mesmo formato do §2.6 trocando o valor por `WAIT_FOR_RESPONSE` (§2.7). | Opcional. |
| 6 | Qual endereço o cliente imprime em "realizada no servidor [IP:porta]" no PUT | O do **líder** (que executou), enviado dentro do `PUT_OK`. | Sim. |
| 7 | GET de key inexistente no servidor, mas o cliente já tem timestamp maior que zero | **Revertido:** devolve `NULL`, seguindo o texto literal, como instrui o `agentes/requisitos.md` §8.7. A espera passou a valer só para chave que existe em versão antiga. | Sim, vale confirmar: é o único ponto em que o sistema entrega ao cliente algo mais antigo do que ele já viu. |
| 8 | "Informações recebidas do líder" aparece também na primeira subida dos seguidores | Sim, aparece sempre que o `SYNC_RESPONSE` chegar. | Sim, se isso incomodar. |
| 9 | Menu do cliente antes do INIT | Se PUT/GET for escolhido antes do INIT, mostrar uma mensagem curta pedindo o INIT. | Opcional. |

---

## 15. Armadilhas comuns em Java (para o assistente lembrar)

1. Comparar `String` com `.equals()`, não com `==`.
2. `Scanner.nextInt()` deixa o `\n` no buffer. Prefira `nextLine()` e converta com `Integer.parseInt`.
3. Sempre feche `Socket` e streams (use `try-with-resources`, que existe no Java 8).
4. `wait()` e `notifyAll()` só podem ser chamados dentro de `synchronized` no **mesmo objeto**.
5. Use `while` (não `if`) ao redor de `wait()`.
6. O nome da classe pública deve ser igual ao nome do arquivo.
7. Um `ServerSocket` não pode usar uma porta já ocupada. Se um servidor travado ainda estiver rodando, a porta fica presa.
8. No CMD do Windows, o Ctrl+C encerra o programa. É assim que o professor "derruba" um servidor.
9. Lambdas existem no Java 8, mas classes aninhadas que estendem `Thread` deixam o relatório mais fácil de explicar.
10. Se `javac` reclamar de "unchecked", é aviso, não erro. Mesmo assim, prefira tipos genéricos explícitos.

---

## 16. Como compilar e executar

```
cd C:\kvstore
javac -cp gson-2.10.1.jar Servidor.java Cliente.java Mensagem.java
```

Servidores (um por console). Exemplo com o líder na porta 10097:

```
java -cp .;gson-2.10.1.jar Servidor
```

No Windows, o separador do classpath é **ponto e vírgula** (`;`). No Linux e no macOS seria dois pontos (`:`).

O programa pergunta, em ordem: IP próprio, porta própria, IP do líder, porta do líder.

| Servidor | IP próprio | Porta própria | IP do líder | Porta do líder |
|---|---|---|---|---|
| Líder | 127.0.0.1 | 10097 | 127.0.0.1 | 10097 |
| Seguidor 1 | 127.0.0.1 | 10098 | 127.0.0.1 | 10097 |
| Seguidor 2 | 127.0.0.1 | 10099 | 127.0.0.1 | 10097 |

Clientes (um por console):

```
java -cp .;gson-2.10.1.jar Cliente
```

Depois, `INIT` e informar os 3 servidores.