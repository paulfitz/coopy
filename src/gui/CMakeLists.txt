SET (COMPILE_GUI TRUE CACHE BOOL "Compile GUI?")

IF (COMPILE_GUI)

  if (HAVE_TR1)
    ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
  endif ()

  SET(COOPY_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
  SET(CMAKE_MODULE_PATH ${COOPY_MODULE_PATH})

  INCLUDE_DIRECTORIES(${SHEET_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/ssrender/include)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/widesheet)

  # May need icons for windows
  SET (DECOR)
  if(MINGW)
    FIND_PROGRAM(windres_bin NAMES windres i586-mingw32msvc-windres)
    MESSAGE(STATUS "windres: ${windres_bin}")
    IF (windres_bin)
      set(CMAKE_RC_COMPILER ${windres_bin})
      SET(CMAKE_RC_OUTPUT_EXTENSION .obj)  # doesn't work, for some reason...
      SET(CMAKE_RC_COMPILE_OBJECT
        "<CMAKE_RC_COMPILER> --output-format=coff -I${CMAKE_CURRENT_SOURCE_DIR}/src -I${CMAKE_CURRENT_SOURCE_DIR} -o <OBJECT> <SOURCE>")
      ENABLE_LANGUAGE(RC)
      set(DECOR src/decor.rc)
    ELSE (windres_bin)
      MESSAGE(STATUS "Cannot find windres, ignoring program icon on windows")
    ENDIF (windres_bin)
  endif(MINGW)

  FIND_PACKAGE(wxWidgets)
  IF (wxWidgets_FOUND)
    INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})

    FOREACH(targ coopy ssmerge_gui)
      ADD_EXECUTABLE(${targ} WIN32 src/${targ}.cpp ${DECOR})
      TARGET_LINK_LIBRARIES(${targ} ${wxWidgets_LIBRARIES})
      SET_PROPERTY(TARGET ${targ} 
        PROPERTY COMPILE_DEFINITIONS ${wxWidgets_DEFINITIONS})
      IF (NOT MINGW)
        TARGET_LINK_LIBRARIES(${targ} X11)
      ENDIF (NOT MINGW)
      INSTALL(TARGETS ${targ} COMPONENT wizard DESTINATION bin)
    ENDFOREACH ()
    TARGET_LINK_LIBRARIES(ssmerge_gui sheet)
    TARGET_LINK_LIBRARIES(coopy widesheet_management)

  ELSE (wxWidgets_FOUND)
    MESSAGE(STATUS "Skipping GUI, wxWidgets not found")
  ENDIF (wxWidgets_FOUND)
ENDIF (COMPILE_GUI)

