#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include "server.h"

#define LISTEN_QUEUE_SIZE 10

static struct addrinfo* find_available_addresses(char *port) {
    struct addrinfo hints, *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family =  AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;

    int err = getaddrinfo(NULL, port, &hints, &result);
    if (err != 0) {
        fprintf(stderr, "getaddrinfo %s\n", gai_strerror(err));
        return NULL;
    }

    return result;
}

static int try_bind_socket(struct addrinfo *address) {
    int opt_val;
    int file_descriptor = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    if (file_descriptor == -1) {
        return -1;
    }

    if (setsockopt(file_descriptor, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == -1) {
        close(file_descriptor);
        return -1;
    }

    if (bind(file_descriptor, address->ai_addr, address->ai_addrlen) == -1) {
        close(file_descriptor);
        return -1;
    }

    return file_descriptor;
}

static int find_file_descriptor(char *port) {
    int file_descriptor = -1;
    struct addrinfo *address, *index;

    address = find_available_addresses(port);
    if (address == NULL) {
        return -1;
    }

    for (index = address; index != NULL; index = index->ai_next) {
        file_descriptor = try_bind_socket(index);
        if (file_descriptor != -1) {
            break;
        }
    }

    freeaddrinfo(address);

    if (index == NULL) {
        return -1;
    }

    return file_descriptor;
}

atto_server_t atto_init_server(char *port) {
    atto_server_t server = {
        port,
        find_file_descriptor(port)
    };

    if (listen(server.file_descriptor, LISTEN_QUEUE_SIZE) == -1) {
        fprintf(stderr, "Failed to listen on port %s.\n", port);
        close(server.file_descriptor);
    }

    return server;
}

int atto_handle_next_connection(atto_server_t *server, char *response) {
    if (server == NULL) {
        fprintf(stderr, "Unable to wait for next connection. Server is NULL.\n");
        return -1;
    }

    if (server->file_descriptor < 0) {
        fprintf(stderr, "Unable to wait for next connection. Invalid server file descriptor.\n");
        return -1;
    }

    struct sockaddr_storage addr;
    socklen_t addr_size = sizeof addr;

    int connection_file_descriptor = accept(server->file_descriptor, (struct sockaddr *) &addr, &addr_size);
    if (connection_file_descriptor == -1) {
        if (errno == EINTR) {
            return 0;
        }

        perror("Failed to wait for next connection");

        if (errno == EBADF) {
            return -1;
        }

        return 0;
    }

    int response_length = (response == NULL) ? 0 : strlen(response);
    if (write(connection_file_descriptor, response, response_length) == -1) {
        perror("Failed to write response");
        return 0;
    }

    if (close(connection_file_descriptor) == -1) {
        perror("Failed to close file descriptor");
        return 0;
    }

    return 0;
}

int atto_close_server(atto_server_t *server) {
    if (server == NULL) {
        return 0;
    }

    return close(server->file_descriptor);
}
