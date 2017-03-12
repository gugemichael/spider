env = Environment(CC = 'g++', CPPFLAGS = ('-std=c++14 -g'))

LIBS = 'm',
LIBPATH = ['/usr/lib', '/usr/local/lib'],

env.Append(CCFLAGS = '-g')

spider = env.Program('./bin/spider', [
		'src/spider.cpp',
		Glob('src/scheduler/*.cpp'),
		Glob('src/common/*.cpp'),
], CPPPATH = ['src'], LIBS=['glog'])


