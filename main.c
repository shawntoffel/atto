#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "response.h"
#include "server.h"
#include "env.h"

static const char *default_port = "8080";
static int running = 1;

static void interupt_handler(int signum)
{
    printf("Stopping server\n");
    running = 0;
}

int main(int argc, char** argv) 
{
    struct sigaction action;
    action.sa_handler = interupt_handler;
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    
    char *port = (char *)default_port;
    atto_set_from_env("PORT", &port);

    atto_response_t response = atto_default_response();
    atto_set_from_env("RESP_STATUS", &response.status_code);
    atto_set_from_env("RESP_CONTENT_TYPE", &response.content_type);
    atto_set_from_env("RESP_BODY", &response.body);

    char *body = atto_build_response(response);

    atto_server_t server = atto_init_server(port);

    printf("Started server on port: %s...\n", port);

    while(running) {
        if (atto_handle_next_connection(&server, body) < 0) {
            free(body);
            return EXIT_FAILURE;
        }
    }

    free(body);
    atto_close_server(&server);

    return 0;
}