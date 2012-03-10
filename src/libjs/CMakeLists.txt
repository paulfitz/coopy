if (USE_JAVASCRIPT)

  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
  add_library(coopy_js JsWrap.cpp include/coopy/JsWrap.h)

  add_executable(codify codify.cpp)
  
endif ()