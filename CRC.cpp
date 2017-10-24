// Cyclic Redundancy Check

#include <bits/stdc++.h>
using namespace std;

int xora(int a, int b)
{
	if(a==b)	return 0;
	else	return 1;
}
int* divide(int* d,int* g,int n,int m)
{
	int msb,i,j,k;
	int *r=new int[m+n];
	for(i=0;i<m+n-1;i++)
		r[i]=d[i];
	
	for(i=0;i<n;i++)
	{
		k=0;
		for(j=i;j<m+i;j++)
		{
			if(d[i]!=0)
			r[j]=xora(r[j],g[k]);
			k++;
		}
	}
	cout<<"\nThe code bits added are : ";
	for(i=n;i<n+m-1;i++)
	{
		d[i]=r[i];
		cout<<d[i];
	}cout<<"\n";
	return d;
}

int main()
{
	string s;
	cin>>s;
	int n = s.length();
	int d[n];
	for(int i=0;i<n;i++)	d[i] = s[i]-48;
	cin>>s;
	int rn = s.length();
	int g[n];
	int crc[n+rn-1];
	for(int i=0;i<n;i++)	crc[i] = d[i];
	for(int i=0;i<rn;i++)
	{
		g[i] = s[i]-48;
	}
	for(int i=0;i<rn-1;i++)	crc[i+n] = 0;
	int *final = divide(crc,g,n,rn);

	for(int i=0;i<n+rn-1;i++)	cout<<final[i];
}
