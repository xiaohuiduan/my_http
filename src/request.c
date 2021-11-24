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

    GHashTable *headers = http_request->headers;

    request_data->real_url = http_request->url + 1;
    char *content_type = g_hash_table_lookup(headers, "Content-Type");

    if (content_type != 0)
    {

        if (strstr(content_type, "application/x-www-form-urlencoded") != 0)
        {
            char *body = http_request->body;
            char *index = body;
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
        else
        {
            
            char *boundary = strstr(content_type, "boundary=") + 9;
            char end_boundary[100] = {0};
            sprintf(end_boundary, "%s--", boundary);

            char *body = http_request->body;
            char *index = body;
            GHashTable *data = g_hash_table_new(g_str_hash, g_str_equal);
            request_data->data = data;
            char *end_boundary_index = strstr(index, end_boundary);

            while (g_ascii_strncasecmp(index,end_boundary_index,strlen(end_boundary_index)) !=0)
            {
                index = strstr(index, boundary) + strlen(boundary) + 2;
                char *filename = strstr(index, "filename=") + 9;
                char *filename_end = strstr(index, "\r\n");
                index = filename_end + 2;
                *filename_end = '\0';

                char *content_type = strstr(index, "Content-Type:") + 14;
                char *content_type_end = strstr(index, "\r\n");
                index = content_type_end + 4; // 跳过\r\n\r\n
                *content_type_end = '\0';

                char *content = index;
                char *content_end = strstr(index, boundary);

                if (content_end != 0) //代表有多份数据
                {
                    *(content_end - 2) = '\0'; // data_end\r\n\r\n
                    index = content_end;
                }
                // g_hash_table_insert(data, filename, content);
                
                g_hash_table_insert(data,filename, content);
            }
        }
    }
}
