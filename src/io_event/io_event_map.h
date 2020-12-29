#ifndef IO_EVENT_MAP_H
#define IO_EVENT_MAP_H


#include "io_event/io_event.h"
#include "io_event/io_request_event.h"
#include "io_event/io_sync_event.h"


struct io_event_map;


typedef struct io_event_map io_event_map_t;


io_event_map_t* io_event_map_init();


void io_event_map_deinit(io_event_map_t*);


void io_event_map_add_req_event(io_event_map_t*,
				io_request_event_t*);


void io_event_map_add_io_event(io_event_map_t*,
			       io_request_event_t*,
			       io_event_t*,
			       io_sync_event_t*);


io_event_t* io_event_map_find(io_event_map_t*, int);


#endif
