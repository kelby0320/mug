#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <string.h>
#include <stdio.h>

#include "types/misc/mug_save_bag.h"


struct test_struct {
    int i;
    double d;
    char ch;
};


static int setup(void **state)
{
    mug_save_bag_t *save_bag = mug_save_bag_alloc();
    mug_save_bag_ctor(save_bag);
    *state = save_bag;
    return 0;
}


static int teardown(void **state)
{
    mug_save_bag_t *save_bag = (mug_save_bag_t*)*state;
    mug_save_bag_dtor(save_bag);
    free(save_bag);
    return 0;
}


static void test_put_get_int(void **state)
{
    mug_save_bag_t *save_bag = (mug_save_bag_t*)*state;

    char *test_key = "test";
    int test_value = 5;

    mug_save_bag_put_int(save_bag, test_key, test_value);

    int returned_value;
    int success = mug_save_bag_get_int(save_bag, test_key, &returned_value);

    assert_true(success == 0);
    assert_true(returned_value == test_value);
}


static void test_put_get_dbl(void **state)
{
    mug_save_bag_t *save_bag = (mug_save_bag_t*)*state;

    char *test_key = "test";
    double test_value = 5.0;

    mug_save_bag_put_dbl(save_bag, test_key, test_value);

    double returned_value;
    int success = mug_save_bag_get_dbl(save_bag, test_key, &returned_value);

    assert_true(success == 0);
    assert_true(returned_value == test_value);
}


static void test_put_get_ptr(void **state)
{
    mug_save_bag_t *save_bag = (mug_save_bag_t*)*state;

    char *test_key = "test";
    int test_value = 5;
    int *test_ptr = &test_value;

    mug_save_bag_put_ptr(save_bag, test_key, test_ptr);

    void *returned_void_ptr;
    int success = mug_save_bag_get_ptr(save_bag, test_key, &returned_void_ptr);
    int *returned_pointer = (int*)returned_void_ptr;

    assert_true(success == 0);
    assert_true(returned_pointer == test_ptr);
    assert_true(*returned_pointer == test_value);
}


static void test_put_get_str(void **state)
{
    mug_save_bag_t *save_bag = (mug_save_bag_t*)*state;

    char *test_key = "test";
    char *test_value = "value";

    mug_save_bag_put_str(save_bag, test_key, test_value);

    char returned_value[16];
    int success = mug_save_bag_get_str(save_bag, test_key, returned_value);

    assert_true(success == 0);
    assert_true(strcmp(returned_value, test_value) == 0);
}


static void test_put_get_struct(void **state)
{
    mug_save_bag_t *save_bag = (mug_save_bag_t*)*state;

    char *test_key = "test";
    struct test_struct test_value = {
        .i = 5,
        .d = 5.0,
        .ch = 'a'
    };

    mug_save_bag_put_struct(save_bag, test_key, &test_value, sizeof(struct test_struct));

    struct test_struct return_value;
    int success = mug_save_bag_get_struct(save_bag, test_key, &return_value, sizeof(struct test_struct));

    assert_true(success == 0);
    assert_true(return_value.i == test_value.i);
    assert_true(return_value.d == test_value.d);
    assert_true(return_value.ch == test_value.ch);
}


static void test_resize(void **state)
{
    const int num_items = 17;
    char *keys[] = {
        "item_0",
        "item_1",
        "item_2",
        "item_3",
        "item_4",
        "item_5",
        "item_6",
        "item_7",
        "item_8",
        "item_9",
        "item_10",
        "item_11",
        "item_12",
        "item_13",
        "item_14",
        "item_15",
        "item_16",
    };

    mug_save_bag_t *save_bag = (mug_save_bag_t*)*state;

    for (int i = 0; i < num_items; i++) {
        mug_save_bag_put_int(save_bag, keys[i], i);
    }

    assert_true(mug_save_bag_size(save_bag) == num_items);
    assert_true(mug_save_bag_capacity(save_bag) == 32);

    for (int i = 0; i < num_items; i++) {
        int value;
        int success = mug_save_bag_get_int(save_bag, keys[i], &value);

        assert_true(success == 0);
        assert_true(value == i);
    }
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_put_get_int, setup, teardown),
        cmocka_unit_test_setup_teardown(test_put_get_dbl, setup, teardown),
        cmocka_unit_test_setup_teardown(test_put_get_ptr, setup, teardown),
        cmocka_unit_test_setup_teardown(test_put_get_str, setup, teardown),
        cmocka_unit_test_setup_teardown(test_put_get_struct, setup, teardown),
        cmocka_unit_test_setup_teardown(test_resize, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}