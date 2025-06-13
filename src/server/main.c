#include <stdio.h>
#include <asm/termbits.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>

////////////////////////// OWN INCLUDES
#include "http.h"
#include "defines.h"


////////////////////////// DEFINES
#define MAX_CLIENTS 69
//const int PAGE_SIZE = getpagesize();
#define PAGE_SIZE 4096
#define MAX(_x, _y) (((_x) > (_y) ? (_x) : (_y)))
#define MIN(_x, _y) (((_x) < (_y) ? (_x) : (_y)))
#define ERROR_IF_ZERO(_x, _msg)		do{if((_x) <= 0){perror(_msg); has_error = 1; goto clean_and_exit;}}while(0);
#define ERROR_IF_NEGATIVE(_x, _msg) do{if((_x) < 0) {perror(_msg); has_error = 1; goto clean_and_exit;}}while(0);
#define LOG_IF_NEGATIVE(_x, _msg)	do{if((_x) < 0) {perror(_msg);		   }}while(0);
#define DEBUG_ENDPOINT(_msg)		printf("debugging started...\n" _msg); int a = 1; while(a){asm("nop");};

////////////////////////// GLOBALS
sig_atomic_t finished = 0;
bool has_error = 0;

////////////////////////// DECLARATIONS
void sigint_handler(int _sig_num);


