
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/lib_json)

file(GLOB folder_source src/lib_json/*.cpp)
file(GLOB_RECURSE folder_header include/*.h)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})

ADD_LIBRARY(coopy_jsoncpp ${folder_source} ${folder_header})

ADD_EXECUTABLE(test_jsoncpp test_jsoncpp.cpp)
TARGET_LINK_LIBRARIES(test_jsoncpp coopy_jsoncpp)

# touch 1

