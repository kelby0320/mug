#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H


#include "core/handler.h"


struct routing_table;


typedef struct routing_table routing_table_t;


routing_table_t* routing_table_alloc();
void routing_table_ctor(routing_table_t*);
void routing_table_dtor(routing_table_t*);
size_t routing_table_size(const routing_table_t*);
size_t routing_table_capacity(const routing_table_t*);
void routing_table_add_handler(routing_table_t*, const char*, mug_request_handler_t);
mug_request_handler_t routing_table_get_handler(const routing_table_t*, const char*);


#endif
