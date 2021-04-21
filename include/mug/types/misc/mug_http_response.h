#ifndef MUG_HTTP_RESPONSE_H
#define MUG_HTTP_RESPONSE_H


#include "types/misc/mug_http_status_code.h"
#include "types/misc/mug_headers.h"
#include "types/misc/mug_body.h"


struct mug_http_response;


typedef struct mug_http_response mug_http_response_t;


mug_http_response_t* mug_http_response_alloc();
void mug_http_response_ctor(mug_http_response_t*);
void mug_http_response_dtor(mug_http_response_t*);
mug_http_status_code_t mug_http_response_status_code(const mug_http_response_t*);
void mug_http_response_set_status_code(mug_http_response_t*, mug_http_status_code_t);
void mug_http_response_message(const mug_http_response_t*, char*);
void mug_http_response_set_message(mug_http_response_t*, const char*);
mug_headers_t* mug_http_response_headers(const mug_http_response_t*);
mug_body_t* mug_http_response_body(const mug_http_response_t*);


#endif