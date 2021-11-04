#ifndef MY_HTTP_REQUEST_H
#define MY_HTTP_REQUEST_H

#include "glib.h"
struct http_request
{
    char *method;
    char *url;
    char *version;
    GHashTable *headers;
    char *body;
};

struct request_data
{
    char *real_url;
    GHashTable *data;
};

void parse_request(struct http_request *req, char *http_data);

/// 解析get请求参数
/// \param request_data
/// \param url
void parse_get_data(struct http_request *http_request, struct request_data *request_data);

/// 解析post请求参数
/// \param http_request http请求的参数
/// \param http_data 解析请求的参数
void parse_post_data(struct http_request *http_request, struct request_data *request_data);

#endif //MY_HTTP_DEAL_URL_REQUEST_H