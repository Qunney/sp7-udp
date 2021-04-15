#include <iostream>
#include <sys/types.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>

using namespace std;

#define DEFAULT_PORT 5555
#define BUFFER_SIZE 1024
#define SOCKET_CLOSE_SYMBOL '#'

bool find_exit_symbol(const char* msg);

int main(int argc, char const* argv[]) 
{
	int sock, msg, port, serv_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	socklen_t client_len;
	argc >= 2 ? port = atoi(argv[1]) : port = DEFAULT_PORT;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0) 
	{
		cout << "ServerError: socket establishing error!";
		exit(0);
	}
	cout <<  "ServerInfo: socket establishing success!" << endl;
	server_address.sin_port = htons(port);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	serv_len = sizeof(server_address);
	int ret = bind(sock,(struct sockaddr *)&server_address, serv_len);	
	if (ret < 0) 
	{
		cout << "ServerError: can't binding connection.\n";
		return -1;
	}
	cout << "ServerInfo: binding to port" << port << endl;
	char buffer[BUFFER_SIZE];
	bool isExit = false;
	client_len = sizeof(client_address);
	while(!isExit) 
	{
		cout << "client: ";
		msg = recvfrom(sock,
					   buffer,
					   BUFFER_SIZE,
				 	   0,
				 	   (struct sockaddr *)&client_address,
				 	   &client_len);
		cout << buffer << endl;
		if (find_exit_symbol(buffer))
		{
			break;
		}
		if(msg > 0) 
		{
			sendto(sock,
			       "OK\n",
		 		   BUFFER_SIZE, 0,
				   (struct sockaddr *)&client_address,
				   client_len);
		}
	}

	cout << "\n Good bye!" << endl;
	close(sock);
	return 0;
}

bool find_exit_symbol(const char* msg) 
{
	for(int i=0; i < strlen(msg); ++i)
	{
		if(msg[i] == SOCKET_CLOSE_SYMBOL)
		{
			return true;
		}
	}
	return false;
}