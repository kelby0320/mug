#ifndef TASK_EXECUTOR_H
#define TASK_EXECUTOR_H


#include "task_executor/work_item.h"
#include "task_executor/work_queue.h"


struct task_executor;


typedef struct task_executor task_executor_t;


task_executor_t* task_executor_alloc();
void task_executor_ctor(task_executor_t*);
void task_executor_dtor(task_executor_t*);


#endif