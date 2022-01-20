#ifndef MUG_HTTP_METHOD_H
#define MUG_HTTP_METHOD_H


typedef enum {
    HTTP_GET,
    HTTP_HEAD,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_METHOD_UNKNOWN
} mug_http_method_t;


#endif