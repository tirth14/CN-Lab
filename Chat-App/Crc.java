import java.util.Scanner;

class Crc
{
	public static void main(String args[])
	{
		Scanner sc=new Scanner(System.in);
		int m,g[],n,d[],z[],r[],msb,i,j,k;

		System.out.print("Enter no. of data bits : ");
		n=sc.nextInt();
		System.out.print("Enter no. of generator bits : ");
		m=sc.nextInt();
		d=new int[n+m];
		g=new int[m];
		String da = sc.nextLine();
		System.out.print("Enter data bits : ");
		da = sc.nextLine();
		for(i=0;i<da.length();i++)
			d[i]=da.charAt(i)-48;
		System.out.print("Enter generator bits : ");
		da = sc.nextLine();
		for(j=0;j<da.length();j++)
			g[j]=da.charAt(j)-48;
		for(i=0;i<m-1;i++)
			d[n+i]=0;

		d = generator(d,g,n,m);
		System.out.print("Generated data with crc: ");
		for(i=0;i<n+m-1;i++)
		{
			System.out.print(d[i]);
		}

		boolean ans = verifier(d,g,n,m);

		if(ans)	System.out.print("\nCorrect data. ");
		else System.out.print("\nIncorrect data. ");

		int a;
		System.out.print("\nEnter bit no. to be altered. ");
		a = sc.nextInt();
		d = alterArg(d,a);
		ans = verifier(d,g,n,m);

		if(ans)	System.out.print("\nCorrect data. ");
		else System.out.print("\nIncorrect data. ");
	}

	public static int[] alterArg(int[] d,int a)
	{
		d[a] = xor(d[a],1);
		return d;
	}

	public static boolean verifier(int[] d,int[] g,int n,int m)
	{
		int z[],r[],msb,i,j,k;
		r=new int[m+n];
		for(i=0;i<m;i++)
			r[i]=d[i];
		z=new int[m];
		for(i=0;i<m;i++)
			z[i]=0;
		
		for(i=0;i<n;i++)
		{
			k=0;
			msb=r[i];
			for(j=i;j<m+i;j++)
			{
				if(msb==0)
				r[j]=xor(r[j],z[k]);
				else
				r[j]=xor(r[j],g[k]);
				k++;
			}
			r[m+i]=d[m+i];
		}System.out.print("\n");
		for(i=0;i<n+m;i++)
		{
			if(r[i] == 1)	return false;
		}
		return true;
	}
	public static int[] generator(int[] d,int[] g,int n,int m)
	{
		int z[],r[],msb,i,j,k;
		r=new int[m+n];
		for(i=0;i<m;i++)
			r[i]=d[i];
		z=new int[m];
		for(i=0;i<m;i++)
			z[i]=0;
		
		for(i=0;i<n;i++)
		{
			k=0;
			msb=r[i];
			for(j=i;j<m+i;j++)
			{
				if(msb==0)
				r[j]=xor(r[j],z[k]);
				else
				r[j]=xor(r[j],g[k]);
				k++;
			}
			r[m+i]=d[m+i];
		}
		System.out.print("\nThe code bits added are : ");
		for(i=n;i<n+m-1;i++)
		{
			d[i]=r[i];
			System.out.print(d[i]);
		}System.out.print("\n");
		return d;
	}

	public static int xor(int x,int y)
	{
		if(x==y)
		return(0);
		else
		return(1);
	}
}
