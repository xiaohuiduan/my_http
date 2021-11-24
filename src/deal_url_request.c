#include "deal_url_request.h"
#include "response_util.h"
#include "config.h"
#include "glib.h"

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
    char re_data[512] = {0};

    sprintf(re_data, "发送的get数据：name:%s,pwd:%s", name, pwd);
    response->body = re_data;
    response->content_length = strlen(response->body);
    return send_response(response, my_socket);

    return send_response(response, my_socket);
}

int url_test2_deal(int type, struct request_data *request_data, struct http_response *response, struct my_socket *my_socket)
{
    char *year, *month;
    year = g_hash_table_lookup(request_data->data, "year");
    month = g_hash_table_lookup(request_data->data, "month");
    build_response_headers(type, response);
    char re_data[100];

    sprintf(re_data, "发送的post数据：year:%s,month:%s", year, month);
    response->body = re_data;
    response->content_length = strlen(response->body);
    return send_response(response, my_socket);
}

int url_upload_deal(int type, struct request_data *request_data, struct http_response *response, struct my_socket *my_socket){
    GHashTable *data = request_data->data;
    GList *keys = g_hash_table_get_keys(data);
    char *file_name = keys->data; // keys->data = "1.txt"
    
    char *file_data = g_hash_table_lookup(data, file_name);
    file_name = file_name + 1; 


    char file_path[100] = UPLOAD_PATH;
    memcpy(file_path + strlen(UPLOAD_PATH), file_name, strlen(file_name) - 1); 

    printf("%s\n",file_path);
    FILE *fp = fopen(file_path, "w+");
    if(fp == NULL){
        printf("open file error\n");
        return -1;
    }
    fprintf(fp, "%s", file_data);
    fclose(fp);
    build_response_headers(HTTP_TEXT, response);

    response->body = "文件上传成功";
    response->content_length = strlen(response->body);
    return send_response(response, my_socket);
    
}
