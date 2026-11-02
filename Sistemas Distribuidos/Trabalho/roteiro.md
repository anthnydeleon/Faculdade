# Roteiro do vídeo — KV Store distribuído

> Arquivo de apoio. **Não vai no ZIP da entrega.**
> Duração alvo: **4 min 30 s**. Limite do enunciado: 5 minutos.
> Tudo em **> citação** é para falar em voz alta, palavra por palavra.
> As ações de teclado estão detalhadas em [execucao.md](execucao.md).

---

## ⚠️ Leia antes de tudo

**As seções I a VIII são do relatório, não do vídeo.** Do que o enunciado pede, só a **II** e a **III** descrevem o screencast:

- **II** — compilação + funcionamento nas 5 consoles visíveis ao mesmo tempo
- **III** — GET normal, GET com `WAIT_FOR_RESPONSE`, e servidor caindo e voltando

As seções IV a VIII são texto escrito, com números de linha, dentro do PDF. Narrar tudo aquilo consumiria os 5 minutos e deixaria de fora justamente o que é cobrado no vídeo.

**O que este roteiro faz:** cumpre II e III por inteiro e aproveita os tempos mortos da demonstração para narrar, em alto nível, o conteúdo de IV, V, VI e VII. As falas não citam número de linha: isso fica no relatório, onde é obrigatório. Os números conferidos estão em [linhas-relatorio.md](linhas-relatorio.md).

### Dependência do Bloco 7

O `Servidor.java` está na versão de entrega e **não produz `WAIT_FOR_RESPONSE` sob demanda**. O motivo é estrutural: o líder só responde `PUT_OK` depois do `REPLICATION_OK` de todos, então nenhum servidor no ar fica atrás do cliente. Para gravar o Bloco 7 é preciso recolocar temporariamente o atraso descrito na seção A.1 do [execucao.md](execucao.md), e removê-lo depois (seção C.1).

**Sem esse bloco o vídeo não mostra o `WAIT_FOR_RESPONSE`, o que o enunciado lista como falta de 2 pontos.** Se você decidir mesmo não usá-lo, pule para a seção "Variante sem o Bloco 7" no fim deste arquivo.

---

# Checklist antes de apertar REC

- [ ] Atraso da seção A.1 reinserido e código recompilado
- [ ] Ensaio completo feito uma vez, sem gravar
- [ ] Nenhuma porta 10097/10098/10099 presa
- [ ] 5 painéis abertos, todos com o `cd` já feito
- [ ] Notificações do Windows e do celular silenciadas
- [ ] Fonte do terminal grande o bastante para ler no vídeo
- [ ] Trocar **[SEU RA]** pelo número real na fala do Bloco 1

---

# BLOCO 1 — Identificação · 0:00 → 0:15

**Ação:** tela parada, cinco painéis vazios visíveis. Antes de começar a gravar, cole em **cada um dos 5 painéis**:

```
cd "C:\Users\antho\OneDrive\Documentos\GitHub\Faculdade\Sistemas Distribuidos\Trabalho"
```

> "Olá, professor. Meu nome é Anthony de Leon, RA [SEU RA]. Este é o projeto do KV Store distribuído. Na tela estão as cinco consoles que vou usar: três servidores à esquerda e dois clientes à direita. Elas ficam visíveis ao mesmo tempo durante todo o vídeo."

---

# BLOCO 2 — Compilação · 0:15 → 0:40

**Ação:** no Console 1, digitar e executar:

```
javac -cp gson-2.10.1.jar Servidor.java Cliente.java Mensagem.java
```

> "Começo pela compilação, com o javac da JDK 8. São apenas as três classes permitidas: Servidor, Cliente e Mensagem. O parâmetro menos c p inclui o Gson, que é usado só para converter o objeto Mensagem em JSON na hora de enviar pela rede. A compilação terminou sem nenhum erro."

