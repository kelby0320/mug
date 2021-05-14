#ifndef MUG_RESULT_H
#define MUG_RESULT_H


#include "types/result/mug_result_type.h"


struct mug_result;


typedef struct mug_result mug_result_t;


mug_result_t* mug_result_alloc();
void mug_result_ctor(mug_result_t*);
mug_result_type_t mug_result_type(const mug_result_t*);

#endif