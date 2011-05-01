cmake_minimum_required(VERSION 2.6)

option(CREATE_BINDINGS "Create SWIG language bindings" ON)

if (CREATE_BINDINGS)

  find_package(Coopy REQUIRED)
  include_directories(${Coopy_INCLUDE_DIRS})
  link_libraries(${Coopy_LIBRARIES})

  set(CREATE_PYTHON FALSE CACHE BOOL "Create Python interface")
  set(CREATE_RUBY FALSE CACHE BOOL "Create Ruby interface")

  FIND_PACKAGE(SWIG REQUIRED)
  INCLUDE(${SWIG_USE_FILE})
  MESSAGE(STATUS "${SWIG_USE_FILE}")
  SET (MIN_SWIG_VERSION "1.3.29")
  IF ("${SWIG_VERSION}" VERSION_LESS "${MIN_SWIG_VERSION}")
    MESSAGE( FATAL_ERROR "Please update your SWIG. Current version: ${SWIG_VERSION}. The minimum version needed: ${MIN_SWIG_VERSION}")
  ENDIF ("${SWIG_VERSION}" VERSION_LESS "${MIN_SWIG_VERSION}")

  SET_SOURCE_FILES_PROPERTIES(coopy.i PROPERTIES CPLUSPLUS ON)

  configure_file(${CMAKE_SOURCE_DIR}/../tests/named_numbers.csv ${CMAKE_BINARY_DIR}/named_numbers.csv)
  configure_file(${CMAKE_SOURCE_DIR}/../tests/named_numbers_three_23.csv ${CMAKE_BINARY_DIR}/named_numbers_three_23.csv)

  if (CREATE_PYTHON)
    set(CMAKE_SWIG_FLAGS "-Wall;-module;coopy;-threads")
    find_package(PythonLibs)
    include_directories(${PYTHON_INCLUDE_PATH})
    swig_add_module(coopy python coopy.i)
    #ADD_DEFINITIONS(-DPy_DEBUG)
    find_package(PythonInterp REQUIRED)
    execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "from distutils import sysconfig; print(sysconfig.get_python_lib(1,0,prefix='${CMAKE_INSTALL_PREFIX}'))" 
      OUTPUT_VARIABLE PYTHON_INSTDIR OUTPUT_STRIP_TRAILING_WHITESPACE )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/coopy.py ${CMAKE_CURRENT_BINARY_DIR}/_coopy.so DESTINATION ${PYTHON_INSTDIR} )
    configure_file(examples/example.py ${CMAKE_BINARY_DIR}/example.py)
  endif (CREATE_PYTHON)

  if (CREATE_RUBY)
    find_package(Ruby REQUIRED)
    set(CMAKE_SWIG_FLAGS "-Wall;-module;coopy")
    include_directories(${RUBY_INCLUDE_PATH})
    swig_add_module(coopy ruby coopy.i)
    swig_link_libraries(coopy ${RUBY_LIBRARY})
    configure_file(examples/example.rb ${CMAKE_BINARY_DIR}/example.rb)
    set_target_properties(coopy PROPERTIES PREFIX "")
  endif (CREATE_RUBY)

endif ()
