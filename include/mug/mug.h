#ifndef MUG_H
#define MUG_H

#include <stdlib.h>


struct mug_ctx;


typedef struct mug_ctx mug_ctx_t;


typedef enum {
    HTTP_GET,
    HTTP_HEAD,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
} mug_http_method_t;

typedef enum {
    STATUS_OK=200,
    STATUS_BAD_REQUEST=400,
    STATUS_NOT_FOUND=404,
    STATUS_INTERNAL_SERVER_ERROR=500,
} mug_http_status_code_t;


typedef struct {
    mug_http_method_t req_method;
    char *url;
    char **headers;
    size_t headers_size;
    char *body;
    size_t body_size;
} mug_request_t;


typedef struct {
    mug_http_status_code_t status_code;
    char **headers;
    size_t headers_size;
    char *body;
    size_t body_size;
} mug_response_t;


typedef struct {
    char *ip_addr;
    int port;
    char *url;
    mug_http_request_t req_method;
    char **headers;
    size_t headers_size;
    char *body;
    size_t body_size;
} mug_http_request_t;


typedef struct {
    mug_http_status_code_t status_code;
    char **headers;
    size_t headers_size;
    char *body;
    size_t body_size;
} mug_http_response_t;


typedef struct {
    char *path;
    void *buf;
    size_t buf_size;
} mug_fs_request_t;


typedef struct {
    char *path;
    void *buf;
    size_t buf_size;
    size_t bytes_read;
} mug_fs_response_t;


typedef enum {
    RESPONSE_RESULT,
    HTTP_REQUEST_RESULT,
    FS_REQUEST_RESULT,
} mug_result_type_t;


typedef struct {
    mug_result_type_t type;
    void *data;
} mug_result_t;


typedef struct {
    mug_result_t mug_result;
    mug_response_t *result;
} mug_respose_result_t;


typedef struct {
    mug_result_t mug_result;
    mug_http_request_t *result;
    mug_result_t* (*callback)(mug_http_response_t*, void*);
} mug_http_request_result_t;


typedef struct {
    mug_result_t mug_result;
    mug_fs_request_t *result;
    mug_result_t* (*callback)(mug_fs_response_t*, void*);
} mug_fs_resquest_result_t;


#endif