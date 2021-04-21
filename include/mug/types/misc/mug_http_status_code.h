#ifndef MUG_HTTP_STATUS_CODE_H
#define MUG_HTTP_STATUS_CODE_H


typedef enum {
    STATUS_OK=200,
    STATUS_BAD_REQUEST=400,
    STATUS_NOT_FOUND=404,
    STATUS_INTERNAL_SERVER_ERROR=500,
} mug_http_status_code_t;


#endif