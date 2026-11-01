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

/*
 * =====================================================================
 * Cliente.java
 *
 * Usuario do KV Store. Faz PUT e GET em um dos tres servidores, escolhido
 * ao acaso a cada operacao, sem conhecer qual deles e o lider.
 *
 * O cliente mantem, para cada chave, o maior timestamp que ja recebeu, e
 * envia esse valor em toda leitura. E esse controle que impede o sistema
 * de lhe devolver uma versao anterior a que ele ja leu.
 * =====================================================================
 */
public class Cliente {

    // =================================================================
    // ESTRUTURAS DE DADOS DO CLIENTE
    // =================================================================

    /*
     * LISTA DE SERVIDORES.
     * Para que serve: guarda onde ficam os tres servidores, para o cliente
     * poder sortear um a cada operacao.
     * O que armazena: tres enderecos no formato "ip:porta".
     */
    private static ArrayList<String> servidores = new ArrayList<String>();

    /*
     * TABELA DE TIMESTAMPS DO CLIENTE.
     * Para que serve: registrar a versao mais nova que este cliente ja leu
     * de cada chave, valor que acompanha toda requisicao GET e garante a
     * leitura monotonica.
     * O que armazena: chave -> ultimo timestamp visto. Comeca vazia, e uma
     * chave que nunca foi vista vale 0.
     */
    private static HashMap<String, Long> meusTimestamps = new HashMap<String, Long>();

    /*
     * OBJETO DE TRAVA (lock).
     * Para que serve: a tabela de timestamps e usada por duas threads (a do
     * menu e a que recebe respostas tardias), entao todo acesso a ela
     * acontece dentro de synchronized (trava).
     */
    private static final Object trava = new Object();

    /** Sorteador usado para escolher um servidor aleatorio a cada operacao. */
    private static Random sorteio = new Random();

    /** Conversor objeto <-> JSON. */
    private static Gson gson = new Gson();

    /** Porta em que este cliente escuta respostas tardias do GET. */
    private static int minhaPorta;

    /** Indica se o INIT ja foi feito. PUT e GET so funcionam depois dele. */
    private static boolean inicializado = false;

    // =================================================================
    // [Enunciado 6 - menu interativo do cliente]
    // Oferece as opcoes INIT, PUT e GET em um laco que nao termina por
    // escolha do usuario: o cliente permanece ativo apos cada operacao.
    // =================================================================
    public static void main(String[] args) {
        Scanner teclado = new Scanner(System.in);

        while (true) {
            System.out.println("1 - INIT");
            System.out.println("2 - PUT");
            System.out.println("3 - GET");
            System.out.print("Escolha: ");

            // Se a entrada do teclado terminar (console fechado, por exemplo),
            // nextLine() lancaria excecao. Saindo do laco, a thread que escuta
            // as respostas assincronas continua viva e o processo segue ativo.
            if (!teclado.hasNextLine()) {
                return;
            }

            String opcao = teclado.nextLine().trim();

            // PUT e GET escolhidos antes do INIT apenas voltam ao menu, sem
            // imprimir nada: sem os enderecos dos servidores nao ha para quem
            // enviar a requisicao.
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

    // =================================================================
    // [Enunciado 4a - INIT do cliente]
    // Captura do teclado o IP e a porta dos tres servidores, abre a porta
    // de escuta do cliente e inicia a thread que recebe respostas tardias.
    // =================================================================
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
            // [Enunciado 4c - resposta assincrona do GET]
            // A porta 0 faz o sistema operacional escolher qualquer porta
            // livre. Depois perguntamos qual foi, com getLocalPort().
            //
            // A porta e aberta UMA unica vez. Num INIT repetido, os enderecos
            // dos servidores acima sao reescritos, mas a porta de escuta e a
            // thread que a atende sao reaproveitadas: abrir outra deixaria a
            // anterior aberta sem ninguem usando, e a resposta tardia de um GET
            // poderia aparecer duas vezes.
            if (!inicializado) {
                ServerSocket escuta = new ServerSocket(0);
                minhaPorta = escuta.getLocalPort();

                new EscutaRespostas(escuta).start();

                inicializado = true;
            }
        } catch (IOException e) {
            // Nao foi possivel abrir a porta de escuta. O cliente continua no
            // menu, mas sem o INIT concluido.
        }
    }

