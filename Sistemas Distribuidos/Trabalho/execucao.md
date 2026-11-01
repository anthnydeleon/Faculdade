# Execução: o que digitar em cada console

> Arquivo de apoio. **Não vai no ZIP da entrega.**
> Tudo o que está em `bloco de código` é para digitar literalmente.

A pasta do projeto é sempre esta:

```
C:\Users\antho\OneDrive\Documentos\GitHub\Faculdade\Sistemas Distribuidos\Trabalho
```

O caminho tem espaços, então o `cd` **precisa** das aspas.

---

## Mapa dos 5 consoles

| Console | Papel | Porta | Observação |
|---|---|---|---|
| **1** | Servidor **LÍDER** | 10097 | O único que grava |
| **2** | Servidor seguidor | 10098 | Replica rápido |
| **3** | Servidor seguidor | 10099 | **O atrasado** — é ele que gera o `WAIT_FOR_RESPONSE` |
| **4** | Cliente A | — | Quem faz os PUT |
| **5** | Cliente B | — | Quem faz os GET |

Separar "quem escreve" de "quem lê" não é capricho: o cliente que faz o PUT **fica travado** esperando o `PUT_OK`, então ele não consegue fazer o GET na janela em que o servidor 10099 está atrasado. Por isso são dois clientes com papéis distintos.

---

# PARTE A — Antes de ligar a gravação

## A.1 Reinserir o atraso da demonstração

O `Servidor.java` **já está na versão de entrega**: o bloco de demonstração foi removido. Mas sem ele o `WAIT_FOR_RESPONSE` não acontece sob demanda, e o vídeo é obrigado a mostrar esse caso. Então ele precisa voltar **só para gravar**, e sair de novo antes do ZIP.

Abra o [Servidor.java](Servidor.java) e procure a linha **430**:

```java
    private static void tratarReplication(Mensagem pedido, Socket conexao) throws IOException {
```

Logo **abaixo** dela, cole este bloco inteiro:

```java

        // ===============================================================
        // DEMO - APAGAR DEPOIS DE GRAVAR O VIDEO
        //
        // Atraso proposital na replicacao, SO no servidor da porta 10099.
        // Nao faz parte do sistema: serve apenas para gravar o video.
        //
        // Com ele, este servidor fica 25 segundos com a versao ANTIGA da
        // chave, enquanto o lider e o outro seguidor ja tem a nova. Um
        // cliente que leu a versao nova em outro servidor e depois faz GET
        // aqui cai no caso WAIT_FOR_RESPONSE de forma previsivel.
        if (minhaPorta == 10099) {
            try {
                Thread.sleep(25000);
            } catch (InterruptedException e) {
                return;
            }
        }
        // FIM DO BLOCO DE DEMO - APAGAR DEPOIS DE GRAVAR
        // ===============================================================
```

**Por que 25 segundos:** é o tempo de trocar de console e repetir o GET algumas vezes sem pressa. Menos que isso e a janela fecha antes de o sorteio cair no 10099.

**Por que isso é necessário:** o líder só responde `PUT_OK` depois que **todos** os seguidores confirmam o `REPLICATION_OK`. Logo, um servidor que está no ar **nunca** fica atrás do cliente — a janela de atraso simplesmente não existe por conta própria. Segurar um seguidor de propósito é a única forma de produzir o caso.

**Efeito colateral:** enquanto o bloco existir, **todo PUT leva 25 segundos**. No vídeo isso vira vantagem: é o momento exato de narrar a replicação.

## A.2 Compilar

Abra um CMD qualquer:

```
cd "C:\Users\antho\OneDrive\Documentos\GitHub\Faculdade\Sistemas Distribuidos\Trabalho"
javac -cp gson-2.10.1.jar Servidor.java Cliente.java Mensagem.java
```

**Não deve aparecer nada.** Se aparecer erro, pare e resolva antes de gravar.

## A.3 Garantir que nenhum servidor antigo ficou rodando

Se uma execução anterior travou, a porta fica presa e o servidor novo morre **em silêncio**. Confira:

```
netstat -ano | findstr "10097 10098 10099"
```

