###########################################################
# This is a sample cmake cross-compile setup script
# For compiling ssfossil for windows under linux
#
# Usage: replace paths as needed below, then:
#  mkdir winnt
#  cd winnt
#  cmake -DCMAKE_TOOLCHAIN_FILE=../scripts/mingwin.cmake ..
#
###########################################################

# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER /usr/bin/i586-mingw32msvc-gcc)
SET(CMAKE_CXX_COMPILER "/usr/bin/i586-mingw32msvc-g++")

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  /usr/i586-mingw32msvc /home/paulfitz/mingw/install /home/paulfitz/mingw/install/bin )

LINK_DIRECTORIES(/home/paulfitz/mingw/install/lib)
INCLUDE_DIRECTORIES(/home/paulfitz/mingw/install/include)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
