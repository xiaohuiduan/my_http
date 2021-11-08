//
// Created by xiaohui on 2021/11/5.
// 请求路由为 x，则函数为url_x_deal
//

#ifndef MY_HTTP_DEAL_URL_REQUEST_H
#define MY_HTTP_DEAL_URL_REQUEST_H

#include "response.h"
#include "request.h"
#include "response_util.h"
#include "my_socket.h"

// 请求html 和 图片资源统一的方法。

/// 请求html 和 图片资源统一的方法。
/// @param url_filename 资源请求的url 如：1.html,2.jpg
/// @param response 返回的响应数据结构
/// @param socket client socket
int res_url_deal(char *url_filename, int type, struct http_response *response, struct my_socket *my_socket);

int url_test1_deal(int type, struct request_data *request_data, struct http_response *response, struct my_socket *my_socket);

int url_test2_deal(int type, struct request_data *request_data, struct http_response *response, struct my_socket *my_socket);

#endif //MY_HTTP_DEAL_URL_REQUEST_H
