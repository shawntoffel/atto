#ifndef _SERVER_H_
#define _SERVER_H_

typedef struct atto_server {
    const char *port;
    const int file_descriptor;
} atto_server_t;

atto_server_t atto_init_server(char *port);
int atto_handle_next_connection(atto_server_t *server, char *response);
int atto_close_server(atto_server_t *server);

#endif