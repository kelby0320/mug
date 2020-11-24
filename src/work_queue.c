#include "work_queue.h"


struct work_queue {
    struct work_item* front;
    struct work_item* back;
    size_t size;
};


work_queue_t* work_queue_init()
{
    work_queue_t *queue = (work_queue_t*)malloc(sizeof(work_queue_t));

    queue->front = NULL;
    queue->back = NULL;
    queue->size = 0;
}


void work_queue_deinit(work_queue_t *queue)
{
    /* Free queue linked list */
    if (queue->size > 0) {
        struct work_item *p, *next;
        
        p = queue->front;

        while (p) {
            next = p->next;
            free(p);
            p = next;
        }
    }

    free(queue);
}


void work_queue_enqueue(work_queue_t *queue, struct work_item *item)
{
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
}


struct work_item* work_queue_dequeue(work_queue_t *queue)
{
    /* Return NULL when queue is empty */
    if (queue->size == 0) {
        return NULL;
    }

    struct work_item* item = queue->front;

    if (queue->front == queue->back) {
        /* Case when queue size is 1 */
        queue->front == NULL;
        queue->back == NULL;
    } else {
        /* Generic case */
        queue->front = queue->front->next;        
    }

    queue->size--;

    return item;
}


size_t work_queue_size(work_queue_t *queue)
{
    return queue->size;
}