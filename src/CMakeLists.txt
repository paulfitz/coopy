
set(DESTINATION_LIB DESTINATION lib)
if (MINGW)
  set(DESTINATION_LIB RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
  add_definitions(-DNEED_UNIMAIN) # unicode hack
endif ()
include_directories(unicode)

option(PROFILING "Turn on profiling" OFF)
IF (PROFILING)
#  FOREACH(target ssformat ssmerge csv sheet coopy_full remotesql remotesql_sqlxx sheet_core)
#    SET_TARGET_PROPERTIES(${target} PROPERTIES COMPILE_FLAGS -pg LINK_FLAGS -pg)
#  ENDFOREACH()
  ADD_DEFINITIONS(-pg)
  SET(CMAKE_EXE_LINKER_FLAGS "-pg")
  SET(CMAKE_SHARED_LINKER_FLAGS "-pg")
  SET(CMAKE_MODULE_LINKER_FLAGS "-pg")
  LINK_LIBRARIES(profiler) # google-perftools
ENDIF (PROFILING)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" HAVE_TR1)
if (HAVE_TR1)
  # We can't set the -std=c++0x flag globally, since it is an error for C
  # files.  So we make C++ header files check that it has been added in
  # individual targets.
  add_definitions(-DSHOULD_HAVE_TR1)
endif ()

option(USE_SQLITE "Enable Sqlite support" ON)
option(USE_COOPYHX "Enable use of CoopyHX diff algorithm" OFF)
option(USE_JSON "Enable JSON support" ON)
option(USE_GNUMERIC "Enable gnumeric integration (For XLS, ODF, etc)" OFF)
option(USE_ACCESS "Enable Access formats (via mdbtools)" OFF)
option(USE_SOCIALCALC "Enable SocialCalc support (via javascript)" OFF)
option(TEST_JAVASCRIPT "Run Javascript tests" OFF)
option(USE_JACKCESS "Enable Jackcess (experimental)" OFF)
option(CREATE_SHARED_LIBRARIES "Compile shared libraries rather than static" TRUE)
option(JUST_HIGHLIGHT "Pare down for highlighter diffs only" OFF)
option(EMBED_GETOPT "Embed getopt_long" OFF)
option(USE_EMSCRIPTEN "Add targets useful with Emscripten" OFF)
if (CREATE_SHARED_LIBRARIES)
  set(BUILD_SHARED_LIBS ON)
  if (UNIX)
    link_libraries(dl pthread)
  endif()
endif()

if (USE_GNUMERIC)
  option(COMPILE_PLAIN_EXECUTABLES "Compile bins with minimal dependencies" ON)
  link_directories(${GNUMERIC_ROOT}/bin ${GNUMERIC_ROOT}/lib)
else ()
  set(COMPILE_PLAIN_EXECUTABLES OFF)
endif ()

if (COMPILE_PLAIN_EXECUTABLES)
  # un-plain, executables with gnumeric dependencies go in special component
  set (BIGBIN spreadsheet_utilities)
  set (CSVBIN csv_utilities)
  set (BASEBIN shared_utilities)
else ()
  set (BIGBIN utilities)
  set (CSVBIN utilities)
  set (BASEBIN utilities)
endif()

set (BASELIB libraries)

# for binary builds -- completely optional
SET (STATLIB "${CMAKE_BINARY_DIR}/static_libs")
IF (EXISTS ${STATLIB})
    MESSAGE(STATUS "static_libs directory present: ${STATLIB}")
    LINK_DIRECTORIES(${STATLIB})
    ADD_DEFINITIONS(-static-libgcc)
    ADD_DEFINITIONS(-DSQLITE_OMIT_LOAD_EXTENSION=TRUE)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    SET(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -static-libgcc")
    FILE(GLOB statlibs ${STATLIB}/*.a)
    LINK_LIBRARIES(${statlibs})
#    LINK_LIBRARIES(gconf-2 bonobo-2)  # chance not including these
ENDIF (EXISTS ${STATLIB})

export(TARGETS FILE ${COOPY_DEPENDENCIES})

if (TEST_JAVASCRIPT OR USE_SOCIALCALC)
  include(CoopyJS)
  set(USE_JAVASCRIPT TRUE)
  add_subdirectory(libjs)
endif ()

add_subdirectory(libsqlite)
add_subdirectory(libgnumeric)
add_subdirectory(libaccess)
add_subdirectory(libsocialcalc)
add_subdirectory(libjackcess)

add_subdirectory(libcoopy_core)
add_subdirectory(libcoopy_sqldiff)
add_subdirectory(libcoopyhx)
add_subdirectory(libcsv)
add_subdirectory(libremotesql)
add_subdirectory(jsoncpp)
add_subdirectory(libcoopy_full)
add_subdirectory(libcoopy_fold)
set(SHEET_INCLUDE_DIRS 
  ${CMAKE_SOURCE_DIR}/src/libcoopy_core/include 
  ${CMAKE_SOURCE_DIR}/src/libcoopy_fold/include 
  ${CMAKE_SOURCE_DIR}/src/libcsv
  ${CMAKE_SOURCE_DIR}/src/libcsv/include
  ${CMAKE_SOURCE_DIR}/src/libcoopy_full/include)

add_subdirectory(ssmerge)
add_subdirectory(ssdiff)
add_subdirectory(sspatch)
add_subdirectory(ssrender)
add_subdirectory(ssformat)
add_subdirectory(ssfold)
add_subdirectory(ssfossil)
add_subdirectory(widesheet)
add_subdirectory(gui)
add_subdirectory(light)
add_subdirectory(emcc)

SET(TESTS ${CMAKE_SOURCE_DIR}/tests)
set(TEST_STORE ${CMAKE_CURRENT_BINARY_DIR}/test_store)
make_directory(${TEST_STORE})

add_subdirectory(testing)
add_subdirectory(ruby)

include(CoopyDescribe)
