#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "event_source_map.h"
#include "event_source/event_source.h"


#define DEFAULT_MAP_SIZE 32
#define MAP_UPSIZE_THRESHOLD 0.75
#define MAP_DOWNSIZE_THRESHOLD 0.25


struct item {
    int key;
    event_source_t *value;
};


struct event_source_map {
    struct item* map;
    int size;
    int capacity;
};


static int is_item_set(const struct item*);
static int get_insert_location(const event_source_map_t*, int);
static int get_retrieve_location(const event_source_map_t*, int);
static void table_insert(event_source_map_t*, struct item);
static struct item* table_retrieve(const event_source_map_t*, int);
static void table_remove(event_source_map_t*, int);
static void table_resize(event_source_map_t*, int);
static void move_event_source_map(event_source_map_t*, event_source_map_t*);


event_source_map_t* event_source_map_alloc()
{
    return (event_source_map_t*)malloc(sizeof(event_source_map_t));
}


void event_source_map_ctor(event_source_map_t *event_source_map)
{
    event_source_map->map = (struct item*)calloc(sizeof(struct item), DEFAULT_MAP_SIZE);
    event_source_map->capacity = DEFAULT_MAP_SIZE;
    event_source_map->size = 0;
}


void event_source_map_ctor2(event_source_map_t *event_source_map, size_t size)
{
    event_source_map->map = (struct item*)calloc(sizeof(struct item), size);
    event_source_map->capacity = size;
    event_source_map->size = 0;
}


void event_source_map_dtor(event_source_map_t *event_source_map)
{
    for (int i = 0; i < event_source_map->capacity; i++) {
        if (is_item_set(&event_source_map->map[i])) {
            event_source_t *event_source = event_source_map->map[i].value;
            free(event_source);
        }
    }

    free(event_source_map->map);
}


int event_source_map_size(const event_source_map_t *event_source_map)
{
    return event_source_map->size;
}


int event_source_map_capacity(const event_source_map_t *event_source_map)
{
    return event_source_map->capacity;
}


void event_source_map_add_event_source(event_source_map_t *event_source_map, event_source_t *event_source)
{
    struct item item = {
        .key = event_source_get_fd(event_source),
        .value = event_source
    };

    table_insert(event_source_map, item);
}


event_source_t* event_source_map_find_event_source(const event_source_map_t *event_source_map, int fd)
{
    struct item *item = table_retrieve(event_source_map, fd);

    if (item == NULL) {
        return NULL;
    }

    return item->value;
}


void event_source_map_remove_event_source(event_source_map_t *event_source_map, int fd)
{
    table_remove(event_source_map, fd);
}


static int is_item_set(const struct item *item)
{
    if (item->value != NULL) {
        return 1;
    }

    return 0;
}


static int get_insert_location(const event_source_map_t *event_source_map, int fd)
{
    int loc = fd % event_source_map->capacity;

    while (is_item_set(&event_source_map->map[loc])) {
        loc = (loc + 1) % event_source_map->capacity;
    }

    return loc;
}


static int get_retrieve_location(const event_source_map_t *event_source_map, int fd)
{
    int loc = fd % event_source_map->capacity;
    int count = 0;

    while (event_source_map->map[loc].key != fd && count < event_source_map->capacity) {
        loc = (loc + 1) % event_source_map->capacity;
        count++;
    }

    if (count == event_source_map->capacity) {
        return -1;
    }

    return loc;
}


static void table_insert(event_source_map_t *event_source_map, struct item item)
{
    if (event_source_map->size + 1 > ceil(event_source_map->capacity * MAP_UPSIZE_THRESHOLD)) {
        int new_capacity = event_source_map->capacity * 2;
        table_resize(event_source_map, new_capacity);
    }

    int loc = get_insert_location(event_source_map, item.key);

    event_source_map->map[loc] = item;
    event_source_map->size++;
}


static struct item* table_retrieve(const event_source_map_t *event_source_map, int fd)
{
    int loc = get_retrieve_location(event_source_map, fd);

    if (loc == -1) {
        return NULL;
    }

    struct item *item = &event_source_map->map[loc];

    return item;
}


static void table_remove(event_source_map_t *event_source_map, int fd)
{
    int loc = get_retrieve_location(event_source_map, fd);

    if (loc != -1) {
        memset(&event_source_map->map[loc], 0, sizeof(struct item));
        event_source_map->size--;
    }

    if (event_source_map->size < floor(event_source_map->capacity * MAP_DOWNSIZE_THRESHOLD)) {
        int new_capacity = event_source_map->capacity / 2;
        table_resize(event_source_map, new_capacity);
    }
}


static void table_resize(event_source_map_t *event_source_map, int new_capacity)
{
    event_source_map_t *new_event_source_map = event_source_map_alloc();
    event_source_map_ctor2(new_event_source_map, new_capacity);


    for (int i = 0; i < event_source_map->capacity; i++) {
        if (is_item_set(&event_source_map->map[i])) {
            table_insert(new_event_source_map, event_source_map->map[i]);
        }
    }

    move_event_source_map(event_source_map, new_event_source_map);
}


static void move_event_source_map(event_source_map_t *dest, event_source_map_t *src)
{
    free(dest->map);

    dest->map = src->map;
    dest->capacity = src->capacity;
    dest->size = src->size;

    free(src);
}