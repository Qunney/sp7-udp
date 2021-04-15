#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>

#define SOCKET_CLOSE_CONNECTION_SYMBOL '#'
#define DEFAULT_PORT 5555
#define BUFFER_SIZE 1024

using namespace std;


bool find_exit_symbol(const char* msg);

int main(int argc, char const* argv[]) 
{
	int sock, status, port;
	struct sockaddr_in client_address;
	struct sockaddr_in server_address;
	struct hostent *hp;
	socklen_t len;
	argc >= 3 ? port = atoi(argv[2]) : port = DEFAULT_PORT;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0) 
	{
		cout << "ClientError: establishing socket error!";
		exit(1);
	}
	server_address.sin_port = htons(port);
	server_address.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	if ( hp == 0 ) cout << "Unknown host" << endl;
	bcopy((char *)hp->h_addr, 
			(char *)&server_address.sin_addr,
			hp->h_length);

	cout << "\nClient socket created, port:" << port << endl;
	char buffer[BUFFER_SIZE];
	len = sizeof(client_address);
	cout << "Enter # to stop messaging with server" << endl;
	while(true) 
	{
		cout << "client:";
		cin.getline(buffer, BUFFER_SIZE);
		status = sendto(sock,
		                buffer,
		                BUFFER_SIZE,
		                0,
						(struct sockaddr *)&server_address,
						len);
		if(status < 0) 
		{
			cout << "ServerError: Cant send msg";
			close(sock);
			exit(1);
		}
		if(find_exit_symbol(buffer))
		{
			break;
		}
		cout << "server: ";
		recvfrom(sock,
		         buffer,
		         BUFFER_SIZE,
		         0,
				 (struct sockaddr *)&client_address,
				 &len);
		cout << buffer << endl;
	}

	cout << "\n Good bye!" << endl;
	close(sock);
	return 0;
}

bool find_exit_symbol(const char* msg) 
{
	for(int i=0; i < strlen(msg); ++i)
	{
		if(msg[i] == SOCKET_CLOSE_CONNECTION_SYMBOL)
		{
			return true;
		}
	}
	return false;
}