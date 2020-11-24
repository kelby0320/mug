#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>


#include "work_queue.h"


static int setup(void **state)
{
    work_queue_t *queue = work_queue_init();
    *state = queue;
    return 0;
}


static int teardown(void **state)
{
    work_queue_t *queue = (work_queue_t*)*state;
    work_queue_deinit(queue);
    return 0;
}


static void enqueue_dequeue(void **state)
{
    work_queue_t *queue = (work_queue_t*)*state;

    struct work_item *item = (struct work_item*)malloc(sizeof(struct work_item));

    item->func = 0xDEADBEEF;
    item->arg = 0xDEADBEEF;

    work_queue_enqueue(queue, item);

    assert_true(work_queue_size(queue) == 1);

    struct work_item *ret = work_queue_dequeue(queue);

    assert_true(ret->func == item->func);
    assert_true(ret->arg == item->arg);
    assert_true(work_queue_size(queue) == 0);

    free(item);  // Only free item. ret == item
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(enqueue_dequeue, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}