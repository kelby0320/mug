project('mug', 'c', version: '0.0.1')

deps = [
     dependency('threads')
]

src = [
    'src/event_ctx.c',
    'src/io_event.c',
    'src/io_fs_event.c',
    'src/io_http_event.c',
    'src/io_request_event.c',
    'src/io_sync_event.c',
    'src/mug.c',
    'src/routing_table.c',
    'src/thread_pool.c',
    'src/work_queue.c'
]

incdirs = include_directories('include/mug', 'src')

muglib = library('mug', src, include_directories: incdirs, dependencies: deps)

subdir('test')
