if (HAVE_TR1)
  ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
endif ()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libsheet_core/include)
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

ADD_LIBRARY(sheet_plus_csv_sql CsvFile.cpp PatchParser.cpp 
  Options.cpp Diff.cpp
  CsvTextBook.cpp ShortTextBook.cpp JsonProperty.cpp JsonBook.cpp)

set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Options.cpp 
    APPEND PROPERTY COMPILE_DEFINITIONS 
    COOPY_VERSION=${COOPY_VERSION})


TARGET_LINK_LIBRARIES(sheet_plus_csv_sql sheet_core sqlite csv jsoncpp)
export(TARGETS sheet_plus_csv_sql APPEND FILE ${COOPY_DEPENDENCIES})

ADD_LIBRARY(sheet PolyBook.cpp ${BOOK_HOOK0})
TARGET_LINK_LIBRARIES(sheet sheet_plus_csv_sql)
export(TARGETS sheet APPEND FILE ${COOPY_DEPENDENCIES})

ADD_LIBRARY(sheet_full PolyBook.cpp ${BOOK_HOOK})
TARGET_LINK_LIBRARIES(sheet_full sheet_plus_csv_sql)
export(TARGETS sheet_full APPEND FILE ${COOPY_DEPENDENCIES})
#else ()
#  ADD_LIBRARY(sheet_full Dummy.cpp)
#  TARGET_LINK_LIBRARIES(sheet_full sheet)
#endif ()

if (USE_GNUMERIC)
  TARGET_LINK_LIBRARIES(sheet_full gnumeric)
  export(TARGETS gnumeric APPEND FILE ${COOPY_DEPENDENCIES})
endif ()

if (USE_REMOTE_SQL)
  TARGET_LINK_LIBRARIES(sheet_full remotesql jsoncpp)
  export(TARGETS remotesql jsoncpp APPEND FILE ${COOPY_DEPENDENCIES})
endif ()

IF (USE_ACCESS)
  TARGET_LINK_LIBRARIES(sheet_full access)
  export(TARGETS access APPEND FILE ${COOPY_DEPENDENCIES})
endif ()

