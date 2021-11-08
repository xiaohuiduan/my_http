//
// Created by xiaohui on 2021/10/29.
//

#include <unistd.h>
#include <stdio.h>
#include "response.h"
#include "config.h"
#include <sys/socket.h>

/// send response to the browser (response contain headers and body)
/// \param response
/// \param p_client_fd
int send_response(struct http_response *response, struct my_socket *my_socket)
{
    if (DO_SSL)
    {
        return send_ssl_response(response, my_socket);
    }
    else
    {
        return send_normal_response(response, my_socket);
    }
}

void send_normal_headers(gpointer key, gpointer value, gpointer stream)
{
    fprintf((FILE *)stream, "%s: %s\r\n", (char *)key, (char *)value);
}

int send_normal_response(struct http_response *response, struct my_socket *my_socket)
{
    int client_fd = (my_socket->client_socket);
    GHashTable *headers = response->headers;
    FILE *stream = fdopen(client_fd, "w+");
    if (stream == NULL)
    { // error checking
        printf("Error creating a file\n");
        printf("%s\n", strerror(errno));
        return -1;
    }
    fprintf(stream, "%s %s %s\r\n", response->version, response->status_code, response->reason_phrase);
    g_hash_table_foreach(headers, send_normal_headers, stream);
    fprintf(stream, "\r\n");

    if (response->body != NULL)
    {
        fwrite(response->body, 1, response->content_length, stream);
    }
    fflush(stream);
    fclose(stream);
    shutdown(client_fd, SHUT_RDWR);
    return 1;
}

void send_ssl_headers(gpointer key, gpointer value, gpointer stream)
{
    int data_len = strlen((char *)value) + strlen((char *)key) + 4;
    char *data = (char *)malloc(data_len);
    sprintf(data, "%s: %s\r\n", (char *)key, (char *)value);

    SSL_write((SSL *)stream, data, data_len);
    free(data);
}

int send_ssl_response(struct http_response *response, struct my_socket *my_socket)
{
    SSL *ssl = my_socket->ssl;
    int client_fd = (my_socket->client_socket);
    // 写入响应 version status_code reason_phrase
    GHashTable *headers = response->headers;
    int data_len = strlen(response->version) + strlen(response->status_code) + strlen(response->reason_phrase) + 4;
    char *data = (char *)malloc(data_len);
    sprintf(data, "%s %s %s\r\n", response->version, response->status_code, response->reason_phrase);
    SSL_write(ssl, data, data_len);

    free(data);

    // 写入响应 headers
    g_hash_table_foreach(headers, send_ssl_headers, ssl);
    SSL_write(ssl, "\r\n", 2);

    // 写入消息体
    if (response->body != NULL)
    {
        SSL_write(ssl, response->body, response->content_length);
    }

    SSL_shutdown(ssl);
    shutdown(client_fd, SHUT_RDWR);
    SSL_free(ssl);
    return 1;
}
