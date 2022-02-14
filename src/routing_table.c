#include <stdlib.h>
#include <string.h>

#include "routing_table.h"
#include "core/handler.h"


#define DEFAULT_CAPACITY 32


struct item {
    char *uri;
    mug_request_handler_t handler;
};


struct routing_table {
    struct item *table;
    size_t size;
    size_t capacity;
};


static void expand_table_size(routing_table_t*);
static void set_item(struct item*, const char*, mug_request_handler_t);


routing_table_t *routing_table_alloc()
{
    return (routing_table_t*)malloc(sizeof(routing_table_t));
}


void routing_table_ctor(routing_table_t *routing_table)
{
    routing_table->table = (struct item*)malloc(sizeof(struct item) * DEFAULT_CAPACITY);
    routing_table->size = 0;
    routing_table->capacity = DEFAULT_CAPACITY;
}


void routing_table_dtor(routing_table_t *routing_table)
{
    for (int i = 0; i < routing_table->size; i++) {
        free(routing_table->table[i].uri);
    }

    free(routing_table->table);
}


size_t routing_table_size(const routing_table_t *routing_table)
{
    return routing_table->size;
}


size_t routing_table_capacity(const routing_table_t *routing_table)
{
    return routing_table->capacity;
}


void routing_table_add_handler(routing_table_t *routing_table, const char *route, mug_request_handler_t handler)
{
    if (routing_table->size == routing_table->capacity) {
        expand_table_size(routing_table);
    }

    struct item *item = &routing_table->table[routing_table->size];
    set_item(item, route, handler);

    routing_table->size++;
}


mug_request_handler_t routing_table_get_handler(const routing_table_t *routing_table, const char *route)
{
    for (int i = 0; i < routing_table->size; i++) {
        const char *item_route = routing_table->table[i].uri;
        if (strcmp(item_route, route) == 0) {
            return routing_table->table[i].handler;
        }
    }

    return NULL;
}


static void expand_table_size(routing_table_t *routing_table)
{
    size_t new_capacity = routing_table->capacity * 2;
    struct item *new_table = (struct item*)malloc(sizeof(struct item) * new_capacity);

    for (int i = 0; i < routing_table->size; i++) {
        new_table[i] = routing_table->table[i];
    }

    free(routing_table->table);

    routing_table->table = new_table;
    routing_table->capacity = new_capacity;
}


static void set_item(struct item *item, const char *route, mug_request_handler_t handler)
{
    item->uri = (char*)malloc(sizeof(char) * strlen(route) + 1);
    strcpy(item->uri, route);
    item->handler = handler;
}