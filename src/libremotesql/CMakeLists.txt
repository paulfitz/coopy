if (HAVE_TR1)
  add_definitions("-std=c++0x" -DHAVE_TR1)
endif ()

option(USE_REMOTE_SQL "Enable remote sql access" OFF)
if (USE_REMOTE_SQL)

option(USE_MYSQL "Enable mysql access" ON)
option(USE_POSTGRES "Enable postgresql access" ON)

# option(USE_ODBC OFF "Enable odbc access")
set (USE_ODBC OFF)

#if (NOT USE_MYSQL)
#  message(FATAL_ERROR "Please turn on USE_MYSQL")
#endif ()

if (USE_MYSQL)
  add_definitions(-DUSE_MYSQL)
  find_path(INCLUDE_MYSQL mysql/mysql.h)
  if (NOT INCLUDE_MYSQL)
    find_path(INCLUDE_MYSQL mysql.h)
  endif ()
  if (NOT INCLUDE_MYSQL)
    message(FATAL_ERROR "Cannot find mysqlclient header files")
  endif ()
  include_directories(${INCLUDE_MYSQL}/mysql)
  include_directories(${INCLUDE_MYSQL})
  find_library(LIB_MYSQL mysqlclient /usr/lib /usr/lib/x86_64-linux-gnu)
  if (NOT LIB_MYSQL)
    message(FATAL_ERROR "Cannot find mysqlclient library")
  endif ()
  link_libraries(${LIB_MYSQL} z)
  if (MINGW)
    link_libraries(${LIB_MYSQL} ws2_32 mswsock netapi32)
  endif ()
ENDIF ()

if (USE_POSTGRES)
  add_definitions(-DUSE_POSTGRES)
  include_directories(/usr/include/postgresql)
  link_libraries(pq)
endif ()

if (USE_ODBC)
  add_definitions(-DUSE_ODBC)
  include_directories(/usr/include/iodbc)
  link_libraries(iodbc)
endif ()

if (USE_MYSQL OR USE_POSTGRES OR USE_ODBC)

  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sqlxx)
  add_library(coopy_remotesql_sqlxx sqlxx/sqlxx.cc sqlxx/strutilsxx.cc)
  export(TARGETS coopy_remotesql_sqlxx APPEND FILE ${COOPY_DEPENDENCIES})
  install(TARGETS coopy_remotesql_sqlxx COMPONENT ${BASELIB} ${DESTINATION_LIB})

  include_directories(${CMAKE_SOURCE_DIR}/src/libcoopy_core/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

  add_library(coopy_remotesql RemoteSqlSheet.cpp 
    RemoteSqlTextBook.cpp
    include/coopy/RemoteSqlSheet.h
    include/coopy/RemoteSqlTextBook.h)
  target_link_libraries(coopy_remotesql coopy_remotesql_sqlxx coopy_core)
  export(TARGETS coopy_remotesql APPEND FILE ${COOPY_DEPENDENCIES})
  install(TARGETS coopy_remotesql COMPONENT ${BASELIB} ${DESTINATION_LIB})

  add_executable(test_sqlxx sqlxx/test.cpp)
  target_link_libraries(test_sqlxx coopy_remotesql)

  add_executable(test_remotesql test_remotesql.cpp)
  target_link_libraries(test_remotesql coopy_remotesql)

ENDIF ()

endif()
