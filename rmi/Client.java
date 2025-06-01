import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;

import java.util.Scanner;

public class Client {

	Scanner in; 
	CalculatorInterface calculator; 
	
	public Client() {
		in = new Scanner(System.in);

        System.out.print("Digite o endereço ip do servidor RMI: ");
        String ip = in.next();
	    	try {  
                System.setProperty("java.rmi.server.hostname", ipLocal);
	            calculator =(CalculatorInterface)Naming.lookup("rmi://"+ip+"/Calculator");
                System.out.println("Conectado ao servidor RMI!");
	    	}
	        catch(RemoteException e ) {  
	            System.out.println();  
	            System.out.println( "RemoteException: " + e.toString() );
                System.exit(1);
	        }  
	        catch(NotBoundException e ) {  
	            System.out.println();  
	            System.out.println( "NotBoundException: " + e.toString() );  
                System.exit(1);  
	        }  
	        catch(Exception e ) {  
	            System.out.println();  
	            System.out.println( "Exception: " + e.toString() );  
                System.exit(1);  
	        }
	}
	
	public void execute() {
		String operation;
		
		try {
			System.out.println("Digite expressões matemáticas (ou 'exit' para sair):");
            in.nextLine();
			while(true) {
                System.out.print(">> ");
				operation = in.nextLine();
                if (operation.equals("exit")) {
                    break;
                }
                try {
                    System.out.println(calculator.calculate(operation)+"\n");
                } catch(Exception e) {
                    System.out.println("Operação Inválida!\n"+e);
                }
			}
        System.out.println("Finalizando cliente RMI.");
		in.close();
		}catch(Exception e) {		
			System.out.println( "Exception: " + e.toString()); 
		}
	}
	
	public static void main(String[] args) {
		Client c = new Client();
		c.execute();
	}
}
