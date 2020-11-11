#include <pthread.h>
#include <stdlib.h>


#include "thread_pool.h"


typedef struct {
    int fd;
    work_item_t* next;
} work_item_t;


struct thread_pool {
    work_item_t *queue;
    size_t queue_size;
    pthread_mutex_t queue_mux;
    pthread_t *threads;
    size_t threads_size;
};