#ifndef _RESPONSE_H_
#define _RESPONSE_H_

typedef struct atto_response {
    char *status_code;
    char *content_type;
    char *body;
} atto_response_t;

atto_response_t atto_default_response();
char* atto_build_response(atto_response_t resp);

#endif