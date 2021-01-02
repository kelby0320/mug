#ifndef IO_EVENT_H
#define IO_EVENT_H


typedef enum {
    IO_REQUEST_EVENT,
    IO_HTTP_EVENT,
    IO_FS_EVENT,
    IO_SYNC_EVENT,
} io_event_type_t;


struct io_event;


typedef struct io_event io_event_t;


typedef void (*deinit_func_t)(io_event_t*);

struct io_event {
    io_event_type_t type;
    struct io_event* next;
    int fd;
    deinit_func_t deinit_func;
};


void io_event_deinit(io_event_t*);

#endif