int main()
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, sigint_handler);

	int master_socket_fd = 0;
	int operation_res;
	int opt = 1;
	struct sockaddr_in addr, client_addr;

	//init variables
	memset(&client_addr, 0, sizeof(client_addr));
	memset(&addr, 0, sizeof(addr));

	master_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	ERROR_IF_NEGATIVE(master_socket_fd, "erorr creating socket\n");	//TODO: add errno there later

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8080);

	operation_res = setsockopt(master_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	ERROR_IF_NEGATIVE(operation_res, "setsockopt failed\n");

	operation_res = bind(master_socket_fd, (struct sockaddr*)&addr, sizeof(addr));
	ERROR_IF_NEGATIVE(operation_res, "error binding");

	operation_res = listen(master_socket_fd, 69);
	ERROR_IF_NEGATIVE(operation_res, "error listening");

	fd_set read_fd;
	int list_of_sockets[MAX_CLIENTS];

	//init all of them to -1 (illegal socket num)
	for(int i = 0; i < MAX_CLIENTS; ++i) {
		list_of_sockets[i] = -1;
	}

	while(true) {
		//reset set of listening sockets
		FD_ZERO(&read_fd);

		//add master to it
		FD_SET(master_socket_fd, &read_fd);

		//add all active sockets as well
		for(int sock_index = 0; sock_index < MAX_CLIENTS; ++sock_index) {
			if(list_of_sockets[sock_index] > 0) {
				FD_SET(list_of_sockets[sock_index], &read_fd);
			}
		}

		//define a range over which we have to listen for activity
		int max_sd = master_socket_fd;

		for(int i = 0; i < MAX_CLIENTS; ++i) {
			int temp_descriptor = list_of_sockets[i];
			max_sd = MAX(max_sd, temp_descriptor);
		}

		//set correct timeout for activity - usefull for cleaning
		struct timeval select_timeout = {0};
		select_timeout.tv_usec = 300;

		//wait for activity on any of the sockets
		int activity = select(max_sd + 1, &read_fd, NULL, NULL, &select_timeout);
		if(finished) break;

		if(activity < 0 && errno != EINTR) {
			perror("activity error");
		}

		// if master socket has activity it means we have new connection waiting for us
		if(FD_ISSET(master_socket_fd, &read_fd)) {
			int addrlen = sizeof(client_addr);
			int new_socket = accept(master_socket_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);
			ERROR_IF_NEGATIVE(new_socket, "accept error\n");

			//we have a new fucker, now we have to add it to our fucked up list
			for(int sock_index = 0; sock_index < MAX_CLIENTS; ++sock_index) {
				if(list_of_sockets[sock_index] == -1) {
					list_of_sockets[sock_index] = new_socket;
					break;
				}
			}
		}

		//now we can iterate over rest of those miserable connections
		for(int i = 0; i < MAX_CLIENTS; ++i) {
			int active_socket = list_of_sockets[i];

			//socket is inactive
			if(active_socket <= 0) continue;

			//if it is not this one, continue
			if(!FD_ISSET(active_socket, &read_fd)) continue;


			//prepare arena for this socket
			arena_s arena;
			arena_init(&arena);

			//check how much we read
			//char request_buffer[PAGE_SIZE];	//assume maximum for header is page size
			char* request_buffer;

			int bytes_pending;
			operation_res = ioctl(active_socket, FIONREAD, &bytes_pending);
			ERROR_IF_NEGATIVE(operation_res, "ioctl error when checking amount of pending bytes\n");
			printf("Pending bytes: %d\n", bytes_pending);
			request_buffer = (char*)get_raw_memory(&arena, sizeof(char) * (bytes_pending + 1));

			int flags = fcntl(active_socket, F_GETFL, 0);
			fcntl(active_socket, F_SETFL, flags | O_NONBLOCK);

			int bytes_read = 0;
			bytes_read = recv(active_socket, request_buffer, bytes_pending, MSG_WAITALL);
			ERROR_IF_NEGATIVE(bytes_read, "read error");

			fcntl(active_socket, F_SETFL, flags);

			//if nothing it means that the socket wants to disconnect =c
			if(!bytes_read) {
				printf("want to disconnect\n");
				//remove it from list of sockets
				for(int sock_index = 0; sock_index < MAX_CLIENTS; ++sock_index) {
					if(list_of_sockets[sock_index] == active_socket) {
						list_of_sockets[sock_index] = -1;
						break;
					}
				}
				operation_res = close(active_socket);
				LOG_IF_NEGATIVE(operation_res, "closing error, continuing");
				continue;
			}


			//socket sent something to us now we:
			//-parse and verify header
			//-read body
			//-respond
			request_buffer[bytes_read] = '\0';
			printf("read %d bytes\n", bytes_read);
			printf("contents: \n%s\n", request_buffer);

			//create and init those suckers
			http_body_s http_body = {0};
			http_header_s http_header = {0};
			http_response_s http_response = {0};


			http_parse_request(&arena, &http_header, &http_body, request_buffer);
			//TODO: validate it

			//if expected size is bigger than current it means that we are fucked
			if(http_body.current_size < http_body.expected_size && http_header.method != HTTP_GET) {
				perror("there should be more data read. missing: ");
				printf("%d bytes\n", http_body.expected_size - http_body.current_size);
				DEBUG_ENDPOINT("debuggin started...");
			}

			http_create_response(&arena, &http_header, &http_body, &http_response);

			http_send_response(&arena, &http_response, active_socket);

			/*const char* test_resp = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 177\r\nConnection: close\r\n\r\n<html>\n<body style=\"display: grid; place-items: center; height: 100vh; margin: 0;\">\n<button onclick=\"window.location.href='/'\">Click to Send GET Request</button>\n</body>\n</html>";*/
			/*write(active_socket, test_resp, strlen(test_resp));*/

			//socket wants to disconnect now
			if(!http_header.keep_connection) {
				list_of_sockets[i] = -1;
				operation_res = close(active_socket);
				LOG_IF_NEGATIVE(operation_res, "closing error, continuing");
			}
		}
	}

clean_and_exit:
	for(int i = 0; i < MAX_CLIENTS; ++i) {
		if(list_of_sockets[i] <= 0) continue;
		int res = close(list_of_sockets[i]);
		printf("i: %d, fd: %d\n", i, list_of_sockets[i]);
		list_of_sockets[i] = -1;	//should be redundant
		LOG_IF_NEGATIVE(res, "error closing socket during cleaning");
	}
	operation_res = close(master_socket_fd);
	LOG_IF_NEGATIVE(operation_res, "closing error, continuing");

	printf("socket cleaned, exiting server...\n");

	if(has_error) exit(1);
	else		  return 0;
}


////////////////////////// DEFINITIONS
void sigint_handler(int _sig_num)
{
	if(!(_sig_num == SIGINT)) return;

	printf("\nrecieved <C-c>, cleaning...\n");
	finished = 1;
}
