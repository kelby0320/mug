#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "types/misc/mug_save_bag.h"


#define SAVE_BAG_DEFAULT_CAPACITY 16
#define SAVE_BAG_RESIZE_THRESHOLD 0.75


enum item_type {
    INTEGER_ITEM,
    DOUBLE_ITEM,
    POINTER_ITEM,
    STRING_ITEM,
    STRUCT_ITEM
};


union item_value {
    int i;
    double d;
    void *p;
    char *str;
    void *s;
};


struct item {
    enum item_type type;
    char *key;
    union item_value value;
};


struct mug_save_bag {
    struct item *items;
    int size;
    int capacity;
};


static unsigned long hash(const char*);
static int get_insert_location(const mug_save_bag_t*, const char*);
static int get_retrieve_location(const mug_save_bag_t*, const char*);
static void free_item(struct item*);
static int is_item_set(const struct item*);
static void set_item_key(struct item*, const char*);
static void set_integer_item(struct item*, const char*, int);
static void set_double_item(struct item*, const char*, double);
static void set_pointer_item(struct item*, const char*, void*);
static void set_str_item(struct item*, const char*, const char*);
static void set_struct_item(struct item*, const char*, const void*, size_t);
static void table_insert(mug_save_bag_t*, struct item);
static struct item* table_retrieve(const mug_save_bag_t*, const char*);
static void table_resize(mug_save_bag_t*);
static void move_save_bag(mug_save_bag_t*, mug_save_bag_t*);


mug_save_bag_t* mug_save_bag_alloc()
{
    mug_save_bag_t *save_bag = (mug_save_bag_t*)malloc(sizeof(mug_save_bag_t));
    return save_bag;
}


void mug_save_bag_ctor(mug_save_bag_t *save_bag)
{
    save_bag->items = (struct item*)calloc(sizeof(struct item), SAVE_BAG_DEFAULT_CAPACITY);
    save_bag->capacity = SAVE_BAG_DEFAULT_CAPACITY;
    save_bag->size = 0;
}


void mug_save_bag_ctor2(mug_save_bag_t *save_bag, size_t size)
{
    save_bag->items = (struct item*)calloc(sizeof(struct item), size);
    save_bag->capacity = size;
    save_bag->size = 0;
}


void mug_save_bag_dtor(mug_save_bag_t *save_bag)
{
    for (int i = 0; i < save_bag->capacity; i++) {
        if (is_item_set(&save_bag->items[i])) {
            free_item(&save_bag->items[i]);
        }
    }

    free(save_bag->items);
}


int mug_save_bag_size(const mug_save_bag_t *save_bag)
{
    return save_bag->size;
}


int mug_save_bag_capacity(const mug_save_bag_t *save_bag)
{
    return save_bag->capacity;
}


void mug_save_bag_put_int(mug_save_bag_t *save_bag, const char *key, int value)
{
    struct item item;
    set_integer_item(&item, key, value);

    table_insert(save_bag, item);
}


void mug_save_bag_put_dbl(mug_save_bag_t *save_bag, const char *key, double value)
{
    struct item item;
    set_double_item(&item, key, value);

    table_insert(save_bag, item);
}


void mug_save_bag_put_ptr(mug_save_bag_t *save_bag, const char *key, void *value)
{
    struct item item;
    set_pointer_item(&item, key, value);

    table_insert(save_bag, item);
}


void mug_save_bag_put_str(mug_save_bag_t *save_bag, const char *key, const char *value)
{
    struct item item;
    set_str_item(&item, key, value);

    table_insert(save_bag, item);
}


void mug_save_bag_put_struct(mug_save_bag_t *save_bag, const char *key, const void *value, size_t value_size)
{
    struct item item;
    set_struct_item(&item, key, value, value_size);

    table_insert(save_bag, item);
}


int mug_save_bag_get_int(const mug_save_bag_t *save_bag, const char *key, int *out)
{
    struct item *item = table_retrieve(save_bag, key);

    if (item == NULL) {
        return -1;
    }

    *out = item->value.i;

    return 0;
}


int mug_save_bag_get_dbl(const mug_save_bag_t *save_bag, const char *key, double *out)
{
    struct item *item = table_retrieve(save_bag, key);

    if (item == NULL) {
        return -1;
    }

    *out = item->value.d;

    return 0;
}

int mug_save_bag_get_ptr(const mug_save_bag_t *save_bag, const char *key, void **out)
{
    struct item *item = table_retrieve(save_bag, key);

    if (item == NULL) {
        return -1;
    }

    *out = item->value.p;

    return 0;
}


