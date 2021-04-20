#ifndef MUG_REQUEST_H
#define MUG_REQUEST_H


#include "types/mug_invocation.h"
#include "types/mug_headers.h"
#include "types/mug_body.h"
#include "types/mug_http_request.h"


#define MAX_URI_LEN 128


typedef struct {
    mug_invocation_t invocation;
    mug_http_method_t method;
    char uri[MAX_URI_LEN];
    mug_headers_t *headers;
    mug_body_t *body;
} mug_request_invocation_t;


#endif