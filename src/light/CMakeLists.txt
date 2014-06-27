
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cpp)

add_library(coopy_light cpp/DiffRender.cpp cpp/coopy/DiffRender.h cpp/coopy/LogDiffRender.h cpp/coopy/HtmlDiffRender.h)
install(TARGETS coopy_light COMPONENT ${BASELIB} ${DESTINATION_LIB})

include_directories(${SHEET_INCLUDE_DIRS})

add_executable(test_light test_light.cpp)
target_link_libraries(test_light coopy_light coopy_full)

if (USE_JAVASCRIPT)
  include_directories(${CMAKE_SOURCE_DIR}/src/libjs/include)

  js_to_cpp(${CMAKE_CURRENT_SOURCE_DIR}/js/coopy.js coopy_js.cpp coopy_js)
  js_to_cpp(${CMAKE_CURRENT_SOURCE_DIR}/js/coopy_test.js coopy_test_js.cpp 
    coopy_test_js)

  add_executable(test_light_js test_light_js.cpp coopy_js.cpp coopy_test_js.cpp)
  target_link_libraries(test_light_js coopy_full coopy_js)
endif ()
