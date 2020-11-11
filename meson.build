project('mug', 'c', version: '0.0.1')

deps = [
     dependency('threads')
]

src = [
    'io_event.c',
    'io_fs_event.c',
    'io_http_event.c',
    'io_request_event.c',
    'mug.c',
    'route.c',
    'thread_pool.c',
]

incdirs = include_directories('include/mug', 'src')

muglib = library('mug', src, include_directories: incdirs, dependencies: deps)

subdir('test')
