#include <stdlib.h>

#include "task_executor/i_task_executor.h"
#include "task_executor/__i_task_executor.h"
#include "task_executor/task_executor_state.h"
#include "task_executor/work_item.h"


void i_task_executor_submit(i_task_executor_t *i_executor, task_func_t func, void *arg)
{
    i_executor->submit_func(i_executor, func, arg);
}


work_queue_t* i_task_executor_queue(const i_task_executor_t *i_task_executor)
{
    return i_task_executor->queue_func(i_task_executor);
}


void i_task_executor_start_work(const i_task_executor_t *i_task_executor)
{
    i_task_executor->start_work_func(i_task_executor);
}


void i_task_executor_finish_work(const i_task_executor_t *i_task_executor)
{
    i_task_executor->finish_work_func(i_task_executor);
}