#include <stdlib.h>
#include <string.h>

#include "event_dispatcher.h"
#include "event/event.h"
#include "event/event_type.h"
#include "event_handler/event_handler.h"


#define NUMBER_OF_EVENTS 7


struct event_dispatcher {
    event_handler_func_t map[NUMBER_OF_EVENTS];
};


event_dispatcher_t* event_dispatcher_alloc()
{
    return (event_dispatcher_t*)malloc(sizeof(event_dispatcher_t));
}


void event_dispatcher_ctor(event_dispatcher_t *dispatcher)
{
    memset(dispatcher->map, 0, NUMBER_OF_EVENTS * sizeof(event_handler_func_t));
}


int event_dispatcher_register_handler(event_dispatcher_t *dispatcher, event_type_t event_type, event_handler_func_t handler)
{
    int loc = (int)event_type;

    if (dispatcher->map[loc] != 0) {
        return -1;
    }

    dispatcher->map[loc] = handler;
    return 0;
}


int event_dispatcher_raise_event(const event_dispatcher_t *dispatcher, const event_t *event)
{
    int loc = (int)event_type(event);

    event_handler_func_t handler = dispatcher->map[loc];

    if (handler == 0) {
        return -1;
    }

    handler(event);
    return 0;
}