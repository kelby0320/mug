#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "event_source/listen_sock_event_source.h"
#include "event_source/event_source.h"
#include "event/event.h"
#include "event/external_event.h"
#include "event/event_type.h"


#define TEST_PORT "8080"
#define TEST_MESSAGE "Test Message"


static int create_listening_socket()
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, TEST_PORT, &hints, &bind_address);

    int listen_sock = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    bind(listen_sock, bind_address->ai_addr, bind_address->ai_addrlen);
    listen(listen_sock, 10);

    return listen_sock;
}


static int get_connected_socket()
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    struct addrinfo *conn_address;
    getaddrinfo(0, TEST_PORT, &hints, &conn_address);

    int conn_sock = socket(conn_address->ai_family, conn_address->ai_socktype, conn_address->ai_protocol);

    connect(conn_sock, conn_address->ai_addr, conn_address->ai_addrlen);

    return conn_sock;
}


static int setup(void **state)
{
    int sfd = create_listening_socket();

    listen_sock_event_source_t *listen_sock_event_source = listen_sock_event_source_alloc();
    listen_sock_event_source_ctor(listen_sock_event_source, sfd);

    int connected_sock = get_connected_socket();
    write(connected_sock, TEST_MESSAGE, strlen(TEST_MESSAGE) + 1);

    *state = listen_sock_event_source;

    close(connected_sock);

    return 0;
}


static int teardown(void **state)
{
    listen_sock_event_source_t *listen_sock_event_source = (listen_sock_event_source_t*)*state;
    int sfd = event_source_get_fd((event_source_t*)listen_sock_event_source);

    free(listen_sock_event_source);
    close(sfd);

    return 0;
}


static void test_get_event(void **state)
{
    event_source_t *event_source = (event_source_t*)*state;

    event_t* new_event = event_source_get_event(event_source);

    assert_true(event_type(new_event) == NEW_CONNECTION_EVENT);

    int cfd = external_event_fd((external_event_t*)new_event);

    char buf[32];
    int num_read = read(cfd, buf, 32);

    assert_true(num_read == strlen(TEST_MESSAGE) + 1);
    assert_true(strcmp(buf, TEST_MESSAGE) == 0);
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_get_event, setup, teardown)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}