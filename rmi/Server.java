import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;
import java.net.UnknownHostException;

public class Server {
    public Server() {
        try {
            // Obtém o IP local da máquina
            String ip = IPUtil.getLocalIPv4();
            System.out.println("IP local (IPv4): " + ip);

            // Inicia o RMI Registry na porta 1099
            LocateRegistry.createRegistry(1099);

            // Registra o objeto remoto
            Naming.rebind("rmi://" + ip + "/Calculator", new Calculator());
            System.out.println("Servidor RMI iniciado em: rmi://" + ip + "/Calculator");

        } catch (Exception e) {
            System.out.println("Erro ao registrar o objeto: " + e);
        }
    }

    public static void main(String[] args) {
        new Server();
    }
}
