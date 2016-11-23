import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.*;

public class RMIimp extends UnicastRemoteObject implements RMI
{

	HashMap<String, Integer> map = new HashMap<String, Integer>();
	private static final long serialVerionUID = 1L;
	RMIimp() throws RemoteException
	{
		super();
		map.put("Foo",700);
		map.put("Bar",205);
		map.put("Alice",122);
		map.put("Bob",202);
	}
	public String doConnect(String name) throws RemoteException
	{
		try
		{
			int n = map.get(name);
			return "\nServer says: Hi "+name+". Your token is: "+n+"\n";
		}
		catch(Exception e)
		{
			return "\nCannot find token for "+name+" !!\n";
		}
	}
}