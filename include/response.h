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

// 发送数据,根据配置选择调用 send_normal_response or send_ssl_response
int send_response(struct http_response *response, struct my_socket *my_socket);

// 发送header,根据配置选择调用 send_normal_headers or send_ssl_headers
int send_headers(gpointer key, gpointer value, gpointer stream);

// 发送http header数据
void send_normal_headers(gpointer key, gpointer value, gpointer stream);

// // 发送http header数据
int send_normal_response(struct http_response *response, struct my_socket *my_socket);

// 发送https header数据
int send_ssl_response(struct http_response *response, struct my_socket *my_socket);

// // 发送https header数据
void send_ssl_headers(gpointer key, gpointer value, gpointer stream);
// void send_body(char* body,FILE *stream);

#endif //MY_HTTP_RESPONSE_H
