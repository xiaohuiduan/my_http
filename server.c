#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "response.h"
#include "thread_process_http.h"

#define PORT 8899                      // 服务器监听端口


int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    server_addr.sin_port = htons(PORT);
    int opt = 1;

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    int suc = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (suc == -1) {
        printf("绑定事变");
    }
    listen(server_socket, 500);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        pthread_t *new_thread;
        new_thread = (pthread_t*) malloc( sizeof(*new_thread));

        pthread_create(new_thread,NULL,process_http,&client_socket);

    }

    return 0;
}


