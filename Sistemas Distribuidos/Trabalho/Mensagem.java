import java.util.HashMap;

// [Secao 8 - Classe Mensagem]
// Objeto trocado em toda comunicacao TCP, tanto entre cliente e servidor
// quanto entre servidores (observacoes das Secoes 4 e 5 do PDF). Vai pela
// rede convertido em JSON pelo Gson. Cada tipo de mensagem usa so os campos
// de que precisa; os outros ficam vazios.
public class Mensagem {

    // Tipos de mensagem. PUT, PUT_OK, GET, WAIT_FOR_RESPONSE, REPLICATION e
    // REPLICATION_OK sao os nomes usados no PDF. GET_RESPONSE, SYNC_REQUEST e
    // SYNC_RESPONSE foram criados aqui, porque o PDF nao da nome a essas
    // mensagens.
    public static final String PUT = "PUT";                             // cliente -> servidor; seguidor -> lider (4b, 5c)
    public static final String PUT_OK = "PUT_OK";                       // lider -> cliente, direto ou repassado (5c.3)
    public static final String GET = "GET";                             // cliente -> servidor (4c, 5f)
    public static final String GET_RESPONSE = "GET_RESPONSE";           // servidor -> cliente, na hora ou depois (5f)
    public static final String WAIT_FOR_RESPONSE = "WAIT_FOR_RESPONSE"; // servidor atrasado -> cliente (5f)
    public static final String REPLICATION = "REPLICATION";             // lider -> seguidor (5c.2, 5d)
    public static final String REPLICATION_OK = "REPLICATION_OK";       // seguidor -> lider (5d, 5e)
    public static final String SYNC_REQUEST = "SYNC_REQUEST";           // seguidor -> lider ao subir (5g)
    public static final String SYNC_RESPONSE = "SYNC_RESPONSE";         // lider -> seguidor com as tabelas (5g)

    // tipo da mensagem: uma das constantes acima
    private String tipo;

    // key da operacao
    private String chave;

    // value da operacao. No GET de chave inexistente vale "NULL"
    private String valor;

    // no GET enviado e o timestamp do cliente; nas respostas, o do servidor
    private long timestamp;

    // endereco onde o cliente escuta a resposta atrasada do GET. Vai no PUT e
    // no GET; o lider tambem usa esses campos nos prints do PUT
    private String ipCliente;
    private int portaCliente;

    // servidor que executou a operacao ou enviou a mensagem. O cliente usa no
    // print da resposta; o lider usa para registrar o seguidor no SYNC_REQUEST
    private String ipServidor;
    private int portaServidor;

    // copia das duas tabelas do lider (key -> value e key -> timestamp).
    // So vai preenchida no SYNC_RESPONSE (5g)
    private HashMap<String, String> dados;
    private HashMap<String, Long> tempos;

    // usado pelo Gson para recriar o objeto a partir do JSON
    public Mensagem() {
    }

    // cria a mensagem ja com o tipo definido
    public Mensagem(String tipo) {
        this.tipo = tipo;
    }

    // getters e setters dos atributos

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
