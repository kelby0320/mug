#include <stdio.h>

#include "mug.h"


int main()
{
    printf("Create mug_ctx\n");
    mug_ctx_t *mug_ctx = mug_ctx_init(8080, 10);

    printf("Serve\n");
    mug_ctx_serve(mug_ctx);

    return 0;
}