Se aparecer alguma linha com `LISTENING`, pegue o número da última coluna (o PID) e derrube:

```
taskkill /F /PID <o_numero_que_apareceu>
```

> Não use `taskkill /F /IM java.exe`. Isso derruba também o servidor de linguagem Java do VSCode.

## A.4 Montar as 5 consoles visíveis ao mesmo tempo

Isso vale **2 pontos**. O jeito mais seguro é o Windows Terminal, numa janela só:

1. Tecla Windows, digite `Terminal`, abra.
2. `Alt+Shift++` → divide em duas colunas, o foco vai para a direita.
3. `Alt+Shift+-` → divide a coluna da direita. Agora a direita tem 2 painéis.
4. `Alt+Seta Esquerda` → volta para a coluna da esquerda.
5. `Alt+Shift+-` → divide a esquerda.
6. `Alt+Shift+-` de novo → a esquerda fica com 3 painéis.

Resultado: **3 painéis na esquerda (servidores) e 2 na direita (clientes)**. Use `Alt+Setas` para circular entre eles.

Em **cada um dos 5 painéis**, cole primeiro:

```
cd "C:\Users\antho\OneDrive\Documentos\GitHub\Faculdade\Sistemas Distribuidos\Trabalho"
```

> Se os atalhos derem trabalho, abra 5 janelas de CMD separadas e arrume na tela. O que o professor exige é ver as 5 ao mesmo tempo, não que estejam na mesma janela.

## A.5 Ensaiar uma vez com a gravação desligada

Faça a Parte B inteira uma vez antes de gravar. A parte do `WAIT_FOR_RESPONSE` depende de sorteio e você precisa sentir o ritmo.

---

# PARTE B — A sequência de execução

## Passo 1 — Console 1: servidor LÍDER

```
java -cp ".;gson-2.10.1.jar" Servidor
```

Responda:

```
IP do servidor: 127.0.0.1
Porta do servidor: 10097
IP do lider: 127.0.0.1
Porta do lider: 10097
```

> IP e porta **iguais** aos do líder = este processo é o líder. Depois disso ele fica em silêncio, só esperando conexões. Silêncio aqui é sinal de que está certo.

## Passo 2 — Console 2: seguidor 10098

```
java -cp ".;gson-2.10.1.jar" Servidor
```

```
IP do servidor: 127.0.0.1
Porta do servidor: 10098
IP do lider: 127.0.0.1
Porta do lider: 10097
```

Deve aparecer, quase na hora:

```
Informações recebidas do líder
```

> Isso é o seguidor se registrando no líder e pedindo o estado completo. Aparece mesmo estando tudo vazio, porque ele sempre pede a cópia ao subir.

## Passo 3 — Console 3: seguidor 10099 (o atrasado)

```
java -cp ".;gson-2.10.1.jar" Servidor
```

```
IP do servidor: 127.0.0.1
Porta do servidor: 10099
IP do lider: 127.0.0.1
Porta do lider: 10097
```

```
Informações recebidas do líder
```

## Passo 4 — Console 4: Cliente A

```
java -cp ".;gson-2.10.1.jar" Cliente
```

Aparece o menu. Digite `1` para o INIT e informe os três servidores:

```
Escolha: 1
IP do servidor 1: 127.0.0.1
Porta do servidor 1: 10097
IP do servidor 2: 127.0.0.1
Porta do servidor 2: 10098
IP do servidor 3: 127.0.0.1
Porta do servidor 3: 10099
```

## Passo 5 — Console 5: Cliente B

Exatamente o mesmo do Passo 4.

---

## Passo 6 — PUT inicial (Console 4, Cliente A)

```
Escolha: 2
Digite a key: curso
Digite o value: SD1
```

**Vai demorar 25 segundos.** É o esperado. Enquanto isso, observe:

- **Console 1 (líder)** imprime na hora:
  ```
  Cliente 127.0.0.1:xxxxx PUT key:curso value:SD1
  ```
  (ou `Encaminhando PUT key:curso value:SD1` em um dos seguidores, se o sorteio cair nele — aí o líder imprime a linha de cima logo em seguida)
