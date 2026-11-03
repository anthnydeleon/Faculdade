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

// O programa funciona como lider ou como seguidor.
// So o lider grava; os seguidores guardam uma copia e respondem GET.
public class Servidor {

    // chave -> valor guardado neste servidor
    private static HashMap<String, String> tabelaValores = new HashMap<String, String>();

    // chave -> timestamp (versao) do valor guardado
    private static HashMap<String, Long> tabelaTimestamps = new HashMap<String, Long>();

    // ultimo timestamp gerado. So o lider usa, somando 1 a cada PUT.
    private static long contadorTimestamp = 0;

    // enderecos "ip:porta" dos seguidores, para o lider enviar o REPLICATION.
    // HashSet para nao repetir o mesmo seguidor se ele se registrar de novo.
    private static HashSet<String> seguidores = new HashSet<String>();

    // trava usada em todo acesso as tabelas e a lista de seguidores, para
    // duas threads nao mexerem nelas ao mesmo tempo. Tambem e usada no
    // wait/notifyAll da espera do GET.
    private static final Object trava = new Object();

    private static String meuIp;
    private static int minhaPorta;
    private static String ipLider;
    private static int portaLider;
    private static boolean souLider;

    private static Gson gson = new Gson();

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
        
        souLider = meuIp.equals(ipLider) && minhaPorta == portaLider;

