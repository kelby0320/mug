#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "task_executor/i_task_executor.h"
#include "task_executor/task_executor_state.h"
#include "task_executor/work_queue.h"


static void process_work_item(work_queue_t*);
static void execute_work_item(struct work_item*);


void* pool_worker(void* arg)
{
    i_task_executor_t *executor = (i_task_executor_t*)arg;

    pthread_t pid = pthread_self();

    for (;;) {
        i_task_executor_start_work(executor);

        work_queue_t *queue = i_task_executor_queue(executor);
        if (work_queue_size(queue) > 0) {
            process_work_item(queue);
        } 

        i_task_executor_finish_work(executor);
    }

    return NULL;
}


static void process_work_item(work_queue_t *queue)
{
    struct work_item *work_item = work_queue_dequeue(queue);
    execute_work_item(work_item);

}


static void execute_work_item(struct work_item *item)
{
    void *arg = item->arg;

    item->func(arg);

    if (arg != NULL) {
        free(arg);
    }

    free(item);
}