- **Console 2 (10098)** imprime quase imediatamente:
  ```
  REPLICATION key:curso value:SD1 ts:1
  ```
- **Console 3 (10099)** fica 25 segundos em silêncio e só então imprime a mesma linha.
- Só aí o **Console 1** imprime:
  ```
  Enviando PUT_OK ao Cliente 127.0.0.1:xxxxx da key:curso ts:1
  ```
- E o **Console 4** fecha o ciclo:
  ```
  PUT_OK key: curso value SD1 timestamp 1 realizada no servidor 127.0.0.1:10097
  ```

## Passo 7 — GET normal (Console 5, Cliente B)

```
Escolha: 3
Digite a key: curso
```

Resposta imediata:

```
GET key: curso value: SD1 obtido do servidor 127.0.0.1:10098, meu timestamp 0 e do servidor 1
```

E no console do servidor que atendeu:

```
Cliente 127.0.0.1:xxxxx GET key:curso ts:0. Meu ts é 1, portanto devolvendo SD1
```

> A porta no final da linha muda a cada execução, porque o cliente sorteia o servidor. Qualquer um dos três serve.

---

## Passo 8 — WAIT_FOR_RESPONSE (a parte que exige ritmo)

Esta é a única parte com pressa. Leia os três sub-passos antes de começar.

### 8.1 — Console 4 (Cliente A): dispare o segundo PUT

```
Escolha: 2
Digite a key: curso
Digite o value: SD2
```

**A partir deste Enter você tem 25 segundos.** Nesse intervalo:

- líder (10097) e seguidor 10098 já têm `curso = SD2` com `ts 2`;
- o seguidor **10099 ainda tem `SD1` com `ts 1`**;
- o Cliente A está travado esperando o `PUT_OK`.

### 8.2 — Console 5 (Cliente B): primeiro, pegue o ts novo

```
Escolha: 3
Digite a key: curso
```

Olhe o final da linha de resposta:

- Se diz **`127.0.0.1:10097`** ou **`:10098`** → deu certo, você recebeu `SD2` com `ts 2`. Siga para o 8.3.
- Se diz **`:10099`** → caiu no atrasado antes da hora; ele devolveu `SD1` mesmo. **Repita o GET** até cair em 10097 ou 10098.

### 8.3 — Console 5: agora force a queda no 10099

Repita o GET da mesma key até a resposta vir do **10099**:

```
Escolha: 3
Digite a key: curso
```

Quando cair nele, aparece a linha diferenciada:

```
GET key: curso WAIT_FOR_RESPONSE do servidor 127.0.0.1:10099, meu timestamp 2
```

E no **Console 3 (10099)**:

```
Cliente 127.0.0.1:xxxxx GET key:curso ts:2. Meu ts é 1, portanto devolvendo WAIT_FOR_RESPONSE
```

> Repare que o menu do Cliente B **volta na hora**. Ele não ficou bloqueado — é esse o ponto da comunicação assíncrona.

### 8.4 — Espere os 25 segundos acabarem

Quando o `sleep` do 10099 terminar, três coisas acontecem em sequência:

1. **Console 3** imprime `REPLICATION key:curso value:SD2 ts:2`
2. **Console 4** destrava: `PUT_OK key: curso value SD2 timestamp 2 realizada no servidor 127.0.0.1:10097`
3. **Console 5** recebe a resposta atrasada, sozinho, sem você digitar nada:
   ```
   GET key: curso value: SD2 obtido do servidor 127.0.0.1:10099, meu timestamp 2 e do servidor 2
   ```

**O item 3 é a prova do requisito.** Se ele não aparecer, refaça o passo 8.

---

## Passo 9 — Servidor cai e volta

### 9.1 — Console 3: derrube o 10099

Clique no painel do Console 3 e aperte **`Ctrl+C`**.

### 9.2 — Console 4 (Cliente A): grave algo com ele fora do ar

```
Escolha: 2
Digite a key: cidade
Digite o value: SantoAndre
```

**Desta vez é instantâneo.** O líder tenta falar com o 10099, leva recusa de conexão e segue em frente sem ele — por isso não há os 25 segundos.

