if (HAVE_TR1)
  ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
endif ()

if (USE_ACCESS)

  find_package(Mdbtools REQUIRED)
  find_package(GLIB2 REQUIRED)

  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libsheet_core/include)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcsv)
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
  INCLUDE_DIRECTORIES(${GLIB2_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${Mdbtools_INCLUDE_DIRS})

  # tweak for apple build
  if (APPLE)
    option(NEED_FAKE_INTL "create a fake intl library (may be needed with OSX)" TRUE)
  endif()
  if (NEED_FAKE_INTL)
    add_library(intl ../libgnumeric/dummy.c)
    link_libraries(intl)
  endif()


  add_library(access AccessTextBook.cpp AccessSheet.cpp)
  target_link_libraries(access sheet_core)
  target_link_libraries(access ${Mdbtools_LIBRARIES} ${GLIB2_LIBRARIES})

  add_executable(test_access test_access.cpp)
  target_link_libraries(test_access access)
  target_link_libraries(test_access ${Mdbtools_LIBRARIES} ${GLIB2_LIBRARIES})

endif ()
