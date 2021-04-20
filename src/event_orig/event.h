#ifndef EVENT_H
#define EVENT_H


typedef enum {
    EVENT_IN,
    EVENT_OUT
} event_type_t;


struct event {
    event_type_t type;
    int fd;
};


#endif
