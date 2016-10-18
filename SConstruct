env = Environment(CC = 'g++', CPPFLAGS = ('-std=c++11 -g'))

LIBS = 'm',
LIBPATH = ['/usr/lib', '/usr/local/lib'],

env.Program('spider', ['src/spider.cpp'], CPPPATH = ['src'], LIBS=['glog'])
