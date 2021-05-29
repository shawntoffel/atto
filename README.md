# atto
[![Docker Image Version (latest by date)](https://img.shields.io/docker/v/shawntoffel/atto?sort=semver&style=flat-square)](https://hub.docker.com/r/shawntoffel/atto/tags) [![Docker Image Size (latest by date)](https://img.shields.io/docker/image-size/shawntoffel/atto?sort=semver&style=flat-square)](https://hub.docker.com/r/shawntoffel/atto/tags)

Atto is a very small footprint HTTP server that returns a configurable string in response to a GET request. It is primarily used for prototyping, testing, and demonstration purposes. Especially with regard to container orchestration.  

The following environment variables can be used to configure Atto:

* `PORT` - The port to run on. (Default: 8080)
* `RESP_STATUS` - The HTTP status code you want Atto to return. (Default: 200)
* `RESP_CONTENT_TYPE` - The HTTP content-type you want Atto to return. (Default: text/plain; charset=utf-8)
* `RESP_BODY` - The string response body. (Default: Hello!)

---

Atto can also be used as a library by downloading the `atto.h` and `atto.c` files. A mimimal server might look like the following:
```c
#include "atto.h"

static char *body = "HTTP/1.1 200 OK\r\nServer: atto\r\nContent-type: text/plain\r\n\r\nHello!";

int main(int argc, char** argv) 
{
    atto_server_t server = atto_init_server("8080");
    for (;;) atto_handle_next_connection(&server, body);
    atto_close_server(&server);
    
    return 0;
}
```
