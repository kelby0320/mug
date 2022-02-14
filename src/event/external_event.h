#ifndef EXTERNAL_EVENT_H
#define EXTERNAL_EVENT_H


struct exteranl_event;


typedef struct external_event external_event_t;


external_event_t* exteranl_event_alloc();
void external_event_ctor(external_event_t*, int);
int external_event_fd(const external_event_t*);



#endif