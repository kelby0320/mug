#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


#include "thread_pool.h"


typedef struct {
    int fd;
    work_item_t* next;
} work_item_t;


struct thread_pool {
    work_item_t *queue;
    size_t queue_size;
    pthread_mutex_t queue_mtx;
    pthread_t *pool;
    size_t pool_size;
};


thread_pool_t* thread_pool_init(int pool_size)
{
    if (pool_size <= 0) {
        pool_size = sysconf(_SC_NPROCESSORS_ONLN);
    }

    thread_pool_t *tpool = (thread_pool_t*)malloc(sizeof(thread_pool_t));

    tpool->queue = NULL;
    tpool->queue_size = 0;

    tpool->pool = (pthread_t*)malloc(sizeof(pthread_t) * pool_size);
    tpool->pool_size = pool_size;

    pthread_mutexattr_t mtx_attr;

    pthread_mutexattr_init(&mtx_attr);
    pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_NORMAL);

    pthread_mutex_init(&tpool->queue_mtx, &mtx_attr);

    pthread_mutexattr_destroy(&mtx_attr);
}


void thread_pool_deinit(thread_pool_t* tpool)
{
    /* TODO - free queue linked list*/
    free(tpool->pool);

    pthread_mutex_destroy(&tpool->queue_mtx);

    free(tpool);
}


int thread_pool_size(const thread_pool_t* tpool)
{
    return tpool->pool_size;
}