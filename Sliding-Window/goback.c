// Sliding Window GoBack N protocol

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int npackets = 10;
int time_out =50;
int gen_time = 50;
int win_size =4;
int packets_sent = 0;
int critical = 70;
int sender[10][3];
int p_time =0;
int err=0;

void send_pack(int pack_no)
{
	int prop_time = rand() % 100;
	int p_time = p_time+ gen_time;
	sender[pack_no][0] = p_time;
	sender[pack_no][1] = prop_time;
	printf("Generating Packet %d\n",pack_no+1 );
}


int main()
{
	srand((unsigned)time(NULL));
	while(packets_sent < npackets)
	{
		int count=0;
		int index = packets_sent;
		while(index<npackets && count<win_size)
		{
			send_pack(index);
			count++;
			index++;
		}
		index =packets_sent;
		count=0;
		err = 0;
		while(index<npackets && count<win_size)
		{
			int r_time = sender[index][1];
			if(r_time> critical)
			{
				printf("Packet %d failed in transit. \n", index+1 );
				err = 1;
			}
			else
			{
				if(!err)
				{
					printf("Recieved Packet %d\n",index+1);
					packets_sent++;
				}
				else
				{
					printf("Recieved Packet but not accepted %d\n",index+1);
				}
			}
			count++;
			index++;
		}

	}
}
