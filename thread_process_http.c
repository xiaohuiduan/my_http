
#include <unistd.h>
#include "thread_process_http.h"
#include "request.h"
#include "response.h"
#include "response_util.h"
#include <string.h>
#include <stdio.h>

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

    if (g_ascii_strncasecmp(p_http_request->method, "GET", sizeof(*p_http_request->method)) == 0)
    {

        char *url = 0; // 去除/之后的路由url
        int type = parse_request_type(p_http_request->url, &url);

        // 如果是请求html文本或者是请求图片，则直接返回信息
        if (type == HTTP_IMAGE || type == HTTP_TEXT)
        {
            build_response_headers(type, p_response);
            build_response_body(type, url, p_response);
            send_response(p_response, client_socket);
        }

        // 如果发送的是get请求1?id=1&get=2
        if (type == GET_PARAMS_DATA)
        {
            // 解析get请求的数据
            struct request_data request_get_data;
            struct request_data *p_request_get_data = &request_get_data;
            memset(p_request_get_data, 0, sizeof(struct request_data));
            parse_get_data(p_http_request, p_request_get_data);

            // 如果请求的网页连接是test1
            if (g_ascii_strncasecmp(p_request_get_data->real_url, "test1", sizeof(p_request_get_data->real_url)) == 0)
            {
                char *name, *pwd;
                name = g_hash_table_lookup(p_request_get_data->data, "name");
                pwd = g_hash_table_lookup(p_request_get_data->data, "pwd");
                build_response_headers(type, p_response);
                char *re_data;
                strcat(name, pwd);
                p_response->body = name;
                p_response->content_length = strlen(name);
                send_response(p_response, client_socket);
            }
        }
    }
    else if (g_ascii_strncasecmp(p_http_request->method, "POST", sizeof(*p_http_request->method)) == 0)
    {

        char *url = 0; // 去除/之后的路由url
        int type = parse_request_type(p_http_request->url, &url);
        struct request_data request_post_data;
        struct request_data *p_request_post_data = &request_post_data;
        memset(p_request_post_data, 0, sizeof(struct request_data));
        parse_post_data(p_http_request, p_request_post_data);
        
        // 如果请求的网页连接是test1
        if (g_ascii_strncasecmp(p_request_post_data->real_url, "test1", sizeof(*p_request_post_data->real_url)) == 0)
        {
            char *name, *pwd;
            name = g_hash_table_lookup(p_request_post_data->data, "name");
            pwd = g_hash_table_lookup(p_request_post_data->data, "pwd");
            build_response_headers(type, p_response);
            char *re_data;
            strcat(name, pwd);
            p_response->body = name;
            p_response->content_length = strlen(name);
            send_response(p_response, client_socket);
        }
    }
}