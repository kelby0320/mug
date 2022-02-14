#ifndef __MUG_CONTINUATION_INVOCATION_H
#define __MUG_CONTINUATION_INVOCATION_H


#include "types/invocation/mug_invocation.h"
#include "types/invocation/__mug_invocation.h"
#include "types/misc/mug_save_bag.h"


struct mug_continuation_invocation {
    mug_invocation_t invocation;
    mug_save_bag_t *save_bag;
};


#endif