//
// Created by xiaohui on 2021/10/29.
//

#include <stdlib.h>
#include "request.h"
#include "glib.h"

/// 解析request请求 to method url version headers and body
/// \param req
/// \param http_data
void parse_request(struct http_request *req, char *http_data) {
    char *index = http_data;
    char *method = index;
    char *url = 0;
    char *version = 0;
    for (; *index && *index != '\r'; index++) {
        if (*index == ' ') {
            if (url == 0) {
                url = index + 1;
            } else {
                version = index + 1;
            }

            *index = '\0';
        }
    }

    *index = '\0';
    index = index + 2; // 跳过\n进行第二行
    req->method = method;
    req->url = url;
    req->version = version;

    GHashTable *headers = g_hash_table_new(g_str_hash,g_str_equal);
    req->headers = headers;

    while (*index != '\0') {
        char *key = index;
        char *value = 0;
        for (; *index && *index != '\r'; index++) {
            if (*index == ':' && value == 0) {
                *index = '\0';
                value = index + 1;
            }
        }
        *index = '\0';
        g_hash_table_insert(headers,key,value);
        index = index + 2;

        if (*index == '\r'){
            char* result = 0;

            char *content_len_key = "Content-Length";
            result = g_hash_table_lookup(headers,content_len_key);

            if (result != 0){
                int len = atoi(result);
                index = index + 2;
                *(index + len) = '\0';
                req->body = index;
            }
        }
    }

}

