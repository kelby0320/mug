#ifndef LIST_H
#define LIST_H


#include <stdlib.h>


struct list_item {
	struct list_item *next;
	struct list_item *prev;
	void *data;
};


typedef struct {
	struct list_item *head;
	struct list_item *tail;
	ssize_t size;
} list_t;


typedef struct {
	list_t* list;
	struct list_item *cur;
} list_iter_t;


list_t* list_create();


void list_destroy(list_t*, void (*)(void*));


int list_size(const list_t* list);


void list_push_front(list_t*, void*);


void* list_pop_front(list_t*);


void list_push_back(list_t*, void*);


void* list_pop_back(list_t*);


void list_iter_ins_after(const list_iter_t*, void*);


void list_iter_ins_before(const list_iter_t*, void*);


list_iter_t list_iter(list_t*);


void list_iter_begin(list_iter_t*);


void list_iter_end(list_iter_t*);


void list_iter_next(list_iter_t*);


void list_iter_prev(list_iter_t*);


int list_iter_has_next(const list_iter_t*);


int list_iter_has_prev(const list_iter_t*);


void* list_iter_cur_item(const list_iter_t*);


#endif
