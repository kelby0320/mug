#include <stdlib.h>

#include "types/result/mug_result.h"
#include "types/result/__mug_result.h"
#include "types/result/mug_result_type.h"


mug_result_t* mug_result_alloc()
{
    return (mug_result_t*)malloc(sizeof(mug_result_t));
}


void mug_result_ctor(mug_result_t *result)
{
    result->type = MUG_RESULT_BASE;
}


mug_result_type_t mug_result_type(const mug_result_t *result)
{
    return result->type;
}
