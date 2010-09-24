# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER /usr/bin/i586-mingw32msvc-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/i586-mingw32msvc-g++)

IF (NOT HOME)
  SET(HOME $ENV{HOME})
ENDIF (NOT HOME)

SET(GNUMERIC_DEPLOY ${HOME}/cvs/gnumeric/gnumeric/tools/win32/out/release/deploy)
SET(CMAKE_FIND_ROOT_PATH  /usr/i586-mingw32msvc ${GNUMERIC_DEPLOY} ${GNUMERIC_DEPLOY}/bin)

LINK_DIRECTORIES(${GNUMERIC_DEPLOY}/lib)
INCLUDE_DIRECTORIES(${GNUMERIC_DEPLOY}/include)
INCLUDE_DIRECTORIES(/usr/i586-mingw32msvc/include)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
