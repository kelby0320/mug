#ifndef LIST_H
#define LIST_H


#include <stdlib.h>


struct list_item {
	struct list_item *next;
	struct list_item *prev;
	void *data;
};


struct list {
	struct list_item *head;
	struct list_item *tail;
	ssize_t size;
};


struct list_iter {
	struct list* list;
	struct list_item *cur;
};


struct list* list_create();


void list_destroy(struct list*, void (*)(void*));


int list_size(const struct list* list);


void list_push_front(struct list*, void*);


void* list_pop_front(struct list*);


void list_push_back(struct list*, void*);


void* list_pop_back(struct list*);


void list_iter_ins_after(const struct list_iter*, void*);


void list_iter_ins_before(const struct list_iter*, void*);


struct list_iter list_iter(struct list*);


void list_iter_begin(struct list_iter*);


void list_iter_end(struct list_iter*);


void list_iter_next(struct list_iter*);


void list_iter_prev(struct list_iter*);


int list_iter_has_next(const struct list_iter*);


int list_iter_has_prev(const struct list_iter*);


void* list_iter_cur_item(const struct list_iter*);


#endif
