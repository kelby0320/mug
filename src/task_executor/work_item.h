#ifndef WORK_ITEM_H
#define WORK_ITEM_H


#include "types/result/mug_result.h"
#include "task_executor/task_func.h"


struct work_item {
    void *arg;
    task_func_t func;
    struct work_item* next;
};


#endif