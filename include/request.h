#ifndef MY_HTTP_REQUEST_H
#define MY_HTTP_REQUEST_H

#include "glib.h"
/**
 * @brief http请求的数据结构
 * 
 */
struct http_request
{
    char *method;        //请求方法 get post
    char *url;           // 请求路由
    char *version;       // 请求版本
    GHashTable *headers; // 请求头
    char *body;          // 请求体 get请求没有body
};

/**
 * @brief http请求的数据 real_url为去除/的url，data为请求的数据 key:value
 * 
 */
struct request_data
{
    char *real_url;
    GHashTable *data;
};

/// @brief 解析请求的数据，将其解析成http_request的数据结构
/// @param req
/// @param http_data http请求的数据
void parse_request(struct http_request *req, char *http_data);

/// @brief 将http_request中的url数据进行解析，得到get请求中的参数  /get?a=1&b=2
/// @param http_request http请求的数据结构
/// @param request_data 解析后的数据结构 key:value
void parse_get_data(struct http_request *http_request, struct request_data *request_data);

/// @brief 将http_request中的body数据进行解析，得到post请求中body的参数
/// @param http_request http请求的数据结构
/// @param request_data 解析后的数据结构 key:value
void parse_post_data(struct http_request *http_request, struct request_data *request_data);

#endif //MY_HTTP_DEAL_URL_REQUEST_H