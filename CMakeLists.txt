cmake_minimum_required(VERSION 2.6)

project(coopy)

enable_testing()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
message(STATUS "Libraries are placed in ${LIBRARY_OUTPUT_PATH}")
message(STATUS "Executables are placed in ${EXECUTABLE_OUTPUT_PATH}")
# Make sure the directories actually exist
make_directory(${LIBRARY_OUTPUT_PATH})
make_directory(${EXECUTABLE_OUTPUT_PATH})

# three-way merges of a 2D array
add_subdirectory(merge3x2d)

# probing properties of sheets
add_subdirectory(scanss)

# fossil integration
add_subdirectory(sheet_scm)
