INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libsheet_core/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcsv)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libgnumeric/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

# We want Sqlite.  To avoid having two copies, we "steal" the
# copy in fossil.  It would be trivial to replace this with
# a standard sqlite amalgamation if needed.

SET(SQLITE_DIR ${CMAKE_SOURCE_DIR}/src/ssfossil/fossil/src)
INCLUDE_DIRECTORIES(${SQLITE_DIR})

set(BOOK_HOOK ${CMAKE_BINARY_DIR}/generated_code/BookHook.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BookHook.cpp.in ${BOOK_HOOK}
                IMMEDIATE)

ADD_LIBRARY(sheet include/coopy/CsvFile.h CsvFile.cpp ${SQLITE_DIR}/sqlite3.c
  SqliteTextBook.cpp PolyBook.cpp ${BOOK_HOOK})
TARGET_LINK_LIBRARIES(sheet sheet_core csv)
if (GNUMERIC)
  TARGET_LINK_LIBRARIES(sheet gnumeric)
endif ()
