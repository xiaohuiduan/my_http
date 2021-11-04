#include "deal_url_request.h"
#include "response_util.h"

void res_url_deal(char *url, int type, struct http_response *p_response, int client_socket)
{
    build_response_headers(type, p_response);
    // 返回图片或者text html
    build_response_body(type, url, p_response);
    // 发送请求
    send_response(p_response, client_socket);
}
void url_test1_deal(int type, struct request_data *request_data, struct http_response *response, int client_socket)
{
    char *name, *pwd;
    name = g_hash_table_lookup(request_data->data, "name");
    pwd = g_hash_table_lookup(request_data->data, "pwd");
    build_response_headers(type, response);
    char *re_data;
    strcat(name, pwd);
    response->body = name;
    response->content_length = strlen(name);
    send_response(response, client_socket);
}

void url_test2_deal(int type, struct request_data *request_data, struct http_response *response, int client_socket)
{
    char *year, *month;
    year = g_hash_table_lookup(request_data->data, "year");
    month = g_hash_table_lookup(request_data->data, "month");
    build_response_headers(type, response);
    char *re_data;
    // strcat(year, month);
    response->body = "你好";
    response->content_length = strlen( response->body);
    send_response(response, client_socket);
}