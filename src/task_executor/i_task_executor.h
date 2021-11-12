#ifndef I_TASK_EXECUTOR_H
#define I_TASK_EXECUTOR_H


#include "task_executor/work_item.h"
#include "task_executor/work_queue.h"


struct i_task_executor;


typedef struct i_task_executor i_task_executor_t;


void i_task_executor_submit(i_task_executor_t*, task_func_t, void*);
work_queue_t* i_task_executor_queue(const i_task_executor_t*);
void i_task_executor_start_work(const i_task_executor_t*);
void i_task_executor_finish_work(const i_task_executor_t*);


#endif