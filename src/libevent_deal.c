#include "libevent_deal.h"
#include <event.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "thread_process_http.h"
#include "request.h"
#include "response.h"
#include "response_util.h"
#include "deal_url_request.h"

// on_accept 接收到一个client之后，马上创建一个线程来处理这个client
void on_accept(int server_socket_fd, short event, void *arg) {
    SSL_CTX *ctx = (SSL_CTX *) (arg);

    int new_client_fd = accept(server_socket_fd, NULL, NULL);

    if (new_client_fd < 0) {
        perror("accept");
        return;
    }
    if (new_client_fd > FD_SETSIZE) {
        close(new_client_fd);
        return;
    }

    struct socket_event *socket_event = (struct socket_event *) malloc(sizeof(struct socket_event));
    memset(socket_event, 0, sizeof(struct socket_event));
    socket_event->client_fd = new_client_fd;
    if (DO_SSL) //+
    {
        SSL *ssl;
        create_ssl(&ssl, ctx, new_client_fd);
        socket_event->ssl = ssl;
    }

    // 接收到一个新的连接时，创建一个线程
    create_new_thread(socket_event);
}

// 创建一个线程，处理这个client，这个线程会调用thread_process_http这个函数
void create_new_thread(struct socket_event *socket_event) {
    // printf("分配一个线程给client：%d\n", socket_event->client_fd);
    pthread_t tid;
    pthread_create(&tid, NULL, thread_process_http, (void *) socket_event);
    pthread_detach(tid);
}

// thread_process_http函数使用libevent调用on_read函数来read client的数据
void *thread_process_http(void *arg) {

    struct socket_event *socket_event = (struct socket_event *) arg;
    // 在线程中注册libevent读事件
    struct event_base *base = event_base_new();
    struct event read_event;
    socket_event->base = base;
    socket_event->read_event = &read_event;

    // read 设置为持久化（EV_PERSIST），防止读取完毕后，线程退出
    event_set(&read_event, socket_event->client_fd, EV_READ | EV_PERSIST, on_read, socket_event);
    event_base_set(base, &read_event);
    event_add(&read_event, NULL);
    event_base_dispatch(base);
}

// on_read函数监听client数据，然后使用libevent调用on_write函数来write client的数据
void on_read(int client_fd, short event, void *arg) {

    struct socket_event *p_socket_event = (struct socket_event *) arg;

    char msg[MAX_BUF_SIZE] = {0};
    int size;
    if (DO_SSL) {
        size = SSL_read(p_socket_event->ssl, msg, MAX_BUF_SIZE);
    } else {
//        size = read(client_fd, msg, MAX_BUF_SIZE);
        size = recv(client_fd, msg, MAX_BUF_SIZE, 0);
    }
    if (size == 0) {
        printf("read error the size is %d, so close the client_fd\n", size);
        close(client_fd);
        return;
    }

    struct event write_event;
    p_socket_event->write_event = &write_event;
    p_socket_event->buf = msg;

    event_set(&write_event, client_fd, EV_WRITE, on_write, p_socket_event);
    event_base_set(p_socket_event->base, &write_event);
    event_add(&write_event, NULL);
}

void on_write(int fd, short event, void *arg) {
    struct socket_event *socket_event = (struct socket_event *) arg;
    deal_data_and_response(socket_event);
}

int deal_data_and_response(struct socket_event *p_socket_event) {
    char *buf = p_socket_event->buf;
    struct my_socket *my_socket = (struct my_socket *) malloc(sizeof(struct my_socket));
    my_socket->client_socket = p_socket_event->client_fd;
    my_socket->ssl = p_socket_event->ssl;

    // 请求的request数据结构
    struct http_request http_request;
    struct http_request *p_http_request = &http_request;
    memset(p_http_request, 0, sizeof(struct http_request));
    parse_request(p_http_request, buf);

    if (p_http_request->url == NULL) {
        printf("URL内容为空\n");
        return -1;
    }

    // 解析get/post请求的数据
    struct request_data request_data;
    struct request_data *p_request_data = &request_data;
    memset(p_request_data, 0, sizeof(struct request_data));

    // 返回response的数据结构
//    struct http_response http_response;
//    struct http_response *p_response = &http_response;
    struct http_response *p_response = (struct http_response *) malloc(sizeof(struct http_response));

    memset(p_response, 0, sizeof(struct http_response));

    char *url = 0;                                            // 去除/之后的路由url 如 /1.html -> 1.html
    int type = parse_request_type(p_http_request->url, &url); // 解析请求的类型请求资源文件（html，png）或者发送get/post数据请求
    // 如果是请求html文本或者是请求图片，则直接返回信息
    if (type == HTTP_IMAGE || type == HTTP_TEXT) {
        res_url_deal(url, type, p_response, my_socket);
        return 0;
    }

    // 解析get请求和post请求的参数
    if (g_ascii_strncasecmp(p_http_request->method, "GET", sizeof(*p_http_request->method)) == 0) {
        parse_get_data(p_http_request, p_request_data);
    } else if (g_ascii_strncasecmp(p_http_request->method, "POST", sizeof(*p_http_request->method)) == 0) {
        parse_post_data(p_http_request, p_request_data);
    }

    // 如果请求的网页连接是test1
    if (g_ascii_strncasecmp(p_request_data->real_url, "test1", sizeof(p_request_data->real_url)) == 0) {
        url_test1_deal(type, p_request_data, p_response, my_socket);
    }

    // 如果请求的网页连接是test2
    if (g_ascii_strncasecmp(p_request_data->real_url, "test2", sizeof(p_request_data->real_url)) == 0) {
        url_test2_deal(type, p_request_data, p_response, my_socket);
    }
    return 0;
}
