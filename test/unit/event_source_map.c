#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <string.h>

#include "event_source_map.h"
#include "event_source/internal_event_source.h"


static void create_event_sources(event_source_t**, int, int);


static int setup(void **state)
{
    event_source_map_t *event_source_map = event_source_map_alloc();
    event_source_map_ctor(event_source_map);
    *state = event_source_map;
    return 0;
}


static int teardown(void **state)
{
    event_source_map_t *event_source_map = (event_source_map_t*)*state;
    event_source_map_dtor(event_source_map);
    free(event_source_map);
    return 0;
}


static void test_add_find_remove(void **state)
{
    event_source_map_t *event_source_map = (event_source_map_t*)*state;

    const int event_source_fd = 3;
    internal_event_source_t *internal_event_source = internal_event_source_alloc();
    internal_event_source_ctor(internal_event_source, event_source_fd);

    assert_true(event_source_map_size(event_source_map) == 0);

    event_source_map_add_event_source(event_source_map, (event_source_t*)internal_event_source);
    event_source_t *found_event_source = event_source_map_find_event_source(event_source_map, event_source_fd);

    assert_true(found_event_source == (event_source_t*)internal_event_source);
    assert_true(event_source_map_size(event_source_map) == 1);

    event_source_map_remove_event_source(event_source_map, event_source_fd);
    found_event_source = event_source_map_find_event_source(event_source_map, event_source_fd);

    assert_true(found_event_source == NULL);
    assert_true(event_source_map_size(event_source_map) == 0);
}


static void test_upsize_downsize(void **state)
{
    event_source_map_t *event_source_map = (event_source_map_t*)*state;

    int num_event_sources = 40;
    event_source_t **event_sources = (event_source_t**)malloc(sizeof(event_source_t*) * num_event_sources);
    create_event_sources(event_sources, num_event_sources, 3);
    
    assert_true(event_source_map_size(event_source_map) == 0);
    assert_true(event_source_map_capacity(event_source_map) == 32);

    for (int i = 0; i < num_event_sources; i++) {
        event_source_map_add_event_source(event_source_map, event_sources[i]);
    }

    assert_true(event_source_map_size(event_source_map) == num_event_sources);
    assert_true(event_source_map_capacity(event_source_map) == 64);

    for (int i = 0; i < num_event_sources; i++) {
        int fd = event_source_get_fd(event_sources[i]);
        event_source_t *found_event_source = event_source_map_find_event_source(event_source_map, fd);
        assert_true(found_event_source == event_sources[i]);
    }

    int remove_event_sources = 30;
    for (int i = 0; i < remove_event_sources; i++) {
        int fd = event_source_get_fd(event_sources[i]);
        event_source_map_remove_event_source(event_source_map, fd);
    }

    assert_true(event_source_map_size(event_source_map) == num_event_sources - remove_event_sources);
    assert_true(event_source_map_capacity(event_source_map) == 32);

    free(event_sources);
}


static void create_event_sources(event_source_t **event_sources, int len, int start_val)
{
    for (int i = 0; i < len; i++) {
        internal_event_source_t *internal_event_source = internal_event_source_alloc();
        internal_event_source_ctor(internal_event_source, start_val++);
        event_sources[i] = (event_source_t*)internal_event_source;
    }
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_add_find_remove, setup, teardown),
        cmocka_unit_test_setup_teardown(test_upsize_downsize, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}