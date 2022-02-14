#ifndef __MUG_CONTINUATION_RESULT_H
#define __MUG_CONTINUATION_RESULT_H


#include "types/result/mug_result.h"
#include "types/result/__mug_result.h"
#include "core/handler.h"
#include "types/misc/mug_save_bag.h"


struct mug_continuation_result {
    mug_result_t result;
    mug_continuation_handler_t handler;
    mug_save_bag_t *save_bag;
};


#endif