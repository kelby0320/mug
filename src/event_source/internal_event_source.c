#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

#include "event/event.h"
#include "event_source/event_source.h"
#include "event_source/__event_source.h"
#include "event_source/internal_event_source.h"


struct internal_event_source {
    event_source_t event_source;
};


#define UNIX_SOCKET_PREFIX "mug_sock"
#define SOCKET_PATH_BUFFER_SIZE 128
#define UNIX_SOCKET_LISTEN_BACKLOG 10


static int create_unix_socket();
static void make_temp_socket_path(char*, int);
static int file_exists(char*);


event_t* __get_internal_event_func(event_source_t* event_source)
{
    // Read event_t object address from event_source socket
    event_t *event;
    read(event_source->fd, event, sizeof(event_t*));

    return event;
}


internal_event_source_t* internal_event_source_alloc()
{
    (internal_event_source_t*)malloc(sizeof(internal_event_source_t));
}


void internal_event_source_ctor(internal_event_source_t *internal_event_source)
{
    event_source_t* event_source = (event_source_t*)internal_event_source;
    event_source_ctor(event_source);

    int sfd = create_unix_socket();

    internal_event_source->event_source.fd = sfd;
    internal_event_source->event_source.get_event_func = __get_internal_event_func;
}


static int create_unix_socket()
{
    char socket_path_buf[SOCKET_PATH_BUFFER_SIZE];
    make_temp_socket_path(&socket_path_buf, SOCKET_PATH_BUFFER_SIZE);

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path_buf, sizeof(addr.sun_path) - 1);

    bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));

    listen(sfd, UNIX_SOCKET_LISTEN_BACKLOG);

    return sfd;
}


static void make_temp_socket_path(char *buf, int bufsize)
{
    do {
        snprintf(buf, bufsize, "/tmp/%s_%d", UNIX_SOCKET_PREFIX, rand() % 1000);
    } while (!file_exists(buf));
}


static int file_exists(char *filepath)
{
    struct stat stat_buf;
    return (stat(filepath, &stat_buf) == 0);
}