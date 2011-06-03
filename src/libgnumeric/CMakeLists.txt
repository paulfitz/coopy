
if (USE_GNUMERIC)

  # for binary builds -- completely optional
  SET (STATLIB "${CMAKE_BINARY_DIR}/static_libs_gnumeric")
  SET (ADD_LIBS)
  IF (EXISTS ${STATLIB})
      MESSAGE(STATUS "static_libs directory present: ${STATLIB}")
      LINK_DIRECTORIES(${STATLIB})
      ADD_DEFINITIONS(-static-libgcc)
      FILE(GLOB statlibs ${STATLIB}/*.a)
      SET(ADD_LIBS ${statlibs} gconf-2 bonobo-2)
  ENDIF (EXISTS ${STATLIB})


  if (NOT MINGW)
    include(FindPkgConfig)
    pkg_check_modules (GLIB2 REQUIRED glib-2.0)
    include_directories(${GLIB2_INCLUDE_DIRS})
    pkg_search_module (GOFFICE REQUIRED libgoffice-1.0 libgoffice-0.9 libgoffice-0.8 libgoffice-1)
    include_directories(${GOFFICE_INCLUDE_DIRS})
    pkg_search_module (GOBJECT REQUIRED gobject-2.0)
    include_directories(${GOBJECT_INCLUDE_DIRS})
    pkg_search_module (GSF REQUIRED libgsf-1)
    pkg_search_module (GDK_PIXBUF REQUIRED gdk-pixbuf-2.0)
    include_directories(${GDK_PIXBUF_INCLUDE_DIRS})
    pkg_search_module (GNOMEPRINTUI libgnomeprintui-2.2)
  else ()
    link_libraries(goffice-0.8 gobject-2.0)
  endif ()

  set(GNUMERIC_ROOT "" CACHE STRING "Location of Gnumeric source") 
  message(STATUS "GNUMERIC_ROOT=${GNUMERIC_ROOT}")

  if (GNUMERIC_ROOT)
    include_directories(${GNUMERIC_ROOT})
    include_directories(${GNUMERIC_ROOT}/src)
    # accept also mingw cross-compiling install directory
    include_directories(${GNUMERIC_ROOT}/include)
    include_directories(${GNUMERIC_ROOT}/include/glib-2.0)
    include_directories(${GNUMERIC_ROOT}/lib/glib-2.0/include)
    include_directories(${GNUMERIC_ROOT}/include/libgoffice-0.8)
    include_directories(${GNUMERIC_ROOT}/include/gtk-2.0)
    include_directories(${GNUMERIC_ROOT}/lib/gtk-2.0/include)
    include_directories(${GNUMERIC_ROOT}/include/cairo)
    include_directories(${GNUMERIC_ROOT}/include/pango-1.0)
    include_directories(${GNUMERIC_ROOT}/include/libgsf-1)
    include_directories(${GNUMERIC_ROOT}/include/atk-1.0)
    include_directories(${GNUMERIC_ROOT}/include/libxml2)
    include_directories(${GNUMERIC_ROOT}/include/libspreadsheet-1.10/spreadsheet/)
    link_directories(${GNUMERIC_ROOT}/bin ${GNUMERIC_ROOT}/lib)
  endif ()

  option(INSTALL_GNUMERIC "Add components of gnumeric to install (for packaging)" OFF)
  
  if (INSTALL_GNUMERIC)
    SET(PERMS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
    install(DIRECTORY ${GNUMERIC_ROOT}/bin/ DESTINATION bin FILE_PERMISSIONS ${PERMS} COMPONENT ${BIGBIN} FILES_MATCHING PATTERN "*.dll")
    install(DIRECTORY ${GNUMERIC_ROOT}/bin/ DESTINATION bin FILE_PERMISSIONS ${PERMS} COMPONENT ${BIGBIN} FILES_MATCHING PATTERN "ss*.exe")
    install(DIRECTORY ${GNUMERIC_ROOT}/bin/ DESTINATION bin FILE_PERMISSIONS ${PERMS} COMPONENT ${BIGBIN} FILES_MATCHING PATTERN "gnumeric.exe")
    install(DIRECTORY ${GNUMERIC_ROOT}/bin/ DESTINATION bin FILE_PERMISSIONS ${PERMS} COMPONENT ${BIGBIN} FILES_MATCHING PATTERN "g-spawn*.exe")
    install(DIRECTORY ${GNUMERIC_ROOT}/bin/ DESTINATION bin FILE_PERMISSIONS ${PERMS} COMPONENT ${BIGBIN} FILES_MATCHING PATTERN "gtk*.exe")
    install(DIRECTORY ${GNUMERIC_ROOT}/bin/ DESTINATION bin FILE_PERMISSIONS ${PERMS} COMPONENT ${BIGBIN} FILES_MATCHING PATTERN "gdk*.exe")
    install(DIRECTORY ${GNUMERIC_ROOT}/etc/ DESTINATION etc COMPONENT ${BIGBIN} FILES_MATCHING PATTERN conf.d EXCLUDE PATTERN "*")
    install(DIRECTORY ${GNUMERIC_ROOT}/share/gtk-2.0 DESTINATION share COMPONENT ${BIGBIN})
    install(DIRECTORY ${GNUMERIC_ROOT}/share/gnumeric DESTINATION share COMPONENT ${BIGBIN})
    install(DIRECTORY ${GNUMERIC_ROOT}/lib/gnumeric DESTINATION lib FILE_PERMISSIONS ${PERMS} COMPONENT ${BIGBIN})
    install(DIRECTORY ${GNUMERIC_ROOT}/lib/goffice DESTINATION lib FILE_PERMISSIONS ${PERMS} COMPONENT ${BIGBIN})
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/tests/ DESTINATION share/coopy/examples COMPONENT examples FILES_MATCHING PATTERN "*.xls")
  endif ()

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

  target_link_libraries(gnumeric spreadsheet)
  target_link_libraries(gnumeric ${GLIB2_LIBRARIES})
  target_link_libraries(gnumeric ${GOBJECT_LIBRARIES})
  target_link_libraries(gnumeric ${GOFFICE_LIBRARIES})
  target_link_libraries(gnumeric ${GSF_LIBRARIES})
  target_link_libraries(gnumeric ${GNOMEPRINTUI_LIBRARIES})

  target_link_libraries(gnumeric sheet_core)
  target_link_libraries(gnumeric ${ADD_LIBS})

  add_executable(test_gnumeric test_gnumeric.cpp)
  target_link_libraries(test_gnumeric gnumeric)

  add_executable(gnumeric_ss2html ss2html.cpp)
  target_link_libraries(gnumeric_ss2html gnumeric)

endif ()