**Deixe o prompt limpo visível por 2 segundos** antes de seguir. É a prova de que compila.

---

# BLOCO 3 — Servidores e estruturas de dados · 0:40 → 1:10

**Ação:** no **Console 1**, subir o líder:

```
java -cp ".;gson-2.10.1.jar" Servidor
```

Responder aos quatro prompts:

```
IP do servidor: 127.0.0.1
Porta do servidor: 10097
IP do lider: 127.0.0.1
Porta do lider: 10097
```

> "Subo o primeiro servidor. Na inicialização ele captura do teclado o IP e a porta dele e o IP e a porta do líder. Como os dois pares são iguais, este processo se reconhece como líder."

**Ação:** no **Console 2**:

```
java -cp ".;gson-2.10.1.jar" Servidor
```

```
IP do servidor: 127.0.0.1
Porta do servidor: 10098
IP do lider: 127.0.0.1
Porta do lider: 10097
```

Quando aparecer `Informações recebidas do líder`:

> "Agora o segundo servidor. Ele se apresenta ao líder e já recebe uma cópia do estado, por isso imprime 'Informações recebidas do líder' mesmo com tudo vazio."

**Ação:** no **Console 3**:

```
java -cp ".;gson-2.10.1.jar" Servidor
```

```
IP do servidor: 127.0.0.1
Porta do servidor: 10099
IP do lider: 127.0.0.1
Porta do lider: 10097
```

> "E o terceiro. Cada servidor guarda quatro estruturas: a tabela de valores, que é o mapa de chave para valor; a tabela de timestamps, que guarda a versão de cada chave; o contador de timestamp, que só o líder usa como relógio lógico; e a lista de seguidores, com o endereço de cada servidor registrado."

---

# BLOCO 4 — Clientes · 1:10 → 1:30

**Ação:** nos **Consoles 4 e 5**, um de cada vez:

```
java -cp ".;gson-2.10.1.jar" Cliente
```

Em cada um, escolher a opção `1` e informar os três servidores:

```
Escolha: 1
IP do servidor 1: 127.0.0.1
Porta do servidor 1: 10097
IP do servidor 2: 127.0.0.1
Porta do servidor 2: 10098
IP do servidor 3: 127.0.0.1
Porta do servidor 3: 10099
```

> "Subo os dois clientes e faço o INIT em cada um, informando os três servidores. O cliente guarda esses endereços numa lista e mantém um mapa de chave para o último timestamp que ele já viu daquela chave. O cliente não sabe qual servidor é o líder: ele sorteia um a cada operação."

---

# BLOCO 5 — PUT, threads e concorrência · 1:30 → 2:10

**Ação:** no **Console 4** (Cliente A):

```
Escolha: 2
Digite a key: curso
Digite o value: SD1
```

Este PUT leva 25 segundos. Use o tempo para narrar as seções VI e VII:

> "Vou gravar a chave `curso` com o valor `SD1`. O líder recebeu o pedido, gerou o timestamp e gravou nas tabelas dele."

> "Sobre as threads do servidor: a thread principal só aceita conexões, e para cada conexão cria uma thread que atende aquele pedido do início ao fim. Além dela, o líder cria uma thread por seguidor para enviar a replicação, e assim os dois são avisados em paralelo. É por isso que o 10098 já replicou."

> "Sobre a proteção de concorrência: todo acesso às tabelas acontece dentro de um bloco synchronized, sempre no mesmo objeto de trava. No PUT, esse bloco gera o timestamp e grava. Repare que ele não faz rede nenhuma por dentro: a lista de seguidores é copiada ali e usada só depois, com a trava liberada, senão um PUT travaria todos os outros."

Quando o `PUT_OK` aparecer:

> "O líder esperou o REPLICATION_OK dos dois seguidores e só então respondeu PUT_OK com o timestamp 1."

---

# BLOCO 6 — GET normal · 2:10 → 2:25

**Ação:** no **Console 5** (Cliente B):

