if (HAVE_TR1)
  add_definitions("-std=c++0x" -DHAVE_TR1)
endif ()

include_directories(${CMAKE_SOURCE_DIR}/src/libcoopy_core/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libcsv)
include_directories(${CMAKE_SOURCE_DIR}/src/libgnumeric/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libsqlite/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libremotesql/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libaccess/include)
include_directories(${CMAKE_SOURCE_DIR}/src/jsoncpp/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(BOOK_HOOK ${CMAKE_BINARY_DIR}/generated_code/BookHook.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BookHook.cpp.in ${BOOK_HOOK}
                IMMEDIATE)

set(USE_NONE TRUE)
set(BOOK_HOOK0 ${CMAKE_BINARY_DIR}/generated_code/BookHook0.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BookHook.cpp.in ${BOOK_HOOK0}
                IMMEDIATE)

set(NEED_POLYBOOK  PatchParser.cpp Options.cpp Diff.cpp PoolImpl.cpp Coopy.cpp)

add_library(coopy_csv_sql CsvFile.cpp 
  CsvTextBook.cpp ShortTextBook.cpp JsonProperty.cpp JsonBook.cpp)

set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Options.cpp 
    APPEND PROPERTY COMPILE_DEFINITIONS 
    COOPY_VERSION=${COOPY_VERSION})


target_link_libraries(coopy_csv_sql coopy_core coopy_sqlite 
  coopy_csv coopy_jsoncpp)
export(TARGETS coopy_csv_sql APPEND FILE ${COOPY_DEPENDENCIES})
install(TARGETS coopy_csv_sql COMPONENT ${BASELIB} DESTINATION lib)


if (COMPILE_PLAIN_EXECUTABLES)
  add_library(coopy_plain PolyBook.cpp ${BOOK_HOOK0} ${NEED_POLYBOOK})
  target_link_libraries(coopy_plain coopy_csv_sql)
  export(TARGETS coopy_plain APPEND FILE ${COOPY_DEPENDENCIES})
  install(TARGETS coopy_plain COMPONENT ${BASELIB} DESTINATION lib)
endif ()

add_library(coopy_full PolyBook.cpp ${BOOK_HOOK} ${NEED_POLYBOOK})
target_link_libraries(coopy_full coopy_csv_sql)
export(TARGETS coopy_full APPEND FILE ${COOPY_DEPENDENCIES})
install(TARGETS coopy_full COMPONENT ${BASELIB} DESTINATION lib)

if (USE_GNUMERIC)
  target_link_libraries(coopy_full coopy_gnumeric)
  export(TARGETS coopy_gnumeric APPEND FILE ${COOPY_DEPENDENCIES})
endif ()

if (USE_REMOTE_SQL)
  target_link_libraries(coopy_full coopy_remotesql)
endif ()

if (USE_ACCESS)
  target_link_libraries(coopy_full coopy_access)
  export(TARGETS coopy_access APPEND FILE ${COOPY_DEPENDENCIES})
endif ()

export(TARGETS coopy_jsoncpp APPEND FILE ${COOPY_DEPENDENCIES})
