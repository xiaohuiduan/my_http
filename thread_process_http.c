
#include <unistd.h>
#include "thread_process_http.h"
#include "request.h"
#include "response.h"
#include "response_util.h"
#include <string.h>
#include <stdio.h>
#include "deal_url_request.h"

void *process_http(void *client)
{

    char buf[1024];
    int client_socket = *(int *)client;
    read(client_socket, buf, 1024);
    // 请求的request数据结构
    struct http_request http_request;
    struct http_request *p_http_request = &http_request;
    memset(p_http_request, 0, sizeof(struct http_request));
    parse_request(p_http_request, buf);
    // 返回response的数据结构
    struct http_response http_response;
    struct http_response *p_response = &http_response;
    memset(p_response, 0, sizeof(struct http_response));

    // 解析get请求的数据
    struct request_data request_data;
    struct request_data *p_request_data = &request_data;
    memset(p_request_data, 0, sizeof(struct request_data));
    char *url = 0;                                            // 去除/之后的路由url
    int type = parse_request_type(p_http_request->url, &url); // 解析请求的类型html png 或者发送get/post数据请求

    // 如果是请求html文本或者是请求图片，则直接返回信息
    if (type == HTTP_IMAGE || type == HTTP_TEXT)
    {
        res_url_deal(p_response, p_http_request, url, type);
        return;
    }

    // 解析get请求和post请求的参数
    if (g_ascii_strncasecmp(p_http_request->method, "GET", sizeof(*p_http_request->method)) == 0)
    {
        parse_get_data(p_http_request, p_request_data);
    }
    else if (g_ascii_strncasecmp(p_http_request->method, "POST", sizeof(*p_http_request->method)) == 0)
    {
        parse_post_data(p_http_request, p_request_data);
    }

    // 如果请求的网页连接是test1
    if (g_ascii_strncasecmp(p_request_data->real_url, "test1", sizeof(p_request_data->real_url)) == 0)
    {
        url_test1_deal(type, p_request_data, p_response, client_socket);
    }

    // 如果请求的网页连接是test2
    if (g_ascii_strncasecmp(p_request_data->real_url, "test2", sizeof(p_request_data->real_url)) == 0)
    {
        url_test2_deal(type, p_request_data, p_response, client_socket);
    }

    
}