option(USE_REMOTE_SQL OFF "Enable remote sql access")
if (USE_REMOTE_SQL)

option(USE_MYSQL ON "Enable mysql access")

# option(USE_POSTGRES OFF "Enable postgres access")
# option(USE_ODBC OFF "Enable odbc access")
set (USE_POSTGRES OFF)
set (USE_ODBC OFF)

IF (USE_MYSQL)
  ADD_DEFINITIONS(-DUSE_MYSQL)
  FIND_PATH(INCLUDE_MYSQL mysql/mysql.h)
  IF (NOT INCLUDE_MYSQL)
    MESSAGE(FATAL_ERROR "Cannot find mysqlclient header files")
  ENDIF ()
  INCLUDE_DIRECTORIES(${INCLUDE_MYSQL}/mysql)
  FIND_LIBRARY(LIB_MYSQL mysqlclient)
  IF (NOT LIB_MYSQL)
    MESSAGE(FATAL_ERROR "Cannot find mysqlclient library")
  ENDIF ()
  LINK_LIBRARIES(${LIB_MYSQL})
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

  ADD_LIBRARY(remotesql RemoteSqlSheet.cpp 
			RemoteSqlTextBook.cpp
			include/coopy/RemoteSqlSheet.h
			include/coopy/RemoteSqlTextBook.h)
  TARGET_LINK_LIBRARIES(remotesql remotesql_sqlxx sheet_core)

  ADD_EXECUTABLE(test_sqlxx sqlxx/test.cpp)
  TARGET_LINK_LIBRARIES(test_sqlxx remotesql)

  ADD_EXECUTABLE(test_remotesql test_remotesql.cpp)
  TARGET_LINK_LIBRARIES(test_remotesql remotesql)

ENDIF ()

endif()