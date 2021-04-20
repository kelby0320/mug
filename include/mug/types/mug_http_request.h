#ifndef MUG_HTTP_REQUEST
#define MUG_HTTP_REQUEST


#include "types/mug_headers.h"
#include "types/mug_body.h"


#define MAX_URL_LEN 256


typedef enum {
    HTTP_GET,
    HTTP_HEAD,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
} mug_http_method_t;


struct mug_http_request {
    mug_http_method_t method;
    char url[MAX_URL_LEN];
    mug_headers_t *headers;
    mug_body_t *body;
};


#endif