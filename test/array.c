#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <cmocka.h>


#include "array.h"


static int setup(void **state)
{
	array_t* array = array_create(sizeof(int), 10);
	*state = array;
	return 0;
}


static int teardown(void **state)
{
	array_t* array = *state;
	array_destroy(array);
	return 0;
}


static void verify_initial(void **state)
{
	array_t* array = *state;

	int len = array_size(array);

	for (int i = 0; i < len; i++) {
		int* item = array_get(array, i);
		assert_true(*item == 0);
	}
}


static void set_array(void **state)
{
	array_t* array = *state;

	int len = array_size(array);

	for (int i = 0; i < len; i++) {
		int ret = array_set(array, i, &i);
		assert_true(ret == i);
	}

	for (int i = 0; i < len; i++) {
		int* item = array_get(array, i);
		assert_true(*item == i);
	}
}


static void get_invalid(void **state)
{
	array_t* array = *state;

	int* item = array_get(array, 11);

	assert_true(item == NULL);
}


static void set_invalid(void **state)
{
	array_t* array = *state;

	int len = array_size(array);

	int ret = array_set(array, 11, &len);
	assert_true(ret == -11);

	for (int i = 0; i < len; i++) {
		int* item = array_get(array, i);
		assert_true(*item == 0);
	}
}


static void double_size(void **state)
{
	array_t* array = *state;

	int len = array_size(array);

	for (int i = 0; i < len; i++) {
		array_set(array, i, &i);
	}

	int new_len = len * 2;
	int ret = array_resize(array, new_len);
	assert_true(ret == new_len);

	for (int i = 0; i < new_len; i++) {
		int* item = array_get(array, i);
		if (i < len)
			assert_true(*item == i);
		else
			assert_true(*item == 0);
	}
}


static void half_size(void **state)
{
	array_t* array = *state;

	int len = array_size(array);

	for (int i = 0; i < len; i++) {
		array_set(array, i, &i);
	}

	int new_len = len / 2;
	int ret = array_resize(array, new_len);
	assert_true(ret == new_len);

	for (int i = 0; i < new_len; i++) {
		int* item = array_get(array, i);
		assert_true(*item == i);
	}
}


int main()
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(verify_initial, setup, teardown),
		cmocka_unit_test_setup_teardown(set_array, setup, teardown),
		cmocka_unit_test_setup_teardown(get_invalid, setup, teardown),
		cmocka_unit_test_setup_teardown(set_invalid, setup, teardown),
		cmocka_unit_test_setup_teardown(double_size, setup, teardown),
		cmocka_unit_test_setup_teardown(half_size, setup, teardown),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
