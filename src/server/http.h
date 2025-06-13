#ifndef __HTTP__
#define __HTTP__

#include "../../std_lib/allocators/arena.h"
#include "defines.h"
#include <stdbool.h>

//networking
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>

////////////////////////// TYPEDEFS
typedef enum {
	UNKNOWN_METHOD = 0,
	HTTP_GET,
	HTTP_POST,
} http_method_e;

/* aint got time for this shit just yet...
typedef enum {
	UNKNOWN = 0,

} http_content_type_e;
*/

typedef enum {
	UNKNOWN_STATUS = 0,
	HTTP_200 = 200,
	HTTP_201,
	HTTP_400 = 400,
	HTTP_401,
	HTTP_402,
	HTTP_403,
	HTTP_404,
	HTTP_500 = 500,
}http_status_e;

typedef enum {
	UNKNOWN_STANDARD = 0,
	HTTP_1_1,
	HTTP_2,
}http_standard_e;

typedef enum {
	UNKNOWN_CONNECTION_STATUS = 0,
	HTTP_CLOSE_CONNECTION,
	HTTP_KEEP_CONNECTION,
}http_connection_e;

typedef struct {
	http_method_e method;
	bool keep_connection;
	char* path;
	http_standard_e http_standard;
}http_header_s;

typedef struct {
	uint current_size;
	uint expected_size;
	uint8_t* data;
}http_body_s;

typedef struct {
	uint body_size;
	http_status_e status;
	http_standard_e http_standard;
	char* content_type;
	http_connection_e keep_connection;
	uint8_t* body;
}http_response_s;

////////////////////////// DECLARATIONS
int http_parse_request(arena_s*, http_header_s*, http_body_s*, const char*);
int http_read_body(arena_s*, http_body_s*);
int http_create_response(arena_s*, http_header_s*, http_body_s*, http_response_s*);
int http_send_response(arena_s*, const http_response_s*, const int);

#endif//__HTTP__
