#ifndef MUG_HTTP_RESPONSE_H
#define MUG_HTTP_RESPONSE_H


#include "types/mug_headers.h"
#include "types/mug_body.h"


#define MAX_MSG_LEN 128


typedef enum {
    STATUS_OK=200,
    STATUS_BAD_REQUEST=400,
    STATUS_NOT_FOUND=404,
    STATUS_INTERNAL_SERVER_ERROR=500,
} mug_http_status_code_t;


typedef struct {
    mug_http_status_code_t status_code;
    char message[MAX_MSG_LEN];
    mug_headers_t *headers;
    mug_body_t *body;
} mug_http_response_t;


#endif