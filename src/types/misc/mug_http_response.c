#include <stdlib.h>
#include <string.h>

#include "types/misc/mug_http_response.h"
#include "types/misc/mug_http_status_code.h"
#include "types/misc/mug_http_version.h"
#include "types/misc/mug_headers.h"
#include "types/misc/mug_body.h"


#define MAX_MSG_LEN 128


struct mug_http_response {
    mug_http_version_t version;
    mug_http_status_code_t status_code;
    char message[MAX_MSG_LEN];
    mug_headers_t *headers;
    mug_body_t *body;
};


mug_http_response_t* mug_http_response_alloc()
{
    return (mug_http_response_t*)malloc(sizeof(mug_http_response_t));
}


void mug_http_response_ctor(mug_http_response_t *http_response)
{
    http_response->version = HTTP_1_1;

    http_response->status_code = STATUS_OK;
    memset(http_response->message, 0, MAX_MSG_LEN);

    mug_headers_t *headers = mug_headers_alloc();
    mug_headers_ctor(headers);
    http_response->headers = headers;

    mug_body_t *body = mug_body_alloc();
    mug_body_ctor(body);
    http_response->body = body;
}


void mug_http_response_dtor(mug_http_response_t *http_response)
{
    mug_headers_dtor(http_response->headers);
    free(http_response->headers);

    mug_body_dtor(http_response->body);
    free(http_response->body);
}


mug_http_version_t mug_http_response_http_version(const mug_http_response_t *http_response)
{
    return http_response->version;
}


void mug_http_response_set_http_version(mug_http_response_t *http_response, mug_http_version_t version)
{
    http_response->version = version;
}


mug_http_status_code_t mug_http_response_status_code(const mug_http_response_t *http_response)
{
    return http_response->status_code;
}


void mug_http_response_set_status_code(mug_http_response_t *http_response, mug_http_status_code_t status_code)
{
    http_response->status_code = status_code;
}


void mug_http_response_message(const mug_http_response_t *http_response, char *buf)
{
    strcpy(buf, http_response->message);
}


void mug_http_response_set_message(mug_http_response_t *http_response, const char *buf)
{
    strcpy(http_response->message, buf);
}


mug_headers_t* mug_http_response_headers(const mug_http_response_t *http_response)
{
    return http_response->headers;
}


mug_body_t* mug_http_response_body(const mug_http_response_t *http_response)
{
    return http_response->body;
}