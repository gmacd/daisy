# scons -Q release=0 (default)
# scons -Q release=1

filesStr = """
src/main.cpp
"""

env = Environment()

for key, value in ARGLIST:
    if key == 'release' and value == 1:
        env.Append(CCFLAGS = '-g')


env.Program('daisy', Split(filesStr))
