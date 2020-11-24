#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include <stdlib.h>


struct work_queue;


typedef struct work_queue work_queue_t;


struct work_item {
    void *arg;
    int (*func)(void*);
    struct work_item* next;
};


work_queue_t* work_queue_init();


void work_queue_deinit(work_queue_t*);


void work_queue_enqueue(work_queue_t*, struct work_item*);


struct work_item* work_queue_dequeue(work_queue_t*);


size_t work_queue_size(work_queue_t*);


#endif