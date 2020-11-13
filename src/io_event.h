#ifndef IO_EVENT_H
#define IO_EVENT_H


typedef enum {
    IO_REQUEST_EVENT,
    IO_HTTP_EVENT,
    IO_FS_EVENT,
} io_event_type_t;


typedef enum {
    IO_READY,
    IO_IN_PROGRESS,
    IO_COMPLETE,
} io_event_status_t;


typedef struct {
    io_event_type_t type;
    io_event_status_t status;
    int fd;
    io_event_t *prev;
    void* data;
} io_event_t;


struct io_event_map;


typedef struct io_event_map io_event_map_t;


io_event_map_t* io_event_map_init();


int io_event_map_deinit(io_event_map_t*);


#endif