#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H


struct routing_table;


typedef struct routing_table routing_table_t;


routing_table_t* routing_table_init();


void routing_table_deinit(routing_table_t*);


#endif