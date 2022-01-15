tests = [
	#'event_dispatcher.c',
	'event_service.c',
	'event_source_map.c',
	'routing_table.c'
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
subdir('task_executor')
subdir('types')