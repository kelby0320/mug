#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>

#include "event_source/http_response_event_source.h"
#include "event_source/event_source.h"
#include "event/http_response_event.h"
#include "event/external_event.h"
#include "core/handler.h"
#include "types/result/mug_result.h"
#include "types/invocation/mug_continuation_invocation.h"


#define TEST_FILE_DESCRIPTOR_NUMBER 1234


mug_result_t* test_continuation_handler(mug_continuation_invocation_t *continuation_invocation)
{
    return NULL;
}


static int setup(void **state)
{
    mug_continuation_handler_t continuation_handler = test_continuation_handler;

    http_response_event_source_t *http_response_event_source = http_response_event_source_alloc();
    http_response_event_source_ctor(http_response_event_source, continuation_handler, TEST_FILE_DESCRIPTOR_NUMBER);

    *state = http_response_event_source;
    return 0;
}


static int teardown(void **state)
{
    // No teardown required
    return 0;
}


static void test_get_fd(void **state)
{
    event_source_t *event_source = (event_source_t*)*state;

    int fd = event_source_get_fd(event_source);

    assert_true(fd == TEST_FILE_DESCRIPTOR_NUMBER);
}


static void test_get_event(void **state)
{
    event_source_t *event_source = (event_source_t*)*state;

    http_response_event_t *http_response_event = (http_response_event_t*)event_source_get_event(event_source);

    mug_continuation_handler_t continuation_handler = http_response_event_continuation_handler(http_response_event);
    int fd = external_event_fd((external_event_t*)http_response_event);

    assert_true(continuation_handler == test_continuation_handler);
    assert_true(fd == TEST_FILE_DESCRIPTOR_NUMBER);
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_get_fd, setup, teardown),
        cmocka_unit_test_setup_teardown(test_get_event, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}