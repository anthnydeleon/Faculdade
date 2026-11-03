import java.util.HashMap;

// Objeto trocado pela rede entre cliente e servidores, convertido em JSON.
// Cada tipo de mensagem usa so os campos que precisa; os outros ficam vazios.
public class Mensagem {

    // tipos de mensagem
    public static final String PUT = "PUT";
    public static final String PUT_OK = "PUT_OK";
    public static final String GET = "GET";
    public static final String GET_RESPONSE = "GET_RESPONSE";
    public static final String WAIT_FOR_RESPONSE = "WAIT_FOR_RESPONSE";
    public static final String REPLICATION = "REPLICATION";
    public static final String REPLICATION_OK = "REPLICATION_OK";
    public static final String SYNC_REQUEST = "SYNC_REQUEST";
    public static final String SYNC_RESPONSE = "SYNC_RESPONSE";

    // um dos tipos acima
    private String tipo;

    private String chave;

    // no GET de chave inexistente vale "NULL"
    private String valor;

    // no GET enviado e o timestamp do cliente; nas respostas, o do servidor
    private long timestamp;

    // endereco onde o cliente escuta a resposta atrasada do GET
    private String ipCliente;
    private int portaCliente;

    // servidor que executou a operacao
    private String ipServidor;
    private int portaServidor;

    // copia das tabelas do lider, usadas so no SYNC_RESPONSE
    private HashMap<String, String> dados;
    private HashMap<String, Long> tempos;

    // usado pelo Gson para recriar o objeto a partir do JSON
    public Mensagem() {
    }

    public Mensagem(String tipo) {
        this.tipo = tipo;
    }

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
