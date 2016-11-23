import java.rmi.Naming;

public class RMIserver
{
	public static void main(String[] args) throws Exception
	{
		RMIimp rMIimp = new RMIimp();
		Naming.rebind("RMI",rMIimp);
		System.out.println("RMI object bound to name 'RMI' and is ready to use");
	}
}