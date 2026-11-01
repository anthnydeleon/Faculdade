import com.google.gson.Gson;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Scanner;

/*
 * =====================================================================
 * Servidor.java
 *
 * Um dos tres servidores do KV Store. Um deles e o lider (fixo, escolhido
 * na inicializacao): so ele grava. Os outros dois sao seguidores: guardam
 * uma copia e respondem GET.
 *
 * O mesmo programa cumpre os dois papeis: qual deles vale e decidido na
 * inicializacao, comparando o endereco proprio com o endereco do lider.
 * =====================================================================
 */
public class Servidor {

    // =================================================================
    // ESTRUTURAS DE DADOS DO SERVIDOR
    // Todas sao static porque existe um unico servidor por processo.
    // =================================================================

    /*
     * TABELA DE VALORES.
     * Para que serve: armazenar o conteudo do KV Store neste servidor.
     * O que armazena: chave -> valor (o ultimo valor conhecido da chave).
     */
    private static HashMap<String, String> tabelaValores = new HashMap<String, String>();

    /*
     * TABELA DE TIMESTAMPS.
     * Para que serve: diz qual a VERSAO do valor guardado acima. E o que
     * permite comparar quem esta mais atualizado, o cliente ou o servidor.
     * O que armazena: chave -> timestamp (numero da versao daquela chave).
     * Valor e versao ficam em duas tabelas distintas indexadas pela mesma
     * chave, em vez de um unico mapa de objetos compostos.
     */
    private static HashMap<String, Long> tabelaTimestamps = new HashMap<String, Long>();

    /*
     * CONTADOR DE TIMESTAMP (so o lider usa).
     * Para que serve: e o relogio logico do sistema.
     * O que armazena: o ultimo numero de versao gerado. Cada PUT soma 1.
     */
    private static long contadorTimestamp = 0;

    /*
     * LISTA DE SEGUIDORES (so o lider usa).
     * Para que serve: o lider precisa saber para quem mandar REPLICATION.
     * O que armazena: o endereco de cada seguidor no formato "ip:porta".
     * E um HashSet para que o mesmo seguidor, se reiniciar e se registrar
     * de novo, nao apareca duas vezes na lista.
     */
    private static HashSet<String> seguidores = new HashSet<String>();

    /*
     * OBJETO DE TRAVA (lock).
     * Para que serve: serializar o acesso as estruturas acima. Toda leitura
     * ou escrita nas tabelas, no contador e na lista de seguidores acontece
     * dentro de synchronized (trava), de modo que duas threads nunca operem
     * sobre os mesmos dados ao mesmo tempo.
     * E tambem o monitor usado nas chamadas wait() e notifyAll() da espera
     * associada ao WAIT_FOR_RESPONSE.
     */
    private static final Object trava = new Object();

    // =================================================================
    // IDENTIFICACAO DESTE SERVIDOR (preenchida na inicializacao)
    // =================================================================

    /** IP em que este servidor escuta. */
    private static String meuIp;

    /** Porta em que este servidor escuta. */
    private static int minhaPorta;

    /** IP do lider do sistema (fixo, nao ha troca de lider). */
    private static String ipLider;

    /** Porta do lider do sistema. */
    private static int portaLider;

    /** true se este processo e o lider. */
    private static boolean souLider;

    /** Conversor objeto <-> JSON. Pode ser reutilizado por varias threads. */
    private static Gson gson = new Gson();

