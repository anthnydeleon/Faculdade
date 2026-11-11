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
import java.util.Random;
import java.util.Scanner;

// Cliente do KV Store. A cada PUT ou GET sorteia um dos tres servidores e
// nao sabe qual deles e o lider.
public class Cliente {

    // Enderecos "ip:porta" dos tres servidores, digitados no INIT (4a).
    // O sorteio do PUT e do GET escolhe uma posicao desta lista.
    private static ArrayList<String> servidores = new ArrayList<String>();

    // [Secao 4 - Observacoes] Timestamps proprios deste cliente:
    // chave -> ultimo timestamp que o cliente ja viu para ela.
    // Comeca vazio, e chave ausente vale 0: e o "inicializado em zero" do
    // PDF. O valor vai no GET (4c) para o servidor nao devolver versao antiga.
    private static HashMap<String, Long> meusTimestamps = new HashMap<String, Long>();

    // Trava do meusTimestamps. Duas threads usam esse mapa: a do menu (PUT e
    // GET) e a EscutaRespostas (respostas do GET que chegam depois).
    private static final Object trava = new Object();

    // sorteia o servidor de cada PUT e GET (4b e 4c)
    private static Random sorteio = new Random();

    // converte o objeto Mensagem em JSON e o JSON de volta em Mensagem
    private static Gson gson = new Gson();

    // Porta do ServerSocket onde o cliente recebe a resposta atrasada do GET.
    // Vai dentro de cada PUT e GET.
    private static int minhaPorta;

    // Fica true depois do primeiro INIT. Antes disso a lista de servidores
    // esta vazia, e PUT e GET sao ignorados.
    private static boolean inicializado = false;

    /*
     * [Secao 6 - Menu interativo]
     * Menu com somente as opcoes INIT, PUT e GET. O cliente fica no laco
     * para sempre.
     */
    public static void main(String[] args) {
        Scanner teclado = new Scanner(System.in);

        while (true) {
            System.out.println("1 - INIT");
            System.out.println("2 - PUT");
            System.out.println("3 - GET");
            System.out.print("Escolha: ");

            // evita erro se a entrada do teclado for fechada
            if (!teclado.hasNextLine()) {
                return;
            }

            String opcao = teclado.nextLine().trim();

            if (opcao.equals("1")) {
                inicializar(teclado);
            } else if (opcao.equals("2")) {
                if (inicializado) {
                    enviarPut(teclado);
                }
            } else if (opcao.equals("3")) {
                if (inicializado) {
                    enviarGet(teclado);
                }
            }
        }
    }

    /*
     * [Secao 4a - Inicializacao]
     * Le do teclado o IP e a porta dos tres servidores. O cliente nao sabe
     * qual deles e o lider. Aqui tambem abre a porta de escuta usada para receber a resposta
     * atrasada do GET (4c).
     */
    private static void inicializar(Scanner teclado) {
        servidores.clear();

        // [4a] le IP e porta de cada um dos tres servidores
        for (int i = 1; i <= 3; i++) {
            System.out.print("IP do servidor " + i + ": ");
            String ip = teclado.nextLine().trim();

            System.out.print("Porta do servidor " + i + ": ");
            String porta = teclado.nextLine().trim();

            servidores.add(ip + ":" + porta);
        }

        try {
            // [4c - recebimento assincrono] a porta de escuta so e aberta no
            // primeiro INIT; nos seguintes ela e reaproveitada. A porta 0
            // pede ao sistema uma porta livre.
            if (!inicializado) {
                ServerSocket escuta = new ServerSocket(0);
                minhaPorta = escuta.getLocalPort();

                new EscutaRespostas(escuta).start();

                inicializado = true;
            }
        } catch (IOException e) {
        }
    }

    /*
     * [Secao 4c - Recebimento assincrono da resposta do GET]
     * Thread que fica no accept() da porta de escuta. Quando um servidor
     * conecta para entregar o value atrasado, ela imprime a resposta. Enquanto
     * isso o menu continua livre para outras operacoes.
     */
    private static class EscutaRespostas extends Thread {

        // ServerSocket aberto no INIT, na porta minhaPorta
        private ServerSocket escuta;

        public EscutaRespostas(ServerSocket escuta) {
            this.escuta = escuta;
        }

        public void run() {
            while (true) {
                try {
                    Socket conexao = escuta.accept();
                    Mensagem resposta = receberMensagem(conexao);
                    conexao.close();

                    if (resposta != null) {
                        imprimirRespostaGet(resposta);
                    }
                } catch (IOException e) {
                }
            }
        }
    }

