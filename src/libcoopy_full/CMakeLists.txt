#if (HAVE_TR1)
#  add_definitions("-std=c++0x" -DHAVE_TR1)
#endif ()

include_directories(${CMAKE_SOURCE_DIR}/src/libcoopy_core/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libcsv/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libgnumeric/include)
if (USE_SQLITE)
  include_directories(${CMAKE_SOURCE_DIR}/src/libsqlite/include)
endif ()
include_directories(${CMAKE_SOURCE_DIR}/src/libremotesql/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libaccess/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libsocialcalc/include)
if (USE_JSON)
  include_directories(${CMAKE_SOURCE_DIR}/src/jsoncpp/include)
endif ()
include_directories(${CMAKE_SOURCE_DIR}/src/libjackcess/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
if (USE_COOPYHX)
  include_directories(${CMAKE_SOURCE_DIR}/src/libcoopyhx/include)
endif ()

set(BOOK_HOOK ${CMAKE_BINARY_DIR}/generated_code/BookHook.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BookHook.cpp.in ${BOOK_HOOK}
                IMMEDIATE)

set(USE_NONE TRUE)
set(BOOK_HOOK0 ${CMAKE_BINARY_DIR}/generated_code/BookHook0.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BookHook.cpp.in ${BOOK_HOOK0}
                IMMEDIATE)

configure_file(${CMAKE_SOURCE_DIR}/conf/CoopyVersion.h.in 
  ${CMAKE_BINARY_DIR}/generated_code/coopy/CoopyVersion.h
  @ONLY)
include_directories(${CMAKE_BINARY_DIR}/generated_code)

set(NEED_POLYBOOK  PatchParser.cpp Options.cpp Diff.cpp PoolImpl.cpp Coopy.cpp)

set(JSON_ADDITIONS)
if (USE_JSON)
  set(JSON_ADDITIONS JsonProperty.cpp JsonBook.cpp)
endif ()

add_library(coopy_csv_sql 
  CsvTextBook.cpp ShortTextBook.cpp ${JSON_ADDITIONS})

set(GETOPT_ADDITIONS)
if (EMBED_GETOPT)
  set(GETOPT_ADDITIONS 
    ${CMAKE_CURRENT_SOURCE_DIR}/gnulib_getopt/getopt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gnulib_getopt/getopt1.c)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Options.cpp 
    APPEND PROPERTY COMPILE_DEFINITIONS 
    USE_GNULIB_GETOPT=1)
endif ()

# set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Options.cpp 
#     APPEND PROPERTY COMPILE_DEFINITIONS 
#     COOPY_VERSION=${COOPY_VERSION})


target_link_libraries(coopy_csv_sql coopy_csv)
if (USE_SQLITE)
   target_link_libraries(coopy_csv_sql coopy_sqlite)
endif ()
if (USE_JSON)
   target_link_libraries(coopy_csv_sql coopy_jsoncpp)
endif ()
if (USE_COOPYHX)
  add_definitions(-DUSE_COOPYHX)
  target_link_libraries(coopy_csv_sql coopy_coopyhx)
endif ()
export(TARGETS coopy_csv_sql APPEND FILE ${COOPY_DEPENDENCIES})
install(TARGETS coopy_csv_sql COMPONENT ${BASELIB} DESTINATION lib)


foreach(src PolyBook.cpp ${BOOK_HOOK0} ${NEED_POLYBOOK})
  if (HAVE_TR1)
    set_property(SOURCE ${src} 
      APPEND PROPERTY COMPILE_FLAGS "-std=c++0x")
    set_property(SOURCE ${src} 
      APPEND PROPERTY COMPILE_DEFINITIONS HAVE_TR1)
  endif ()
endforeach()

if (COMPILE_PLAIN_EXECUTABLES)
  add_library(coopy_plain PolyBook.cpp ${BOOK_HOOK0} ${NEED_POLYBOOK} ${GETOPT_ADDITIONS} ${CMAKE_BINARY_DIR}/generated_code/coopy/CoopyVersion.h)
  target_link_libraries(coopy_plain coopy_csv_sql)
  export(TARGETS coopy_plain APPEND FILE ${COOPY_DEPENDENCIES})
  install(TARGETS coopy_plain COMPONENT ${BASELIB} DESTINATION lib)
endif ()

add_library(coopy_full PolyBook.cpp ${BOOK_HOOK} ${NEED_POLYBOOK} ${GETOPT_ADDITIONS} ${CMAKE_BINARY_DIR}/generated_code/coopy/CoopyVersion.h)
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

if (USE_SOCIALCALC)
  target_link_libraries(coopy_full coopy_socialcalc)
  export(TARGETS coopy_socialcalc APPEND FILE ${COOPY_DEPENDENCIES})
endif ()

if (USE_JACKCESS)
  target_link_libraries(coopy_full coopy_jackcess)
  export(TARGETS coopy_jackcess APPEND FILE ${COOPY_DEPENDENCIES})
endif ()

if (USE_JSON)
  export(TARGETS coopy_jsoncpp APPEND FILE ${COOPY_DEPENDENCIES})
endif ()
