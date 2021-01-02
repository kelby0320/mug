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
    if (map->head == NULL) {
	map->head = (io_event_t*)ev;
	((io_event_t*)ev)->next = NULL;
    } else {
	io_event_t *tmp = map->head;
	map->head = (io_event_t*)ev;
	((io_event_t*)ev)->next = tmp;
    }
}


void io_event_map_add_io_event(io_event_map_t *map,
			       io_request_event_t *req_ev,
			       io_event_t *ev,
			       io_sync_event_t *sync_ev)
{
    /* TODO */
}


io_event_t* io_event_map_find(io_event_map_t *map, int fd)
{
    for (io_event_t *p = map->head; p != NULL; p = p->next) {
	if (p->fd == fd)
	    return p;
    }

    return NULL;
}
