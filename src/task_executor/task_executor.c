#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "task_executor/i_task_executor.h"
#include "task_executor/__i_task_executor.h"
#include "task_executor/task_executor.h"
#include "task_executor/work_queue.h"
#include "task_executor/work_item.h"
#include "task_executor/pool_worker.h"


struct task_executor {
    i_task_executor_t i_task_executor;
    work_queue_t *queue;
    pthread_t *pool;
    int pool_size;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
};


static void connect_virtual_functions(task_executor_t*);
static void mtx_init(task_executor_t*);
static void fill_pool(pthread_t*, int, void* (*)(void*), void*);
static void cancel_threads(task_executor_t*);

/* Virtual Functions */
void task_executor_submit(i_task_executor_t*, task_func_t, void*);
work_queue_t* task_executor_queue(const i_task_executor_t*);
void task_executor_start_work(const i_task_executor_t*);
void task_executor_finish_work(const i_task_executor_t*);


task_executor_t* task_executor_alloc()
{
    return (task_executor_t*)malloc(sizeof(task_executor_t));
}


void task_executor_ctor(task_executor_t *executor)
{
    executor->queue = work_queue_alloc();
    work_queue_ctor(executor->queue);

    executor->pool_size = sysconf(_SC_NPROCESSORS_ONLN);
    executor->pool = (pthread_t*)malloc(sizeof(pthread_t) * executor->pool_size);

    mtx_init(executor);
    pthread_cond_init(&executor->cond, NULL); 

    connect_virtual_functions(executor);

    fill_pool(executor->pool, executor->pool_size, pool_worker, executor);
}


void task_executor_dtor(task_executor_t *executor)
{
    cancel_threads(executor);
    work_queue_dtor(executor->queue);
    free(executor->queue);
    pthread_mutex_destroy(&executor->mtx);
    free(executor->pool);
}


static void connect_virtual_functions(task_executor_t *executor)
{
    executor->i_task_executor.submit_func = task_executor_submit;
    executor->i_task_executor.queue_func = task_executor_queue;
    executor->i_task_executor.start_work_func = task_executor_start_work;
    executor->i_task_executor.finish_work_func = task_executor_finish_work;
}


static void mtx_init(task_executor_t *executor)
{
    pthread_mutexattr_t mtx_attr;

    pthread_mutexattr_init(&mtx_attr);
    pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_NORMAL);

    pthread_mutex_init(&executor->mtx, &mtx_attr);

    pthread_mutexattr_destroy(&mtx_attr);
}


static void fill_pool(pthread_t* pool, int pool_size, void* (*func)(void*), void* arg)
{
    for (int i = 0; i < pool_size; i++) {
        pthread_create(&pool[i], NULL, func, arg);
    }
}


mug_result_t* test_func(void *args)
{
    return NULL;
}

static void cancel_threads(task_executor_t *executor)
{
    for (int i = 0; i < executor->pool_size; i++) {
        pthread_cancel(executor->pool[i]);
    }
}


/* Virtual Functions */


void task_executor_submit(i_task_executor_t *i_executor, task_func_t func, void* arg)
{
    task_executor_t *executor = (task_executor_t*)i_executor;

    struct work_item *item = (struct work_item*)malloc(sizeof(struct work_item));

    item->func = func;
    item->arg = arg;

    pthread_mutex_lock(&executor->mtx);

    work_queue_enqueue(executor->queue, item);

    pthread_mutex_unlock(&executor->mtx);

    pthread_cond_signal(&executor->cond);
}


work_queue_t* task_executor_queue(const i_task_executor_t *i_task_executor)
{
    task_executor_t *executor = (task_executor_t*)i_task_executor;
    return executor->queue;
}


void task_executor_start_work(const i_task_executor_t *i_task_executor)
{
    task_executor_t *executor = (task_executor_t*)i_task_executor;

    pthread_mutex_lock(&executor->mtx);

    while (work_queue_size(executor->queue) == 0) {
        pthread_cond_wait(&executor->cond, &executor->mtx);
    }
}


void task_executor_finish_work(const i_task_executor_t *i_task_executor)
{
    task_executor_t *executor = (task_executor_t*)i_task_executor;
    pthread_mutex_unlock(&executor->mtx);
}