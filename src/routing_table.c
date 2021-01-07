#include <stdlib.h>
#include <string.h>


#include "routing_table.h"


struct route_item {
    char *route;
    route_handler_t handler;
};


struct routing_table {
    struct route_item *routes;
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
    /* Deallocate route in each route_item */
    int size = tbl->routes_size;

    for (int i = 0; i < size; i++) {
        free(tbl->routes[i].route);
    }

    free(tbl->routes);
    free(tbl);
}


void routing_table_add_route(routing_table_t *tbl, char *route, route_handler_t handler)
{
    struct route_item *new_item = NULL;

    if (tbl->routes_size == 0) {
	tbl->routes = (struct route_item*)malloc(sizeof(struct route_item));
	new_item = &tbl->routes[0];
    } else {
	int old_size = (int)tbl->routes_size;
	int new_size = old_size + 1;

	/* Allocate new memory to hold one more route item */
	struct route_item *new_routes = (struct route_item*)malloc(sizeof(struct route_item) + new_size);

	/* Copy existing route items */
	for (int i = 0; i < old_size; i++) {
	    new_routes[i] = tbl->routes[i];
	}

	/* Swap routes pointers */
	free(tbl->routes);
	tbl->routes = new_routes;
	tbl->routes_size = new_size;

	/* New item is append to the list */
	new_item = &tbl->routes[new_size - 1];
    }

    new_item->route = (char*)malloc(strlen(route) + 1);
    strcpy(new_item->route, route);
    new_item->handler = handler;
}


route_handler_t routing_table_find_route(routing_table_t *tbl, char *route)
{
    int size = tbl->routes_size;
    for (int i = 0; i < size; i++) {
	if (strcmp(tbl->routes[i].route, route) == 0) {
	    /* Route found! */
	    return tbl->routes[i].handler;
	}
    }

    /* Route not found */
    return NULL;
}
