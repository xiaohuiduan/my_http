// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <string.h>
// #include <stdio.h>
// #include <unistd.h>
// #include "response.h"
// #include "thread_process_http.h"
// #include "config.h"
// #include "my_socket.h"

// #include <event2/event.h>
// #include <event2/buffer.h>
// #include <event2/bufferevent.h>
// #include <event2/bufferevent_ssl.h>

// // 读取数据
// void socket_read_cb(evutil_socket_t fd, short events, void *arg)
// {
//     // int fd = (int *)arg;
//     char msg[1024] = {0};
//     read(fd, msg, 1024);
//     printf("%s\n", msg);
//     // SSL *ssl = (SSL *)arg;
//     // char msg[4096];
//     // memset(msg, 0, sizeof(msg));
//     // int nLen = SSL_read(ssl, msg, sizeof(msg));
//     // if (nLen > 0)
//     // {
//     //     printf("%s\n", msg);
//     // }
//     // else
//     // {
//     //     printf("read error\n");
//     // }
//     // fprintf(stderr, "Get Len %d %s ok\n", nLen, msg);
//     // strcat(msg, "\n this is from server========server resend to client");
//     // SSL_write(ssl, msg, strlen(msg));
// }

// /// 接收到一个请求
// void do_accept(evutil_socket_t listener, short event, void *arg)
// {
//     struct event_base *base = (struct event_base *)arg;

//     int fd = accept(listener, NULL, NULL);
//     if (fd < 0)
//     {
//         perror("accept");
//     }
//     else if (fd > FD_SETSIZE)
//     {
//         close(fd);
//     }
//     else
//     {
//         SSL_CTX *ctx;
//         // 初始化参数

//         // ssl_socket_init(&ctx);

//         // SSL *ssl;
//         // create_ssl(&ssl, ctx, fd);
//         struct event *ev = event_new(NULL, -1, 0, NULL, NULL);
//         //将动态创建的结构体作为event的回调参数
//         event_assign(ev, base, fd, EV_READ | EV_PERSIST,
//                      socket_read_cb, NULL);
//         event_add(ev, NULL);
//     }
// }

// int main()
// {
//     // 初始化服务器端口
//     int server_socket = init_socket(PORT);

//     // SSL_CTX *ctx;
//     // // 初始化参数
//     // if (DO_SSL)
//     // {
//     //     ssl_socket_init(&ctx);
//     // }

//     evutil_socket_t listener;
//     struct sockaddr_in sin;
//     struct event_base *base;
//     struct event *listener_event;
//     base = event_base_new();
//     if (!base)
//     {
//         fprintf(stderr, "Could not initialize libevent!\n");
//         return 1;
//     }
//     // evutil_make_socket_nonblocking(server_socket);
//     listener_event = event_new(base, server_socket, EV_READ | EV_PERSIST, do_accept, (void *)base);
//     event_add(listener_event, NULL);
//     event_base_dispatch(base);

//     // int client_socket;
//     // SSL *ssl;

//     // while (1)
//     // {
//     //     client_socket = accept(server_socket, NULL, NULL);

//     //     if (client_socket == -1)
//     //     {
//     //         printf("accept error");
//     //         continue;
//     //     }

//     //     struct my_socket my_socket;

//     //     memset(&my_socket, 0, sizeof(struct my_socket));
//     //     my_socket.client_socket = client_socket;

//     //     pid_t pid = fork();
//     //     if (pid == 0)
//     //     {
//     //         if (DO_SSL)
//     //         {
//     //             create_ssl(&ssl, ctx, client_socket);
//     //             my_socket.ssl = ssl;
//     //         }

//     //         process_http(&my_socket);
//     //         exit(0);
//     //     }
//     // }

//     return 0;
// }
