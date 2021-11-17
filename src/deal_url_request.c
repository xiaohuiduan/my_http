#include "deal_url_request.h"
#include "response_util.h"

int res_url_deal(char *url_filename, int type, struct http_response *p_response, struct my_socket *my_socket)
{
    build_response_headers(type, p_response);
    build_response_file_body(type, url_filename, p_response);

    return send_response(p_response, my_socket);
}

int url_test1_deal(int type, struct request_data *request_data, struct http_response *response, struct my_socket *my_socket)
{
    char *name, *pwd;
    name = g_hash_table_lookup(request_data->data, "name");
    pwd = g_hash_table_lookup(request_data->data, "pwd");
    build_response_headers(type, response);
    char *re_data;
    strcat(name, pwd);
    response->body = name;
    response->content_length = strlen(name);
    return send_response(response, my_socket);
}

int url_test2_deal(int type, struct request_data *request_data, struct http_response *response, struct my_socket *my_socket)
{
    char *year, *month;
    year = g_hash_table_lookup(request_data->data, "year");
    month = g_hash_table_lookup(request_data->data, "month");
    build_response_headers(type, response);
    char *re_data;
    // strcat(year, month);
    response->body = "你好";
    response->content_length = strlen(response->body);
    return send_response(response, my_socket);
}