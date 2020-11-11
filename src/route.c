#include "route.h"

struct route_item {
    char *route;
    void (*handler)(void*, void*);
};

