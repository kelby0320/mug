#include <stdlib.h>


#include "routing_table.h"


struct route_item {
    char *route;
    void (*handler)(void*, void*);
};


struct routing_table {
    struct route_item **routes;
    size_t routes_size;
};


routing_table_t* routing_table_init()
{
    routing_table_t* tbl = (routing_table_t*)malloc(sizeof(routing_table_t));

    tbl->routes = NULL;
    tbl->routes_size = 0;

    return tbl;
}


void routing_table_deinit(routing_table_t *tbl)
{
    int size = tbl->routes_size;

    for (int i = 0; i < size; i++) {
        free(tbl->routes[i]);
    }

    free(tbl);
}