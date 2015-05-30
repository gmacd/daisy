# scons -Q release=0 (default)
# scons -Q release=1

filesStr = """
src/main.cpp
"""

cppflags = '-std=c++14'

cpppath = Split('third/glfw/include third/bgfx/include')
libpath = Split('third/glfw third/bgfx')

libs = Split('glfw3 bgfxRelease')
frameworks = Split('Cocoa OpenGL IOKit CoreVideo')

env = Environment(
    CPPFLAGS = cppflags,
    CPPPATH = cpppath,
    LIBS=libs,
    LIBPATH=libpath,
    FRAMEWORKS=frameworks)

for key, value in ARGLIST:
    if key == 'release' and value == 1:
        env.Append(CCFLAGS = '-g')


env.Program('daisy', Split(filesStr))
