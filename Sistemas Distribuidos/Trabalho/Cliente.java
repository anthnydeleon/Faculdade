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

// O cliente faz PUT e GET em um servidor sorteado a cada operacao.
// Guarda o ultimo timestamp visto de cada chave e envia esse valor no GET.
public class Cliente {

    // enderecos "ip:porta" dos tres servidores
    private static ArrayList<String> servidores = new ArrayList<String>();

    // chave -> ultimo timestamp que este cliente ja viu
    private static HashMap<String, Long> meusTimestamps = new HashMap<String, Long>();

    // trava da tabela de timestamps, que e usada pela thread do menu e pela
    // thread que recebe as respostas atrasadas
    private static final Object trava = new Object();

    private static Random sorteio = new Random();
    private static Gson gson = new Gson();

    // porta onde o cliente recebe a resposta atrasada do GET
    private static int minhaPorta;

    // PUT e GET so funcionam depois do INIT
    private static boolean inicializado = false;

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

    // INIT: le os enderecos dos tres servidores e abre a porta de escuta.
    private static void inicializar(Scanner teclado) {
        servidores.clear();

        for (int i = 1; i <= 3; i++) {
            System.out.print("IP do servidor " + i + ": ");
            String ip = teclado.nextLine().trim();

            System.out.print("Porta do servidor " + i + ": ");
            String porta = teclado.nextLine().trim();

            servidores.add(ip + ":" + porta);
        }

        try {
            // a porta de escuta so e aberta no primeiro INIT; nos seguintes
            // ela e reaproveitada. A porta 0 pede ao sistema uma porta livre.
            if (!inicializado) {
                ServerSocket escuta = new ServerSocket(0);
                minhaPorta = escuta.getLocalPort();

                new EscutaRespostas(escuta).start();

                inicializado = true;
            }
        } catch (IOException e) {
        }
    }

    // Fica esperando os servidores enviarem a resposta atrasada de um GET
    // que recebeu WAIT_FOR_RESPONSE. Assim o cliente nao fica bloqueado.
    private static class EscutaRespostas extends Thread {

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

    // PUT: le key e value, envia a um servidor sorteado e espera o PUT_OK.
    private static void enviarPut(Scanner teclado) {
        System.out.print("Digite a key: ");
        String chave = teclado.nextLine();

        System.out.print("Digite o value: ");
        String valor = teclado.nextLine();

        try {
            Socket conexao = conectarServidorAleatorio();

            Mensagem pedido = new Mensagem(Mensagem.PUT);
            pedido.setChave(chave);
            pedido.setValor(valor);
            // IP do cliente tirado do proprio socket da conexao
            pedido.setIpCliente(conexao.getLocalAddress().getHostAddress());
            pedido.setPortaCliente(minhaPorta);

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

    // GET: le a key e envia junto o ultimo timestamp visto dela (0 se nunca viu).
    private static void enviarGet(Scanner teclado) {
        System.out.print("Digite a key: ");
        String chave = teclado.nextLine();

        long meuTimestamp;
        synchronized (trava) {
            Long t = meusTimestamps.get(chave);
            meuTimestamp = (t == null) ? 0L : t.longValue();
        }

        try {
            Socket conexao = conectarServidorAleatorio();

            Mensagem pedido = new Mensagem(Mensagem.GET);
            pedido.setChave(chave);
            pedido.setTimestamp(meuTimestamp);
            pedido.setIpCliente(conexao.getLocalAddress().getHostAddress());
            pedido.setPortaCliente(minhaPorta);

            enviarMensagem(conexao, pedido);
            Mensagem resposta = receberMensagem(conexao);
            conexao.close();

            if (resposta != null) {
                // servidor atrasado: o cliente volta ao menu e a resposta chega
                // depois pela porta de escuta. O timestamp nao e atualizado aqui.
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

    // Imprime a resposta do GET e atualiza o timestamp local se o do servidor
    // for maior. Usado na resposta imediata e na atrasada.
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

    // Sorteia um dos tres servidores e abre uma conexao com ele.
    private static Socket conectarServidorAleatorio() throws IOException {
        String endereco = servidores.get(sorteio.nextInt(servidores.size()));
        String[] partes = endereco.split(":");
        return new Socket(partes[0], Integer.parseInt(partes[1]));
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
