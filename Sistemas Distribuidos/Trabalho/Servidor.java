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

// O mesmo programa roda como lider ou como seguidor,
// conforme o endereco do lider digitado na inicializacao.
// So o lider executa o PUT; os seguidores guardam uma copia e respondem GET.
public class Servidor {

    // Tabela hash local com os valores: chave -> value.
    // E a "tabela de hash local" do item 5c.1 do PDF.
    private static HashMap<String, String> tabelaValores = new HashMap<String, String>();

    // Tabela hash local com as versoes: chave -> timestamp do value guardado.
    // Fica separada da tabelaValores para nao precisar de uma classe a mais
    // (o PDF so permite Servidor, Cliente e Mensagem).
    private static HashMap<String, Long> tabelaTimestamps = new HashMap<String, Long>();

    // Ultimo timestamp gerado. So o lider usa: soma 1 a cada PUT, entao o
    // timestamp e um contador (nota de rodape 1 do PDF).
    private static long contadorTimestamp = 0;

    // Enderecos "ip:porta" dos seguidores que se registraram no lider pelo
    // SYNC_REQUEST. O lider envia o REPLICATION para cada um deles.
    // HashSet para nao repetir o mesmo seguidor quando ele cai e volta.
    private static HashSet<String> seguidores = new HashSet<String>();

    // [Secao 5 - Observacoes: protecao contra acessos concorrentes]
    // Trava usada em todo acesso as tabelas, ao contador e a lista de
    // seguidores, para duas threads nao alterarem esses dados ao mesmo tempo.
    // Tambem e o objeto do wait/notifyAll da espera do GET (5f).
    private static final Object trava = new Object();

    // endereco deste servidor, digitado na inicializacao (5a)
    private static String meuIp;
    private static int minhaPorta;

    // endereco do lider, digitado na inicializacao (5a)
    private static String ipLider;
    private static int portaLider;

    // true se este servidor e o lider (endereco proprio igual ao do lider)
    private static boolean souLider;

    // converte o objeto Mensagem em JSON e o JSON de volta em Mensagem
    private static Gson gson = new Gson();

    /*
     * [Secao 5a - Inicializacao]
     * Le do teclado o IP e a porta deste servidor e o IP e a porta do lider,
     * e abre a porta para receber conexoes.
     */
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

        // [5a] e lider quando o endereco digitado para o lider e o dele mesmo
        souLider = meuIp.equals(ipLider) && minhaPorta == portaLider;

