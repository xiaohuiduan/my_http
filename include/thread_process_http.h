//
// Created by xiaohui on 2021/10/31.
//

#ifndef MY_HTTP_THREAD_PROCESS_HTTP_H
#define MY_HTTP_THREAD_PROCESS_HTTP_H
#include "my_socket.h"
/**
 * @brief 线程处理http请求，每个http请求都会创建一个线程
 * 
 * @param client_socket client的套字节
 * @return int 0:成功 -1:失败 
 */
int process_http(struct my_socket *my_socket);
#endif //MY_HTTP_THREAD_PROCESS_HTTP_H
