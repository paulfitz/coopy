cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/conf" ${CMAKE_MODULE_PATH})
include(CoopyLangs)

project(coopy)

include(CoopyOptions)
include(CoopyVersion)

add_subdirectory(src)

include(CoopyPack)
include(CoopyDoc)
