import net.objecthunter.exp4j.Expression;
import net.objecthunter.exp4j.ExpressionBuilder;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject; 

public class Calculator extends UnicastRemoteObject implements CalculatorInterface {
     public Calculator() throws RemoteException {
        super();
    }

    @Override
    public double calculate(String operation) throws RemoteException {
        Expression e = new ExpressionBuilder(operation).build();
        double result = e.evaluate();
        System.out.println("Operação calculada: "+operation+" = "+result);
        return result;
    }
}