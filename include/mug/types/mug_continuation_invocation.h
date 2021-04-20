#ifndef MUG_CONTINUATION_INVOCATION_H
#define MUG_CONTINUATION_INVOCATION_H


#include "types/mug_invocation.h"
#include "types/mug_save_bag.h"


typedef struct {
    mug_continuation_invocation_t continuation_invocation;
    mug_save_bag_t *save_bag;
} mug_continuation_invocation_t;


#endif