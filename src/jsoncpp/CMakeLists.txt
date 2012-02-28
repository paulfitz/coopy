
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/lib_json)

file(GLOB folder_source src/lib_json/*.cpp)
file(GLOB_RECURSE folder_header include/*.h)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})

add_library(coopy_jsoncpp ${folder_source} ${folder_header})
install(TARGETS coopy_jsoncpp COMPONENT ${BASELIB} DESTINATION lib)

add_executable(test_jsoncpp test_jsoncpp.cpp)
target_link_libraries(test_jsoncpp coopy_jsoncpp)

