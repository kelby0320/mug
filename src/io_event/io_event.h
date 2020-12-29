#ifndef IO_EVENT_H
#define IO_EVENT_H


typedef enum {
    IO_REQUEST_EVENT,
    IO_HTTP_EVENT,
    IO_FS_EVENT,
    IO_SYNC_EVENT,
} io_event_type_t;


struct io_event {
    io_event_type_t type;
    struct io_event* next;
    int fd;
};


typedef struct io_event io_event_t;


#endif
