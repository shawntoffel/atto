#include <stdio.h>
#include <stdlib.h>
#include "response.h"

#define SERVER_NAME "atto"

static const char * response_template = "HTTP/1.1 %s\r\nServer: %s\r\nContent-type: %s\r\n\r\n%s";

atto_response_t atto_default_response() {
    atto_response_t response;
    response.status_code = "200 OK";
    response.content_type = "text/plain; charset=utf-8";
    response.body = "Hello!";
    return response;
}

static int execute_template(char *buffer, int size, atto_response_t resp) {
    return snprintf(buffer, size, response_template, resp.status_code, SERVER_NAME, resp.content_type, resp.body);
}

char* atto_build_response(atto_response_t resp) {
    //Execute first with NULL buffer to determine required size.
    int size = execute_template(NULL, 0, resp) + 1;

    char *buffer = malloc(size);
    execute_template(buffer, size, resp);
    
    return buffer;
}