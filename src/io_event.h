#ifndef IO_EVENT_H
#define IO_EVENT_H


typedef enum {
    IO_REQUEST_EVENT,
    IO_HTTP_EVENT,
    IO_FS_EVENT,
    IO_SYNC_EVENT,
} io_event_type_t;


typedef enum {
    IO_READY,
    IO_IN_PROGRESS,
    IO_COMPLETE,
} io_event_status_t;


struct io_event {
    io_event_type_t type;
    io_event_status_t status;
    struct io_event* next;
    int fd;
    void* data;
};


struct io_event_map;


typedef struct io_event_map io_event_map_t;
typedef struct io_event io_event_t;


io_event_map_t* io_event_map_init();


void io_event_map_deinit(io_event_map_t*);


#endif