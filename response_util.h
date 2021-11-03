//
// Created by xiaohui on 2021/11/2.
//

#ifndef MY_HTTP_RESPONSE_UTIL_H
#define MY_HTTP_RESPONSE_UTIL_H

#define HTTP_TEXT 1
#define HTTP_IMAGE 2
#define GET_PARAMS_DATA 3

#include "response.h"

int parse_request_type(char *url,char ** filename);

void build_response_headers(int type, struct http_response **response);

void build_response_body(int type,char *filename, struct http_response **response);




#endif //MY_HTTP_RESPONSE_UTIL_H
