#include <stdio.h>

#include "mug.h"


static void test_handler1(void*, void*);


int main()
{
    printf("Create mug_ctx\n");
    mug_ctx_t *mug_ctx = mug_ctx_init(8080, 10);

    routing_table_t *tbl = mug_ctx_routing_table(mug_ctx);
    routing_table_add_route(tbl, "/test_route", test_handler);

    printf("Serve\n");
    mug_ctx_serve(mug_ctx);

    return 0;
}


static void test_handler(void *a, void *b)
{
    printf("Test Handler\n");
}
