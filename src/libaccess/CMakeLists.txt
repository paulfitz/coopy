if (HAVE_TR1)
  ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
endif ()

if (USE_ACCESS)

  find_package(Mdbtools REQUIRED)
  find_package(GLIB2 REQUIRED)

  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcoopy_core/include)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcsv)
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
  INCLUDE_DIRECTORIES(${GLIB2_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${Mdbtools_INCLUDE_DIRS})

  add_library(coopy_access AccessTextBook.cpp AccessSheet.cpp)
  target_link_libraries(coopy_access coopy_core)
  target_link_libraries(coopy_access ${Mdbtools_LIBRARIES} ${GLIB2_LIBRARIES})
  install(TARGETS coopy_access COMPONENT ${BASELIB} ${DESTINATION_LIB})
  if (MINGW)
    target_link_libraries(coopy_access iconv)
  endif()

  add_executable(test_access test_access.cpp)
  target_link_libraries(test_access coopy_access)
  target_link_libraries(test_access ${Mdbtools_LIBRARIES} ${GLIB2_LIBRARIES})

endif ()