    // =================================================================
    // [Enunciado 5a - inicializacao do servidor]
    // Le do teclado, nesta ordem: IP proprio, porta propria, IP do lider
    // e porta do lider. E lider se os dois pares forem iguais.
    // =================================================================
    public static void main(String[] args) {
        Scanner teclado = new Scanner(System.in);

        System.out.print("IP do servidor: ");
        meuIp = teclado.nextLine().trim();

        System.out.print("Porta do servidor: ");
        minhaPorta = Integer.parseInt(teclado.nextLine().trim());

        System.out.print("IP do lider: ");
        ipLider = teclado.nextLine().trim();

        System.out.print("Porta do lider: ");
        portaLider = Integer.parseInt(teclado.nextLine().trim());

        // Detecta o papel deste servidor comparando os dois enderecos.
        souLider = meuIp.equals(ipLider) && minhaPorta == portaLider;

        // [Enunciado 5b - atendimento simultaneo com threads]
        // A thread principal fica so aceitando conexoes. Cada conexao
        // aceita vira uma thread separada, para que o servidor continue
        // atendendo outros clientes enquanto trata um pedido.
        try {
            // A porta de escuta e aberta ANTES de qualquer outra coisa. Se ela
            // falhar (porta ja ocupada, por exemplo), este servidor nao tem
            // como atender ninguem, e entao nem deve se apresentar ao lider:
            // o lider ficaria com o endereco de um servidor que nao existe.
            ServerSocket escuta = new ServerSocket(minhaPorta);

            // [Enunciado 5g - recuperacao do servidor]
            // Todo servidor nao lider se apresenta ao lider assim que sobe.
            // Isso resolve duas coisas de uma vez: o lider passa a saber para
            // quem enviar o REPLICATION, e este servidor recebe de volta os
            // dados que nao tem (tanto na primeira subida quanto depois de
            // cair e voltar).
            if (!souLider) {
                new RegistraNoLider().start();
            }

            while (true) {
                Socket conexao = escuta.accept();
                new AtendeConexao(conexao).start();
            }
        } catch (IOException e) {
            // Se a porta de escuta falhar, nao ha o que fazer: o servidor encerra.
        }
    }

    // =================================================================
    // [Enunciado 5b - atendimento simultaneo com threads] CLASSE DE THREAD
    //
    // Papel desta thread: atender UMA conexao TCP do inicio ao fim.
    // Le a Mensagem que chegou, decide o que fazer conforme o tipo e
    // responde pela mesma conexao. Enquanto ela trabalha, a thread
    // principal continua livre para aceitar novas conexoes.
    // =================================================================
    private static class AtendeConexao extends Thread {

        /** A conexao TCP que esta thread foi criada para atender. */
        private Socket conexao;

        public AtendeConexao(Socket conexao) {
            this.conexao = conexao;
        }

        public void run() {
            try {
                Mensagem pedido = receberMensagem(conexao);

                if (pedido != null) {
                    if (Mensagem.PUT.equals(pedido.getTipo())) {
                        tratarPut(pedido, conexao);
                    } else if (Mensagem.GET.equals(pedido.getTipo())) {
                        tratarGet(pedido, conexao);
                    } else if (Mensagem.REPLICATION.equals(pedido.getTipo())) {
                        tratarReplication(pedido, conexao);
                    } else if (Mensagem.SYNC_REQUEST.equals(pedido.getTipo())) {
                        tratarSyncRequest(pedido, conexao);
                    }
                }
            } catch (IOException e) {
                // A outra ponta fechou a conexao, ou esta requisicao nao pode
                // ser concluida (por exemplo, o encaminhamento do PUT falhou).
                // Nada a fazer aqui: o finally abaixo encerra a conexao.
            } finally {
                // O fechamento fica no finally para acontecer SEMPRE, de erro
                // ou de sucesso. Isso importa: se a conexao ficasse aberta
                // depois de uma falha, quem esta do outro lado esperaria uma
                // resposta que nunca viria. Com o fechamento, o readLine()
                // remoto devolve null e aquele fluxo e encerrado.
                try {
                    conexao.close();
                } catch (IOException e) {
                    // Ja estava fechada.
                }
            }
        }
    }

    // =================================================================
    // [Enunciado 5g - recuperacao do servidor] CLASSE DE THREAD
    //
    // Papel desta thread: apresentar este seguidor ao lider (mensagem
    // SYNC_REQUEST) e trazer de volta o estado completo das tabelas.
    // Roda em segundo plano para nao atrasar a abertura da porta de escuta
    // deste servidor. Se o lider ainda nao estiver no ar, a conexao falha e
    // ela tenta de novo a cada 1 segundo, sem imprimir nada, ate conseguir.
    // Termina assim que a resposta do lider chega.
    // =================================================================
    private static class RegistraNoLider extends Thread {

