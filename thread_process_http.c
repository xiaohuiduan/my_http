//
// Created by xiaohui on 2021/10/31.
//

#include <unistd.h>
#include "thread_process_http.h"
#include "request.h"
#include "response.h"
#include "response_util.h"
#include <string.h>
#include <stdio.h>

void *process_http(void *client) {

    char buf[1024];
    int client_socket = *(int *) client;
    read(client_socket, buf, 1024);
    struct http_request req;
    memset(&req, 0, sizeof(req));
    parse_request(&req, buf);


    struct http_response response;
    memset(&response, 0, sizeof(struct http_response));

    if (g_ascii_strncasecmp(req.method, "GET", sizeof(*req.method)) == 0) {
        struct http_response *p = &response;

        char *url = 0;
        int type = parse_request_type(req.url, &url);
        // 如果是请求html文本或者是请求图片，则直接返回信息
        if (type == HTTP_IMAGE || type == HTTP_TEXT) {
            build_response_headers(type, &p);
            build_response_body(type, url, &p);
            send_response(p, client_socket);
        }
        // 如果发送的是get请求1?id=1&get=2
        if (type == GET_PARAMS_DATA) {
            struct request_data request_data;
            memset(&request_data, 0,sizeof(request_data));
            parse_get_data(&request_data,req.url);
        }

    } else if (g_ascii_strncasecmp(req.method, "POST", sizeof(*req.method)) == 0) {
        printf("%s\n",req.body);

    }
}