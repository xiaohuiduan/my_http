//
// Created by xiaohui on 2021/11/2.
//

#ifndef MY_HTTP_RESPONSE_UTIL_H
#define MY_HTTP_RESPONSE_UTIL_H

#define HTTP_TEXT 1
#define HTTP_IMAGE 2
#define GET_PARAMS_DATA 3

#include "response.h"

/// @brief 解析http请求的数据类型
/// @param url 请求的url，如果url=/***.html 或者 /***.jpg，则认为要打开本地文件进行read，如果为 /***?name=***&age=***，或者***则认为是发送数据
/// @param filename 将 /1.html -> 1.html
/// @param type 请求的类型
int parse_request_type(char *url, char **filename);

/// @brief 为response添加响应头
/// @param type
void build_response_headers(int type, struct http_response *response);

void build_response_file_body(int type, char *filename, struct http_response *response);

#endif //MY_HTTP_RESPONSE_UTIL_H
