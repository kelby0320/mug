#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <cmocka.h>


#include "list.h"


/* 
 * Create an empty list for use in a test.
 */
static int setup_empty(void **state)
{
	list_t* list = list_create();
	*state = list;
	return 0;
}


/* 
 * Destroy the empty list used by the test.
 * 
 * Note: Dynamically allocated memory referenced by nodes in the list will
 * note be freed.  The list should either be empty at this point or not
 * contain any references to dynamically allocated memory.
 */
static int teardown_empty(void **state)
{
	list_t* list = *state;
	list_destroy(list, NULL);
	return 0;
}


/* 
 * Create a list filled with items for use in a test.
 */
static int setup_full(void **state)
{
	list_t* list = list_create();

	for (int i = 0; i < 3; i++) {
		char *str = malloc(sizeof(char) * 8);
		sprintf(str, "test%d", i);
		list_push_front(list, str);
	}

	*state = list;
	return 0;	
}


/* 
 * Helper function to free list item data allocated in setup_full.
 */
static void free_item_data(void *data)
{
	free(data);
}


/* 
 * Destroy the list use by a test.
 *
 * Note: This function expects each item in the list to hold a 
 * refernce to dynamically allocated memory that can be freed using
 * free_item_data.
 */
static int teardown_full(void **state)
{
	list_t* list = *state;
	list_destroy(list, free_item_data);
	return 0;
}


/* 
 * Push and pop a test value on the front of the list.
 */
static void push_pop_front(void **state)
{
	list_t* list = *state;
	
	char *str = "test";
	list_push_front(list, str);
	
	assert_true(list_size(list) == 1);

	str = NULL;
	str = list_pop_front(list);

	assert_true(list_size(list) == 0);
	assert_string_equal(str, "test");

	assert_ptr_equal(list->head, NULL);
	assert_ptr_equal(list->tail, NULL);
}


/* 
 * Push and pop 3 test values on the front of the list.
 */
static void push_pop_front_3(void **state)
{
	list_t* list = *state;

	char strs[3][8] = {
		"test1",
		"test2",
		"test3",
	};

	for (int i = 0; i < 3; i++) {
		list_push_front(list, strs[i]);
	}
	assert_true(list_size(list) == 3);

	char *str = list_pop_front(list);
	assert_string_equal(str, "test3");
	assert_true(list_size(list) == 2);

	str = list_pop_front(list);
	assert_string_equal(str, "test2");
	assert_true(list_size(list) == 1);

	str = list_pop_front(list);
	assert_string_equal(str, "test1");
	assert_true(list_size(list) == 0);

	assert_ptr_equal(list->head, NULL);
	assert_ptr_equal(list->tail, NULL);
}


/*
 * Push and pop a test value on the back of the list.
 */
static void push_pop_back(void **state)
{
	list_t* list = *state;
	
	char *str = "test";
	list_push_back(list, str);
	
	assert_true(list_size(list) == 1);

	str = NULL;
	str = list_pop_back(list);

	assert_true(list_size(list) == 0);
	assert_string_equal(str, "test");

	assert_ptr_equal(list->head, NULL);
	assert_ptr_equal(list->tail, NULL);
}


/* 
 * Push and pop 3 test values on the back of the list.
 */
static void push_pop_back_3(void **state)
{
	list_t* list = *state;

	char strs[3][8] = {
		"test1",
		"test2",
		"test3",
	};

	for (int i = 0; i < 3; i++) {
		list_push_back(list, strs[i]);
	}
	assert_true(list_size(list) == 3);

	char *str = list_pop_back(list);
	assert_string_equal(str, "test3");
	assert_true(list_size(list) == 2);

	str = list_pop_back(list);
	assert_string_equal(str, "test2");
	assert_true(list_size(list) == 1);

	str = list_pop_back(list);
	assert_string_equal(str, "test1");
	assert_true(list_size(list) == 0);

	assert_ptr_equal(list->head, NULL);
	assert_ptr_equal(list->tail, NULL);
}


/*
 * Pop from the front of an empty list.
 */
static void pop_front_empty_list(void **state)
{
	list_t* list = *state;

	char *val = list_pop_front(list);

	assert_ptr_equal(val, NULL);
	assert_true(list_size(list) == 0);
}


/* 
 * Pop from the back of an empty list.
 */
static void pop_back_empty_list(void **state)
{
	list_t* list = *state;

	char *val = list_pop_back(list);

	assert_ptr_equal(val, NULL);
	assert_true(list_size(list) == 0);
}


/* 
 * Iterate forward through a list.
 */
static void iterate_list_forward(void **state)
{
	list_t* list = *state;
	list_iter_t iter = list_iter(list);
	char str[8];
	char *data;

	assert_true(list_iter_has_next(&iter));

	for (int i = 2; list_iter_has_next(&iter); i--, list_iter_next(&iter)) {
		sprintf(str, "test%d", i);
		data = list_iter_cur_item(&iter);		
		assert_string_equal(data, str);
	}

	assert_true(list_size(list) == 3);
}


/* 
 * Iterate backward through a list.
 */
static void iterate_list_backward(void **state)
{
	list_t* list = *state;
	list_iter_t iter = list_iter(list);
	char str[8];
	char *data;

	list_iter_end(&iter);

	assert_true(list_iter_has_prev(&iter));

	for (int i = 0; list_iter_has_prev(&iter); i++, list_iter_prev(&iter)) {
		sprintf(str, "test%d", i);
		data = list_iter_cur_item(&iter);		
		assert_string_equal(data, str);
	}

	assert_true(list_size(list) == 3);
}


/* 
 * Seek to the beginning and end of a list.
 */
static void iterate_begin_end(void **state)
{
	list_t* list = *state;
	list_iter_t iter = list_iter(list);
	char *data;

	/* Assert iter is at head */
	data = list_iter_cur_item(&iter);
	assert_string_equal(data, "test2");

	/* Assert iter is at tail */
	list_iter_end(&iter);
	data = list_iter_cur_item(&iter);
	assert_string_equal(data, "test0");

	/* Assert iter is at head */
	list_iter_begin(&iter);
	data = list_iter_cur_item(&iter);
	assert_string_equal(data, "test2");		
	
}


int main()
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(push_pop_front, setup_empty, teardown_empty),
		cmocka_unit_test_setup_teardown(push_pop_front_3, setup_empty, teardown_empty),
		cmocka_unit_test_setup_teardown(push_pop_back, setup_empty, teardown_empty),
		cmocka_unit_test_setup_teardown(push_pop_back_3, setup_empty, teardown_empty),
		cmocka_unit_test_setup_teardown(pop_front_empty_list, setup_empty, teardown_empty),
		cmocka_unit_test_setup_teardown(pop_back_empty_list, setup_empty, teardown_empty),
		cmocka_unit_test_setup_teardown(iterate_list_forward, setup_full, teardown_full),
		cmocka_unit_test_setup_teardown(iterate_list_backward, setup_full, teardown_full),
		cmocka_unit_test_setup_teardown(iterate_begin_end, setup_full, teardown_full),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
