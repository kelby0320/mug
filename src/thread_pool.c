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
    pthread_mutex_t queue_mux;
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
}


int thread_pool_deinit(thread_pool_t* tpool)
{
    /* TODO */
    return 0;
}


int thread_pool_size(const thread_pool_t* tpool)
{
    return tpool->pool_size;
}