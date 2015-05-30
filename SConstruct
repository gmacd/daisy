cppflags = '-std=c++14'

cpppath = Split('#/third/glfw/include')
libpath = Split('#/third/glfw')

libs = Split('glfw3')
frameworks = Split('Cocoa OpenGL IOKit CoreVideo')

env = Environment(
    CPPFLAGS = cppflags,
    CPPPATH = cpppath,
    LIBS = libs,
    LIBPATH = libpath,
    FRAMEWORKS = frameworks)

for key, value in ARGLIST:
    if key == 'release' and value == 1:
        env.Append(CCFLAGS = '-g')

Export('env')
env.SConscript('src/Sconscript', variant_dir='build', duplicate=0)