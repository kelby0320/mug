#ifndef MUG_CONTINUATION_RESULT_H
#define MUG_CONTINUATION_RESULT_H


#include "core/handler.h"
#include "types/misc/mug_save_bag.h"


struct mug_continuation_result;


typedef struct mug_continuation_result mug_continuation_result_t;


mug_continuation_result_t* mug_continuation_result_alloc();
void mug_continuation_result_ctor(mug_continuation_result_t*);
void mug_continuation_result_dtor(mug_continuation_result_t*);
mug_continuation_handler_t mug_continuation_result_continuation_handler(const mug_continuation_result_t*);
void mug_continuation_result_set_continuation_handler(mug_continuation_result_t*, mug_continuation_handler_t);
mug_save_bag_t* mug_continuation_result_save_bag(const mug_continuation_result_t*);


#endif