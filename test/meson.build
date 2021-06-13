tests = [
    'event_source_map.c',
	'event_dispatcher.c'
]

deps = [
     dependency('cmocka')
]

foreach t : tests
	fname = t
	tname = t.split('.')[0]
	exe = executable(tname, fname,
	    include_directories: incdirs,
	    dependencies: deps,
	    link_with: muglib)
	test(tname, exe)
endforeach

subdir('event_source')
subdir('types')