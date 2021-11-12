#ifndef WORK_ITEM_H
#define WORK_ITEM_H


#include "types/result/mug_result.h"


typedef mug_result_t* (*task_func_t)(void*);


struct work_item {
    void *arg;
    task_func_t func;
    struct work_item* next;
};


#endif