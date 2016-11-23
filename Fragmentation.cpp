#include <bits/stdc++.h>
using namespace std;

struct packet
{
	int mf;
	int offset;
	int length;
	string data;
};

vector<packet> fragment(packet parent,int mtu)
{
	int newsize = mtu;
	int rem = parent.length;
	vector<packet> child;
	bool flag = true;
	int temp =0;
	while(flag)
	{
		packet p;
		p.offset = 0;
		if(rem <= newsize)
		{
			flag = false;
			p.offset = temp;
			p.mf = 0;
			p.length = rem;
			for(int i =temp;i<(temp+p.length);i++)
			{
				p.data += parent.data[i];
			}
			child.push_back(p);
			rem =0;
		}
		else
		{
			p.offset = temp;
			p.mf = 1;
			p.length = mtu;
			for(int i=temp;i<temp+p.length;i++)
			{
				p.data += parent.data[i];
			}
			temp += p.length;
			child.push_back(p);
			rem = parent.length - temp;
		}
	}
	return child;
}

packet reassemble(vector<packet> frag)
{
	packet p;
	p.mf = 0;
	p.length = 0;
	for(int i=0;i<frag.size();i++)
	{
		p.length += frag[i].length;
	}
	string c = "";
	for(int i=0;i<frag.size();i++)
	{
		c += frag[i].data;
	}
	p.offset =0;
	p.data = c;
	return p;
}

int main()
{
	string data;
	int mtu;
	cout<<"Enter data to transmit\n";
	cin>>data;
	packet toFragment;
	toFragment.data = data;
	toFragment.mf = 0;
	toFragment.length = data.size();
	cout<<"Enter MTU :\n";
	cin>>mtu;
	vector<packet> vf = fragment(toFragment,mtu);
	for(int i=0;i<vf.size();i++)
	{
		printf("Packet 1: \n mf:\t%d\t offset:\t%d\t length:\t%d\t data:\t", vf[i].mf,vf[i].offset,vf[i].length);
		cout<<vf[i].data<<endl;
	}
	packet reass = reassemble(vf);
	cout<<"reassembled packet: \n";
	printf("mf:\t%d\toffset:\t%d\tlength:\t%d\t",reass.mf,reass.offset,reass.length );
	cout<<reass.data<<endl;
}