    // =================================================================
    // [Enunciado 4c - resposta assincrona do GET] CLASSE DE THREAD
    //
    // Papel desta thread: ficar permanentemente em accept(), esperando que
    // algum servidor ligue de volta com a resposta tardia de um GET que
    // recebeu WAIT_FOR_RESPONSE. Gracas a ela o cliente nunca fica
    // bloqueado esperando: ele volta ao menu e a resposta chega depois.
    // =================================================================
    private static class EscutaRespostas extends Thread {

        /** Porta de escuta do cliente, aberta no INIT. */
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
                        // Esta e a resposta tardia de um GET que havia recebido
                        // WAIT_FOR_RESPONSE. Agora sim o timestamp local e
                        // atualizado e a linha final do GET e impressa.
                        imprimirRespostaGet(resposta);
                    }
                } catch (IOException e) {
                    // Conexao com problema: ignora e continua escutando.
                }
            }
        }
    }

    // =================================================================
    // [Enunciado 4b - PUT do cliente]
    // Captura key e value, envia o PUT a um servidor sorteado e espera o
    // PUT_OK, que traz o timestamp gerado pelo lider.
    // =================================================================
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
            // Informa onde o servidor pode encontrar este cliente. O IP e
            // tirado do proprio socket: e exatamente o endereco pelo qual
            // este servidor consegue responder de volta.
            pedido.setIpCliente(conexao.getLocalAddress().getHostAddress());
            pedido.setPortaCliente(minhaPorta);

            enviarMensagem(conexao, pedido);
            Mensagem resposta = receberMensagem(conexao);
            conexao.close();

            if (resposta != null) {
                // Registra a versao recem gravada na tabela local de timestamps.
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
            // O servidor sorteado nao respondeu. A operacao e abandonada sem
            // nenhuma impressao e o cliente volta ao menu.
        }
    }

    // =================================================================
    // [Enunciado 4c - GET do cliente]
    // Captura so a key. O timestamp NAO vem do teclado: e o ultimo que o
    // cliente ja viu para aquela chave (0 se nunca viu).
    // =================================================================
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
                // [Enunciado 4c - GET do cliente] resposta WAIT_FOR_RESPONSE
                // O servidor avisou que esta atrasado e cortou a conexao. O
                // cliente NAO fica bloqueado: imprime a linha diferenciada,
                // volta ao menu, e a resposta de verdade chega depois pela
                // porta de escuta (thread EscutaRespostas).
                // O timestamp local NAO e atualizado aqui, porque nenhum valor
                // novo foi lido nesta interacao.
                if (Mensagem.WAIT_FOR_RESPONSE.equals(resposta.getTipo())) {
                    // Formato distinto do usado na resposta normal do GET:
                    // sem "value:" e sem o timestamp do servidor, porque
                    // nenhum valor foi lido nesta interacao.
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
            // O servidor sorteado nao respondeu. A operacao e abandonada sem
            // nenhuma impressao e o cliente volta ao menu.
        }
    }

    /*
     * Imprime a resposta de um GET. Fica em um metodo separado porque a
     * resposta pode chegar por dois caminhos: na hora (metodo enviarGet)
     * ou depois, pela porta de escuta (thread EscutaRespostas).
     *
     * "meu timestamp" e o que o cliente tinha ANTES desta resposta: e o
     * valor que ele enviou no pedido, e serve para comparar com o do
     * servidor e mostrar que a leitura nunca anda para tras.
     */
    private static void imprimirRespostaGet(Mensagem resposta) {
        long meuTimestamp;
        synchronized (trava) {
            Long t = meusTimestamps.get(resposta.getChave());
            meuTimestamp = (t == null) ? 0L : t.longValue();

            // Avanca o timestamp local se o servidor devolveu versao mais nova.
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

    /*
     * Sorteia um dos tres servidores e abre uma conexao TCP com ele. A
     * escolha e uniforme e independe de qual servidor seja o lider.
     */
    private static Socket conectarServidorAleatorio() throws IOException {
        String endereco = servidores.get(sorteio.nextInt(servidores.size()));
        String[] partes = endereco.split(":");
        return new Socket(partes[0], Integer.parseInt(partes[1]));
    }

    // =================================================================
    // ENVIO E RECEBIMENTO DA CLASSE Mensagem PELA REDE
    // Mesmo protocolo do servidor: uma mensagem JSON por linha.
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
