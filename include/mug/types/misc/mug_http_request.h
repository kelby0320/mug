#ifndef MUG_HTTP_REQUEST
#define MUG_HTTP_REQUEST


#include "types/misc/mug_http_method.h"
#include "types/misc/mug_headers.h"
#include "types/misc/mug_body.h"


struct mug_http_request;


typedef struct mug_http_request mug_http_request_t;


mug_http_request_t* mug_http_request_alloc();
void mug_http_request_ctor(mug_http_request_t*);
void mug_http_request_dtor(mug_http_request_t*);
mug_http_method_t mug_http_request_http_method(const mug_http_request_t*);
void mug_http_request_set_http_method(mug_http_request_t*, mug_http_method_t);
void mug_http_request_http_url(const mug_http_request_t*, char*);
void mug_http_request_set_http_url(mug_http_request_t*, const char*);
mug_headers_t* mug_http_request_headers(const mug_http_request_t*);
mug_body_t* mug_http_request_body(const mug_http_request_t*);


#endif