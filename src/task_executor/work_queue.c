#include <stdlib.h>

#include "task_executor/work_queue.h"
#include "task_executor/work_item.h"


struct work_queue {
    struct work_item* front;
    struct work_item* back;
    int size;
};


static void free_list(struct work_item*);
static void enqueue_from_empty(work_queue_t*, struct work_item*);
static void enqueue_non_empty(work_queue_t*, struct work_item*);
static void remove_front(work_queue_t*);


work_queue_t* work_queue_alloc()
{
    return (work_queue_t*)malloc(sizeof(work_queue_t));
}


void work_queue_ctor(work_queue_t* queue)
{
    queue->front = NULL;
    queue->back = NULL;
    queue->size = 0;
}


void work_queue_dtor(work_queue_t *queue)
{
    if (queue->size > 0) {
        free_list(queue->front);
    }
}


void work_queue_enqueue(work_queue_t *queue, struct work_item *item)
{
    if (queue->front == NULL) {
        enqueue_from_empty(queue, item);
    } else {
        enqueue_non_empty(queue, item);
    }
}


struct work_item* work_queue_dequeue(work_queue_t *queue)
{
    if (queue->size == 0) {
        return NULL;
    }

    struct work_item* item = queue->front;

    remove_front(queue);

    return item;
}


int work_queue_size(work_queue_t *queue)
{
    return queue->size;
}


static void free_list(struct work_item *front)
{
    struct work_item *p, *next;
    p = front;

    while (p) {
        next = p->next;
        free(p);
        p = next;
    }
}


static void enqueue_from_empty(work_queue_t *queue, struct work_item *item)
{
    queue->front = item;
    queue->back = item;
    queue->size = 1;
}


static void enqueue_non_empty(work_queue_t *queue, struct work_item *item)
{
    queue->back->next = item;
    item->next = NULL;
    queue->back = item;
    queue->size++;
}


static void remove_front(work_queue_t *queue)
{
    /* Case when queue size is 1 */
    if (queue->front == queue->back) {
        queue->front = NULL;
        queue->back = NULL;
    } else { /* Generic case */
        queue->front = queue->front->next;        
    }

    queue->size--;
}
