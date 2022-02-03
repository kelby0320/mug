#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


int main()
{
    const struct CMUnitTest tests[] = {
        
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}