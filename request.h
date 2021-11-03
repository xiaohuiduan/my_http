#include "glib.h"

struct http_request{
    char *method;
    char *url;
    char *version;
    GHashTable *headers;
    char *body;
};


struct request_data {
    char *real_url;
    GHashTable *data;
};

void parse_request(struct http_request *req,char *http_data);

/// 解析get请求参数
/// \param request_data
/// \param url
void parse_get_data(struct request_data *request_data, char *url);

/// 解析post请求参数
/// \param req
/// \param url
void parse_post_data(struct request_data *req,char *url);