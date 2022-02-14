#ifndef __I_TASK_EXECUTOR_H
#define __I_TASK_EXECUTOR_H


#include "task_executor/i_task_executor.h"
#include "task_executor/task_executor_state.h"
#include "task_executor/work_item.h"
#include "task_executor/work_queue.h"


typedef void (*submit_func_t)(i_task_executor_t*, task_func_t, void*);
typedef work_queue_t* (*queue_func_t)(const i_task_executor_t*);
typedef void (*start_work_func_t)(const i_task_executor_t*);
typedef void (*finish_work_func_t)(const i_task_executor_t*);


struct i_task_executor {
    submit_func_t submit_func;
    queue_func_t queue_func;
    start_work_func_t start_work_func;
    finish_work_func_t finish_work_func;
};


#endif