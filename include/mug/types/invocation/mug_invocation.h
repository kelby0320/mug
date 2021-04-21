#ifndef MUG_INVOCATION_H
#define MUG_INVOCATION_H


#include "types/invocation/mug_invocation_type.h"


struct mug_invocation;


typedef struct mug_invocation mug_invocation_t;


mug_invocation_t* mug_invocation_alloc();
void mug_invocation_ctor(mug_invocation_t*);
mug_invocation_type_t mug_invocation_type(const mug_invocation_t*);


#endif