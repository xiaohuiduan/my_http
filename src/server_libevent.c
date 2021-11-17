#include "my_socket.h"
#include "config.h"
#include <event.h>
#include "libevent_deal.h"

int main()
{
    SSL_CTX *ctx;
    if (DO_SSL)
    {
        ssl_socket_init(&ctx);
    }
    //失败则程序退出
    int server_socket = init_socket(PORT);
    struct event_base *base = event_base_new();
    struct event listener_event;
    event_set(&listener_event, server_socket, EV_READ | EV_PERSIST,
              on_accept, ctx);
    event_base_set(base, &listener_event);
    event_add(&listener_event, NULL);
    event_base_dispatch(base);
    // 下面的不会被执行
    printf("执行结束\n");
    return 0;
}