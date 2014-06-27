option(STRICT_COMPILE "Strict compile flags" FALSE)
if (STRICT_COMPILE)
  option(VERY_STRICT_COMPILE "Very strict compile flags" FALSE)
  add_definitions(-Wall)
  if (VERY_STRICT_COMPILE)
    add_definitions(-Werror -Wfatal-errors)
  endif (VERY_STRICT_COMPILE)
endif (STRICT_COMPILE)

enable_testing()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
message(STATUS "Libraries are placed in ${LIBRARY_OUTPUT_PATH}")
message(STATUS "Executables are placed in ${EXECUTABLE_OUTPUT_PATH}")

if (MINGW)
  SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()

make_directory(${LIBRARY_OUTPUT_PATH})
make_directory(${EXECUTABLE_OUTPUT_PATH})

set(COOPY_DEPENDENCIES ${CMAKE_BINARY_DIR}/CoopyDependencies.cmake)
