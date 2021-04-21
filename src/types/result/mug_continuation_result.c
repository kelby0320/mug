#include <stdlib.h>

#include "types/result/mug_result.h"
#include "types/result/__mug_result.h"
#include "types/result/mug_continuation_result.h"
#include "types/result/__mug_continuation_result.h"
#include "types/result/mug_result_type.h"
#include "types/misc/mug_save_bag.h"
#include "core/handler.h"


mug_continuation_result_t* mug_continuation_result_alloc()
{
    return (mug_continuation_result_t*)malloc(sizeof(mug_continuation_result_t));
}


void mug_continuation_result_ctor(mug_continuation_result_t *continuation_result)
{
    mug_result_t *result = (mug_result_t*)continuation_result;
    result->type = MUG_CONTINUATION_RESULT_BASE;

    continuation_result->handler = NULL;

    mug_save_bag_t *save_bag = mug_save_bag_alloc();
    mug_save_bag_ctor(save_bag);
    continuation_result->save_bag = save_bag;
}


void mug_continuation_result_dtor(mug_continuation_result_t *continuation_result)
{
    mug_save_bag_dtor(continuation_result->save_bag);
    free(continuation_result->save_bag);
}


mug_continuation_handler_t mug_continuation_result_continuation_handler(const mug_continuation_result_t *continuation_result)
{
    return continuation_result->handler;
}


void mug_continuation_result_set_continuation_handler(mug_continuation_result_t *continuation_result, mug_continuation_handler_t continuation_handler)
{
    continuation_result->handler = continuation_handler;
}


mug_save_bag_t* mug_continuation_result_save_bag(const mug_continuation_result_t *continuation_result)
{
    return continuation_result->save_bag;
}