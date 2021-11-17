// #include<netinet/in.h>  
// #include<sys/socket.h>  
// #include<unistd.h>  
  
// #include<stdio.h>  
// #include<string.h>  
  
// #include<event.h>  
// #include<listener.h>  
// #include<bufferevent.h>  
// #include<thread.h>  
  
  
// void listener_cb(evconnlistener *listener, evutil_socket_t fd,  
//                  struct sockaddr *sock, int socklen, void *arg);  
  
// void socket_read_cb(bufferevent *bev, void *arg);  
// void socket_event_cb(bufferevent *bev, short events, void *arg);  
  
// int main()  
// {  
//     //evthread_use_pthreads();//enable threads  
  
//     struct sockaddr_in sin;  
//     memset(&sin, 0, sizeof(struct sockaddr_in));  
//     sin.sin_family = AF_INET;  
//     sin.sin_port = htons(9999);  
  
//     event_base *base = event_base_new();  
//     evconnlistener *listener  
//             = evconnlistener_new_bind(base, listener_cb, base,  
//                                       LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,  
//                                       10, (struct sockaddr*)&sin,  
//                                       sizeof(struct sockaddr_in));  
  
//     event_base_dispatch(base);  
  
//     evconnlistener_free(listener);  
//     event_base_free(base);  
  
//     return 0;  
// }  
  
  
// //一个新客户端连接上服务器了  
// //当此函数被调用时，libevent已经帮我们accept了这个客户端。该客户端的
// //文件描述符为fd
// void listener_cb(evconnlistener *listener, evutil_socket_t fd,  
//                  struct sockaddr *sock, int socklen, void *arg)  
// {  
//     printf("accept a client %d\n", fd);  
  
//     event_base *base = (event_base*)arg;  
  
//     //为这个客户端分配一个bufferevent  
//     bufferevent *bev =  bufferevent_socket_new(base, fd,  
//                                                BEV_OPT_CLOSE_ON_FREE);  
  
//     bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, NULL);  
//     bufferevent_enable(bev, EV_READ | EV_PERSIST);  
// }  
  
// //   http_event_cb
// void socket_read_cb(bufferevent *bev, void *arg)  
// {  
//     char msg[4096];  
  
//     size_t len = bufferevent_read(bev, msg, sizeof(msg)-1 );  
  
//     msg[len] = '\0';  
//     printf("server read the data %s\n", msg);  
  
//     char reply[] = "I has read your data";  
//     bufferevent_write(bev, reply, strlen(reply) );  
// }  
  
  
// void socket_event_cb(bufferevent *bev, short events, void *arg)  
// {  
//     if (events & BEV_EVENT_EOF)  
//         printf("connection closed\n");  
//     else if (events & BEV_EVENT_ERROR)  
//         printf("some other error\n");  
  
//     //这将自动close套接字和free读写缓冲区  
//     bufferevent_free(bev);  
// }  

#include "stdio.h"
#include "stdlib.h"

void main(){

    char content_len[100];
    // itoa(100,content_len,10);
   sprintf(content_len,"%d",2140);

    printf("%s",content_len);
}