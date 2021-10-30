//
// Created by xiaohui on 2021/10/29.
//

#include <unistd.h>
#include <bits/types/FILE.h>
#include <stdio.h>
#include "response.h"

void send_headers(gpointer key, gpointer value, gpointer stream) {
    fprintf((FILE *) stream, "%s: %s\r\n", (char *) key, (char *) value);
}

void send_body(char *body, FILE *stream) {
    fprintf(stream, "\r\n%s\r\n", body);
}


void send_response(struct http_response *response, int client_fd) {
    GHashTable *headers = response->headers;
    FILE *stream = fdopen(client_fd, "w+");
    fprintf(stream, "%s %s %s\r\n", response->version, response->status_code, response->reason_phrase);
    g_hash_table_foreach(headers, send_headers, stream);
    send_body(response->body, stream);

    fflush(stream);
    fclose(stream);

}
