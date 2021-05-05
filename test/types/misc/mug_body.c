#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <string.h>

#include "types/misc/mug_body.h"


const int TEST_INT_ARRAY[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
const int TEST_INT_ARRAY_LEN = sizeof(TEST_INT_ARRAY) / sizeof(int);
const char *TEST_STR = "This is a test string.";


static int setup(void **state)
{
    mug_body_t *body = mug_body_alloc();
    mug_body_ctor(body);
    *state = body;
    return 0;
}


static int setup_with_int_content(void **state)
{
    mug_body_t *body = mug_body_alloc();
    mug_body_ctor(body);
    
    mug_body_set_content(body, TEST_INT_ARRAY, sizeof(TEST_INT_ARRAY));

    *state = body;
    return 0;
}


static int setup_with_str_content(void **state)
{
    mug_body_t *body = mug_body_alloc();
    mug_body_ctor(body);

    mug_body_set_content_str(body, TEST_STR);

    *state = body;
    return 0;
}


static int teardown(void **state)
{
    mug_body_t *body = (mug_body_t*)*state;
    mug_body_dtor(body);
    free(body);
    return 0;
}


static void test_get_content(void **state)
{
    mug_body_t *body = (mug_body_t*)*state;

    int buf_size = mug_body_len(body);
    int buffer[buf_size];

    int copied = mug_body_as_bytes(body, buffer, buf_size);

    assert_true(copied == buf_size);
    assert_true(memcmp(TEST_INT_ARRAY, buffer, sizeof(TEST_INT_ARRAY)) == 0);
}


static void test_get_content_str(void **state)
{
    mug_body_t *body = (mug_body_t*)*state;

    int buf_size = mug_body_len(body) + 1;

    char buffer[buf_size];
    int copied = mug_body_as_c_str(body, buffer, buf_size);

    assert_true(copied == strlen(TEST_STR) + 1);
    assert_true(strcmp(TEST_STR, buffer) == 0);
}


static void test_append_get_content(void **state)
{
    mug_body_t *body = (mug_body_t*)*state;

    const int append_ints[] = {10, 11, 12, 13, 14, 15};
    const int append_ints_len = sizeof(append_ints) / sizeof(int);

    int expected_result_buf_len = TEST_INT_ARRAY_LEN + append_ints_len;
    int expected_result[expected_result_buf_len];

    memcpy(expected_result, TEST_INT_ARRAY, TEST_INT_ARRAY_LEN * sizeof(int));
    memcpy(expected_result + TEST_INT_ARRAY_LEN, append_ints, append_ints_len * sizeof(int));

    mug_body_append_content(body, append_ints, append_ints_len * sizeof(int));

    int buffer[expected_result_buf_len];
    int copied = mug_body_as_bytes(body, buffer, expected_result_buf_len * sizeof(int));

    assert_true(copied == expected_result_buf_len * sizeof(int));
    assert_true(memcmp(expected_result, buffer, expected_result_buf_len) == 0);
}


static void test_append_get_content_str(void **state)
{
    mug_body_t *body = (mug_body_t*)*state;

    char str_buf[128];
    mug_body_as_c_str(body, str_buf, 128);

    const char *append_str = " This string is appended.";
    const char null_terminator = '\0';
    const int append_str_len = strlen(append_str);
    const int test_str_len = strlen(TEST_STR);

    int expected_result_buf_len = test_str_len + append_str_len + 1;
    char expected_result[expected_result_buf_len];

    memcpy(expected_result, TEST_STR, test_str_len);
    memcpy(expected_result + test_str_len, append_str, append_str_len);
    memcpy(expected_result + test_str_len + append_str_len, &null_terminator, 1);

    mug_body_append_content_str(body, append_str);

    char buffer[expected_result_buf_len];
    int copied = mug_body_as_c_str(body, buffer, expected_result_buf_len);

    assert_true(copied == expected_result_buf_len);
    assert_true(strcmp(expected_result, buffer) == 0);
}


static void test_large_content(void **state)
{
    mug_body_t *body = (mug_body_t*)*state;

    const char *test_paragraph = 
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
        "Aenean id ante ultrices, vulputate purus quis, commodo lorem. "
        "Etiam at molestie magna. "
        "Cras cursus placerat felis vehicula tempus. "
        "Duis eleifend, dolor sed dignissim pretium, nibh ex condimentum leo, vel imperdiet ligula urna eget urna. "
        "Mauris augue mauris, feugiat id quam eu, faucibus pellentesque eros. "
        "Maecenas dapibus hendrerit convallis. "
        "Aliquam eu ligula sit amet nisl convallis cursus non vitae arcu. "
        "Suspendisse mauris quam, rutrum non mi eu, posuere tempus nunc. "
        "Cras nec quam ut nisi sodales ornare. "
        "Pellentesque dictum massa eu nulla lacinia interdum at quis lectus.";

    mug_body_set_content_str(body, test_paragraph);

    int buf_size = mug_body_len(body) + 1;

    char buffer[buf_size];
    int copied = mug_body_as_c_str(body, buffer, buf_size);

    assert_true(copied == strlen(test_paragraph) + 1);
    assert_true(strcmp(test_paragraph, buffer) == 0);
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_get_content, setup_with_int_content, teardown),
        cmocka_unit_test_setup_teardown(test_get_content_str, setup_with_str_content, teardown),
        cmocka_unit_test_setup_teardown(test_append_get_content, setup_with_int_content, teardown),
        cmocka_unit_test_setup_teardown(test_append_get_content_str, setup_with_str_content, teardown),
        cmocka_unit_test_setup_teardown(test_large_content, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}