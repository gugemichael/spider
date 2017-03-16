env = Environment(CC = 'g++', CPPFLAGS = ('-std=c++11 -g -Wno-deprecated'))
#env = Environment(tools = ['gcc','gnulink'],CC='clang', CPPFLAGS = ('-std=c++14 -g'))

INCLUDE_PATH = ['']
LIBPATH = ['/usr/lib', '/usr/local/lib'],

env.Append(CCFLAGS = '-g')

spider = env.Program('./bin/spider', [
		'src/spider.cpp',
		Glob('src/scheduler/*.cpp'),
		Glob('src/common/*.cpp'),
		Glob('src/spider/fetcher/*.cpp'),
], CPPPATH = ['src'] + INCLUDE_PATH, LIBS=['glog','curl'])


