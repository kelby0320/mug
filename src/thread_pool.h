#ifndef THREAD_POOL_H
#define THREAD_POOL_H


struct thread_pool;


typedef struct thread_pool thread_pool_t;


thread_pool_t* thread_pool_init(int);


int thread_pool_deinit(thread_pool_t*);


int thread_pool_size(const thread_pool_t*);


#endif