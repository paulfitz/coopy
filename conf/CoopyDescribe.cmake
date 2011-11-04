
configure_file(${CMAKE_SOURCE_DIR}/conf/CoopyConfig.cmake.in
  ${CMAKE_BINARY_DIR}/CoopyConfig.cmake @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/conf/coopy_source.txt.in
  ${CMAKE_BINARY_DIR}/coopy_source.txt IMMEDIATE)
