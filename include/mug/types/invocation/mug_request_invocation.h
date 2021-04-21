#ifndef MUG_REQUEST_INVOCATION_H
#define MUG_REQUEST_INVOCATION_H


#include "types/misc/mug_http_method.h"
#include "types/misc/mug_headers.h"
#include "types/misc/mug_body.h"


struct mug_request_invocation;


typedef struct mug_request_invocation mug_request_invocation_t;


mug_request_invocation_t* mug_request_invocation_alloc();
void mug_request_invocation_ctor(mug_request_invocation_t*);
void mug_request_invocation_dtor(mug_request_invocation_t*);
mug_http_method_t mug_request_invocation_http_method(const mug_request_invocation_t*);
void mug_request_invocation_set_http_method(mug_request_invocation_t*, mug_http_method_t);
void mug_request_invocation_uri(const mug_request_invocation_t*, char*);
void mug_request_invocation_set_uri(mug_request_invocation_t*, const char*);
mug_headers_t* mug_request_invocation_headers(const mug_request_invocation_t*);
mug_body_t* mug_request_invocation_body(const mug_request_invocation_t*);


#endif