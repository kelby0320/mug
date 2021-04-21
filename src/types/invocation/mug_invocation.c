#include <stdlib.h>

#include "types/invocation/mug_invocation.h"
#include "types/invocation/__mug_invocation.h"
#include "types/invocation/mug_invocation_type.h"


mug_invocation_t* mug_invocation_alloc()
{
    return (mug_invocation_t*)malloc(sizeof(mug_invocation_t));
}


void mug_invocation_ctor(mug_invocation_t *invocation)
{
    invocation->type = MUG_INVOCATION_BASE;
}


mug_invocation_type_t mug_invocation_type(const mug_invocation_t *invocation)
{
    return invocation->type;
}