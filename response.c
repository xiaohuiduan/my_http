//
// Created by xiaohui on 2021/10/29.
//

#include <unistd.h>
#include <stdio.h>
#include "response.h"

void send_headers(gpointer key, gpointer value, gpointer stream) {
    fprintf((FILE *) stream, "%s: %s\r\n", (char *) key, (char *) value);
}

/// send response to the browser (response contain headers and body)
/// \param response
/// \param client_fd
void send_response(struct http_response *response, int client_fd) {
    GHashTable *headers = response->headers;
    FILE *stream = fdopen(client_fd, "w+");
    fprintf(stream, "%s %s %s\r\n", response->version, response->status_code, response->reason_phrase);
    g_hash_table_foreach(headers, send_headers, stream);
    fprintf(stream, "\r\n");

    if (response->body != NULL) {
        fwrite(response->body, 1, response->content_length, stream);
    }
    fflush(stream);
    fclose(stream);
    close(client_fd);

}
