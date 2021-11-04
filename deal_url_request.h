//
// Created by xiaohui on 2021/11/5. 
// 请求路由为 x，则函数为url_x_deal
//

#ifndef MY_HTTP_DEAL_URL_REQUEST_H
#define MY_HTTP_DEAL_URL_REQUEST_H

#include "response.h"
#include "request.h"

// 请求html 和 图片资源统一的方法。
void res_url_deal(char* url,int type,struct http_response *response,int socket);

void url_test1_deal(int type,struct request_data *request_data,struct http_response *response,int client_socket);

void url_test2_deal(int type,struct request_data *request_data,struct http_response *response,int client_socket);

#endif //MY_HTTP_DEAL_URL_REQUEST_H
