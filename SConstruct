env = Environment(CC = 'gcc', CPPFLAGS = ('-std=c++11 -g -Wno-deprecated'))

INCLUDE_PATH = ['']

# dependency libraries
env.Append(LIBPATH=['/usr/local/lib', 'src/utils'])
env.Append(LIBS=['curl','boost_regex-mt','log'])

# c log moudle
log = SConscript(['src/utils/SConscript']) 

spider = env.Program('./bin/spider', [
		'src/spider.cpp',
		log,
		Glob('src/engine//*.cpp'),
		Glob('src/scheduler/*.cpp'),
		Glob('src/common/*.cpp'),
		Glob('src/spider/fetcher/*.cpp'),
		Glob('src/spider/middleware/*.cpp')
], CPPPATH = ['src'] + INCLUDE_PATH)


