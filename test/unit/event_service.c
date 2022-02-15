#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#include "event_service.h"
#include "event/request_completed_event.h"


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
    int *sockets = (int*)malloc(sizeof(int*) * 2);
    sockets[0] = create_listening_socket();
    sockets[1] = get_connected_socket();

    event_service_t *event_service = event_service_alloc();
    event_service_ctor(event_service, sockets[1]);

    state[0] = event_service;
    state[1] = sockets;

    return 0;
}


static int teardown(void **state)
{
    event_service_t *event_service = (event_service_t*)state[0];
    int *sockets = (int*)state[1];

    unlink(TEST_SOCKET_FILE_PATH);
    close(sockets[0]);
    close(sockets[1]);

    free(event_service);
    free(sockets);

    return 0;
}


static void test_raise_request_completed(void **state)
{
    event_service_t *event_service = (event_service_t*)state[0];
    int *sockets = (int*)state[1];
    int dummy_client_fd = 10;

    mug_http_response_t *http_response = mug_http_response_alloc();
    mug_http_response_ctor(http_response);

    event_service_raise_request_completed(event_service, http_response, dummy_client_fd);

    request_completed_event_t *event = NULL;
    int client_fd = accept(sockets[0], NULL, NULL);
    read(client_fd, &event, sizeof(request_completed_event_t*));

    int req_fd = request_completed_event_fd(event);
    assert_true(req_fd == dummy_client_fd);

    mug_http_response_dtor(http_response);
    free(http_response);
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_raise_request_completed, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}