        try {
            ServerSocket escuta = new ServerSocket(minhaPorta);

            // o seguidor se registra no lider e recebe os dados que ja existem,
            // tanto na primeira vez que sobe quanto depois de cair e voltar
            if (!souLider) {
                new RegistraNoLider().start();
            }

            // cada conexao aceita e atendida por uma thread separada, assim o
            // servidor continua aceitando outros pedidos ao mesmo tempo
            while (true) {
                Socket conexao = escuta.accept();
                new AtendeConexao(conexao).start();
            }
        } catch (IOException e) {
        }
    }

    // Atende uma conexao: le a mensagem e chama o tratamento do tipo dela.
    private static class AtendeConexao extends Thread {

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

    // Envia SYNC_REQUEST ao lider e copia os dados recebidos.
    // Tenta de novo a cada 1 segundo se o lider nao responder.
    private static class RegistraNoLider extends Thread {

        public void run() {
            while (true) {
                try {
                    Socket conexaoLider = new Socket(ipLider, portaLider);

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

                            // acorda os GET que estao esperando atualizacao
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

    // PUT: seguidor encaminha ao lider; lider grava, replica e responde PUT_OK.
    private static void tratarPut(Mensagem pedido, Socket conexao) throws IOException {

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

        System.out.println("Cliente " + pedido.getIpCliente() + ":" + pedido.getPortaCliente()
                + " PUT key:" + pedido.getChave() + " value:" + pedido.getValor());

        long novoTimestamp;
        ArrayList<String> destinos;

        // gera o novo timestamp e grava. A lista de seguidores e copiada para
        // ser usada fora da trava, sem bloquear o servidor durante a rede.
        synchronized (trava) {
            contadorTimestamp = contadorTimestamp + 1;
            novoTimestamp = contadorTimestamp;
            tabelaValores.put(pedido.getChave(), pedido.getValor());
            tabelaTimestamps.put(pedido.getChave(), Long.valueOf(novoTimestamp));
            destinos = new ArrayList<String>(seguidores);
        }

        // envia REPLICATION para cada seguidor em paralelo
        ArrayList<ReplicaEmSeguidor> replicacoes = new ArrayList<ReplicaEmSeguidor>();
        for (int i = 0; i < destinos.size(); i++) {
            ReplicaEmSeguidor replicacao = new ReplicaEmSeguidor(destinos.get(i),
                    pedido.getChave(), pedido.getValor(), novoTimestamp);
            replicacoes.add(replicacao);
            replicacao.start();
        }

        // espera todos os REPLICATION_OK: cada thread so termina depois de
        // receber a confirmacao do seu seguidor
        for (int i = 0; i < replicacoes.size(); i++) {
            try {
                replicacoes.get(i).join();
            } catch (InterruptedException e) {
            }
        }

        System.out.println("Enviando PUT_OK ao Cliente " + pedido.getIpCliente()
                + ":" + pedido.getPortaCliente() + " da key:" + pedido.getChave()
                + " ts:" + novoTimestamp);

        Mensagem resposta = new Mensagem(Mensagem.PUT_OK);
        resposta.setChave(pedido.getChave());
        resposta.setValor(pedido.getValor());
        resposta.setTimestamp(novoTimestamp);
        resposta.setIpServidor(meuIp);
        resposta.setPortaServidor(minhaPorta);

        enviarMensagem(conexao, resposta);
    }

    // Envia REPLICATION a um seguidor e espera o REPLICATION_OK.
    // Se o seguidor estiver fora do ar, ele e ignorado neste PUT e recupera
    // os dados depois, pelo SYNC_REQUEST.
    private static class ReplicaEmSeguidor extends Thread {

        private String endereco;
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

                Mensagem replicacao = new Mensagem(Mensagem.REPLICATION);
                replicacao.setChave(chave);
                replicacao.setValor(valor);
                replicacao.setTimestamp(timestamp);
                replicacao.setIpServidor(meuIp);
                replicacao.setPortaServidor(minhaPorta);

                enviarMensagem(conexaoSeguidor, replicacao);
                receberMensagem(conexaoSeguidor);
                conexaoSeguidor.close();
            } catch (IOException e) {
            }
        }
    }

    // REPLICATION: seguidor grava o valor e responde REPLICATION_OK.
    private static void tratarReplication(Mensagem pedido, Socket conexao) throws IOException {

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

        synchronized (trava) {
            Long atual = tabelaTimestamps.get(pedido.getChave());

            // so grava se o timestamp recebido for maior, assim uma
            // replicacao atrasada nao apaga uma mais nova
            if (atual == null || pedido.getTimestamp() > atual.longValue()) {
                tabelaValores.put(pedido.getChave(), pedido.getValor());
                tabelaTimestamps.put(pedido.getChave(), Long.valueOf(pedido.getTimestamp()));
            }

            // acorda os GET que estao esperando atualizacao
            trava.notifyAll();
        }

        Mensagem resposta = new Mensagem(Mensagem.REPLICATION_OK);
        resposta.setChave(pedido.getChave());
        resposta.setTimestamp(pedido.getTimestamp());
        resposta.setIpServidor(meuIp);
        resposta.setPortaServidor(minhaPorta);

        enviarMensagem(conexao, resposta);
    }

    // GET: o cliente envia a chave e o ultimo timestamp que ja viu dela.
    // Se este servidor tiver uma versao mais antiga, responde WAIT_FOR_RESPONSE
    // e entrega o valor depois; senao, responde na hora.
    private static void tratarGet(Mensagem pedido, Socket conexao) throws IOException {

        String valorLocal;
        long timestampLocal;

        // le valor e timestamp juntos para os dois serem da mesma versao
        synchronized (trava) {
            valorLocal = tabelaValores.get(pedido.getChave());
            timestampLocal = timestampDaChave(pedido.getChave());
        }

        boolean chaveExiste = (valorLocal != null);

        // servidor atrasado: a chave existe, mas com timestamp menor que o do cliente.
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
        resposta.setTimestamp(timestampLocal);
        resposta.setIpServidor(meuIp);
        resposta.setPortaServidor(minhaPorta);

        enviarMensagem(conexao, resposta);
    }

    // Espera a chave ser atualizada e envia a resposta para a porta do cliente.
    private static class EsperaAtualizacao extends Thread {

        private Mensagem pedido;

        public EsperaAtualizacao(Mensagem pedido) {
            this.pedido = pedido;
        }

        public void run() {
            String valorLocal;
            long timestampLocal;

            synchronized (trava) {
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

    // Timestamp da chave, ou 0 se nao existir.
    private static long timestampDaChave(String chave) {
        Long t = tabelaTimestamps.get(chave);
        return (t == null) ? 0L : t.longValue();
    }

    // SYNC_REQUEST: lider registra o seguidor e envia uma copia das tabelas.
    private static void tratarSyncRequest(Mensagem pedido, Socket conexao) throws IOException {
        Mensagem resposta = new Mensagem(Mensagem.SYNC_RESPONSE);

        synchronized (trava) {
            seguidores.add(pedido.getIpServidor() + ":" + pedido.getPortaServidor());
            resposta.setDados(new HashMap<String, String>(tabelaValores));
            resposta.setTempos(new HashMap<String, Long>(tabelaTimestamps));
        }

        resposta.setIpServidor(meuIp);
        resposta.setPortaServidor(minhaPorta);

        enviarMensagem(conexao, resposta);
    }

    // Envia a mensagem como uma linha JSON.
    private static void enviarMensagem(Socket conexao, Mensagem mensagem) throws IOException {
        PrintWriter saida = new PrintWriter(
                new OutputStreamWriter(conexao.getOutputStream(), "UTF-8"), true);
        saida.println(gson.toJson(mensagem));
    }

    // Le uma linha JSON e converte em Mensagem. Retorna null se a conexao fechou.
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