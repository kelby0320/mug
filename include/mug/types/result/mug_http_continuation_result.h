#ifndef MUG_HTTP_CONTINUATION_RESULT_H
#define MUG_HTTP_CONTINUATION_RESULT_H


#include "types/misc/mug_http_request.h"


struct mug_http_continuation_result;


typedef struct mug_http_continuation_result mug_http_continuation_result_t;


mug_http_continuation_result_t *mug_http_continuation_result_alloc();
void mug_http_continuation_result_ctor(mug_http_continuation_result_t*);
void mug_http_continuation_result_dtor(mug_http_continuation_result_t*);
mug_http_request_t* mug_http_continuation_result_http_request(const mug_http_continuation_result_t*);
void mug_http_continuation_result_set_http_request(mug_http_continuation_result_t*, mug_http_request_t*);


#endif