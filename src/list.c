#include <stdlib.h>


#include "list.h"


struct list* list_create()
{
	struct list* list = (struct list*)malloc(sizeof(struct list));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return list;
}


void list_destroy(struct list* list, void (*dtor)(void*))
{
	struct list_item *p, *next;
	p = list->head;
	
	while (p != NULL) {
		next = p->next;
		if (dtor != NULL) {
			dtor(p->data);
		}
		free(p);
		p = next;
	}

	free(list);
}


int list_size(const struct list* list)
{
	return list->size;
}


static struct list_item* create_list_item(void* data)
{
	struct list_item *item = (struct list_item*)malloc(sizeof(struct list_item));
	item->next = NULL;
	item->prev = NULL;
	item->data = data;
	return item;
}


void list_push_front(struct list* list, void* data)
{
	struct list_item *item = create_list_item(data);
	
	if (list->head == NULL) {
		/* Case: Empty List */
		list->head = item;
		list->tail = item;
	}
	else {
		struct list_item *tmp = list->head;
		list->head = item;
		item->next = tmp;
		tmp->prev = item;
	}

	list->size++;
}


void* list_pop_front(struct list* list)
{
	/* Don't pop from an empty list */
	if (list->head == NULL)
		return NULL;

	void *data = list->head->data;
	struct list_item *tmp = list->head->next;
	free(list->head);
	list->head = tmp;

	/* Is the list empty after pop? */
	if (list->head == NULL)
		list->tail = NULL;

	list->size--;

	return data;
}


void list_push_back(struct list* list, void* data)
{
	struct list_item *item = create_list_item(data);
	
	if (list->tail == NULL) {
		/* Case: Empty List */
		list->tail = item;
		list->head = item;
	}
	else {
		struct list_item *tmp = list->tail;
		list->tail = item;
		item->prev = tmp;
		tmp->next = item;
	}

	list->size++;	
}


void* list_pop_back(struct list* list)
{
	/* Don't dequeue from an empty list? */
	if (list->tail == NULL)
		return NULL;

	void *data = list->tail->data;
	struct list_item *tmp = list->tail->prev;
	free(list->tail);
	list->tail = tmp;

	/* Is the list empty after dequeue? */
	if (list->tail == NULL)
		list->head = NULL;

	list->size--;

	return data;	
}


void list_iter_ins_after(const struct list_iter* iter, void* data)
{
	struct list_item *item = create_list_item(data);
	
	if (iter->cur == NULL) {
		/* Case: Empty List */
		iter->list->head = item;
		iter->list->tail = item;
		iter->list->size++;
	}
	else {
		item->prev = iter->cur;
		item->next = iter->cur->next;
		iter->cur->next = item;

		/* Reset tail if we've added to the end */
		if (item->next == NULL)
			iter->list->tail = item;
		
		iter->list->size++;
	}
}


void list_iter_ins_before(const struct list_iter* iter, void* data)
{
	struct list_item *item = create_list_item(data);
	
	if (iter->cur == NULL) {
		/* Case: Empty List */
		iter->list->head = item;
		iter->list->tail = item;
		iter->list->size++;
	}
	else {
		item->next = iter->cur;
		item->prev = iter->cur->prev;
		iter->cur->prev = item;

		/* Reset head if we've added to the front */
		if (item->prev == NULL)
			iter->list->head = item;
		
		iter->list->size++;
	}
}


struct list_iter list_iter(struct list* list)
{
	struct list_iter iter = {
		.list = list,
		.cur = list->head,
	};

	return iter;
}


void list_iter_begin(struct list_iter* iter)
{
	iter->cur = iter->list->head;
}


void list_iter_end(struct list_iter* iter)
{
	iter->cur = iter->list->tail;
}


void list_iter_next(struct list_iter* iter)
{
	if (iter->cur->next != NULL)
		iter->cur = iter->cur->next;
}


void list_iter_prev(struct list_iter* iter)
{
	if (iter->cur->prev != NULL)
		iter->cur = iter->cur->prev;
}


int list_iter_has_next(const struct list_iter* iter)
{
	return iter->cur->next != NULL;
}


int list_iter_has_prev(const struct list_iter* iter)
{
	return iter->cur->prev != NULL;
}


void* list_iter_cur_item(const struct list_iter* iter)
{
	return iter->cur->data;
}
