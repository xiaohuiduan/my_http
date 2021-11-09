//
// Created by xiaohui on 2021/11/6.
//

#include "my_socket.h"

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory.h>
#include <sys/socket.h>
#include "config.h"

void ssl_socket_init(SSL_CTX **ctx)
{
    SSL_library_init();
    SSL_load_error_strings();
    *ctx = SSL_CTX_new(SSLv23_server_method());

    // 在OpenSSL中创建的SSL会话环境称为CTX，使用不同的协议会话，其环境也不一样的
    int suc_load_ca = SSL_CTX_use_certificate_file(*ctx, CA_PATH,
                                                   SSL_FILETYPE_PEM);
    int suc_load_pv = SSL_CTX_use_PrivateKey_file(*ctx, PK_PATH,
                                                  SSL_FILETYPE_PEM);
    if (suc_load_ca <= 0 || suc_load_pv <= 0)
    {
        printf("load ca or pv error\n");
        exit(1);
    }
}

void create_ssl(SSL **ssl, SSL_CTX *ctx, int client_socket)
{
    // 申请一个ssl套字节
    *ssl = SSL_new(ctx);
    if (*ssl == NULL)
    {
        close(client_socket);
        printf("ssl new error\n");
        exit(1);
    }
    // 将套字节和socket连接
    SSL_set_fd(*ssl, client_socket);

    if (SSL_accept(*ssl) <= 0)
    {
        printf("ssl connect error\n");

        char status[] = "HTTP/1.0 200 OK\r\n";
        char header[] = "Server: DWBServer\r\nContent-Type: text/html;charset=utf-8\r\n\r\n";
        char body[] = "<html><head><title>ERRORING</title></head><body><h2>出错了</h2><p>请使用https哦</p></body></html>";

        write(client_socket, status, sizeof(status));
        write(client_socket, header, sizeof(header));
        write(client_socket, body, sizeof(body));
        
        shutdown(client_socket, SHUT_RDWR);
        exit(1);
    }
}
