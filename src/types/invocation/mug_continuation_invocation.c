#include <stdlib.h>

#include "types/invocation/mug_invocation.h"
#include "types/invocation/mug_continuation_invocation.h"
#include "types/invocation/__mug_continuation_invocation.h"
#include "types/invocation/mug_invocation_type.h"
#include "types/misc/mug_save_bag.h"


mug_continuation_invocation_t* mug_continuation_invocation_alloc()
{
    return (mug_continuation_invocation_t*)malloc(sizeof(mug_continuation_invocation_t));
}


void mug_continuation_invocation_ctor(mug_continuation_invocation_t *continuation_invocation)
{
    mug_invocation_t *invocation = (mug_invocation_t*)continuation_invocation;
    mug_invocation_ctor(invocation);
    invocation->type = MUG_CONTINUATION_INVOCATION_BASE;

    mug_save_bag_t *save_bag = mug_save_bag_alloc();
    mug_save_bag_ctor(save_bag);
    continuation_invocation->save_bag = save_bag;
}


void mug_continuation_invocation_dtor(mug_continuation_invocation_t *continuation_invocation)
{
    mug_save_bag_dtor(continuation_invocation->save_bag);
    free(continuation_invocation->save_bag);
}


mug_save_bag_t* mug_continuation_invocation_save_bag(const mug_continuation_invocation_t *continuation_invocation)
{
    return continuation_invocation->save_bag;
}