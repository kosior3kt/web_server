#include "http.h"


////////////////////////// DEFINITIONS
int http_parse_request(arena_s* _arena, http_header_s* _header, http_body_s* _body, const char* _input_buffer)
{
	int ret_val = 0;	//0 means no error

	//init body in case we will need to read it here
	_body->expected_size = 0;
	_body->current_size = 0;

	//find the end of the header first
	char* header_end = strstr(_input_buffer, "\r\n\r\n");
	if(!header_end) {
		perror("malformed header - no \\r\\n\\r\\n found");
	}
	uint header_size = (uint)(header_end - _input_buffer);


	//prepare buffer that we will work on
	char* input_buffer_before_tokenization = (char*)get_raw_memory(_arena, sizeof(char) * (header_size + 1));
	strncpy(input_buffer_before_tokenization, _input_buffer, header_size);
	*(input_buffer_before_tokenization + header_size) = '\0';

	char* line;
	uint line_len;

	char* word;
	uint word_len;

	//for strtok_r
	char* lines_saveptr;
	char* word_saveptr;



	//delim by new line
	line = strtok_r(input_buffer_before_tokenization, "\r\n", &lines_saveptr);

	//parse first line for method
	word = strtok_r(line, " ", &word_saveptr);
	word_len = strlen(word);
	if(!word) {
		perror("malformed http. expected method, got nothing");
	}

	//assign correct method now
	//TODO: change to hash based approach later on
	if(strncmp(word, "GET", word_len) == 0) {
		_header->method = HTTP_GET;
	} else if(strncmp(word, "POST", word_len) == 0) {
		_header->method = HTTP_POST;
	} else {
		perror("unrecognized header\n");
		printf("got %s\n", word);
	}

	//then for path
	word = strtok_r(NULL, " ", &word_saveptr);
	word_len = strlen(word);
	if(!word) {
		perror("malformed http. expected path, got nothing");
	}

	//assign get memory for path and copy token to it
	_header->path = (char*)get_raw_memory(_arena, sizeof(char) * (word_len + 1));
	strncpy(_header->path, word, word_len);
	*(_header->path + word_len) = '\0';

	//and then for http standard
	word = strtok_r(NULL, " ", &word_saveptr);
	word_len = strlen(word);
	if(!word) {
		perror("malformed http. expected http standard, got nothing");
	}

	//assign get memory for path and copy token to it
	if(strncmp(word, "HTTP/1.1", word_len) == 0) {
		_header->http_standard = HTTP_1_1;
	} else if(strncmp(word, "HTTP/2", word_len) == 0) {
		_header->http_standard = HTTP_2;
	} else {
		_header->http_standard = UNKNOWN_STANDARD;
	}

	//then we can parse rest of lines
	line = strtok_r(NULL, "\n", &lines_saveptr);
	line_len = strlen(line);
	while(line) {
		word = strtok_r(line, " ", &word_saveptr);
		word_len = strlen(word);

		//TODO: change to hash based approach later on
		if(strncmp(word, "Connection:", word_len) == 0) {
			word = strtok_r(line, " ", &word_saveptr);
			word_len = strlen(word);
			if(strncmp(word, "keep-alive", word_len) == 0) {
				_header->keep_connection = true;
			} else if(strncmp(word, "close", word_len) == 0) {
				_header->keep_connection = false;
			} else {
				perror("got unknow connection request\n");
				printf("%s\n", word);
				_header->keep_connection = false;
			}
		} else if(strncmp(word, "Host:", word_len) == 0) {
			word = strtok_r(NULL, " ", &word_saveptr);
			word_len = strlen(word);
			printf("remeber to implement host parsing\n");
		} else if(strncmp(word, "Content-Length:", word_len) == 0) {
			word = strtok_r(NULL, " ", &word_saveptr);
			word_len = strlen(word);
			_body->expected_size = atoi(word);

		} else if(strncmp(word, "Content-Type:", word_len) == 0) {
			word = strtok_r(NULL, " ", &word_saveptr);
			word_len = strlen(word);
			printf("remeber to implement content type parsing\n");
		} else {
			//idk, do sth here
		}
		line = strtok_r(NULL, "\n", &lines_saveptr);
	}


	char* body;
	uint body_len;

	//if we have some expected size it means we can possibly get part of it here
	if(_body->expected_size > 0) {

		//find the beggining of body
		header_end = header_end + 4;
		if(*header_end != '\0') {
			//part of body is still in this request
			body = header_end;
			//body_len = strlen(body);
			body_len = _body->expected_size;

			//now we can put rest of 'section' as body beggining
			_body->data = (uint8_t*)get_raw_memory(_arena, sizeof(uint8_t) * (_body->expected_size + 1));
			strncpy(_body->data, body, body_len);
			_body->current_size = body_len;
			if(_body->current_size == _body->expected_size) {
				*(_body->data + _body->expected_size) = '\0';
			}
		}
	}

	return ret_val;
}

