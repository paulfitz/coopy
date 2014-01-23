include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
add_definitions(-DHX_LINUX -DHXCPP_VISIT_ALLOCS -Dhaxe3 -DHXCPP_FORCE_OBJ_MAP)

file(GLOB_RECURSE folder_headers include/*.h)
file(GLOB_RECURSE folder_sources src/*.cpp)

add_library(coopy_coopyhx_core STATIC ${folder_headers} ${folder_sources} ../CoopyhxLink.cpp)
set_target_properties(coopy_coopyhx_core PROPERTIES COMPILE_FLAGS -fPIC)
