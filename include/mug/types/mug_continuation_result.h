#ifndef MUG_CONTINUATION_RESULT_H
#define MUG_CONTINUATION_RESULT_H


#include "types/mug_result.h"
#include "types/mug_continuation_invocation.h"
#include "types/mug_save_bag.h"


typedef mug_result_t* (*continuation_handler_t)(mug_continuation_invocation_t*);


typedef struct {
    mug_result_t result;
    continuation_handler_t handler;
    mug_save_bag_t *save_bag;
} mug_continuation_result_t;


#endif