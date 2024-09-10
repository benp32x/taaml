/***************************************************************************
 TRIBES AERIAL ASSAULT MASTER LIST / WRITTEN BY DREAMCAST TM <B.R.> (C) 2010

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**************************************************
 * user defined
 **************************************************/

#define MAXSERVERS 100 // maximum number of servers this list should accept
#define MAXTIMEOUT 300 // maximum number of seconds to wait before timing out a server

/**************************************************
 * cross-platform compatibility
 **************************************************/

#ifdef WIN32
	#include <winsock.h> // don't forget to link wsock32.lib before compiling
	SOCKET master;
#else
	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <memory.h>
	#include <sys/socket.h>
	#include <sys/times.h>
	#include <sys/types.h>
	#include <unistd.h>
	int master;
#endif

typedef unsigned char	uch;
typedef unsigned short	u16;
typedef unsigned long	u32;

/**************************************************
 * application specific definitions
 **************************************************/

#define PORT 15101 // udp port this application uses

struct serverinfo
{	uch name[16];
	u16 port;
	u32 ip;
	u32 lastupdate;	// (in seconds)
};

struct serverinfo servers[MAXSERVERS];

/**************************************************
 * application specific methods
 **************************************************/

void console(uch *s, u16 i) // write text to the console
{	u32 ip = servers[i].ip;
	printf("   > SERVER %s (%03u.%03u.%03u.%03u:%05u): \"%s\"\n",
	s,ip&255,(ip>>8)&255,(ip>>16)&255,(ip>>24)&255,htons(servers[i].port),servers[i].name);
}