```
Escolha: 3
Digite a key: curso
```

> "Agora o primeiro caso que o enunciado pede: um GET normal. O cliente envia a chave e o último timestamp que tem dela, que é zero, porque este cliente nunca leu nada. O servidor sorteado tem timestamp 1, que é maior ou igual, então responde na hora com o valor e com o próprio timestamp."

Aponte com o mouse a linha no console do servidor que atendeu:

> "Do lado do servidor fica registrado: o timestamp do cliente era zero, o meu é 1, portanto devolvo `SD1`."

---

# BLOCO 7 — WAIT_FOR_RESPONSE · 2:25 → 3:30

**Ação:** no **Console 4**. *(a partir deste Enter começa a janela de 25 segundos)*

```
Escolha: 2
Digite a key: curso
Digite o value: SD2
```

> "Para mostrar o segundo caso, atualizo a mesma chave para `SD2`. Eu coloquei um atraso artificial na replicação, só no servidor 10099 e só para esta gravação: ele não faz parte do sistema e está removido da versão que entreguei. Com ele, o líder e o 10098 já têm a versão nova, e o 10099 continua com a antiga."

**Ação:** no **Console 5**, repetir até a resposta vir do **10097 ou 10098**:

```
Escolha: 3
Digite a key: curso
```

> "Pelo outro cliente eu leio a chave. Caiu num servidor atualizado: recebi `SD2` com timestamp 2. Agora este cliente sabe que existe a versão 2."

**Ação:** no **Console 5**, repetir até a resposta vir do **10099**:

```
Escolha: 3
Digite a key: curso
```

> "Repito o GET até o sorteio cair no servidor atrasado... caiu no 10099. Ele compara: o timestamp dele é 1, o do cliente é 2. Ele está atrás do que o cliente já viu, então não pode devolver o valor antigo. Responde WAIT_FOR_RESPONSE e encerra a conexão."

> "Repare que o menu do cliente voltou na mesma hora: ele não ficou bloqueado. Quem vai esperar é o servidor, numa thread criada só para isso, que fica em wait dentro de um while até a chave alcançar a versão que o cliente pediu."

**Ação:** não digite nada. Espere o atraso terminar.

> "O wait libera a trava enquanto dorme, então o servidor continua atendendo normalmente. Quando a replicação chegar, o notifyAll acorda essa thread..."

Quando a linha tardia aparecer sozinha no Console 5, aponte com o mouse:

> "Pronto. O valor `SD2`, vindo do 10099, chegou sozinho no cliente, sem eu digitar nada. Quem recebeu foi a thread de escuta do cliente, que fica esperando numa porta que ele abriu no INIT. O cliente nunca leu um valor mais antigo do que o que já tinha visto."

---

# BLOCO 8 — Servidor cai e volta · 3:30 → 4:15

**Ação:** clicar no **Console 3** e apertar:

```
Ctrl+C
```

> "Terceiro caso: derrubo um servidor que não é o líder, o 10099, com Ctrl+C. Ele perde tudo o que tinha."

**Ação:** no **Console 4**. *(desta vez a confirmação é instantânea)*

```
Escolha: 2
Digite a key: cidade
Digite o value: SantoAndre
```

> "Com ele fora do ar, gravo uma chave nova: `cidade`, igual a `SantoAndre`. Desta vez o PUT foi instantâneo, porque o líder tentou falar com o 10099, levou recusa de conexão e seguiu sem ele. Um servidor caído não pode prender o PUT para sempre."

**Ação:** no **Console 3**, subir o servidor de novo:

```
java -cp ".;gson-2.10.1.jar" Servidor
```

```
IP do servidor: 127.0.0.1
Porta do servidor: 10099
IP do lider: 127.0.0.1
Porta do lider: 10097
```

