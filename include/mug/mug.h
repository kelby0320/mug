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


struct mug_request {
    mug_http_method_t req_method;
    char *url;
    char **headers;
    size_t headers_size;
    char *body;
    size_t body_size;
};


struct mug_response {
    mug_http_status_code_t status_code;
    char **headers;
    size_t headers_size;
    char *body;
    size_t body_size;
};


struct mug_http_request {
    char *ip_addr;
    int port;
    char *url;
    mug_http_method_t req_method;
    char **headers;
    size_t headers_size;
    char *body;
    size_t body_size;
};


struct mug_http_response {
    mug_http_status_code_t status_code;
    char **headers;
    size_t headers_size;
    char *body;
    size_t body_size;
};


struct mug_fs_request {
    char *path;
    void *buf;
    size_t buf_size;
};


struct mug_fs_response {
    char *path;
    void *buf;
    size_t buf_size;
    size_t bytes_read;
};


typedef enum {
    RESPONSE_RESULT,
    HTTP_REQUEST_RESULT,
    FS_REQUEST_RESULT,
} mug_result_type_t;


struct mug_result {
    mug_result_type_t type;
    void *data;
};


struct mug_respose_result {
    struct mug_result mug_result;
    struct mug_response *result;
};


struct mug_http_request_result {
    struct mug_result mug_result;
    struct mug_http_request *result;
    struct mug_result* (*callback)(struct mug_http_response*, void*);
};


struct mug_fs_resquest_result {
    struct mug_result mug_result;
    struct mug_fs_request *result;
    struct mug_result* (*callback)(struct mug_fs_response*, void*);
};


/*
 * mug_ctx_t constructor 
 */
mug_ctx_t* mug_ctx_init(int, int);


/* 
 * mug_ctx_t destructor
 */
int mug_ctx_deinit(mug_ctx_t*);


#endif