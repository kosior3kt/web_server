#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>

#define ERROR_IF_ZERO(_x, _msg)		do{if((_x) <= 0){perror(_msg); exit(1);}}while(0);
#define ERROR_IF_NEGATIVE(_x, _msg) do{if((_x) < 0) {perror(_msg); exit(1);}}while(0);
#define LOG_IF_NEGATIVE(_x, _msg)	do{if((_x) < 0) {perror(_msg);		   }}while(0);


int main()
{
	///create a socket
	int socket = 0;
	int operation_res;
	struct sockaddr_in addr;

	//init variables
	memset(&addr, 0, sizeof(addr));

	socket = socket(AF_INET, SOCK_STREAM, 0);
	ERROR_IF_NEGATIVE(master_socket_fd, "erorr creating socket\n");	//TODO: add errno there later

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8080);

	operation_res = bind(, (struct sockaddr*)&addr, sizeof(addr));
	ERROR_IF_NEGATIVE(operation_res, "error binding");

	operation_res = listen(master_socket_fd, 69);
	ERROR_IF_NEGATIVE(operation_res, "error listening");

	fd_set read_fd;
	int list_of_sockets[MAX_CLIENTS];

	//init all of them to -1 (illegal socket num)
	for(int i = 0; i < MAX_CLIENTS; ++i) {
		list_of_sockets[i] = -1;
	}


	printf("dupa\n");
	return 0;
}
