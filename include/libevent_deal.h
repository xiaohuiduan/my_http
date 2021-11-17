// 关于如何使用libevent处理数据

#include <openssl/ssl.h>
#include <openssl/err.h>

struct socket_event
{
    // client 的句柄
    int client_fd;
    // ssl 如果不使用ssl则为null
    SSL *ssl;
    struct event_base *base;
    struct event *read_event;
    struct event *write_event;
    // read函数读取的数据
    char *buf;
};

// on_accept 接收到一个client之后，马上创建一个线程来处理这个client
void on_accept(int server_socket_fd, short event, void *arg);

// 创建一个线程，处理这个client，这个线程会调用thread_process_http这个函数
void create_new_thread(struct socket_event *socket_event);

// thread_process_http函数使用libevent调用on_read函数来read client的数据
void *thread_process_http(void *arg);

// on_read函数监听client数据，然后使用libevent调用on_write函数来write client的数据
void on_read(int client_fd, short event, void *arg);

// 写数据
void on_write(int client_fd, short event, void *arg);

int deal_data_and_response(struct socket_event *socket_event);
