
if (USE_ACCESS)

  find_package(Mdbtools REQUIRED)
  find_package(GLIB2 REQUIRED)

  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libsheet_core/include)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcsv)
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
  INCLUDE_DIRECTORIES(${GLIB2_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${Mdbtools_INCLUDE_DIRS})

  #add_library(access include/coopy/gnumeric_link.h gnumeric_link.c 
  #  GnumericTextBook.cpp GnumericTextBookFactory.cpp
  #  GnumericSheet.cpp)
  #target_link_libraries(gnumeric sheet_core)

  add_executable(test_access test_access.cpp)
  target_link_libraries(test_access ${Mdbtools_LIBRARIES} ${GLIB2_LIBRARIES})

endif ()
