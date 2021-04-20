#ifndef EVENT_H
#define EVENT_H


typedef enum {
    EVENT_BASE,
    EXTERNAL_EVENT_BASE,
    INTERNAL_EVENT_BASE,
    NEW_CONNECTION_EVENT,
    REQUEST_COMPLETED_EVENT,
    HTTP_RESPONSE_EVENT,
    HTTP_REQUEST_EVENT
} event_type_t;


typedef struct {
    event_type_t type;
} event_t;


#endif