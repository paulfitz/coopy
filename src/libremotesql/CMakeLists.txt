option(USE_MYSQL OFF "Enable mysql access")
option(USE_POSTGRES OFF "Enable postgres access")
option(USE_ODBC OFF "Enable odbc access")

IF (USE_MYSQL)
  ADD_DEFINITIONS(-DUSE_MYSQL)
  INCLUDE_DIRECTORIES(/usr/include/mysql)
  LINK_LIBRARIES(mysqlclient)
ENDIF ()

IF (USE_POSTGRES)
  ADD_DEFINITIONS(-DUSE_POSTGRES)
  INCLUDE_DIRECTORIES(/usr/include/postgresql)
  LINK_LIBRARIES(pq)
ENDIF ()

IF (USE_ODBC)
  ADD_DEFINITIONS(-DUSE_ODBC)
  INCLUDE_DIRECTORIES(/usr/include/iodbc)
  LINK_LIBRARIES(iodbc)
ENDIF ()

IF (USE_MYSQL OR USE_POSTGRES OR USE_ODBC)

  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/sqlxx)
  ADD_LIBRARY(remotesql_sqlxx sqlxx/sqlxx.cc sqlxx/strutilsxx.cc)

  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libsheet_core/include)
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

  ADD_LIBRARY(remotesql RemoteSqlSheet.cpp RemoteSqlTextBook.cpp)
  TARGET_LINK_LIBRARIES(remotesql remotesql_sqlxx sheet_core)

  ADD_EXECUTABLE(test_remotesql sqlxx/test.cpp)
  TARGET_LINK_LIBRARIES(test_remotesql remotesql)

ENDIF ()