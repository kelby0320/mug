#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <string.h>

#include "event/event.h"
#include "event/new_connection_event.h"
#include "event/event_type.h"
#include "event_handler/event_handler.h"
#include "event_dispatcher.h"


#define GLOBAL_EVENT_LOG_SIZE 256


char global_event_log[GLOBAL_EVENT_LOG_SIZE];


void dummy_event_handler(void *arg)
{
    strcpy(global_event_log, "Run dummy event handler.");
}


static int setup(void **state)
{
    event_dispatcher_t *dispatcher = event_dispatcher_alloc();
    event_dispatcher_ctor(dispatcher);
    *state = dispatcher;

    memset(global_event_log, 0, GLOBAL_EVENT_LOG_SIZE * sizeof(char));

    return 0;
}


static int teardown(void **state)
{
    event_dispatcher_t *dispatcher = (event_dispatcher_t*)*state;
    free(dispatcher);
    return 0;
}


static void test_register_and_handle(void **state)
{
    event_dispatcher_t *dispatcher = (event_dispatcher_t*)*state;

    int success = event_dispatcher_register_handler(dispatcher, NEW_CONNECTION_EVENT, dummy_event_handler);
    assert_true(success == 0);

    new_connection_event_t *event = new_connection_event_alloc();
    new_connection_event_ctor(event, 3);

    success = event_dispatcher_handle_event(dispatcher, (event_t*)event);

    assert_true(success == 0);
    assert_true(strcmp(global_event_log, "Run dummy event handler.") == 0);
}


static void test_register_twice(void **state)
{
    event_dispatcher_t *dispatcher = (event_dispatcher_t*)*state;

    int success = event_dispatcher_register_handler(dispatcher, NEW_CONNECTION_EVENT, dummy_event_handler);
    assert_true(success == 0);

    success = event_dispatcher_register_handler(dispatcher, NEW_CONNECTION_EVENT, dummy_event_handler);
    assert_true(success == -1);
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_register_and_handle, setup, teardown),
        cmocka_unit_test_setup_teardown(test_register_twice, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}