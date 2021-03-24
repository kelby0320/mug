#ifndef MUG_H
#define MUG_H

#include <stdlib.h>


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


typedef enum {
    RESPONSE_RESULT,
    HTTP_REQUEST_RESULT,
    FS_REQUEST_RESULT,
} mug_result_type_t;


typedef enum {
    MUG_RESPONSE,
    MUG_HTTP_RESPONSE,
    MUG_FS_RESPONSE
} mug_response_type_t;


struct mug_ctx;
struct routing_table;


typedef struct mug_ctx mug_ctx_t;
typedef struct routing_table routing_table_t;


struct mug_request {
    mug_http_method_t req_method;
    char *url;
    char **headers;
    size_t headers_size;
    char *body;
    size_t body_size;
};


struct mug_response_base {
    mug_response_type_t type;
};


struct mug_response {
    struct mug_response_base mug_response_base;
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
    struct mug_response_base mug_response_base; 
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
    struct mug_response_base mug_response_base;
    char *path;
    void *buf;
    size_t buf_size;
    size_t bytes_read;
};


struct mug_continuation {
    struct mug_response_base *responses;
    size_t responses_size;
};


struct mug_result {
    mug_result_type_t type;
};


struct mug_result_set {
    struct mug_result mug_result;
    struct mug_result *results;
    struct mug_result* (*callback)(struct mug_continuation*);
};


struct mug_response_result {
    struct mug_result mug_result;
    struct mug_response *result;
};


struct mug_http_request_result {
    struct mug_result mug_result;
    struct mug_http_request *result;
    struct mug_result* (*callback)(struct mug_http_response*);
    void* data;
};


struct mug_fs_resquest_result {
    struct mug_result mug_result;
    struct mug_fs_request *result;
    struct mug_result* (*callback)(struct mug_fs_response*);
    void* data;
};


typedef struct mug_result* (*route_handler_t)(struct mug_request*);


/* mug_request methods */
struct mug_request* mug_request_init();
void mug_request_deinit(struct mug_request*);


/* mug_response methods */
struct mug_response* mug_response_init();
void mug_response_deinit(struct mug_response*);


/* mug_ctx_t methods */
mug_ctx_t* mug_ctx_init(int, int);
void mug_ctx_deinit(mug_ctx_t*);
void mug_ctx_serve(mug_ctx_t*);
routing_table_t* mug_ctx_routing_table(mug_ctx_t*);


/* routing_table_t methods*/
void routing_table_add_route(routing_table_t*, char*, route_handler_t);


#endif
