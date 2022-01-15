#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <stdio.h>

#include "routing_table.h"
#include "core/handler.h"


static mug_result_t*  dummy_handler(mug_request_invocation_t *arg)
{
    return NULL;
}


static int setup(void **state)
{
    routing_table_t *routing_table = routing_table_alloc();
    routing_table_ctor(routing_table);

    *state = routing_table;

    return 0;
}


static int teardown(void **state)
{
    routing_table_t *routing_table = (routing_table_t*)*state;
    routing_table_dtor(routing_table);
    free(routing_table);

    return 0;
}


static void test_add_get_handler(void **state)
{
    routing_table_t *routing_table = (routing_table_t*)*state;
    const char *route = "/dummy/route";

    routing_table_add_handler(routing_table, route, dummy_handler);

    mug_request_handler_t handler = routing_table_get_handler(routing_table, route);

    assert_true(handler == dummy_handler);
}


static void test_resize_table(void ** state)
{
    routing_table_t *routing_table = (routing_table_t*)*state;
    const char *route_base = "/dummy/route";
    int capacity = (int)routing_table_capacity(routing_table);

    for (int i = 0; i <= capacity; i++) {
        char buf[32];
        sprintf(buf, "%s%d", route_base, i);
        routing_table_add_handler(routing_table, buf, dummy_handler);
    }

    const char *search_str = "/dummy/route32";
    mug_request_handler_t handler = routing_table_get_handler(routing_table, search_str);

    assert_true(handler == dummy_handler);
}



int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_add_get_handler, setup, teardown),
        cmocka_unit_test_setup_teardown(test_resize_table, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}