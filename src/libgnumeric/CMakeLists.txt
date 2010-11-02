
if (USE_GNUMERIC)

  include(FindPkgConfig)
  pkg_check_modules (GLIB2 REQUIRED glib-2.0)
  include_directories(${GLIB2_INCLUDE_DIRS})
  pkg_search_module (GOFFICE REQUIRED libgoffice-1.0 libgoffice-0.9 libgoffice-0.8 libgoffice-1)
  include_directories(${GOFFICE_INCLUDE_DIRS})
  pkg_search_module (GOBJECT REQUIRED gobject-2.0)
  include_directories(${GOBJECT_INCLUDE_DIRS})
  pkg_search_module (GSF REQUIRED libgsf-1)

  link_libraries(${GLIB2_LIBRARIES})
  link_libraries(${GOBJECT_LIBRARIES})
  link_libraries(${GOFFICE_LIBRARIES})
  link_libraries(${GSF_LIBRARIES})

  set(GNUMERIC_ROOT "" CACHE STRING "Location of Gnumeric source") 
  message(STATUS "GNUMERIC_ROOT=${GNUMERIC_ROOT}")

  include_directories(${GNUMERIC_ROOT})
  include_directories(${GNUMERIC_ROOT}/src)
  # accept also mingw cross-compiling install directory
  include_directories(${GNUMERIC_ROOT}/include)
  include_directories(${GNUMERIC_ROOT}/include/libspreadsheet-1.10/spreadsheet/)
  link_directories(${GNUMERIC_ROOT}/bin ${GNUMERIC_ROOT}/lib)
  link_libraries(spreadsheet)

  # tweak for mingw build
  if (MINGW)
    option(NEED_FAKE_RT "create a fake rt library (may be needed with MINGW)" TRUE)
  endif()
  if (NEED_FAKE_RT)
    add_library(rt dummy.c)
    link_libraries(intl)
  endif()

  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libsheet_core/include)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcsv)
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

  add_library(gnumeric include/coopy/gnumeric_link.h gnumeric_link.c 
		       GnumericTextBook.cpp GnumericTextBookFactory.cpp
		       GnumericSheet.cpp)
  target_link_libraries(gnumeric sheet_core)

  add_executable(test_gnumeric test_gnumeric.cpp)
  target_link_libraries(test_gnumeric gnumeric)

  add_executable(ss2html ss2html.cpp)
  target_link_libraries(ss2html gnumeric)

endif ()
