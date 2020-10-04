project('mug', 'c', version: '0.0.1')

src = ['src/list.c', 'src/array.c']

incdirs = include_directories('include', 'src')

muglib = library('mug', src, include_directories: incdirs)

subdir('test')
