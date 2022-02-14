#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include "task_executor/work_item.h"


struct work_queue;


typedef struct work_queue work_queue_t;


work_queue_t* work_queue_alloc();
void work_queue_ctor(work_queue_t*);
void work_queue_dtor(work_queue_t*);
void work_queue_enqueue(work_queue_t*, struct work_item*);
struct work_item* work_queue_dequeue(work_queue_t*);
int work_queue_size(work_queue_t*);


#endif