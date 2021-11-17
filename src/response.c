#include <unistd.h>
#include <stdio.h>
#include "response.h"
#include "config.h"
#include <sys/socket.h>
#include <stdlib.h>

void my_ssl_write(SSL *ssl, void *buf, int num)
{
    int ret = 0;
    int err = 0;
    while (ret <= 0)
    {
        sleep(1);
        ret = SSL_write(ssl, buf, num);
        err = SSL_get_error(ssl, ret);
        if (err == SSL_ERROR_WANT_WRITE)
        {
            continue;
        }
        else if (err == SSL_ERROR_WANT_READ)
        {
            continue;
        }
        else
        {
            break;
        }
    }
}

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
    char content_len[128];
    sprintf(content_len, "%lu", response->content_length);
    if (CHUNK)
    {
        g_hash_table_insert(headers, "Transfer-Encoding", "chunked"); //开启分块传输
    }
    else
    {
        g_hash_table_insert(headers, "Content-Length", content_len);
    }

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
        if (CHUNK)
        {
            int epoch = response->content_length / CHUNK_SIZE;
            int last = response->content_length % CHUNK_SIZE;
            int i = 0;
            while (i < epoch)
            {
                fprintf(stream, "%x\r\n", CHUNK_SIZE);
                fwrite(response->body + i * CHUNK_SIZE, 1, CHUNK_SIZE, stream);
                fprintf(stream, "\r\n");
                i++;
            }
            fprintf(stream, "%x\r\n", last);
            fwrite(response->body + epoch * CHUNK_SIZE, 1, last, stream);
            fprintf(stream, "\r\n");
            fprintf(stream, "%x\r\n", 0);
            fprintf(stream, "\r\n");
        }
        else
        {
            fwrite(response->body, 1, response->content_length, stream);
        }
    }
    fflush(stream);
    // fclose(stream);
    // shutdown(client_fd, SHUT_RDWR);
    return 1;
}

void send_ssl_headers(gpointer key, gpointer value, gpointer stream)
{
    SSL *ssl = (SSL *)stream;
    int data_len = strlen((char *)value) + strlen((char *)key) + 4; //4
    char *data = (char *)malloc(data_len);
    snprintf(data, data_len + 1, "%s: %s\r\n", (char *)key, (char *)value);

    //    SSL_write(ssl, data, data_len);
    my_ssl_write(ssl, data, data_len);
    // free(data);
}

int send_ssl_response(struct http_response *response, struct my_socket *my_socket)
{
    SSL *ssl = my_socket->ssl;
    int client_fd = (my_socket->client_socket);
    // 写入响应 version status_code reason_phrase
    GHashTable *headers = response->headers;

    char content_len[128];
    sprintf(content_len, "%lu", response->content_length);

    if (CHUNK)
    {
        g_hash_table_insert(headers, "Transfer-Encoding", "chunked"); //开启分块传输
    }
    else
    {
        g_hash_table_insert(headers, "Content-Length", content_len);
    }

    int data_len = strlen(response->version) + strlen(response->status_code) + strlen(response->reason_phrase) + 4;
    char *data = (char *)malloc(data_len);
    sprintf(data, "%s %s %s\r\n", response->version, response->status_code, response->reason_phrase);
    //    SSL_write(ssl, data, data_len);
    my_ssl_write(ssl, data, data_len);

    // free(data);

    // 写入响应 headers
    g_hash_table_foreach(headers, send_ssl_headers, ssl);

    //    SSL_write(ssl, "\r\n", 2);
    my_ssl_write(ssl, "\r\n", 2);
    // 写入消息体
    if (response->body != NULL)
    {
        if (CHUNK)
        {
            int epoch = response->content_length / CHUNK_SIZE;
            int last = response->content_length % CHUNK_SIZE;
            int i = 0;
            while (i < epoch)
            {
                my_ssl_write(ssl, "800\r\n", 5);
                my_ssl_write(ssl, response->body + i * CHUNK_SIZE, CHUNK_SIZE);
                my_ssl_write(ssl, "\r\n", 2);
                i++;
            }

            char content_len[100];
            sprintf(content_len, "%x\r\n", last);
            my_ssl_write(ssl, content_len, strlen(content_len));
            my_ssl_write(ssl, response->body + epoch * CHUNK_SIZE, last);
            my_ssl_write(ssl, "\r\n0\r\n\r\n", 2+5);
            // my_ssl_write(ssl, "0\r\n\r\n", 5);
        }
        else
        {
            //            SSL_write(ssl, response->body, response->content_length);
            my_ssl_write(ssl, response->body, response->content_length);
        }
    }

    g_hash_table_destroy(headers);
    //    free(my_socket);
    //    free(response->body);
    //    free(response);
    //    free(data);

    //     SSL_shutdown(ssl);
    //     shutdown(client_fd, SHUT_RDWR);
    //    SSL_clear(ssl);
    return 1;
}
