//
// Created by xiaohui on 2021/11/6.
//
#include <openssl/ssl.h>
#include <openssl/err.h>

#ifndef MY_HTTP_SSL_SOCKET_H
#define MY_HTTP_SSL_SOCKET_H

// 发送数据所使用的fd。
struct my_socket
{
    SSL *ssl;
    int client_socket;
};

// 初始化ssl
void ssl_socket_init(SSL_CTX **ctx);
// 创建ssl套接字
void create_ssl(SSL **ssl, SSL_CTX *ctx, int client_socket);
// 初始化server的套接字
int init_socket(int port);

#endif //MY_HTTP_SSL_SOCKET_H
