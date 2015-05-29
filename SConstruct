# scons -Q release=0 (default)
# scons -Q release=1

filesStr = """
src/main.cpp
"""

libs = Split('glfw3')
libpath = Split('third/glfw')
frameworks = Split('Cocoa OpenGL IOKit CoreVideo')

env = Environment(LIBS=libs, LIBPATH=libpath, FRAMEWORKS=frameworks)

for key, value in ARGLIST:
    if key == 'release' and value == 1:
        env.Append(CCFLAGS = '-g')


env.Program('daisy', Split(filesStr))
