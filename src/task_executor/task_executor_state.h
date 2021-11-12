#ifndef TASK_EXECUTOR_STATE_H
#define TASK_EXECUTOR_STATE_H


typedef enum {
    STARTING,
    RUNNING,
    TERMINATING,
    TERMINATED
} task_executor_state_t;


#endif