INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libsheet_core/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcsv)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

# We want Sqlite.  To avoid having two copies, we "steal" the
# copy in fossil.  It would be trivial to replace this with
# a standard sqlite amalgamation if needed.

SET(SQLITE_DIR ${CMAKE_SOURCE_DIR}/src/ssfossil/fossil/src)
INCLUDE_DIRECTORIES(${SQLITE_DIR})

ADD_LIBRARY(sheet include/coopy/CsvFile.h CsvFile.cpp ${SQLITE_DIR}/sqlite3.c
  SqliteTextBook.cpp PolyBook.cpp)
TARGET_LINK_LIBRARIES(sheet sheet_core csv)
