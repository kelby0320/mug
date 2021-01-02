#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


#include "thread_pool.h"
#include "work_queue.h"


struct thread_pool {
    work_queue_t *queue;
    pthread_t *pool;
    size_t pool_size;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
};


static void fill_pool(pthread_t*, size_t, void* (*)(void*), void*);
static void* pool_func(void*);


static void fill_pool(pthread_t* pool, size_t pool_size, void* (*func)(void*), void* arg)
{
    for (int i = 0; i < (int)pool_size; i++) {
        pthread_create(&pool[i], NULL, func, arg);
    }
}


static void* pool_func(void* arg)
{
    thread_pool_t *tpool = (thread_pool_t*)arg;

    pthread_t pid = pthread_self();

    for (;;) {
	printf("pool_func - %#010x: Mtx Lock\n", (unsigned int)pid);
        pthread_mutex_lock(&tpool->mtx);

        while (work_queue_size(tpool->queue) == 0) {
	    printf("pool_func - %#010x: Cond Wait\n", (unsigned int)pid);
            pthread_cond_wait(&tpool->cond, &tpool->mtx);
	    printf("pool_func - %#010x: Cond Signaled\n", (unsigned int)pid);
        }

        if (work_queue_size(tpool->queue) > 0) {
	    printf("pool_func - %#010x: Do Work\n", (unsigned int)pid);
	    /* Get work item to process */
	    struct work_item *work_item = work_queue_dequeue(tpool->queue);

	    /* Call handling function */
	    work_item->func(work_item->arg);

	    /* Free argument memory */
	    if (work_item->arg != NULL) {
		free(work_item->arg);
	    }

	    /* Free work item memory */
	    free(work_item);
        }

	printf("pool_func - %#010x: Mtx Unlock\n", (unsigned int)pid);
        pthread_mutex_unlock(&tpool->mtx);
    }

    return NULL;
}


thread_pool_t* thread_pool_init(int pool_size)
{
    if (pool_size <= 0) {
        pool_size = sysconf(_SC_NPROCESSORS_ONLN);
    }

    thread_pool_t *tpool = (thread_pool_t*)malloc(sizeof(thread_pool_t));

    tpool->queue = work_queue_init();

    tpool->pool = (pthread_t*)malloc(sizeof(pthread_t) * pool_size);
    tpool->pool_size = pool_size;

    /* Initalize mutex */
    pthread_mutexattr_t mtx_attr;

    pthread_mutexattr_init(&mtx_attr);
    pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_NORMAL);

    pthread_mutex_init(&tpool->mtx, &mtx_attr);

    pthread_mutexattr_destroy(&mtx_attr);

    pthread_cond_init(&tpool->cond, NULL); 
    
    fill_pool(tpool->pool, tpool->pool_size, pool_func, tpool);

    return tpool;
}


void thread_pool_deinit(thread_pool_t* tpool)
{
    /* TODO - Cancel all threads */

    work_queue_deinit(tpool->queue);

    pthread_mutex_destroy(&tpool->mtx);

    free(tpool->pool);

    free(tpool);
}


int thread_pool_size(const thread_pool_t* tpool)
{
    return tpool->pool_size;
}


void thread_pool_submit(thread_pool_t* tpool, int (*func)(void*), void* arg)
{
    struct work_item *item = (struct work_item*)malloc(sizeof(struct work_item));

    item->func = func;
    item->arg = arg;

    pthread_mutex_lock(&tpool->mtx);

    work_queue_enqueue(tpool->queue, item);

    pthread_mutex_unlock(&tpool->mtx);

    pthread_cond_signal(&tpool->cond);
}