int http_read_body(arena_s* _arena, http_body_s* _body)
{
	int ret_val = 0;	//0 means no error
	return ret_val;
}

int http_create_response(arena_s* _arena, http_header_s* _header, http_body_s* _body, http_response_s* _response)
{
	int ret_val = 0;	//0 means no error

	// for now just read html template, and send it. if it doesn't exist send 404

	//TODO: jk - fix / case later
	FILE* fp;
	if(strcmp(_header->path, "/") == 0) {
		fp = fopen("templates/index.html", "rb");
	} else {
		char* temp_path = arena_concat_string(_arena, "templates/", _header->path);
		fp = fopen(temp_path, "rb");
	}

	if(!fp) {
		printf("file: %s not found\n", _header->path);
		return -1;
	}

	//status is fine by default
	_response->status = HTTP_200;

	//TODO: jk - give it some memory later
	_response->http_standard = HTTP_1_1;

	//TODO: jk - give it some memory later and make it enum
	_response->content_type = arena_concat_string(_arena, "", "text/html");

	_response->keep_connection = HTTP_CLOSE_CONNECTION;

	// later on we will do some funky templating in there, but for now it can just be static
	size_t line_size_temp = 0;
	char* line = NULL;
	uint line_size = 0;

	//TODO: jk - make it construct body inplace later
	_response->body = NULL;
	while((line_size = getline(&line, &line_size_temp, fp)) != -1) {
		if(_response->body == NULL) {
			_response->body = arena_concat_string(_arena, "", line);
		} else {
			_response->body = arena_concat_string(_arena, _response->body, line);
		}
		_response->body_size += line_size;
	}
	//final response is constructed in http_send_response

	fclose(fp);
	return ret_val;
}

int http_send_response(arena_s* _arena, const http_response_s* _response, const int _socket)
{
	int ret_val = 0;	//0 means no error

	//create response here string
	char* full_response = NULL;

	//first add header:
	//status line:
	//http standard
	switch(_response->http_standard) {
		case HTTP_1_1:
			full_response = arena_concat_string(_arena, "", "HTTP/1.1 ");
			break;
		case HTTP_2:
			full_response = arena_concat_string(_arena, "", "HTTP/2.0 ");
			break;
	}

	//http status and message
	switch(_response->status) {
		//TODO: jk - make it read status code with %d later...
		case HTTP_200:
			full_response = arena_concat_string(_arena, full_response, "200 OK\r\n");
			break;
		case HTTP_201:
			full_response = arena_concat_string(_arena, full_response, "201 Created\r\n");
			break;
		case HTTP_404:
			full_response = arena_concat_string(_arena, full_response, "404 Not Found\r\n");
			break;
		default:
			printf("got uncovered code: %d\n", _response->status);
			assert(0);
			break;
	}

	//content type
	//for now let's only do html
	full_response = arena_concat_string(_arena, full_response, "Content-Type: text/html\r\n");

	//content length
	char temp_buffer[128];
	memset(temp_buffer, 0, 128);
	snprintf(temp_buffer, 128, "Content-Length: %d\r\n", _response->body_size);
	full_response = arena_concat_string(_arena, full_response, temp_buffer);

	//connection (keep/close)
	switch(_response->keep_connection) {
		case HTTP_CLOSE_CONNECTION:
			full_response = arena_concat_string(_arena, full_response, "Connection: close\r\n\r\n");
			break;
		case HTTP_KEEP_CONNECTION:
			full_response = arena_concat_string(_arena, full_response, "Connection: keep-alive\r\n\r\n");
			break;
	}

	//body
	//TODO: jk - add error handling here later
	full_response = arena_concat_string(_arena, full_response, _response->body);

	printf("==================\n");
	printf("fulle response: %s\n", full_response);
	printf("with len: %d\n", strlen(full_response));
	printf("and body len: %d\n", _response->body_size);
	printf("==================\n");

	write(_socket, full_response, strlen(full_response));
	return ret_val;
}
