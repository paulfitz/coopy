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
SET(CMAKE_CXX_COMPILER /usr/bin/i586-mingw32msvc-g++)
#SET(CMAKE_RC_COMPILER /usr/bin/i586-mingw32msvc-windres)

IF (NOT HOME)
  SET(HOME $ENV{HOME})
ENDIF (NOT HOME)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  /usr/i586-mingw32msvc ${HOME}/mingw/install ${HOME}/mingw/install/bin )

LINK_DIRECTORIES($ENV{HOME}/mingw/install/lib)
INCLUDE_DIRECTORIES($ENV{HOME}/mingw/install/include)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
