INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libsheet_core/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

# We want Sqlite.  To avoid having two copies, we "steal" the
# copy in fossil.  It would be trivial to replace this with
# a standard sqlite amalgamation if needed.

SET(SQLITE_DIR ${CMAKE_SOURCE_DIR}/src/ssfossil/fossil/src)
INCLUDE_DIRECTORIES(${SQLITE_DIR})

ADD_LIBRARY(sqlite ${SQLITE_DIR}/sqlite3.c SqliteTextBook.cpp SqliteSheet.cpp)

TARGET_LINK_LIBRARIES(sqlite sheet_core)
IF (UNIX)
  TARGET_LINK_LIBRARIES(sqlite dl pthread)
ENDIF (UNIX)

ADD_EXECUTABLE(test_sqlite test_sqlite.cpp)
TARGET_LINK_LIBRARIES(test_sqlite sqlite)

if (HAVE_TR1)
  SET (TR1 "-std=c++0x -DHAVE_TR1")
  SET_PROPERTY(SOURCE SqliteTextBook.cpp APPEND PROPERTY COMPILE_FLAGS ${TR1})
  SET_PROPERTY(SOURCE SqliteSheet.cpp APPEND PROPERTY COMPILE_FLAGS ${TR1})
  SET_PROPERTY(SOURCE test_sqlite.cpp APPEND PROPERTY COMPILE_FLAGS ${TR1})
endif ()

