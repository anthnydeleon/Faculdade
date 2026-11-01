import java.util.HashMap;

/*
 * =====================================================================
 * Mensagem.java
 *
 * Unica classe que trafega pela rede. Todo pedido e toda resposta, tanto
 * entre cliente e servidor quanto entre servidores, sao um objeto Mensagem
 * convertido para texto JSON.
 *
 * A mesma classe atende a todos os tipos de mensagem: cada tipo usa apenas
 * os atributos que lhe interessam, e os demais ficam em null ou 0.
 * =====================================================================
 */
public class Mensagem {

    // -----------------------------------------------------------------
    // TIPOS DE MENSAGEM
    // Constantes String que identificam a natureza de cada mensagem.
    // O atributo "tipo" sempre recebe um destes valores, e e por ele que
    // o receptor decide como tratar o que chegou.
    // -----------------------------------------------------------------

    /** Cliente -> Servidor, e Servidor nao lider -> Lider. Pedido de gravacao. */
    public static final String PUT = "PUT";

    /** Lider -> Cliente (direto ou de volta pelo servidor). Gravacao concluida. */
    public static final String PUT_OK = "PUT_OK";

    /** Cliente -> Servidor. Pedido de leitura. */
    public static final String GET = "GET";

    /** Servidor -> Cliente. Resposta do GET (imediata ou tardia). */
    public static final String GET_RESPONSE = "GET_RESPONSE";

    /** Servidor -> Cliente. "Estou atrasado, eu te chamo depois." */
    public static final String WAIT_FOR_RESPONSE = "WAIT_FOR_RESPONSE";

    /** Lider -> Seguidor. Manda copiar (chave, valor, timestamp). */
    public static final String REPLICATION = "REPLICATION";

    /** Seguidor -> Lider. Confirma que copiou. */
    public static final String REPLICATION_OK = "REPLICATION_OK";

    /** Seguidor -> Lider. Registro do seguidor e pedido do estado completo. */
    public static final String SYNC_REQUEST = "SYNC_REQUEST";

    /** Lider -> Seguidor. Estado completo (as duas tabelas). */
    public static final String SYNC_RESPONSE = "SYNC_RESPONSE";

    // -----------------------------------------------------------------
    // ATRIBUTOS
    // Cada informacao transportada tem seu proprio campo tipado, de modo
    // que o receptor le o dado direto do objeto, sem interpretar texto.
    // -----------------------------------------------------------------

    /** Tipo da mensagem. Recebe uma das constantes acima. */
    private String tipo;

    /** A chave (key) do par chave-valor. Usado em PUT, GET, REPLICATION. */
    private String chave;

    /** O valor (value) associado a chave. No GET de chave inexistente vale "NULL". */
    private String valor;

    /** Versao da chave. No GET de ida e o timestamp do cliente (Tx);
     *  nas respostas e o timestamp do servidor. */
    private long timestamp;

    /** IP onde o cliente escuta respostas enviadas fora da conexao original. */
    private String ipCliente;

    /** Porta onde o cliente escuta respostas enviadas fora da conexao original. */
    private int portaCliente;

    /** IP do servidor que originou ou executou a operacao.
     *  O cliente usa para imprimir "realizada no servidor [IP:porta]". */
    private String ipServidor;

    /** Porta do servidor que originou ou executou a operacao. */
    private int portaServidor;

    /** Copia da tabela de valores (chave -> valor). So e preenchido no SYNC_RESPONSE. */
    private HashMap<String, String> dados;

    /** Copia da tabela de timestamps (chave -> timestamp). So no SYNC_RESPONSE.
     *  O tipo generico e declarado por extenso para que os numeros sejam
     *  reconstruidos como Long na conversao de volta do JSON. */
    private HashMap<String, Long> tempos;

    // -----------------------------------------------------------------
    // CONSTRUTORES
    // -----------------------------------------------------------------

    /** Construtor sem argumentos, usado na reconstrucao do objeto a partir do JSON. */
    public Mensagem() {
    }

    /** Atalho para criar uma mensagem ja com o tipo definido. */
    public Mensagem(String tipo) {
        this.tipo = tipo;
    }

    // -----------------------------------------------------------------
    // GETTERS E SETTERS
    // -----------------------------------------------------------------

    public String getTipo() {
        return tipo;
    }

    public void setTipo(String tipo) {
        this.tipo = tipo;
    }

    public String getChave() {
        return chave;
    }

    public void setChave(String chave) {
        this.chave = chave;
    }

    public String getValor() {
        return valor;
    }

    public void setValor(String valor) {
        this.valor = valor;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    public String getIpCliente() {
        return ipCliente;
    }

    public void setIpCliente(String ipCliente) {
        this.ipCliente = ipCliente;
    }

    public int getPortaCliente() {
        return portaCliente;
    }

    public void setPortaCliente(int portaCliente) {
        this.portaCliente = portaCliente;
    }

    public String getIpServidor() {
        return ipServidor;
    }

    public void setIpServidor(String ipServidor) {
        this.ipServidor = ipServidor;
    }

    public int getPortaServidor() {
        return portaServidor;
    }

    public void setPortaServidor(int portaServidor) {
        this.portaServidor = portaServidor;
    }

    public HashMap<String, String> getDados() {
        return dados;
    }

    public void setDados(HashMap<String, String> dados) {
        this.dados = dados;
    }

    public HashMap<String, Long> getTempos() {
        return tempos;
    }

    public void setTempos(HashMap<String, Long> tempos) {
        this.tempos = tempos;
    }
}
