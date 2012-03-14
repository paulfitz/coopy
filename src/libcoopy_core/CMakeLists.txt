INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/gnulib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/light/cpp)

file(GLOB folder_source *.cpp)
file(GLOB_RECURSE folder_header include/*.h)
file(GLOB_RECURSE folder_header2 impl_include/*.h)
set(folder_header ${folder_header} ${folder_header2})

# ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
foreach(src ${folder_source})
  if (HAVE_TR1)
    set_property(SOURCE ${src} 
      APPEND PROPERTY COMPILE_FLAGS "-std=c++0x")
    set_property(SOURCE ${src} 
      APPEND PROPERTY COMPILE_DEFINITIONS HAVE_TR1)
  else ()
    # need hash_map or unordered_map or something for efficient hashing.
    # hash_map is deprecated under newer gcc versions.
    set_property(SOURCE ${src} 
      APPEND PROPERTY COMPILE_FLAGS "-Wno-deprecated")
  endif ()
endforeach()

set(folder_source ${folder_source} gnulib/tmpdir.c gnulib/tempname.c)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})

add_library(coopy_core ${folder_source} ${folder_header})
target_link_libraries(coopy_core coopy_light)
export(TARGETS coopy_core APPEND FILE ${COOPY_DEPENDENCIES})
install(TARGETS coopy_core COMPONENT ${BASELIB} DESTINATION lib)

# touch 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39


