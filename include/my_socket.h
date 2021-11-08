//
// Created by xiaohui on 2021/11/6.
//
#include <openssl/ssl.h>
#include <openssl/err.h>

#ifndef MY_HTTP_SSL_SOCKET_H
#define MY_HTTP_SSL_SOCKET_H

struct my_socket
{
    SSL *ssl;
    int client_socket;
};

void ssl_socket_init(SSL_CTX **ctx);
void create_ssl(SSL **ssl, SSL_CTX *ctx, int client_socket);

#endif //MY_HTTP_SSL_SOCKET_H
