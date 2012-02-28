if (HAVE_TR1)
  ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
endif ()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcoopy_core/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcsv)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libgnumeric/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libsqlite/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libremotesql/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libaccess/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/jsoncpp/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(BOOK_HOOK ${CMAKE_BINARY_DIR}/generated_code/BookHook.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BookHook.cpp.in ${BOOK_HOOK}
                IMMEDIATE)

set(USE_NONE TRUE)
set(BOOK_HOOK0 ${CMAKE_BINARY_DIR}/generated_code/BookHook0.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BookHook.cpp.in ${BOOK_HOOK0}
                IMMEDIATE)

set(NEED_POLYBOOK  PatchParser.cpp Options.cpp Diff.cpp PoolImpl.cpp Coopy.cpp)

ADD_LIBRARY(coopy_csv_sql CsvFile.cpp 
  CsvTextBook.cpp ShortTextBook.cpp JsonProperty.cpp JsonBook.cpp)

set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Options.cpp 
    APPEND PROPERTY COMPILE_DEFINITIONS 
    COOPY_VERSION=${COOPY_VERSION})


TARGET_LINK_LIBRARIES(coopy_csv_sql coopy_core coopy_sqlite 
  coopy_csv coopy_jsoncpp)
export(TARGETS coopy_csv_sql APPEND FILE ${COOPY_DEPENDENCIES})

if (COMPILE_PLAIN_EXECUTABLES)
  ADD_LIBRARY(sheet PolyBook.cpp ${BOOK_HOOK0} ${NEED_POLYBOOK})
  TARGET_LINK_LIBRARIES(sheet coopy_csv_sql)
  export(TARGETS sheet APPEND FILE ${COOPY_DEPENDENCIES})
endif ()

ADD_LIBRARY(coopy_full PolyBook.cpp ${BOOK_HOOK} ${NEED_POLYBOOK})
TARGET_LINK_LIBRARIES(coopy_full coopy_csv_sql)
export(TARGETS coopy_full APPEND FILE ${COOPY_DEPENDENCIES})

if (USE_GNUMERIC)
  TARGET_LINK_LIBRARIES(coopy_full coopy_gnumeric)
  export(TARGETS coopy_gnumeric APPEND FILE ${COOPY_DEPENDENCIES})
endif ()

if (USE_REMOTE_SQL)
  TARGET_LINK_LIBRARIES(coopy_full coopy_remotesql)
endif ()

IF (USE_ACCESS)
  TARGET_LINK_LIBRARIES(coopy_full coopy_access)
  export(TARGETS coopy_access APPEND FILE ${COOPY_DEPENDENCIES})
endif ()

export(TARGETS coopy_jsoncpp APPEND FILE ${COOPY_DEPENDENCIES})
