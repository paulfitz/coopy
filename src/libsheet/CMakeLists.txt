INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libsheet_core/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcsv)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libgnumeric/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(USE_GNUMERIC ${GNUMERIC})
set(BOOK_HOOK ${CMAKE_BINARY_DIR}/generated_code/BookHook.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BookHook.cpp.in ${BOOK_HOOK}
                IMMEDIATE)

set(USE_GNUMERIC)
set(BOOK_HOOK0 ${CMAKE_BINARY_DIR}/generated_code/BookHook0.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BookHook.cpp.in ${BOOK_HOOK0}
                IMMEDIATE)

ADD_LIBRARY(sheet_plus_csv_sql CsvFile.cpp PatchParser.cpp)
TARGET_LINK_LIBRARIES(sheet_plus_csv_sql sheet_core sqlite csv)

ADD_LIBRARY(sheet PolyBook.cpp ${BOOK_HOOK0})
TARGET_LINK_LIBRARIES(sheet sheet_plus_csv_sql)

if (GNUMERIC)
  ADD_LIBRARY(sheet_full PolyBook.cpp ${BOOK_HOOK})
  TARGET_LINK_LIBRARIES(sheet_full gnumeric sheet_plus_csv_sql)
else ()
  ADD_LIBRARY(sheet_full Dummy.cpp)
  TARGET_LINK_LIBRARIES(sheet_full sheet)
endif ()
