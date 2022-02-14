#include <stdlib.h>
#include <string.h>

#include "event_dispatcher.h"
#include "event/event.h"
#include "event/event_type.h"
#include "event_handler/event_handler.h"
#include "task_executor/i_task_executor.h"
#include "task_executor/task_func.h"
#include "event_service.h"


#define NUMBER_OF_EVENTS 7


struct event_dispatcher {
    i_task_executor_t *task_executor;
    event_service_t *event_service;
    task_func_t map[NUMBER_OF_EVENTS];
};


event_dispatcher_t* event_dispatcher_alloc()
{
    return (event_dispatcher_t*)malloc(sizeof(event_dispatcher_t));
}


void event_dispatcher_ctor(event_dispatcher_t *dispatcher, i_task_executor_t *task_executor, event_service_t *event_service)
{
    dispatcher->task_executor = task_executor;
    dispatcher->event_service = event_service;
    memset(dispatcher->map, 0, NUMBER_OF_EVENTS * sizeof(task_func_t));
}


int event_dispatcher_register_handler(event_dispatcher_t *dispatcher, event_type_t event_type, task_func_t handler)
{
    int loc = (int)event_type;

    if (dispatcher->map[loc] != 0) {
        return -1;
    }

    dispatcher->map[loc] = handler;
    return 0;
}


int event_dispatcher_handle_event(const event_dispatcher_t *dispatcher, const event_t *event)
{
    int loc = (int)event_type(event);

    task_func_t handler = dispatcher->map[loc];

    if (handler == 0) {
        return -1;
    }

    struct event_handler_params *params = (struct event_handler_params*)malloc(sizeof(struct event_handler_params));
    params->event = event;
    params->event_service = dispatcher->event_service;

    i_task_executor_t *task_executor = dispatcher->task_executor;
    i_task_executor_submit(task_executor, handler, (void*)params);

    return 0;
}