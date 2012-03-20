
if (USE_JACKCESS)
  message(STATUS "JAVA compilation uses gcj")
  set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/conf/javac;${CMAKE_MODULE_PATH}")
  enable_language(C)
  enable_language(JavaC)
endif ()

