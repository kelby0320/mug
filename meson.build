project('mug', 'c', version: '0.0.1')

cc = meson.get_compiler('c')

deps = [
    dependency('threads'),
    cc.find_library('m')
]

src = [
    'src/core/mug_context.c',
    'src/event/event.c',
    'src/event/external_event.c',
    'src/event/http_request_event.c',
    'src/event/http_response_event.c',
    'src/event/internal_event.c',
    'src/event/new_connection_event.c',
    'src/event/request_completed_event.c',
    'src/event_context/epoll_event_context.c',
    'src/event_context/event_context.c',
    'src/event_handler/event_handler.c',
    'src/event_handler/new_connection.c',
    'src/event_handler/request_completed.c',
    'src/event_source/event_source.c',
    'src/event_source/http_response_event_source.c',
    'src/event_source/internal_event_source.c',
    'src/event_source/listen_sock_event_source.c',
    'src/http/buffer.c',
    'src/http/request_parser.c',
    'src/task_executor/i_task_executor.c',
    'src/task_executor/pool_worker.c',
    'src/task_executor/task_executor.c',
    'src/task_executor/work_queue.c',
    'src/types/invocation/mug_continuation_invocation.c',
    'src/types/invocation/mug_http_continuation_invocation.c',
    'src/types/invocation/mug_invocation.c',
    'src/types/invocation/mug_request_invocation.c',
    'src/types/misc/mug_body.c',
    'src/types/misc/mug_headers.c',
    'src/types/misc/mug_http_request.c',
    'src/types/misc/mug_http_response.c',
    'src/types/misc/mug_save_bag.c',
    'src/types/result/mug_continuation_result.c',
    'src/types/result/mug_http_continuation_result.c',
    'src/types/result/mug_response_result.c',
    'src/types/result/mug_result.c',
    'src/event_dispatcher.c',
    'src/event_service.c',
    'src/event_source_map.c',
    'src/routing_table.c'
]

incdirs = include_directories('include', 'include/mug', 'src')

muglib = library('mug', src, include_directories: incdirs, dependencies: deps)

subdir('test/unit')
subdir('test/app')
