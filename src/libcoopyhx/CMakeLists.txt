if (USE_COOPYHX)

  add_subdirectory(package)

  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
  include_directories(${CMAKE_SOURCE_DIR}/src/libcoopy_core/include)

  add_library(coopy_coopyhx Coopyhx.cpp include/coopy/Coopyhx.h)
  target_link_libraries(coopy_coopyhx coopy_coopyhx_core)

endif()
