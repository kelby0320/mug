#include <stdlib.h>
#include <string.h>

#include "types/invocation/mug_request_invocation.h"
#include "types/invocation/mug_invocation.h"
#include "types/invocation/__mug_invocation.h"
#include "types/misc/mug_http_method.h"
#include "types/misc/mug_headers.h"
#include "types/misc/mug_body.h"


#define MAX_URI_LEN 128


struct mug_request_invocation {
    mug_invocation_t invocation;
    mug_http_method_t method;
    char uri[MAX_URI_LEN];
    mug_headers_t *headers;
    mug_body_t *body;
};


mug_request_invocation_t* mug_request_invocation_alloc()
{
    return (mug_request_invocation_t*)malloc(sizeof(mug_request_invocation_t));
}


void mug_request_invocation_ctor(mug_request_invocation_t *request_invocation)
{
    mug_invocation_t *invocation = (mug_invocation_t*)request_invocation;
    mug_invocation_ctor(invocation);

    request_invocation->invocation.type = MUG_REQUEST_INVOCATION;
    request_invocation->method = HTTP_GET;
    memset(request_invocation->uri, 0, MAX_URI_LEN);

    mug_headers_t *headers = mug_headers_alloc();
    mug_headers_ctor(headers);
    request_invocation->headers = headers;


    mug_body_t *body = mug_body_alloc();
    mug_body_ctor(body);
    request_invocation->body = body;
}


void mug_request_invocation_dtor(mug_request_invocation_t *request_invocation)
{
    mug_headers_dtor(request_invocation->headers);
    free(request_invocation->headers);

    mug_body_dtor(request_invocation->body);
    free(request_invocation->body);
}


mug_http_method_t mug_request_invocation_http_method(const mug_request_invocation_t *request_invocation)
{
    return request_invocation->method;
}


void mug_request_invocation_set_http_method(mug_request_invocation_t *request_invocation, mug_http_method_t http_method)
{
    request_invocation->method = http_method;
}


void mug_request_invocation_uri(const mug_request_invocation_t *request_invocation, char *buf)
{
    strcpy(buf, request_invocation->uri);
}


void mug_request_invocation_set_uri(mug_request_invocation_t *request_invocation, const char *buf)
{
    strcpy(request_invocation->uri, buf);
}


mug_headers_t* mug_request_invocation_headers(const mug_request_invocation_t *request_invocation)
{
    return request_invocation->headers;
}


mug_body_t* mug_request_invocation_body(const mug_request_invocation_t *request_invocation)
{
    return request_invocation->body;
}