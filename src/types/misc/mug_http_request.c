#include <stdlib.h>
#include <string.h>

#include "types/misc/mug_http_request.h"
#include "types/misc/mug_http_method.h"
#include "types/misc/mug_http_version.h"
#include "types/misc/mug_headers.h"
#include "types/misc/mug_body.h"


struct mug_http_request {
    mug_http_method_t method;
    char url[MAX_URL_LEN];
    mug_http_version_t version;
    mug_headers_t *headers;
    mug_body_t *body;
};


mug_http_request_t* mug_http_request_alloc()
{
    return (mug_http_request_t*)malloc(sizeof(mug_http_request_t));
}


void mug_http_request_ctor(mug_http_request_t* http_request)
{
    http_request->method = HTTP_METHOD_UNKNOWN;
    http_request->version = HTTP_VERSION_UNKNOWN;
    memset(http_request->url, 0, MAX_URL_LEN);

    mug_headers_t *headers = mug_headers_alloc();
    mug_headers_ctor(headers);
    http_request->headers = headers;

    mug_body_t *body = mug_body_alloc();
    mug_body_ctor(body);
    http_request->body = body;
}


void mug_http_request_dtor(mug_http_request_t* http_request)
{
    mug_headers_dtor(http_request->headers);
    free(http_request->headers);

    mug_body_dtor(http_request->body);
    free(http_request->body);
}


mug_http_method_t mug_http_request_http_method(const mug_http_request_t *http_request)
{
    return http_request->method;
}


void mug_http_request_set_http_method(mug_http_request_t *http_request, mug_http_method_t method)
{
    http_request->method = method;
}


void mug_http_request_http_url(const mug_http_request_t *http_request, char *buf)
{
    strcpy(buf, http_request->url);
}


void mug_http_request_set_http_url(mug_http_request_t *http_request, const char *buf)
{
    strcpy(http_request->url, buf);
}

mug_http_version_t mug_http_request_http_version(mug_http_request_t *http_request)
{
    return http_request->version;
}


void mug_http_request_set_http_version(mug_http_request_t *http_request, mug_http_version_t version)
{
    http_request->version = version;
}


mug_headers_t* mug_http_request_headers(const mug_http_request_t *http_request)
{
    return http_request->headers;
}


mug_body_t* mug_http_request_body(const mug_http_request_t *http_request)
{
    return http_request->body;
}