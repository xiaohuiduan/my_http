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

        char *filename = 0;
        int type = parse_request_type(req.url, &filename);

        build_response_headers(type, &p);
        build_response_body(type, filename, &p);

        send_response(p, client_socket);

    } else if (g_ascii_strncasecmp(req.method, "POST", sizeof(*req.method)) == 0) {
    }
}