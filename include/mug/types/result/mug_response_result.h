#ifndef MUG_RESPONSE_RESULT_H
#define MUG_RESPONSE_RESULT_H


#include "types/misc/mug_http_response.h"


struct mug_response_result;


typedef struct mug_response_result mug_response_result_t;


mug_response_result_t* mug_response_result_alloc();
void mug_response_result_ctor(mug_response_result_t*);
void mug_response_result_dtor(mug_response_result_t*);
mug_http_response_t* mug_response_result_http_response(const mug_response_result_t*);
void mug_response_result_set_http_response(mug_response_result_t*, mug_http_response_t*);
mug_http_response_t* mug_response_result_move_http_response(mug_response_result_t*, mug_http_response_t**);

#endif