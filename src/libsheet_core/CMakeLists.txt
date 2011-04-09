if (HAVE_TR1)
  ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
endif ()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

# need hash_map or unordered_map or something for efficient hashing.
# hash_map is deprecated under newer gcc versions.
ADD_DEFINITIONS(-Wno-deprecated)

file(GLOB folder_source *.cpp)
file(GLOB_RECURSE folder_header include/*.h)
file(GLOB_RECURSE folder_header2 impl_include/*.h)
SET(folder_header ${folder_header} ${folder_header2})
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})

ADD_LIBRARY(sheet_core ${folder_source} ${folder_header})

# touch 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21


