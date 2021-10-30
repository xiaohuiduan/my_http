#include "glib.h"

struct http_request{
    char *method;
    char *url;
    char *version;
    GHashTable *headers;
    char *body;
};
void parse_request(struct http_request *req,char *http_data);