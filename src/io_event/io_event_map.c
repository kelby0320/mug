#include <stdlib.h>
#include <pthread.h>


#include "io_event/io_event.h"
#include "io_event/io_request_event.h"
#include "io_event/io_sync_event.h"
#include "io_event/io_event_map.h"


struct  io_event_map {
    io_event_t *head;
    pthread_mutex_t mtx;
};


io_event_map_t* io_event_map_init()
{
    io_event_map_t *map = (io_event_map_t*)malloc(sizeof(io_event_map_t));

    map->head = NULL;

    pthread_mutexattr_t mtx_attr;

    pthread_mutexattr_init(&mtx_attr);
    pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_NORMAL);

    pthread_mutex_init(&map->mtx, &mtx_attr);

    pthread_mutexattr_destroy(&mtx_attr);

    return map;
}


void io_event_map_deinit(io_event_map_t *map)
{
    /* TODO - free events linked list */
    if (map->head != NULL) {
	for (io_event_t *p = map->head; p != NULL;) {
	    io_event_t *tmp = p;
	    p = p->next;
	    io_event_deinit(tmp);
	}
    }
    map->head = NULL;
    
    pthread_mutex_destroy(&map->mtx);

    free(map);
}


void io_event_map_add_req_event(io_event_map_t *map, io_request_event_t *ev)
{
    pthread_mutex_lock(&map->mtx);
    
    if (map->head == NULL) {
	map->head = (io_event_t*)ev;
	((io_event_t*)ev)->next = NULL;
    } else {
	io_event_t *tmp = map->head;
	map->head = (io_event_t*)ev;
	((io_event_t*)ev)->next = tmp;
    }

    pthread_mutex_unlock(&map->mtx);
}


void io_event_map_add_io_event(io_event_map_t *map,
			       io_request_event_t *req_ev,
			       io_event_t *ev,
			       io_sync_event_t *sync_ev)
{
    /* TODO */
}


void io_event_map_remove_event(io_event_map_t *map, io_event_t *ev)
{
    pthread_mutex_lock(&map->mtx);

    io_event_t *prev, *p;
    
    for (prev = NULL, p = map->head; p != NULL; prev = p, p = p->next) {
	if (p == ev)
	    break;
    }

    if (p == NULL) {
	/* Node not found */
	return;
    }

    if (prev == NULL) {
	/* Remove from the head of the list */
	map->head = p->next;
    } else if (p->next == NULL) {
	/* Remove from the end of the list */
	prev->next = NULL;
    } else {
	/* Remove from the middle of the list */
	prev->next = p->next;
    }

    /* Deallocate node */
    free(p);
    
    pthread_mutex_unlock(&map->mtx);
}


io_event_t* io_event_map_find(io_event_map_t *map, int fd)
{
    pthread_mutex_lock(&map->mtx);
    
    for (io_event_t *p = map->head; p != NULL; p = p->next) {
	if (p->fd == fd)
	    return p;
    }

    pthread_mutex_unlock(&map->mtx);
    
    return NULL;
}
