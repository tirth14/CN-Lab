import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMI extends Remote
{
	public String doConnect(String name) throws RemoteException;
}