int mug_save_bag_get_str(const mug_save_bag_t *save_bag, const char *key, char *out)
{
    struct item *item = table_retrieve(save_bag, key);

    if (item == NULL) {
        return -1;
    }

    strcpy(out, item->value.str);

    return 0;
}


int mug_save_bag_get_struct(const mug_save_bag_t *save_bag, const char *key, void *out, size_t out_size)
{
    struct item *item = table_retrieve(save_bag, key);

    if (item == NULL) {
        return -1;
    }

    memcpy(out, item->value.s, out_size);

    return 0;
}


static unsigned long hash(const char *str)
{
    // djb2 algorithm
    // http://www.cse.yorku.ca/~oz/hash.html

    unsigned long hash = 5381;
    int len = strlen(str);

    for (int i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) ^ str[i]; // hash(i) = hash(i-1) * 33 ^ str[i]
    }

    return hash;
}


static void free_item(struct item *item)
{
    if (item->type == STRING_ITEM) {
        free(item->value.str);
    }

    free(item->key);
}


static int is_item_set(const struct item *item)
{
    if (item->key != NULL) {
        return 1;
    }

    return 0;
}


static void set_item_key(struct item *item, const char *key)
{
    int len = strlen(key);
    item->key = (char*)malloc(len + 1);
    strcpy(item->key, key);
}


static void set_integer_item(struct item *item, const char *key, int value)
{
    item->type = INTEGER_ITEM;

    set_item_key(item, key);

    item->value.i = value;
}


static void set_double_item(struct item *item, const char *key, double value)
{
    item->type = DOUBLE_ITEM;

    set_item_key(item, key);

    item->value.d = value;
}

static void set_pointer_item(struct item *item, const char *key, void *value)
{
    item->type = POINTER_ITEM;

    set_item_key(item, key);

    item->value.p = value;
}


static void set_str_item(struct item *item, const char *key, const char *value)
{
    item->type = STRING_ITEM;

    set_item_key(item, key);

    int value_len = strlen(value);
    item->value.str = (char*)malloc(value_len + 1);
    strcpy(item->value.str, value);
}


static void set_struct_item(struct item *item, const char *key, const void *value, size_t value_size)
{
    item->type = STRUCT_ITEM;

    set_item_key(item, key);

    item->value.s = malloc(value_size);
    memcpy(item->value.s, value, value_size);
}


static int get_insert_location(const mug_save_bag_t *save_bag, const char *str)
{
    int loc = hash(str) % save_bag->capacity;

    while (is_item_set(&save_bag->items[loc])) {
        loc = (loc + 1) % save_bag->capacity;
    }

    return loc;
}


static int get_retrieve_location(const mug_save_bag_t *save_bag, const char *str)
{
    int loc = hash(str) % save_bag->capacity;
    int count = 0;

    while (strcmp(save_bag->items[loc].key, str) != 0 && count < save_bag->capacity) {
        loc = (loc + 1) % save_bag->capacity;
        count++;
    }

    if (count == save_bag->capacity) {
        return -1;
    }

    return loc;
}


static void table_insert(mug_save_bag_t *save_bag, struct item item)
{
    if (save_bag->size + 1 > ceil(save_bag->capacity * SAVE_BAG_RESIZE_THRESHOLD)) {
        table_resize(save_bag);
    }

    int loc = get_insert_location(save_bag, item.key);

    save_bag->items[loc] = item;
    save_bag->size++;
}


static struct item* table_retrieve(const mug_save_bag_t *save_bag, const char *str)
{
    int loc = get_retrieve_location(save_bag, str);

    if (loc == -1) {
        return NULL;
    }

    struct item *item = &save_bag->items[loc];

    return item;
}


static void table_resize(mug_save_bag_t *save_bag)
{
    int new_capacity = save_bag->capacity * 2;

    mug_save_bag_t *new_save_bag = mug_save_bag_alloc();
    mug_save_bag_ctor2(new_save_bag, new_capacity);


    for (int i = 0; i < save_bag->capacity; i++) {
        if (is_item_set(&save_bag->items[i])) {
            table_insert(new_save_bag, save_bag->items[i]);
        }
    }

    move_save_bag(save_bag, new_save_bag);
}


static void move_save_bag(mug_save_bag_t *dest, mug_save_bag_t *src)
{
    free(dest->items);

    dest->items = src->items;
    dest->capacity = src->capacity;
    dest->size = src->size;

    free(src);
}