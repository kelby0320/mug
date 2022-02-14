#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H


#include "event/event.h"
#include "event/event_type.h"
#include "task_executor/i_task_executor.h"
#include "task_executor/task_func.h"
#include "event_service.h"


struct event_dispatcher;


typedef struct event_dispatcher event_dispatcher_t;


event_dispatcher_t* event_dispatcher_alloc();
void event_dispatcher_ctor(event_dispatcher_t*, i_task_executor_t*, event_service_t*);
int event_dispatcher_register_handler(event_dispatcher_t*, event_type_t, task_func_t);
int event_dispatcher_handle_event(const event_dispatcher_t*, const event_t*);


#endif