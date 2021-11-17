//
// Created by xiaohui on 2021/11/2.
//

#include "response_util.h"
#include "config.h"

void build_response_headers(int type, struct http_response *response)
{
    (response)->version = "HTTP/1.1";
    (response)->status_code = "200";
    (response)->reason_phrase = "OK";
    GHashTable *headers = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(headers, "Server", "C-- Server");

    if (type == HTTP_TEXT)
    {
        g_hash_table_insert(headers, "Content-Type", " text/html;charset=utf-8");
    }
    else if (type == HTTP_IMAGE)
    {
        g_hash_table_insert(headers, "Content-Type", " image/jpeg");
    }
    else if (type == GET_PARAMS_DATA)
    {

        g_hash_table_insert(headers, "Content-Type", " text/html;charset=utf-8");
    }
    (response)->headers = headers;
}

void build_response_file_body(int type, char *filename, struct http_response *response)
{
    char file_path[512] = RES_DIR;
    strcat(file_path, filename);

    FILE *f = fopen(file_path, "rb");
    if (f == NULL)
    {
        printf("文件%s打开失败\n", file_path);
        //        exit(0);
        (response)->body = NULL;
        return;
    }
    size_t size = 0;
    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    void *buf = malloc(size);
    fread(buf, 1, size, f);

    (response)->body = buf;
    (response)->content_length = size;
}

/// 解析请求url的类型html or 图片
/// \param url /1.jpg  /1
/// \param filename /1.jpg or /1.html -> 1.jpg or 1.html
/// \return
int parse_request_type(char *url, char **filename)
{
    int type;
    if (strstr(url, ".jpg"))
    {
        type = HTTP_IMAGE;
    }
    else if (strstr(url, ".png"))
    {
        type = HTTP_IMAGE;
    }
    else if (strstr(url, ".gif"))
    {
        type = HTTP_IMAGE;
    }
    else if (strstr(url, ".jpeg"))
    {
        type = HTTP_IMAGE;
    }
    else if (strstr(url, "favicon.ico"))
    {
        type = HTTP_IMAGE;
    }
    else if (strstr(url, ".html"))
    {
        type = HTTP_TEXT;
    }
    else
    {
        type = GET_PARAMS_DATA;
    }
    *filename = url + 1;

    return type;
}
