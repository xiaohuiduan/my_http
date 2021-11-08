#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "response.h"
#include "thread_process_http.h"
#include "config.h"
#include "my_socket.h"

#define PORT 8899 // 服务器监听端口

int main()
{

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    server_addr.sin_port = htons(PORT);
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    int suc = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (suc == -1)
    {
        printf("绑定事变");
    }
    listen(server_socket, 4096);

    SSL_CTX *ctx;
    // 初始化参数
    if (DO_SSL)
    {
        ssl_socket_init(&ctx);
    }

    int client_socket;
    SSL *ssl;

    while (1)
    {
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1)
        {
            printf("accept error");
            continue;
        }

        struct my_socket my_socket;

        memset(&my_socket, 0, sizeof(struct my_socket));
        my_socket.client_socket = client_socket;

        pid_t pid = fork();
        if (pid == 0)
        {
            if (DO_SSL)
            {
                create_ssl(&ssl, ctx, client_socket);
                my_socket.ssl = ssl;
            }

            process_http(&my_socket);
            exit(0);
        }
    }

    return 0;
}
