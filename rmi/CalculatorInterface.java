import java.rmi.Remote;
import java.rmi.RemoteException;

public interface CalculatorInterface extends Remote{
	double calculate(String operation) throws RemoteException;
}