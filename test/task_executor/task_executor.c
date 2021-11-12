#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmocka.h>

#include "task_executor/i_task_executor.h"
#include "task_executor/task_executor.h"
#include "task_executor/work_item.h"
#include "types/result/mug_result.h"


int results[8];


static int setup(void **state)
{
    task_executor_t *executor = task_executor_alloc();
    task_executor_ctor(executor);
    i_task_executor_t *i_task_executor = (i_task_executor_t*)executor;
    *state = i_task_executor;
    return 0;
}


static int teardown(void **state)
{
    i_task_executor_t *i_task_executor = (i_task_executor_t*)*state;
    task_executor_t *executor = (task_executor_t*) i_task_executor;
    task_executor_dtor(executor);
    free(executor);
    return 0;
}


static mug_result_t* work_func(void *args)
{
    for (int i = 0; i < 8; i++) {
        results[i] = i;
    }

    mug_result_t *mug_result = mug_result_alloc();
    mug_result_ctor(mug_result);
    return mug_result;
}


static void submit_work(void **state)
{
    i_task_executor_t *i_task_executor = (i_task_executor_t*)*state;
    i_task_executor_submit(i_task_executor, work_func, NULL);

    sleep(1);

    for (int i = 0; i < 8; i++) {
        assert_true(results[i] == i);
    }
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(submit_work, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}