> "Subo o 10099 de novo. Ele volta completamente vazio. Por não ser o líder, ele dispara uma thread que se apresenta ao líder, recebe as duas tabelas completas e mescla, aceitando só o que for mais novo. Por isso ele imprime 'Informações recebidas do líder'."

**Ação:** no **Console 5**, repetir até a resposta vir do **10099**:

```
Escolha: 3
Digite a key: cidade
```

> "Para provar a recuperação, faço um GET da chave `cidade` até cair justamente no 10099... caiu, e ele devolveu `SantoAndre`. Essa chave foi gravada enquanto ele estava desligado, então a única forma de ele conhecê-la é tendo copiado do líder ao voltar."

---

# BLOCO 9 — Fecho · 4:15 → 4:30

> "Resumindo: toda a comunicação é por TCP transportando a classe Mensagem; o servidor atende cada conexão numa thread separada; o cliente tem uma thread própria para receber respostas assíncronas; e todas as estruturas compartilhadas são protegidas com synchronized num único objeto de trava. O detalhamento linha a linha está no relatório. Obrigado, professor."

---

# Mapa: seção do enunciado → bloco do vídeo

| Seção | Onde aparece |
|---|---|
| I — Nome e RA | Bloco 1 |
| II — Compilação e 5 consoles | Bloco 2, e as 5 consoles o tempo todo |
| III — GET normal | Bloco 6 |
| III — WAIT_FOR_RESPONSE | Bloco 7 |
| III — Servidor cai e volta | Bloco 8 |
| IV — Funcionalidades e estruturas do servidor | Blocos 3, 5, 6, 7, 8 |
| V — Funcionalidades e estruturas do cliente | Blocos 4, 6, 7 |
| VI — Threads (servidor e cliente) | Blocos 5, 7, 8 |
| VII — Proteção de acessos concorrentes | Bloco 5 |
| VIII — Fontes | Só no relatório |

---


# Riscos e como reagir ao vivo

| Se acontecer | O que fazer |
|---|---|
| O sorteio não cai no 10099 e os 25 s acabam | **Não corte.** Diga "o cliente sorteia o servidor, vou repetir" e refaça o Bloco 7. Há margem até os 5 minutos. |
| Um servidor não sobe e some do nada | Porta presa de um teste anterior. Pare, mate o PID (seção A.3 do [execucao.md](execucao.md)) e recomece. |
| Você digitou a key errada | Siga em frente e refaça a operação. Errar e corrigir não desconta nada. |
| Passou de 5 minutos | Regrave cortando a narração dos blocos 3, 4 e 5, que é a parte opcional. |

## Três coisas que você não deve fazer

1. **Não minimize nenhum painel.** Perder as 5 consoles ao mesmo tempo custa 2 pontos.
2. **Não corte o vídeo** durante o Bloco 7. O professor precisa ver que a resposta chegou sozinha.
3. **Não omita o atraso artificial.** Falar dele em 8 segundos é mais seguro do que deixar o professor descobrir um PUT de 25 segundos sem explicação.

---

# Variante sem o Bloco 7

Se você mesmo assim não quiser reinserir o atraso, o vídeo fica sem o `WAIT_FOR_RESPONSE`. O enunciado trata isso como "o vídeo não mostra o funcionamento pedido", que é **menos 2 pontos**.

Nesse caso, remova o Bloco 7 inteiro, a duração cai para cerca de 3 min 25 s, e no Bloco 9 acrescente esta frase, para que ao menos fique claro que o caso está implementado:

> "O caso do WAIT_FOR_RESPONSE está implementado no tratamento do GET, com uma thread de espera dedicada, mas ele não aparece sob demanda neste vídeo: como o líder só confirma o PUT depois do REPLICATION_OK de todos os seguidores, nenhum servidor no ar chega a ficar atrás do cliente."

Isso é honesto e pode render a consideração do professor, mas **não substitui a demonstração** e provavelmente não evita o desconto. Minha recomendação continua sendo gravar o Bloco 7 com o atraso temporário.