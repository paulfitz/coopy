option(CREATE_SHARED_LIBRARIES "Compile shared libraries rather than static")
if (CREATE_SHARED_LIBRARIES)
  set(BUILD_SHARED_LIBS ON)
  if (UNIX)
    link_libraries(dl pthread)
  endif()
endif()

add_subdirectory(libsheet_core)
add_subdirectory(libcsv)
add_subdirectory(libgnumeric)
add_subdirectory(libsheet)
set(SHEET_INCLUDE_DIRS 
  ${CMAKE_SOURCE_DIR}/src/libsheet_core/include 
  ${CMAKE_SOURCE_DIR}/src/libcsv 
  ${CMAKE_SOURCE_DIR}/src/libgnumeric/include 
  ${CMAKE_SOURCE_DIR}/src/libsheet/include)

add_subdirectory(ssmerge)
add_subdirectory(ssdiff)
add_subdirectory(sspatch)
add_subdirectory(ssrender)
add_subdirectory(ssformat)
add_subdirectory(ssfossil)
add_subdirectory(gui)

add_subdirectory(testing)

option(PROFILING OFF "Turn on profiling")
IF (PROFILING)
  FOREACH(target ssmerge csv sheet sheet_core)
    SET_TARGET_PROPERTIES(${target} PROPERTIES COMPILE_FLAGS -pg LINK_FLAGS -pg)
  ENDFOREACH()
ENDIF (PROFILING)

option(GNUMERIC OFF "Turn on gnumeric integration")
if (GNUMERIC)
  ADD_SUBDIRECTORY(ssinsert)
ENDIF ()
