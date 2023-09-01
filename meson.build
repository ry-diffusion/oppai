project('oppai', 'c',
  version : '23.09u-snapshot',
  default_options : ['warning_level=3', 'c_std=c11'])

evdev_dep = dependency('libevdev')
conf_data = configuration_data()
conf_data.set('version','23.09u-snapshot')

configure_file(input : 'inc/oppai_const.h.in',
               output : 'oppai_const.h',
               configuration : conf_data)


if (get_option('enable_debugging'))
	add_global_arguments('-DBUILD_DEBUG', language : 'c')
endif

exe = executable('oppai', [ 'src/main.c', 'src/cli.c', 'src/log.c',
			    'src/utils.c', 'src/input.c', 'src/worker.c' ],
	dependencies : [evdev_dep],
	include_directories : 'inc',
	install : true)
