option(PROFILING OFF "Turn on profiling")
IF (PROFILING)
#  FOREACH(target ssformat ssmerge csv sheet sheet_full remotesql remotesql_sqlxx sheet_core)
#    SET_TARGET_PROPERTIES(${target} PROPERTIES COMPILE_FLAGS -pg LINK_FLAGS -pg)
#  ENDFOREACH()
  ADD_DEFINITIONS(-pg)
  SET(CMAKE_EXE_LINKER_FLAGS "-pg")
  SET(CMAKE_SHARED_LINKER_FLAGS "-pg")
  SET(CMAKE_MODULE_LINKER_FLAGS "-pg")
ENDIF (PROFILING)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" HAVE_TR1)

option(GNUMERIC OFF "Enable gnumeric integration (For XLS, ODF, etc)")
option(CREATE_SHARED_LIBRARIES "Compile shared libraries rather than static")
if (CREATE_SHARED_LIBRARIES)
  set(BUILD_SHARED_LIBS ON)
  if (UNIX)
    link_libraries(dl pthread)
  endif()
endif()

if (GNUMERIC)
  set (COMPILE_PLAIN_EXECUTABLES TRUE)
endif ()

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



add_subdirectory(libcsv)
add_subdirectory(libsqlite)
add_subdirectory(libgnumeric)

add_subdirectory(libsheet_core)
add_subdirectory(libremotesql)
add_subdirectory(jsoncpp)
add_subdirectory(libsheet)
set(SHEET_INCLUDE_DIRS 
  ${CMAKE_SOURCE_DIR}/src/libsheet_core/include 
  ${CMAKE_SOURCE_DIR}/src/libcsv 
  ${CMAKE_SOURCE_DIR}/src/libsheet/include)

add_subdirectory(ssmerge)
add_subdirectory(ssdiff)
add_subdirectory(sspatch)
add_subdirectory(ssrender)
add_subdirectory(ssformat)
add_subdirectory(ssfossil)
add_subdirectory(gui)

add_subdirectory(testing)

