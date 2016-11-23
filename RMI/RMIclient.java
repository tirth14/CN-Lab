import java.rmi.*;

public class RMIclient
{
	public static void main(String[] args) throws Exception
	{
		if(args.length == 2)
		{
			String url = new String("rmi://"+args[0]+"/RMI");
			RMI rMI = (RMI)Naming.lookup(url);
			String serverReply = rMI.doConnect(args[1]);
			System.out.println("Server ("+url+") Reply: "+serverReply);
		}
		else
		{
			System.err.println("Usage: RMIclient <server> <name>");
		}
	}
}