    /*
     * [Secao 4b - Envio do PUT]
     * Le a key e o value do teclado, envia o PUT a um servidor sorteado e
     * espera o PUT_OK com o timestamp.
     */
    private static void enviarPut(Scanner teclado) {
        System.out.print("Digite a key: ");
        String chave = teclado.nextLine();

        System.out.print("Digite o value: ");
        String valor = teclado.nextLine();

        try {
            // [4b] conecta em um servidor sorteado
            Socket conexao = conectarServidorAleatorio();

            // [4b] o PUT leva key, value e o endereco de escuta do cliente
            Mensagem pedido = new Mensagem(Mensagem.PUT);
            pedido.setChave(chave);
            pedido.setValor(valor);
            // IP do cliente tirado do proprio socket da conexao
            pedido.setIpCliente(conexao.getLocalAddress().getHostAddress());
            pedido.setPortaCliente(minhaPorta);

            // [4b] envia o PUT e espera o PUT_OK
            enviarMensagem(conexao, pedido);
            Mensagem resposta = receberMensagem(conexao);
            conexao.close();

            if (resposta != null) {
                // guarda o timestamp gerado pelo lider
                synchronized (trava) {
                    meusTimestamps.put(chave, Long.valueOf(resposta.getTimestamp()));
                }

                System.out.println("PUT_OK key: " + chave
                        + " value " + valor
                        + " timestamp " + resposta.getTimestamp()
                        + " realizada no servidor " + resposta.getIpServidor()
                        + ":" + resposta.getPortaServidor());
            }
        } catch (IOException e) {
        }
    }

    /*
     * [Secao 4c - Envio do GET]
     * Le so a key do teclado e envia o GET a um servidor sorteado, junto com
     * o ultimo timestamp que o cliente tem para essa key e o endereco de
     * escuta. O timestamp vem do mapa meusTimestamps, nao do teclado.
     */
    private static void enviarGet(Scanner teclado) {
        System.out.print("Digite a key: ");
        String chave = teclado.nextLine();

        // [4c] o timestamp vem do mapa, nao do teclado (0 se nunca viu a key)
        long meuTimestamp;
        synchronized (trava) {
            Long t = meusTimestamps.get(chave);
            meuTimestamp = (t == null) ? 0L : t.longValue();
        }

        try {
            // [4c] conecta em um servidor sorteado
            Socket conexao = conectarServidorAleatorio();

            // [4c] o GET leva key, timestamp e o endereco de escuta do cliente
            Mensagem pedido = new Mensagem(Mensagem.GET);
            pedido.setChave(chave);
            pedido.setTimestamp(meuTimestamp);
            pedido.setIpCliente(conexao.getLocalAddress().getHostAddress());
            pedido.setPortaCliente(minhaPorta);

            enviarMensagem(conexao, pedido);
            Mensagem resposta = receberMensagem(conexao);
            conexao.close();

            if (resposta != null) {
                // [4c - WAIT_FOR_RESPONSE] servidor atrasado: o cliente volta
                // ao menu e a resposta chega depois pela porta de escuta.
                // O timestamp nao e atualizado aqui.
                if (Mensagem.WAIT_FOR_RESPONSE.equals(resposta.getTipo())) {
                    System.out.println("GET key: " + resposta.getChave()
                            + " " + Mensagem.WAIT_FOR_RESPONSE
                            + " do servidor " + resposta.getIpServidor()
                            + ":" + resposta.getPortaServidor()
                            + ", meu timestamp " + meuTimestamp);
                } else {
                    imprimirRespostaGet(resposta);
                }
            }
        } catch (IOException e) {
        }
    }

    // [Secao 4c] Imprime a resposta do GET e atualiza o timestamp local se o
    // do servidor for maior. Usado na resposta imediata e na atrasada.
    private static void imprimirRespostaGet(Mensagem resposta) {
        long meuTimestamp;
        synchronized (trava) {
            Long t = meusTimestamps.get(resposta.getChave());
            meuTimestamp = (t == null) ? 0L : t.longValue();

            if (resposta.getTimestamp() > meuTimestamp) {
                meusTimestamps.put(resposta.getChave(), Long.valueOf(resposta.getTimestamp()));
            }
        }

        System.out.println("GET key: " + resposta.getChave()
                + " value: " + resposta.getValor()
                + " obtido do servidor " + resposta.getIpServidor()
                + ":" + resposta.getPortaServidor()
                + ", meu timestamp " + meuTimestamp
                + " e do servidor " + resposta.getTimestamp());
    }

    // [Secao 4b e 4c] Sorteia um dos tres servidores e abre uma conexao TCP
    // com ele.
    private static Socket conectarServidorAleatorio() throws IOException {
        String endereco = servidores.get(sorteio.nextInt(servidores.size()));
        String[] partes = endereco.split(":");
        return new Socket(partes[0], Integer.parseInt(partes[1]));
    }

    // Envia a Mensagem pela conexao TCP, convertida em uma linha de JSON.
    private static void enviarMensagem(Socket conexao, Mensagem mensagem) throws IOException {
        PrintWriter saida = new PrintWriter(
                new OutputStreamWriter(conexao.getOutputStream(), "UTF-8"), true);
        saida.println(gson.toJson(mensagem));
    }

    // Le uma linha de JSON e converte de volta em Mensagem. Retorna null se o
    // servidor fechou a conexao sem enviar nada.
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