        public void run() {
            while (true) {
                try {
                    Socket conexaoLider = new Socket(ipLider, portaLider);

                    // Envia o proprio endereco, que e o que o lider precisa
                    // guardar para depois conseguir ligar de volta.
                    Mensagem pedido = new Mensagem(Mensagem.SYNC_REQUEST);
                    pedido.setIpServidor(meuIp);
                    pedido.setPortaServidor(minhaPorta);

                    enviarMensagem(conexaoLider, pedido);
                    Mensagem resposta = receberMensagem(conexaoLider);
                    conexaoLider.close();

                    if (resposta != null) {
                        // [Enunciado 5g - recuperacao do servidor]
                        // Mescla nas tabelas locais o estado que o lider enviou.
                        //
                        // [Enunciado 5 (observacoes) - protecao de acesso concorrente]
                        synchronized (trava) {
                            HashMap<String, String> dados = resposta.getDados();
                            HashMap<String, Long> tempos = resposta.getTempos();

                            if (dados != null && tempos != null) {
                                // Aceita apenas o que for mais novo do que o
                                // que ja esta aqui. Numa subida depois de uma
                                // queda as tabelas estao vazias e tudo entra;
                                // a comparacao protege o caso de um
                                // REPLICATION recente ter chegado antes desta
                                // resposta, com versao mais nova que a da
                                // copia enviada pelo lider.
                                for (String chave : dados.keySet()) {
                                    Long ts = tempos.get(chave);
                                    if (ts != null && ts.longValue() > timestampDaChave(chave)) {
                                        tabelaValores.put(chave, dados.get(chave));
                                        tabelaTimestamps.put(chave, ts);
                                    }
                                }
                            }

                            // Acorda as threads de espera do GET: a versao que
                            // elas aguardam pode ter acabado de chegar junto
                            // com este estado.
                            trava.notifyAll();
                        }

                        System.out.println("Informa\u00e7\u00f5es recebidas do l\u00edder");
                        return;
                    }
                } catch (IOException e) {
                    // O lider ainda nao esta no ar. Nada e impresso: so tenta
                    // de novo depois da pausa abaixo.
                }

                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    return;
                }
            }
        }
    }

    // =================================================================
    // [Enunciado 5c - recebe PUT]
    // =================================================================
    private static void tratarPut(Mensagem pedido, Socket conexao) throws IOException {

        if (!souLider) {
            // ---- Caso 1: nao sou o lider. Encaminho o pedido para ele. ----
            System.out.println("Encaminhando PUT key:" + pedido.getChave()
                    + " value:" + pedido.getValor());

            // Abre uma conexao nova com o lider e repassa o MESMO pedido,
            // preservando os dados do cliente original.
            Socket conexaoLider = new Socket(ipLider, portaLider);
            enviarMensagem(conexaoLider, pedido);
            Mensagem respostaLider = receberMensagem(conexaoLider);
            conexaoLider.close();

            // Devolve o PUT_OK ao cliente pela mesma conexao em que ele espera.
            if (respostaLider != null) {
                enviarMensagem(conexao, respostaLider);
            }
            return;
        }

        // ---- Caso 2: sou o lider. Executo a gravacao. ----
        System.out.println("Cliente " + pedido.getIpCliente() + ":" + pedido.getPortaCliente()
                + " PUT key:" + pedido.getChave() + " value:" + pedido.getValor());

        long novoTimestamp;
        ArrayList<String> destinos;

        // [Enunciado 5 (observacoes) - protecao de acesso concorrente]
        // Gerar o timestamp, gravar nas tabelas e copiar a lista de
        // seguidores formam uma operacao unica, que precisa ocorrer sem
        // interferencia de outra thread. Nenhuma operacao de rede ocorre
        // aqui dentro, apenas manipulacao de memoria, de modo que a trava fica
        // retida por muito pouco tempo.
        synchronized (trava) {
            contadorTimestamp = contadorTimestamp + 1;
            novoTimestamp = contadorTimestamp;
            tabelaValores.put(pedido.getChave(), pedido.getValor());
            tabelaTimestamps.put(pedido.getChave(), Long.valueOf(novoTimestamp));

            // A copia da lista e feita aqui, mas usada so depois da trava
            // liberada: reter a trava durante a troca de mensagens pela rede
            // bloquearia todos os outros PUT e GET deste servidor.
            destinos = new ArrayList<String>(seguidores);
        }

        // [Enunciado 5c - recebe PUT] replicacao nos outros servidores
        // Cada seguidor recebe o REPLICATION em sua propria thread, para que os
        // dois sejam avisados ao mesmo tempo, e nao um depois do outro.
        ArrayList<ReplicaEmSeguidor> replicacoes = new ArrayList<ReplicaEmSeguidor>();
        for (int i = 0; i < destinos.size(); i++) {
            ReplicaEmSeguidor replicacao = new ReplicaEmSeguidor(destinos.get(i),
                    pedido.getChave(), pedido.getValor(), novoTimestamp);
            replicacoes.add(replicacao);
            replicacao.start();
        }

        // [Enunciado 5e - recebe REPLICATION_OK]
        // O join() faz esta thread esperar cada thread de replicacao terminar,
        // e cada uma delas so termina depois de ler o REPLICATION_OK do seu
        // seguidor. Portanto, quando este laco acaba, todos os seguidores que
        // estao no ar ja confirmaram a copia: e a hora de responder o PUT_OK.
        for (int i = 0; i < replicacoes.size(); i++) {
            try {
                replicacoes.get(i).join();
            } catch (InterruptedException e) {
                // Nada a fazer: segue para a proxima.
            }
        }

        System.out.println("Enviando PUT_OK ao Cliente " + pedido.getIpCliente()
                + ":" + pedido.getPortaCliente() + " da key:" + pedido.getChave()
                + " ts:" + novoTimestamp);

        Mensagem resposta = new Mensagem(Mensagem.PUT_OK);
        resposta.setChave(pedido.getChave());
        resposta.setValor(pedido.getValor());
        resposta.setTimestamp(novoTimestamp);
        // Informa quem executou de fato a gravacao (o lider), para o cliente
        // poder imprimir "realizada no servidor [IP:porta]".
        resposta.setIpServidor(meuIp);
        resposta.setPortaServidor(minhaPorta);

        enviarMensagem(conexao, resposta);
    }

    // =================================================================
    // [Enunciado 5c - recebe PUT] CLASSE DE THREAD
    //
    // Papel desta thread: levar um REPLICATION a UM seguidor e esperar o
    // REPLICATION_OK dele. O lider cria uma thread destas por seguidor, para
    // avisar todos em paralelo.
    // Se o seguidor estiver fora do ar, a conexao falha e a thread termina sem
    // efeito: aquele seguidor e ignorado neste PUT, o que evita bloquear a
    // resposta ao cliente, e recupera os dados perdidos quando voltar, pelo
    // SYNC_REQUEST do item 5g.
    // =================================================================
    private static class ReplicaEmSeguidor extends Thread {

        /** Endereco do seguidor que esta thread vai avisar, como "ip:porta". */
        private String endereco;

        /** Chave a ser copiada pelo seguidor. */
        private String chave;

        /** Valor associado a essa chave. */
        private String valor;

        /** Versao (timestamp) gerada pelo lider para essa gravacao. */
        private long timestamp;

        public ReplicaEmSeguidor(String endereco, String chave, String valor, long timestamp) {
            this.endereco = endereco;
            this.chave = chave;
            this.valor = valor;
            this.timestamp = timestamp;
        }

        public void run() {
            String[] partes = endereco.split(":");
            try {
                Socket conexaoSeguidor = new Socket(partes[0], Integer.parseInt(partes[1]));

                Mensagem replicacao = new Mensagem(Mensagem.REPLICATION);
                replicacao.setChave(chave);
                replicacao.setValor(valor);
                replicacao.setTimestamp(timestamp);
                replicacao.setIpServidor(meuIp);
                replicacao.setPortaServidor(minhaPorta);

                enviarMensagem(conexaoSeguidor, replicacao);

                // [Enunciado 5e - recebe REPLICATION_OK]
                // A confirmacao chega pela mesma conexao. A leitura abaixo e o
                // que faz esta thread esperar o seguidor terminar de gravar.
                receberMensagem(conexaoSeguidor);
                conexaoSeguidor.close();
            } catch (IOException e) {
                // Seguidor fora do ar: ignorado neste PUT.
            }
        }
    }

    // =================================================================
    // [Enunciado 5d - recebe REPLICATION]
    // Chega no seguidor. Ele copia nas suas tabelas o que o lider gravou e
    // confirma ao lider com um REPLICATION_OK.
    // =================================================================
    private static void tratarReplication(Mensagem pedido, Socket conexao) throws IOException {
        
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
                Thread.sleep(5000);
            } catch (InterruptedException e) {
                return;
            }
        }
        // FIM DO BLOCO DE DEMO - APAGAR DEPOIS DE GRAVAR
        // ===============================================================

        System.out.println("REPLICATION key:" + pedido.getChave()
                + " value:" + pedido.getValor()
                + " ts:" + pedido.getTimestamp());

        // [Enunciado 5 (observacoes) - protecao de acesso concorrente]
        synchronized (trava) {
            Long atual = tabelaTimestamps.get(pedido.getChave());

            // So sobrescreve se a versao que chegou for mais nova que a
            // guardada. Assim uma replicacao que se atrasou na rede nunca
            // apaga uma mais recente que chegou antes dela.
            if (atual == null || pedido.getTimestamp() > atual.longValue()) {
                tabelaValores.put(pedido.getChave(), pedido.getValor());
                tabelaTimestamps.put(pedido.getChave(), Long.valueOf(pedido.getTimestamp()));
            }

            // Acorda as threads de espera do GET (item 5f): alguma delas pode
            // estar esperando exatamente esta versao desta chave.
            trava.notifyAll();
        }

        Mensagem resposta = new Mensagem(Mensagem.REPLICATION_OK);
        resposta.setChave(pedido.getChave());
        resposta.setTimestamp(pedido.getTimestamp());
        resposta.setIpServidor(meuIp);
        resposta.setPortaServidor(minhaPorta);

        enviarMensagem(conexao, resposta);
    }

    // =================================================================
    // [Enunciado 5f - recebe GET]
    // O cliente Cx envia a chave Kx e o timestamp Tx que ja viu dela.
    // A resposta nunca contem uma versao anterior a Tx: se este servidor
    // estiver atras, a entrega e adiada em vez de devolver o valor local.
    // =================================================================
    private static void tratarGet(Mensagem pedido, Socket conexao) throws IOException {

        String valorLocal;
        long timestampLocal;

        // [Enunciado 5 (observacoes) - protecao de acesso concorrente]
        // Le as duas tabelas de uma vez so, para que o valor e o timestamp
        // devolvidos sejam sempre da mesma versao.
        synchronized (trava) {
            valorLocal = tabelaValores.get(pedido.getChave());
            timestampLocal = timestampDaChave(pedido.getChave());
        }

        // Indica se a chave existe neste servidor. Quando nao existe, o valor
        // devolvido e o texto "NULL", que e uma String comum e nao o null do
        // Java.
        boolean chaveExiste = (valorLocal != null);

        // [Enunciado 5f - recebe GET] servidor atrasado: WAIT_FOR_RESPONSE
        // A chave existe aqui, mas em uma versao mais antiga do que a que o
        // cliente ja viu. Responder com o valor local entregaria uma versao
        // anterior a que o cliente ja leu, por isso a resposta e adiada.
        //
        // A condicao exige que a chave exista. Uma chave ausente nao tem
        // versao com que comparar o Tx do cliente, e o caso e resolvido mais
        // abaixo devolvendo o texto "NULL" com timestamp 0.
        if (chaveExiste && timestampLocal < pedido.getTimestamp()) {
            System.out.println("Cliente " + pedido.getIpCliente() + ":" + pedido.getPortaCliente()
                    + " GET key:" + pedido.getChave() + " ts:" + pedido.getTimestamp()
                    + ". Meu ts \u00e9 " + timestampLocal
                    + ", portanto devolvendo " + Mensagem.WAIT_FOR_RESPONSE);

            Mensagem aviso = new Mensagem(Mensagem.WAIT_FOR_RESPONSE);
            aviso.setChave(pedido.getChave());
            aviso.setValor(Mensagem.WAIT_FOR_RESPONSE);
            aviso.setTimestamp(timestampLocal);
            aviso.setIpServidor(meuIp);
            aviso.setPortaServidor(minhaPorta);

            enviarMensagem(conexao, aviso);

            // A comunicacao com o cliente e cortada aqui: quem fecha o socket
            // e a thread AtendeConexao, assim que este metodo retornar. A
            // thread de espera abaixo e que vai ligar de volta para o cliente.
            new EsperaAtualizacao(pedido).start();
            return;
        }

        // Chave que este servidor nao tem: devolve o texto "NULL" e o ts 0.
        if (!chaveExiste) {
            valorLocal = "NULL";
        }

        System.out.println("Cliente " + pedido.getIpCliente() + ":" + pedido.getPortaCliente()
                + " GET key:" + pedido.getChave() + " ts:" + pedido.getTimestamp()
                + ". Meu ts \u00e9 " + timestampLocal
                + ", portanto devolvendo " + valorLocal);

        Mensagem resposta = new Mensagem(Mensagem.GET_RESPONSE);
        resposta.setChave(pedido.getChave());
        resposta.setValor(valorLocal);
        // Junto com o valor vai sempre o timestamp DESTE servidor para a chave.
        resposta.setTimestamp(timestampLocal);
        resposta.setIpServidor(meuIp);
        resposta.setPortaServidor(minhaPorta);

        enviarMensagem(conexao, resposta);
    }

    // =================================================================
    // [Enunciado 5f - recebe GET] CLASSE DE THREAD
    //
    // Papel desta thread: esperar, sem travar o servidor, que a chave pedida
    // alcance uma versao igual ou maior que a que o cliente ja viu, e entao
    // entregar a resposta ao cliente.
    // O wait() libera a trava enquanto a thread dorme, de modo que o servidor
    // continua atendendo PUT, GET e REPLICATION durante toda a espera. Quando
    // um REPLICATION, ou a sincronizacao com o lider, chama notifyAll(), esta
    // thread acorda, reconfere a condicao e, se ela for satisfeita, conecta na
    // porta de escuta do cliente para entregar a resposta.
    // =================================================================
    private static class EsperaAtualizacao extends Thread {

        /*
         * O pedido de GET original. Guarda tudo que esta thread precisa: a
         * chave esperada, o timestamp do cliente (Tx) e o IP e a porta de
         * escuta do cliente, para onde a resposta sera enviada depois.
         */
        private Mensagem pedido;

        public EsperaAtualizacao(Mensagem pedido) {
            this.pedido = pedido;
        }

        public void run() {
            String valorLocal;
            long timestampLocal;

            // [Enunciado 5 (observacoes) - protecao de acesso concorrente]
            synchronized (trava) {
                // O laco e while, e nao if, porque notifyAll() acorda TODAS as
                // threads de espera, inclusive as que aguardam outras chaves.
                // Cada uma precisa reconferir a sua propria condicao e voltar
                // a dormir se ainda nao for a vez dela.
                while (timestampDaChave(pedido.getChave()) < pedido.getTimestamp()) {
                    try {
                        trava.wait();
                    } catch (InterruptedException e) {
                        return;
                    }
                }

                valorLocal = tabelaValores.get(pedido.getChave());
                timestampLocal = timestampDaChave(pedido.getChave());
            }

            try {
                // Liga de volta para o cliente, no IP e na porta de escuta que
                // vieram dentro da Mensagem do GET.
                Socket conexaoCliente = new Socket(pedido.getIpCliente(), pedido.getPortaCliente());

                Mensagem resposta = new Mensagem(Mensagem.GET_RESPONSE);
                resposta.setChave(pedido.getChave());
                resposta.setValor(valorLocal);
                resposta.setTimestamp(timestampLocal);
                resposta.setIpServidor(meuIp);
                resposta.setPortaServidor(minhaPorta);

                enviarMensagem(conexaoCliente, resposta);
                conexaoCliente.close();
            } catch (IOException e) {
                // Cliente inacessivel. Nada a fazer.
            }
        }
    }

    /*
     * Devolve a versao (timestamp) que este servidor tem da chave, ou 0 se ele
     * nunca viu essa chave. Quem chama deve estar dentro de synchronized
     * (trava), porque o metodo le a tabela de timestamps.
     */
    private static long timestampDaChave(String chave) {
        Long t = tabelaTimestamps.get(chave);
        return (t == null) ? 0L : t.longValue();
    }

    // =================================================================
    // [Enunciado 5g - recuperacao do servidor]
    // O lider recebe a apresentacao de um seguidor: anota o endereco dele na
    // lista de seguidores e devolve uma copia completa das duas tabelas, para
    // que o seguidor fique igual ao lider.
    // =================================================================
    private static void tratarSyncRequest(Mensagem pedido, Socket conexao) throws IOException {
        Mensagem resposta = new Mensagem(Mensagem.SYNC_RESPONSE);

        // [Enunciado 5 (observacoes) - protecao de acesso concorrente]
        // O registro e a copia das tabelas acontecem sob a mesma trava,
        // para o seguidor nunca receber uma tabela pela metade. As copias sao
        // HashMap novos, de modo que a conversao para JSON trabalhe sobre um
        // retrato fixo, sem risco de outra thread alterar as tabelas no meio.
        synchronized (trava) {
            seguidores.add(pedido.getIpServidor() + ":" + pedido.getPortaServidor());
            resposta.setDados(new HashMap<String, String>(tabelaValores));
            resposta.setTempos(new HashMap<String, Long>(tabelaTimestamps));
        }

        resposta.setIpServidor(meuIp);
        resposta.setPortaServidor(minhaPorta);

        enviarMensagem(conexao, resposta);
    }

    // =================================================================
    // ENVIO E RECEBIMENTO DA CLASSE Mensagem PELA REDE
    //
    // O protocolo e "uma mensagem por linha": o objeto Mensagem e convertido
    // em uma linha de texto JSON, e o outro lado le essa linha e reconstroi o
    // objeto. Cada metodo e chamado uma unica vez por conexao.
    // =================================================================

    /** Converte a Mensagem em JSON e a escreve na conexao, seguida de nova linha. */
    private static void enviarMensagem(Socket conexao, Mensagem mensagem) throws IOException {
        PrintWriter saida = new PrintWriter(
                new OutputStreamWriter(conexao.getOutputStream(), "UTF-8"), true);
        saida.println(gson.toJson(mensagem));
    }

    /**
     * Le uma linha JSON da conexao e a transforma de volta em Mensagem.
     * Devolve null se a outra ponta fechou a conexao sem enviar nada.
     */
    private static Mensagem receberMensagem(Socket conexao) throws IOException {
        BufferedReader entrada = new BufferedReader(
                new InputStreamReader(conexao.getInputStream(), "UTF-8"));
        String linha = entrada.readLine();
        if (linha == null) {
            return null;
        }
        return gson.fromJson(linha, Mensagem.class);
    }
}
