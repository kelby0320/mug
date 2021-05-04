#ifndef MUG_SAVE_BAG_H
#define MUG_SAVE_BAG_H


struct mug_save_bag;


typedef struct mug_save_bag mug_save_bag_t;


mug_save_bag_t* mug_save_bag_alloc();
void mug_save_bag_ctor(mug_save_bag_t*);
void mug_save_bag_ctor2(mug_save_bag_t*, size_t);
void mug_save_bag_dtor(mug_save_bag_t*);
int mug_save_bag_size(const mug_save_bag_t*);
int mug_save_bag_capacity(const mug_save_bag_t*);
void mug_save_bag_put_int(mug_save_bag_t*, const char*, int);
void mug_save_bag_put_dbl(mug_save_bag_t*, const char*, double);
void mug_save_bag_put_ptr(mug_save_bag_t*, const char*, void*);
void mug_save_bag_put_str(mug_save_bag_t*, const char*, const char*);
void mug_save_bag_put_struct(mug_save_bag_t*, const char*, const void*, size_t);
int mug_save_bag_get_int(const mug_save_bag_t*, const char*, int*);
int mug_save_bag_get_dbl(const mug_save_bag_t*, const char*, double*);
int mug_save_bag_get_ptr(const mug_save_bag_t*, const char*, void**);
int mug_save_bag_get_str(const mug_save_bag_t*, const char*, char*);
int mug_save_bag_get_struct(const mug_save_bag_t*, const char*, void*, size_t);


#endif