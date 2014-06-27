if (USE_JAVASCRIPT)

  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
  add_library(coopy_js JsWrap.cpp include/coopy/JsWrap.h)
  install(TARGETS coopy_js COMPONENT ${BASELIB} ${DESTINATION_LIB})

  add_executable(codify codify.cpp)
  
endif ()