#SET(WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR})
#SET(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

SET(COOPY_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
SET(CMAKE_MODULE_PATH ${COOPY_MODULE_PATH})

# for binary builds -- completely optional
SET (STATLIB "${CMAKE_BINARY_DIR}/static_libs")
IF (EXISTS ${STATLIB})
    MESSAGE(STATUS "static_libs directory present: ${STATLIB}")
    LINK_DIRECTORIES(${STATLIB})
    ADD_DEFINITIONS(-static-libgcc)
    SET(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -static-libgcc")
    FILE(GLOB statlibs ${STATLIB}/*.a)
    LINK_LIBRARIES(${statlibs})
ENDIF (EXISTS ${STATLIB})

# Force exceptions to be propagated through C code, so we can catch
# when fossil aborts.
#ADD_DEFINITIONS(-fexceptions)

#IF (MINGW)
  # langinfo.h missing?
#  MESSAGE(STATUS "Internationalization may not be working under MSYS right now")
#  ADD_DEFINITIONS(-DFOSSIL_I18N=0)
#ENDIF (MINGW)

INCLUDE_DIRECTORIES(${SHEET_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/ssrender/include)

# May need icons for windows
SET (DECOR)
if(MINGW)
  FIND_PROGRAM(windres_bin NAMES windres i586-mingw32msvc-windres)
  MESSAGE(STATUS "windres: ${windres_bin}")
  IF (windres_bin)
    set(CMAKE_RC_COMPILER ${windres_bin})
    SET(CMAKE_RC_OUTPUT_EXTENSION .obj)  # doesn't work, for some reason...
    SET(CMAKE_RC_COMPILE_OBJECT
      "<CMAKE_RC_COMPILER> --output-format=coff -I${WORKING_DIR}/src -o <OBJECT> <SOURCE>")
    ENABLE_LANGUAGE(RC)
    set(DECOR src/decor.rc)
  ELSE (windres_bin)
    MESSAGE(STATUS "Cannot find windres, ignoring program icon on windows")
  ENDIF (windres_bin)
endif(MINGW)

SET (COMPILE_GUI TRUE CACHE BOOL "Compile GUI?")

IF (COMPILE_GUI)
  FIND_PACKAGE(wxWidgets)
  IF (wxWidgets_FOUND)
    INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})
    ADD_EXECUTABLE(coopy WIN32 src/gui.cpp ${DECOR})
    #TARGET_LINK_LIBRARIES(coopy coopy_fossil)
    TARGET_LINK_LIBRARIES(coopy ${wxWidgets_LIBRARIES})
    SET_PROPERTY(TARGET coopy 
      PROPERTY COMPILE_DEFINITIONS ${wxWidgets_DEFINITIONS})
    IF (NOT MINGW)
      TARGET_LINK_LIBRARIES(coopy X11)
    ENDIF (NOT MINGW)
    INSTALL(TARGETS coopy COMPONENT ui DESTINATION bin)
  ELSE (wxWidgets_FOUND)
    MESSAGE(STATUS "Skipping GUI")
  ENDIF (wxWidgets_FOUND)
ENDIF (COMPILE_GUI)

