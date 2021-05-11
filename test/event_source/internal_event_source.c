#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>

#include "event_source/internal_event_source.h"
#include "event_source/event_source.h"
#include "event/event.h"
#include "event/event_type.h"


#define TEST_SOCKET_FILE_PATH "/tmp/test_sock"
#define BACKLOG 10


static int create_listening_socket()
{
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, TEST_SOCKET_FILE_PATH);

    bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));
    listen(sfd, BACKLOG);

    return sfd;
}


static int get_connected_socket()
{
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, TEST_SOCKET_FILE_PATH);

    connect(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));

    return sfd;
}


static int setup(void **state)
{
    unlink(TEST_SOCKET_FILE_PATH);

    int listen_sock = create_listening_socket();

    internal_event_source_t *internal_event_source = internal_event_source_alloc();
    internal_event_source_ctor(internal_event_source, listen_sock);

    event_t *test_event = event_alloc();
    event_ctor(test_event);

    int connected_sock = get_connected_socket();
    write(connected_sock, &test_event, sizeof(event_t*));

    state[0] = internal_event_source;
    state[1] = test_event;

    close(connected_sock);

    return 0;
}


static int teardown(void **state)
{
    internal_event_source_t *internal_event_source = (internal_event_source_t*)state[0];
    event_t *event = (event_t*)state[1];

    int listen_sock = event_source_get_fd((event_source_t*)internal_event_source);

    free(internal_event_source);
    free(event);

    unlink(TEST_SOCKET_FILE_PATH);
    close(listen_sock);

    return 0;
}


static void test_get_event(void **state)
{
    event_source_t *event_source = (event_source_t*)state[0];
    event_t* event = (event_t*)state[1];

    event_t* new_event = event_source_get_event(event_source);

    assert_true(event == new_event);
    assert_true(event_type(event) == event_type(new_event));
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_get_event, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}