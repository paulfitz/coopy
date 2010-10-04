
cmake_minimum_required(VERSION 2.6)

project(ssinsert)

include(FindPkgConfig)
pkg_check_modules (GLIB2 REQUIRED glib-2.0)
include_directories(${GLIB2_INCLUDE_DIRS})
pkg_search_module (GOFFICE REQUIRED libgoffice-1.0 libgoffice-0.9 libgoffice-0.8 libgoffice-1)
include_directories(${GOFFICE_INCLUDE_DIRS})
pkg_search_module (GOBJECT REQUIRED gobject-2.0)
include_directories(${GOBJECT_INCLUDE_DIRS})
pkg_search_module (GSF REQUIRED libgsf-1)

link_libraries(${GLIB2_LIBRARIES})
link_libraries(${GOBJECT_LIBRARIES})
link_libraries(${GOFFICE_LIBRARIES})
link_libraries(${GSF_LIBRARIES})

# adapt to taste
#add_definitions(-DCUSTOM_GNUMERIC) # comment out if stf-parse.c not patched

set(GNUMERIC_ROOT "" CACHE STRING "Location of Gnumeric source") 
message(STATUS "GNUMERIC_ROOT=${GNUMERIC_ROOT}")
#if (NOT GNUMERIC_ROOT)
#  set(GNUMERIC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../gnumeric/gnumeric)
#  set(GNUMERIC_INSTALL /cache/gnumeric)
#endif ()
#message(STATUS "GNUMERIC_ROOT=${GNUMERIC_ROOT}")

include_directories(${GNUMERIC_ROOT})
include_directories(${GNUMERIC_ROOT}/src)
include_directories(${GNUMERIC_INSTALL}/libspreadsheet-1.10/spreadsheet/)
link_directories(${GNUMERIC_INSTALL}/bin)
link_libraries(spreadsheet)

# tweak for mingw build
if (NEED_FAKE_RT)
  add_library(rt dummy.c)
endif()

INCLUDE_DIRECTORIES(${SHEET_INCLUDE_DIRS})
link_libraries(sheet)

add_executable(ssinsert ssinsert.cpp gnumeric_link.h gnumeric_link.c)

