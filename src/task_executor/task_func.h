#ifndef TASK_HANDLER_H
#define TASK_HANDLER_H


#include "types/result/mug_result.h"


typedef mug_result_t* (*task_func_t)(void*);


#endif