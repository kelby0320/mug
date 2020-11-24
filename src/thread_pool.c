#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


#include "thread_pool.h"


typedef struct {
    void *arg;
    int (*func)(void*);
    work_item_t* next;
} work_item_t;


typedef struct {
    work_item_t* front;
    work_item_t* back;
    size_t size;
    pthread_mutex_t mtx;
} work_queue_t;


struct thread_pool {
    work_queue_t *queue;
    pthread_t *pool;
    size_t pool_size;
};


static work_queue_t* work_queue_init();
static void work_queue_deinit(work_queue_t*);
static void work_queue_enqueue(work_queue_t*, work_item_t*);
static work_item_t* work_queue_dequeue(work_queue_t*);
static void fill_pool(pthread_t*, size_t, void* (*)(void*), void*);
static void* pool_func(void*);


static work_queue_t* work_queue_init()
{
    work_queue_t *queue = (work_queue_t*)malloc(sizeof(work_queue_t));

    queue->front = NULL;
    queue->back = NULL;
    queue->size = 0;

    pthread_mutexattr_t mtx_attr;

    pthread_mutexattr_init(&mtx_attr);
    pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_NORMAL);

    pthread_mutex_init(&queue->mtx, &mtx_attr);

    pthread_mutexattr_destroy(&mtx_attr);
}


static void work_queue_deinit(work_queue_t *queue)
{
    /* Free queue linked list */
    if (queue->size > 0) {
        work_item_t *p, *next;
        
        p = queue->front;

        while (p) {
            next = p->next;
            free(p);
            p = next;
        }
    }

    pthread_mutex_destroy(&queue->mtx);

    free(queue);
}


static void work_queue_enqueue(work_queue_t *queue, work_item_t *item)
{
    pthread_mutex_lock(&queue->mtx);

    if (queue->front == NULL) {
        queue->front = item;
        queue->back = item;
        queue->size = 1;
    } else {
        queue->back->next = item;
        item->next = NULL;
        queue->back = item;
        queue->size++;
    }

    pthread_mutex_unlock(&queue->mtx);
}


static work_item_t* work_queue_dequeue(work_queue_t *queue)
{
    /* Return NULL when queue is empty */
    if (queue->size == 0) {
        return NULL;
    }

    pthread_mutex_lock(&queue->mtx);

    work_item_t* item = queue->front;

    if (queue->front == queue->back) {
        /* Case when queue size is 1 */
        queue->front == NULL;
        queue->back == NULL;
    } else {
        /* Generic case */
        queue->front = queue->front->next;        
    }

    queue->size--;

    pthread_mutex_unlock(&queue->mtx);

    return item;
}


static void fill_pool(pthread_t* pool, size_t pool_size, void* (*func)(void*), void* arg)
{
    for (int i = 0; i < pool_size; i++) {
        pthread_create(pool[0], NULL, func, arg);
    }
}


static void* pool_func(void* arg)
{
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

    fill_pool(tpool->pool, tpool->pool_size, pool_func, NULL);
}


void thread_pool_deinit(thread_pool_t* tpool)
{
    work_queue_deinit(tpool->queue);

    free(tpool->pool);

    free(tpool);
}


int thread_pool_size(const thread_pool_t* tpool)
{
    return tpool->pool_size;
}