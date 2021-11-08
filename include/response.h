//
// Created by xiaohui on 2021/10/29.
//

#ifndef MY_HTTP_RESPONSE_H
#define MY_HTTP_RESPONSE_H

#include <glib.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdio.h>
#include "my_socket.h"

struct http_response
{
    char *version;
    char *status_code;
    char *reason_phrase;
    GHashTable *headers;
    void *body;
    size_t content_length;
};

int send_response(struct http_response *response, struct my_socket *my_socket);

int send_headers(gpointer key, gpointer value, gpointer stream);

void send_normal_headers(gpointer key, gpointer value, gpointer stream);
int send_normal_response(struct http_response *response, struct my_socket *my_socket);

int send_ssl_response(struct http_response *response, struct my_socket *my_socket);

void send_ssl_headers(gpointer key, gpointer value, gpointer stream);
// void send_body(char* body,FILE *stream);

#endif //MY_HTTP_RESPONSE_H