        try {
            ServerSocket escuta = new ServerSocket(minhaPorta);

            // [5g] o seguidor se registra no lider e copia os dados dele,
            // tanto na primeira vez que sobe quanto depois de cair e voltar
            if (!souLider) {
                new RegistraNoLider().start();
            }

            /*
             * [Secao 5b - Recebe e responde simultaneamente (threads)]
             * Cada conexao aceita vai para uma thread AtendeConexao, e o laco
             * volta logo ao accept() para receber a proxima.
             */
            while (true) {
                Socket conexao = escuta.accept();
                new AtendeConexao(conexao).start();
            }
        } catch (IOException e) {
        }
    }

    // [Secao 5b] Thread que atende uma conexao: le a Mensagem recebida e chama
    // o tratamento do tipo dela (PUT, GET, REPLICATION ou SYNC_REQUEST).
    // Existe uma thread destas para cada conexao aceita.
    private static class AtendeConexao extends Thread {

        // conexao TCP aceita no main
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
            } finally {
                // fecha sempre, para o outro lado nao ficar esperando resposta
                try {
                    conexao.close();
                } catch (IOException e) {
                }
            }
        }
    }

    /*
     * [Secao 5g - Servidor nao lider cai e volta]
     * Thread que envia SYNC_REQUEST ao lider e copia as tabelas que chegam no
     * SYNC_RESPONSE. Se o lider ainda nao estiver no ar, tenta de novo a cada
     * 1 segundo. O servidor volta sem dados porque as tabelas ficam so na
     * memoria.
     */
    private static class RegistraNoLider extends Thread {

        public void run() {
            while (true) {
                try {
                    Socket conexaoLider = new Socket(ipLider, portaLider);

                    // [5g] pede os dados ao lider, informando o proprio endereco
                    Mensagem pedido = new Mensagem(Mensagem.SYNC_REQUEST);
                    pedido.setIpServidor(meuIp);
                    pedido.setPortaServidor(minhaPorta);

                    enviarMensagem(conexaoLider, pedido);
                    Mensagem resposta = receberMensagem(conexaoLider);
                    conexaoLider.close();

                    if (resposta != null) {
                        synchronized (trava) {
                            HashMap<String, String> dados = resposta.getDados();
                            HashMap<String, Long> tempos = resposta.getTempos();

                            if (dados != null && tempos != null) {
                                // so grava se o timestamp recebido for maior,
                                // para nao sobrescrever um REPLICATION mais
                                // novo que possa ter chegado antes
                                for (String chave : dados.keySet()) {
                                    Long ts = tempos.get(chave);
                                    if (ts != null && ts.longValue() > timestampDaChave(chave)) {
                                        tabelaValores.put(chave, dados.get(chave));
                                        tabelaTimestamps.put(chave, ts);
                                    }
                                }
                            }

                            // acorda os GET que estao esperando atualizacao (5f)
                            trava.notifyAll();
                        }

                        System.out.println("Informa\u00e7\u00f5es recebidas do l\u00edder");
                        return;
                    }
                } catch (IOException e) {
                }

                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    return;
                }
            }
        }
    }

    /*
     * [Secao 5c - Recebe requisicao PUT]
     * Se este servidor nao for o lider, encaminha o PUT ao lider. Se for o
     * lider: (1) grava key e value com um novo timestamp, (2) replica nos
     * seguidores com REPLICATION e (3) responde PUT_OK ao cliente. O passo 3
     * so acontece depois do item 5e (REPLICATION_OK de todos).
     */
    private static void tratarPut(Mensagem pedido, Socket conexao) throws IOException {

        // [5c - nao lider] encaminha o PUT ao lider, com os dados do cliente
        // original, e repassa ao cliente o PUT_OK que o lider devolver
        if (!souLider) {
            System.out.println("Encaminhando PUT key:" + pedido.getChave()
                    + " value:" + pedido.getValor());

            Socket conexaoLider = new Socket(ipLider, portaLider);
            enviarMensagem(conexaoLider, pedido);
            Mensagem respostaLider = receberMensagem(conexaoLider);
            conexaoLider.close();

            if (respostaLider != null) {
                enviarMensagem(conexao, respostaLider);
            }
            return;
        }

        // [5c - lider] daqui para baixo so o lider executa
        System.out.println("Cliente " + pedido.getIpCliente() + ":" + pedido.getPortaCliente()
                + " PUT key:" + pedido.getChave() + " value:" + pedido.getValor());

        long novoTimestamp;
        ArrayList<String> destinos;

        // [5c.1] gera o novo timestamp e grava key e value. Se a key ja
        // existe, o put() substitui o value e o timestamp. A lista de
        // seguidores e copiada para ser usada fora da trava, sem bloquear o
        // servidor enquanto espera a rede.
        synchronized (trava) {
            contadorTimestamp = contadorTimestamp + 1;
            novoTimestamp = contadorTimestamp;
            tabelaValores.put(pedido.getChave(), pedido.getValor());
            tabelaTimestamps.put(pedido.getChave(), Long.valueOf(novoTimestamp));
            destinos = new ArrayList<String>(seguidores);
        }

        // [5c.2] envia REPLICATION a cada seguidor, uma thread por seguidor.
        // A lista guarda as threads para o lider esperar todas no 5e.
        ArrayList<ReplicaEmSeguidor> replicacoes = new ArrayList<ReplicaEmSeguidor>();
        for (int i = 0; i < destinos.size(); i++) {
            ReplicaEmSeguidor replicacao = new ReplicaEmSeguidor(destinos.get(i),
                    pedido.getChave(), pedido.getValor(), novoTimestamp);
            replicacoes.add(replicacao);
            replicacao.start();
        }

        // [Secao 5e - Recebe REPLICATION_OK]
        // Cada thread so termina depois de ler o REPLICATION_OK do seu
        // seguidor, entao o join() em todas espera o OK de todos.
        for (int i = 0; i < replicacoes.size(); i++) {
            try {
                replicacoes.get(i).join();
            } catch (InterruptedException e) {
            }
        }

        System.out.println("Enviando PUT_OK ao Cliente " + pedido.getIpCliente()
                + ":" + pedido.getPortaCliente() + " da key:" + pedido.getChave()
                + " ts:" + novoTimestamp);

        // [5c.3] envia PUT_OK ao cliente com o timestamp da key
        Mensagem resposta = new Mensagem(Mensagem.PUT_OK);
        resposta.setChave(pedido.getChave());
        resposta.setValor(pedido.getValor());
        resposta.setTimestamp(novoTimestamp);
        resposta.setIpServidor(meuIp);
        resposta.setPortaServidor(minhaPorta);

        enviarMensagem(conexao, resposta);
    }

    // [Secao 5c.2 e 5e] Thread que envia o REPLICATION a um seguidor e espera
    // o REPLICATION_OK dele. Se o seguidor estiver fora do ar, a conexao falha
    // e ele fica de fora deste PUT; quando voltar, recupera os dados pelo
    // SYNC_REQUEST (5g).
    private static class ReplicaEmSeguidor extends Thread {

        // "ip:porta" do seguidor de destino
        private String endereco;

        // dados replicados: key, value e timestamp gerado pelo lider
        private String chave;
        private String valor;
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

                // [5c.2] envia (key, value, timestamp) na mensagem REPLICATION
                Mensagem replicacao = new Mensagem(Mensagem.REPLICATION);
                replicacao.setChave(chave);
                replicacao.setValor(valor);
                replicacao.setTimestamp(timestamp);
                replicacao.setIpServidor(meuIp);
                replicacao.setPortaServidor(minhaPorta);

                enviarMensagem(conexaoSeguidor, replicacao);

                // [5e] espera o REPLICATION_OK do seguidor
                receberMensagem(conexaoSeguidor);
                conexaoSeguidor.close();
            } catch (IOException e) {
            }
        }
    }

    /*
     * [Secao 5d - Recebe requisicao REPLICATION]
     * O seguidor grava na tabela local a key, o value e o timestamp vindos
     * do lider e responde REPLICATION_OK.
     */
    private static void tratarReplication(Mensagem pedido, Socket conexao) throws IOException {
    
        // Atraso proposital no servidor de porta 10099, para mostrar o WAIT_FOR_RESPONSE.
        if (minhaPorta == 10099) {
            try {
                Thread.sleep(15000);
            } catch (InterruptedException e) {
                return;
            }
        }

        System.out.println("REPLICATION key:" + pedido.getChave()
                + " value:" + pedido.getValor()
                + " ts:" + pedido.getTimestamp());

        // [5d] grava na tabela de hash local
        synchronized (trava) {
            Long atual = tabelaTimestamps.get(pedido.getChave());

            // so grava se o timestamp recebido for maior, assim uma
            // replicacao atrasada nao apaga uma mais nova
            if (atual == null || pedido.getTimestamp() > atual.longValue()) {
                tabelaValores.put(pedido.getChave(), pedido.getValor());
                tabelaTimestamps.put(pedido.getChave(), Long.valueOf(pedido.getTimestamp()));
            }

            // acorda os GET que estao esperando atualizacao (5f)
            trava.notifyAll();
        }

        // [5d] responde REPLICATION_OK ao lider
        Mensagem resposta = new Mensagem(Mensagem.REPLICATION_OK);
        resposta.setChave(pedido.getChave());
        resposta.setTimestamp(pedido.getTimestamp());
        resposta.setIpServidor(meuIp);
        resposta.setPortaServidor(minhaPorta);

        enviarMensagem(conexao, resposta);
    }

    /*
     * [Secao 5f - Recebe requisicao GET]
     * Compara o timestamp da key neste servidor com o Tx que o cliente
     * enviou (pedido.getTimestamp()). Key inexistente: devolve NULL.
     * Timestamp igual ou maior que Tx: devolve o value na hora. Timestamp
     * menor: devolve WAIT_FOR_RESPONSE, fecha a conexao e envia o value
     * depois, quando a versao nova chegar. Junto com o value vai sempre o
     * timestamp deste servidor para a key.
     */
    private static void tratarGet(Mensagem pedido, Socket conexao) throws IOException {

        String valorLocal;
        long timestampLocal;

        // le value e timestamp juntos para os dois serem da mesma versao
        synchronized (trava) {
            valorLocal = tabelaValores.get(pedido.getChave());
            timestampLocal = timestampDaChave(pedido.getChave());
        }

        boolean chaveExiste = (valorLocal != null);

        // [5f - WAIT_FOR_RESPONSE] a key existe, mas a versao deste servidor e
        // mais antiga que a que o cliente ja viu (ts < Tx). Responde
        // WAIT_FOR_RESPONSE, a conexao e fechada no fim do AtendeConexao e a
        // thread EsperaAtualizacao entrega o value depois.
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

            new EsperaAtualizacao(pedido).start();
            return;
        }

        // [5f - key inexistente] devolve NULL, com timestamp 0
        if (!chaveExiste) {
            valorLocal = "NULL";
        }

        // [5f - resposta imediata] devolve o value e o timestamp deste servidor
        System.out.println("Cliente " + pedido.getIpCliente() + ":" + pedido.getPortaCliente()
                + " GET key:" + pedido.getChave() + " ts:" + pedido.getTimestamp()
                + ". Meu ts \u00e9 " + timestampLocal
                + ", portanto devolvendo " + valorLocal);

        Mensagem resposta = new Mensagem(Mensagem.GET_RESPONSE);
        resposta.setChave(pedido.getChave());
        resposta.setValor(valorLocal);
        resposta.setTimestamp(timestampLocal);
        resposta.setIpServidor(meuIp);
        resposta.setPortaServidor(minhaPorta);

        enviarMensagem(conexao, resposta);
    }

    // [Secao 5f - entrega depois do WAIT_FOR_RESPONSE] Thread que espera a key
    // chegar a este servidor com timestamp igual ou maior que o Tx do cliente.
    // Ela e acordada pelo notifyAll() do REPLICATION (5d) ou da recuperacao
    // (5g). Depois conecta na porta de escuta do cliente e envia o value com
    // o timestamp.
    private static class EsperaAtualizacao extends Thread {

        // GET original: traz a key, o Tx e o endereco de escuta do cliente
        private Mensagem pedido;

        public EsperaAtualizacao(Mensagem pedido) {
            this.pedido = pedido;
        }

        public void run() {
            String valorLocal;
            long timestampLocal;

            synchronized (trava) {
                // while, e nao if: a thread tambem acorda com REPLICATION de
                // outras keys, e entao precisa conferir de novo
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
                // conecta no endereco de escuta que o cliente mandou no GET
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
            }
        }
    }

    // Timestamp da chave neste servidor, ou 0 se ela nao existir.
    // Sempre chamado de dentro de synchronized (trava).
    private static long timestampDaChave(String chave) {
        Long t = tabelaTimestamps.get(chave);
        return (t == null) ? 0L : t.longValue();
    }

    // [Secao 5g - lado do lider] Recebe o SYNC_REQUEST de um seguidor, guarda
    // o endereco dele na lista de seguidores e devolve uma copia das duas
    // tabelas no SYNC_RESPONSE.
    private static void tratarSyncRequest(Mensagem pedido, Socket conexao) throws IOException {
        Mensagem resposta = new Mensagem(Mensagem.SYNC_RESPONSE);

        // registro e copia na mesma trava: um PUT feito ao mesmo tempo ou ja
        // esta na copia, ou ja encontra o seguidor na lista e e replicado
        synchronized (trava) {
            seguidores.add(pedido.getIpServidor() + ":" + pedido.getPortaServidor());
            resposta.setDados(new HashMap<String, String>(tabelaValores));
            resposta.setTempos(new HashMap<String, Long>(tabelaTimestamps));
        }

        resposta.setIpServidor(meuIp);
        resposta.setPortaServidor(minhaPorta);

        enviarMensagem(conexao, resposta);
    }

    // Envia a Mensagem pela conexao TCP, convertida em uma linha de JSON.
    private static void enviarMensagem(Socket conexao, Mensagem mensagem) throws IOException {
        PrintWriter saida = new PrintWriter(
                new OutputStreamWriter(conexao.getOutputStream(), "UTF-8"), true);
        saida.println(gson.toJson(mensagem));
    }

    // Le uma linha de JSON e converte de volta em Mensagem. Retorna null se a
    // outra ponta fechou a conexao sem enviar nada.
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
