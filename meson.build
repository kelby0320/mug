project('mug', 'c', version: '0.0.1')

cc = meson.get_compiler('c')

deps = [
    dependency('threads'),
    cc.find_library('m')
]

# src = [
#     'src/event/event_ctx.c',
#     'src/event/epoll_event_ctx.c',
#     'src/http/http_parser.c',
#     'src/http/http_response_writer.c',
#     'src/io_event/io_event_map.c',
#     'src/io_event/io_event.c',
#     'src/io_event/io_fs_event.c',
#     'src/io_event/io_http_event.c',
#     'src/io_event/io_request_event.c',
#     'src/io_event/io_sync_event.c',
#     'src/event_handler.c',
#     'src/mug.c',
#     'src/routing_table.c',
#     'src/thread_pool.c',
#     'src/work_queue.c'
# ]

src = [
    'src/event/event.c',
    'src/event/external_event.c',
    'src/event/http_request_event.c',
    'src/event/http_response_event.c',
    'src/event/internal_event.c',
    'src/event/new_connection_event.c',
    'src/event/request_completed_event.c',
    'src/event_source/event_source.c',
    'src/event_source/http_response_event_source.c',
    # 'src/event_source/internal_event_source.c',
    # 'src/event_source/listen_sock_event_source.c',
    # 'src/types/invocation/mug_continuation_invocation.c',
    # 'src/types/invocation/mug_http_continuation_invocation.c',
    # 'src/types/invocation/mug_invocation.c',
    # 'src/types/invocation/mug_request_invocation.c',
    'src/types/misc/mug_body.c',
    'src/types/misc/mug_headers.c',
    'src/types/misc/mug_http_request.c',
    'src/types/misc/mug_http_response.c',
    'src/types/misc/mug_save_bag.c',
    # 'src/types/result/mug_continuation_result.c',
    # 'src/types/result/mug_http_continuation_result.c',
    # 'src/types/result/mug_response_result.c',
    # 'src/types/result/mug_result.c'
]

incdirs = include_directories('include/mug', 'include/mug/core', 'include/mug/types', 'src')

muglib = library('mug', src, include_directories: incdirs, dependencies: deps)

subdir('test')
