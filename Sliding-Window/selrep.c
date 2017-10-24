// Sliding Window Protocol : Selective Repeat

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int global_time = 0;

int num_packets = 10;
int window = 4;
int time_out = 50; //ms
int generation_time = 2; // milliseconds
int transmission_time = 20; // milli seconds
int packets_sent = 0;
int critical = 70;
int sender[10][4];

void generatePacket(int packetNumber)
{
	int x = rand() % 100;
	global_time = global_time + generation_time;
	sender[packetNumber][0] = global_time;
	sender[packetNumber][1] = x;

	printf("Generating Packet %d\n", packetNumber+1);
}

int main()
{
	for ( int i = 0 ; i < num_packets ; i++ )
		sender[i][3] = 0;
	int flag, window_index = 0;
	int remaining = num_packets;
	srand((unsigned)time(NULL));
	while (packets_sent < num_packets)
	{
		int local_sent = 0;
		while ( local_sent < window && packets_sent < num_packets)
		{
			int index = window_index;
			int count = 0;
			while ( index < num_packets && count < window)
			{
				if ( sender[index][3] == 0 )
					generatePacket(index);
				count++;
				index++;
			}

			index = window_index;
			count = 0;
			while ( index < num_packets && count < window && packets_sent < num_packets)
			{
				int x = sender[index][1];
				int already_sent = sender[index][3];
				if ( already_sent == 0 )
				{
					if ( x > critical )
					{
						printf("Packet Number %d failed in transit\n", index+1);
						flag = 1;
						// break;
					}
					else{
						sender[index][2] = sender[index][0] + transmission_time;
						sender[index][3] = 1;
						printf("Packet Recieved %d \n", index+1);
						packets_sent++;
						local_sent++;	
						remaining--;					
					}
				}
				count++;
				index++;
			}
			flag = 0;
		}
			window_index += window;
	}
}
