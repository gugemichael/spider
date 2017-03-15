env = Environment(CC = 'g++', CPPFLAGS = ('-std=c++14 -g'))
#env = Environment(tools = ['gcc','gnulink'],CC='/usr/local/Cellar/gcc/6.3.0_1/bin/gcc-6', CPPFLAGS = ('-std=c++14 -g'))

LIBS = 'm',
LIBPATH = ['/usr/local/Cellar/curl/7.53.1/lib', '/usr/lib', '/usr/local/lib'],

env.Append(CCFLAGS = '-g')

spider = env.Program('./bin/spider', [
		'src/spider.cpp',
		Glob('src/scheduler/*.cpp'),
		Glob('src/common/*.cpp'),
		Glob('src/spider/fetcher/*.cpp'),
], CPPPATH = ['src', '/usr/local/Cellar/curl/7.53.1/include/'], LIBS=['glog','folly', 'curl'])


