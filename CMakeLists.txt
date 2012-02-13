cmake_minimum_required(VERSION 2.6)

project(coopy)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/conf" ${CMAKE_MODULE_PATH})

include(CoopyOptions)
include(CoopyVersion)

add_subdirectory(src)

include(CoopyPack)
include(CoopyDoc)
