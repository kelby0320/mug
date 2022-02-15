tests = [
	'mug_save_bag.c',
	'mug_headers.c',
	'mug_body.c'
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