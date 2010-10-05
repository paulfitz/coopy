
cmake_minimum_required(VERSION 2.6)

project(ssinsert)

include_directories(${SHEET_INCLUDE_DIRS})
add_executable(ssinsert ssinsert.cpp)
target_link_libraries(ssinsert sheet)


