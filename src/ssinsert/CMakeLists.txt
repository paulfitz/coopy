
cmake_minimum_required(VERSION 2.6)

project(ssinsert)

include(FindPkgConfig)
pkg_check_modules (GLIB2 REQUIRED glib-2.0)
include_directories(${GLIB2_INCLUDE_DIRS})
pkg_search_module (GOFFICE REQUIRED libgoffice-1.0 libgoffice-0.9 libgoffice-0.8)
include_directories(${GOFFICE_INCLUDE_DIRS})

link_libraries(${GLIB2_LIBRARIES})
link_libraries(${GOFFICE_LIBRARIES})

# adapt to taste
set(GNUMERIC_ROOT ${CMAKE_SOURCE_DIR}/../../../gnumeric/gnumeric)
set(GNUMERIC_INSTALL /cache/gnumeric)
include_directories(${GNUMERIC_ROOT})
include_directories(${GNUMERIC_ROOT}/src)
include_directories(${GNUMERIC_INSTALL}/libspreadsheet-1.10/spreadsheet/)
link_directories(${GNUMERIC_ROOT}/lib)
link_libraries(spreadsheet)

add_executable(ssinsert ssinsert.c)

