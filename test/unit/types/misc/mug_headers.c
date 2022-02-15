#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <string.h>

#include "types/misc/mug_headers.h"


static int setup(void **state)
{
    mug_headers_t *headers = mug_headers_alloc();
    mug_headers_ctor(headers);
    *state = headers;
    return 0;
}


static int teardown(void **state)
{
    mug_headers_t *headers = (mug_headers_t*)*state;
    mug_headers_dtor(headers);
    free(headers);
    return 0;
}

static void test_put_get_header(void **state)
{
    mug_headers_t *headers = (mug_headers_t*)*state;

    assert_true(mug_headers_len(headers) == 0);

    const char *header_str = "Content Type: application/json";
    mug_headers_append(headers, header_str);

    assert_true(mug_headers_len(headers) == 1);

    char buf[128];
    mug_headers_get(headers, 0, buf);

    assert_true(strcmp(header_str, buf) == 0);
}


static void test_expand_headers(void **state)
{
    mug_headers_t *headers = (mug_headers_t*)*state;

    int num_headers = 17;

    char *test_headers[] = {
        "Test Header: 0",
        "Test Header: 1",
        "Test Header: 2",
        "Test Header: 3",
        "Test Header: 4",
        "Test Header: 5",
        "Test Header: 6",
        "Test Header: 7",
        "Test Header: 8",
        "Test Header: 9",
        "Test Header: 10",
        "Test Header: 11",
        "Test Header: 12",
        "Test Header: 13",
        "Test Header: 14",
        "Test Header: 15",
        "Test Header: 16",
        "Test Header: 17",
    };

    for (int i = 0; i < num_headers; i++) {
        mug_headers_append(headers, test_headers[i]);
    }

    assert_true(mug_headers_len(headers) == num_headers);

    for (int i = 0; i < num_headers; i++) {
        char buf[128];
        mug_headers_get(headers, i, buf);
        assert_true(strcmp(buf, test_headers[i]) == 0);
    }
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_put_get_header, setup, teardown),
        cmocka_unit_test_setup_teardown(test_expand_headers, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}