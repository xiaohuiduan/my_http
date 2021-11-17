#include <unistd.h>
#include "request.h"
#include "response.h"
#include "response_util.h"
#include <string.h>
#include <stdio.h>
#include "deal_url_request.h"
#include "config.h"
#include "my_socket.h"
#include "thread_process_http.h"
#include "libevent_deal.h"

int process_http(struct my_socket *my_socket)
{
    char buf[2048] = {0};

    if (DO_SSL)
    {

        if (my_socket->ssl == NULL)
        {
            close(my_socket->client_socket);
            fprintf(stderr, "read from SSL failed.\n");
            return -1;
        }

        if (SSL_accept(my_socket->ssl) < 0)
        {
            close(my_socket->client_socket);
            return -1;
        }

        int rc = SSL_read(my_socket->ssl, buf, sizeof(buf));
        if (rc < 0)
        {
            fprintf(stderr, "read from SSL failed.\n");
            return -1;
        }
    }
    else
    {
        read(my_socket->client_socket, buf, 1024);
    }

    // 请求的request数据结构
    struct http_request http_request;
    struct http_request *p_http_request = &http_request;
    memset(p_http_request, 0, sizeof(struct http_request));
    parse_request(p_http_request, buf);

    if (p_http_request->url == NULL)
    {
        printf("URL内容为空\n");
        return -1;
    }

    // 解析get/post请求的数据
    struct request_data request_data;
    struct request_data *p_request_data = &request_data;
    memset(p_request_data, 0, sizeof(struct request_data));

    // 返回response的数据结构
    struct http_response http_response;
    struct http_response *p_response = &http_response;
    memset(p_response, 0, sizeof(struct http_response));

    char *url = 0;                                            // 去除/之后的路由url 如 /1.html -> 1.html
    int type = parse_request_type(p_http_request->url, &url); // 解析请求的类型请求资源文件（html，png）或者发送get/post数据请求
    // 如果是请求html文本或者是请求图片，则直接返回信息
    if (type == HTTP_IMAGE || type == HTTP_TEXT)
    {
        res_url_deal(url, type, p_response, my_socket);
        return 0;
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
        url_test1_deal(type, p_request_data, p_response, my_socket);
    }

    // 如果请求的网页连接是test2
    if (g_ascii_strncasecmp(p_request_data->real_url, "test2", sizeof(p_request_data->real_url)) == 0)
    {
        url_test2_deal(type, p_request_data, p_response, my_socket);
    }
    return 0;
}