int main(int argc, char *argv[]) // application entry point
{//	responses the server sends generally
	uch cpClientRespH[] = {5,2,0,3,0,0,0,128,18,14,0,4,0,0};
	uch cpServerRespH[] = {5,2,0,1,0,0,0,6,0,0,0,0,0,0,0,0,0,0};
	uch cpEntry[] = {83,6,0,0,0,0,0,0,1,0,1,86,4,0,49,49,54,56};

//	responses the server sends for the test entry
	uch cpDummy[] = {
	0x03,0x00,0x90,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1B,0x00,0x20,0x00,
	0x6D,0x00,0x61,0x00,0x73,0x00,0x74,0x00,0x65,0x00,0x72,0x00,0x20,0x00,0x6C,0x00,
	0x69,0x00,0x73,0x00,0x74,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6E,0x6F,0x74,0x20,
	0x61,0x20,0x73,0x65,0x72,0x76,0x65,0x72,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00};
		
	uch cpReject[] = {
	0xAB,0xCD,0x00,0x01,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x66};

//	common variables
	uch buff[1024], name[16], *serverlist, serverstart = 0, verbose = 0;
	u16 a, c, i = 1, servercount = 0;
	u32 cc;

//	sock variables and startup
	struct sockaddr_in peer;

	int so, psize = sizeof(peer), rsize;
	
	#ifdef WIN32
		WSADATA wsa;
		WSAStartup(MAKEWORD(1,0), &wsa);
	#endif

//	if modifications are made, add additional credits below this line
	printf("TRIBES: AERIAL ASSAULT MASTER LIST [1.0] BY DREAMCAST TM <BR> 2010\n\n");
	printf("usage: (appname).exe [optional: -v] [optional: (server ip address)]\n\n");

//	parse the arguments (check for verbose mode)
	if(argc > 1 && argv[1][0] == '-' && argv[1][1] == 'v')
	{	printf("   > VERBOSE MODE ENABLED\n");
		verbose = 1; i++;
	}

//	if the ip address of the server is defined, create a test entry response
	if((i == 1 && argc > 1) || (i == 2 && argc > 2))
	{	printf("   > TEST ENTRY ADDED USING ADDRESS: (%s)\n\n", argv[i]);
		servers[0].ip = inet_addr(argv[i]);
		servers[0].port = htons(PORT);
		memcpy(cpDummy + 6, (uch*)&c, 2);
		memcpy(cpDummy + 8, (uch*)&servers[0].ip, 4);
		servercount = 1;
		serverstart = 1;
	}else if(verbose) printf("\n");

//	******************************

	peer.sin_addr.s_addr = INADDR_ANY;
	peer.sin_family = AF_INET;
	peer.sin_port = htons(PORT);

	master = socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(master, SOL_SOCKET, SO_REUSEADDR, (char*)&so, sizeof(so));
	
	if(bind(master, (struct sockaddr*)&peer, psize) < 0)
	{	printf("COULD NOT BIND TO PORT %u. FREE PORT AND TRY AGAIN.\n", PORT);
		exit(1);
	}

//	wait for incoming requests and handle them
	for(;;)
	{	if((rsize = recvfrom(master, buff, sizeof(buff), 0, (struct sockaddr*)&peer, &psize)) != -1)
		{	if(buff[0] == 0xAB) // sends "password incorrect" message if player selects test entry
			{	buff[3] = 0;
				sendto(master, cpReject, sizeof(cpReject), 0, (struct sockaddr*)&peer, psize);
			}else switch(buff[3])
			{	case 0x4: // test server entry response
					cpDummy[4] = buff[4];
					cpDummy[5] = buff[5];
					sendto(master, cpDummy, sizeof(cpDummy), 0, (struct sockaddr*)&peer, psize);
				
					break;
				
				case 0x67: // client requesting list
					cc = clock() / CLOCKS_PER_SEC; // time for removing timed out entries

				//	start by removing timed out entries
					for(i = serverstart; i < servercount; i++)
					if(cc - servers[i].lastupdate > MAXTIMEOUT)
					{	if(verbose) console("TIMEOUT", i);
						servers[i] = servers[--servercount];
					}

				//	allocate space for list of servers
					serverlist = malloc(14 + (18 * servercount));
					
				//	set the number of servers (little endian, as 256 ^ n) and add to the list
					cpClientRespH[12] = servercount % 256;
					cpClientRespH[13] = servercount / 256;

					memcpy(serverlist, cpClientRespH, 14);

				//	cycle through the servers and add them to the list
					for(i = 0; i < servercount; i++)
					{	memcpy(cpEntry + 2, (uch*)&servers[i].port, sizeof(servers[i].port));
						memcpy(cpEntry + 4, (uch*)&servers[i].ip, sizeof(servers[i].ip));
						memcpy(serverlist + (14 + (18 * i)), cpEntry, 18);
					}

				//	send the list to the client and free allocated memory
					sendto(master, serverlist, (14 + (servercount * 18)), 0, (struct sockaddr*)&peer, psize);
					free(serverlist);
				
					break;
				
				case 0xD7: // server registering / updating
					if((a = (buff[30] * 2)) > 30) break; // get and check server name length
					
				//	retrieve and verify the server name
					for(c = 0, i = 32; i < 32 + a; i += 2)
					{	if(buff[i] != buff[i + a + 5]) break;	// ensure both instance names match
						if(buff[i] > 15) name[c++] = buff[i];	// only allow certain characters
					}
					if(i < (32 + a)) break; // exit if the loop exited early
					name[c++] = 0;
					
				//	if an entry exists for the ip, exit and either reuse or update its time
					for(i = 0; i < servercount; i++)
					if(servers[i].ip == peer.sin_addr.s_addr && servers[i].port == peer.sin_port) break;
					
				//	ensure adding this entry won't exceed the maximum allowed servers
					if(i == servercount && servercount == MAXSERVERS) break;
					
				//	add server name and last update time
					memcpy(servers[i].name, name, c);
					servers[i].lastupdate = clock() / CLOCKS_PER_SEC;
					
					if(i == servercount)
					{	servers[i].ip = peer.sin_addr.s_addr;
						servers[i].port = peer.sin_port;
						if(verbose) console("STARTED", servercount++);
					}

				//	! break command is intentionally missing here to cascade over case below !
					
				case 0xD1: // end of match / disconnect response
					if(buff[3] == 0xD1)
					{	for(i = serverstart; i < servercount; i++)
						if(servers[i].ip == peer.sin_addr.s_addr && servers[i].port == peer.sin_port)
						{	servers[i] = servers[--servercount];
							if(verbose) console("ENDGAME", i);
							break;
					}	}

				//	send response back to server (for either 0xD7 or 0xD1)
					memcpy(cpServerRespH + 8, (uch*)&peer.sin_port, sizeof(peer.sin_port));
					memcpy(cpServerRespH + 10, (uch*)&peer.sin_addr.s_addr, sizeof(peer.sin_addr.s_addr));
					sendto(master, cpServerRespH, sizeof(cpServerRespH), 0, (struct sockaddr*)&peer, psize);

					break;
				
				default:
					printf("   > UNKNOWN PACKET DROPPED: (%s:%d)\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port));
					break;
	}	}	}
	return 1;
}