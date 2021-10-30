//
// Created by xiaohui on 2021/10/29.
//

#ifndef MY_HTTP_RESPONSE_H
#define MY_HTTP_RESPONSE_H

#include <glib.h>

struct http_response {
    char *version;
    char *status_code;
    char *reason_phrase;
    GHashTable *headers;
    char *body;
};

void send_response(struct http_response *response,int client_fd);

void send_headers(gpointer key, gpointer value,gpointer stream);

void send_body(char* body,FILE *stream);



#endif //MY_HTTP_RESPONSE_H
