#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H


#include "mug.h"

routing_table_t* routing_table_init();


void routing_table_deinit(routing_table_t*);


route_handler_t routing_table_find_route(routing_table_t*, char*);


#endif