```
PUT_OK key: cidade value SantoAndre timestamp 3 realizada no servidor 127.0.0.1:10097
```

### 9.3 — Console 3: suba o 10099 de novo

```
java -cp ".;gson-2.10.1.jar" Servidor
```

```
IP do servidor: 127.0.0.1
Porta do servidor: 10099
IP do lider: 127.0.0.1
Porta do lider: 10097
```

Ele volta **sem nenhum dado** e imediatamente imprime:

```
Informações recebidas do líder
```

### 9.4 — Console 5 (Cliente B): prove que ele recuperou

```
Escolha: 3
Digite a key: cidade
```

Repita até a resposta vir do **10099**:

```
GET key: cidade value: SantoAndre obtido do servidor 127.0.0.1:10099, meu timestamp 0 e do servidor 3
```

> A chave `cidade` foi gravada enquanto o 10099 estava desligado. A **única** forma de ele conhecê-la é tendo copiado do líder ao voltar. É isso que esta linha demonstra.

---

# PARTE C — Depois de gravar (obrigatório)

## C.1 Remover de novo o bloco de demonstração

Abra o [Servidor.java](Servidor.java) e apague o bloco **inteiro** que você colou no passo A.1 — desde

```java
        // ===============================================================
        // DEMO - APAGAR DEPOIS DE GRAVAR O VIDEO
```

até

```java
        // FIM DO BLOCO DE DEMO - APAGAR DEPOIS DE GRAVAR
        // ===============================================================
```

Não deixe nenhum resto do `if (minhaPorta == 10099)`. Para conferir que não sobrou nada, rode:

```
findstr /N "minhaPorta == 10099" Servidor.java
```

**Não pode retornar nada.**

## C.2 Recompilar e conferir

```
javac -cp gson-2.10.1.jar Servidor.java Cliente.java Mensagem.java
```

Suba os 3 servidores e faça um PUT. **Agora ele tem que ser instantâneo.** Se ainda demorar, sobrou pedaço do bloco.

## C.3 Apagar os .class

```
del *.class
```

Mandar `.class` custa **2 pontos**.

## C.4 Montar o ZIP

Só estes cinco arquivos:

```
[SeuRA].pdf
Servidor.java
Cliente.java
Mensagem.java
gson-2.10.1.jar
```

**Não inclua:** `CLAUDE.md`, `execucao.md`, `roteiro.md`, `TUTORIAL-VIDEO.md`, a pasta `agentes/`, o PDF do enunciado, nem nenhum `.class`. Arquivos a mais custam **2 pontos**.

---

# Colinha de uma página

| # | Console | O que digitar |
|---|---|---|
| 1 | 1 | `Servidor` → `127.0.0.1` / `10097` / `127.0.0.1` / `10097` |
| 2 | 2 | `Servidor` → `127.0.0.1` / `10098` / `127.0.0.1` / `10097` |
| 3 | 3 | `Servidor` → `127.0.0.1` / `10099` / `127.0.0.1` / `10097` |
| 4 | 4 | `Cliente` → `1` → três pares `127.0.0.1` / `10097`, `10098`, `10099` |
| 5 | 5 | `Cliente` → `1` → os mesmos três pares |
| 6 | 4 | `2` → `curso` / `SD1` — espera 25 s |
| 7 | 5 | `3` → `curso` — GET normal |
| 8 | 4 | `2` → `curso` / `SD2` — **começa a janela de 25 s** |
| 9 | 5 | `3` → `curso` até cair em 10097/10098 |
| 10 | 5 | `3` → `curso` até cair em **10099** → `WAIT_FOR_RESPONSE` |
| 11 | — | espera os 25 s: a resposta tardia chega sozinha no Console 5 |
| 12 | 3 | `Ctrl+C` |
| 13 | 4 | `2` → `cidade` / `SantoAndre` — instantâneo |
| 14 | 3 | `Servidor` → `127.0.0.1` / `10099` / `127.0.0.1` / `10097` → `Informações recebidas do líder` |
| 15 | 5 | `3` → `cidade` até cair em **10099** |
