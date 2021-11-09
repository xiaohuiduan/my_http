//
// Created by xiaohui on 2021/10/29.
//

#include <stdlib.h>
#include <stdio.h>
#include "request.h"
#include "glib.h"

/// @brief 解析请求的数据，将其解析成http_request的数据结构
/// @param req
/// @param http_data http请求的数据
void parse_request(struct http_request *req, char *http_data)
{
    char *index = http_data;
    char *method = index;
    char *url = 0;
    char *version = 0;
    for (; *index && *index != '\r'; index++)
    {
        if (*index == ' ')
        {
            if (url == 0)
            {
                url = index + 1;
            }
            else
            {
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

    GHashTable *headers = g_hash_table_new(g_str_hash, g_str_equal);
    req->headers = headers;

    while (*index != '\0')
    {
        char *key = index;
        char *value = 0;
        for (; *index && *index != '\r'; index++)
        {
            if (*index == ':' && value == 0)
            {
                *index = '\0';
                value = index + 1;
            }
        }
        *index = '\0';
        g_hash_table_insert(headers, key, value);
        index = index + 2;

        if (*index == '\r')
        {
            char *result = 0;

            char *content_len_key = "Content-Length";
            result = g_hash_table_lookup(headers, content_len_key);

            if (result != 0)
            {
                int len = atoi(result);
                index = index + 2;
                *(index + len) = '\0';
                req->body = index;
                index = index + len;
            }
        }
    }
}

/// @brief 将http_request中的url数据进行解析，得到get请求中的参数  /get?a=1&b=2
/// @param http_request http请求的数据结构
/// @param request_data 解析后的数据结构 key:value
void parse_get_data(struct http_request *http_request, struct request_data *request_data)
{
    char *url = http_request->url + 1;
    request_data->real_url = url;

    while (*url != '?' && *url != '\0')
    {
        url = url + 1;
    }

    // 没有get参数
    if (*url == '\0')
    {
        return;
    }

    *url = '\0';

    char *index = url + 1;

    GHashTable *data = g_hash_table_new(g_str_hash, g_str_equal);
    request_data->data = data;

    while (*index != '\0')
    {
        char *key = index;
        while (*index != '=')
            index++;
        *index = '\0';
        index++;
        char *value = index;
        while (*index != '&' && *index != '\0')
            index++;
        if (*index == '&')
        {
            *index = '\0';
            index = index + 1;
        }
        else
        {
            *index = '\0';
        }
        g_hash_table_insert(data, key, value);
    }
}

/// @brief 将http_request中的body数据进行解析，得到post请求中body的参数
/// @param http_request http请求的数据结构
/// @param request_data 解析后的数据结构 key:value
void parse_post_data(struct http_request *http_request, struct request_data *request_data)
{
    request_data->real_url = http_request->url + 1;

    char *index = http_request->body;
    GHashTable *data = g_hash_table_new(g_str_hash, g_str_equal);
    request_data->data = data;

    while (*index != '\0')
    {
        char *key = index;
        while (*index != '=')
            index++;
        *index = '\0';
        index++;
        char *value = index;
        while (*index != '&' && *index != '\0')
            index++;
        if (*index == '&')
        {
            *index = '\0';
            index = index + 1;
        }
        else
        {
            *index = '\0';
        }
        g_hash_table_insert(data, key, value);
    }
}