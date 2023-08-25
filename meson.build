project('oppai', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

evdev_dep = dependency('libevdev')

exe = executable('oppai', [ 'src/main.c', 'src/enviroment.c',
			    'src/check.c', 'src/input.c', 'src/loop.c' ],
	dependencies : [evdev_dep],
	include_directories : 'inc',
	install : true)

test('basic', exe)
