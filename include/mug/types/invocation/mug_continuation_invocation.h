#ifndef MUG_CONTINUATION_INVOCATION_H
#define MUG_CONTINUATION_INVOCATION_H


#include "types/misc/mug_save_bag.h"


struct mug_continuation_invocation;


typedef struct mug_continuation_invocation mug_continuation_invocation_t;


mug_continuation_invocation_t* mug_continuation_invocation_alloc();
void mug_continuation_invocation_ctor(mug_continuation_invocation_t*);
void mug_continuation_invocation_dtor(mug_continuation_invocation_t*);
mug_save_bag_t* mug_continuation_invocation_save_bag(const mug_continuation_invocation_t*);


#endif