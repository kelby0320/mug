#ifndef MUG_REQUEST_INVOCATION_H
#define MUG_REQUEST_INVOCATION_H


#include "types/misc/mug_http_request.h"

struct mug_request_invocation;


typedef struct mug_request_invocation mug_request_invocation_t;


mug_request_invocation_t* mug_request_invocation_alloc();
void mug_request_invocation_ctor(mug_request_invocation_t*);
void mug_request_invocation_dtor(mug_request_invocation_t*);
mug_http_request_t* mug_request_invocation_http_request(const mug_request_invocation_t*);
void mug_request_invocation_set_http_request(mug_request_invocation_t*, mug_http_request_t*);
mug_http_request_t* mug_request_invocation_move_http_request(mug_request_invocation_t*, mug_http_request_t**);


#endif