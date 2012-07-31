if (USE_SQLITE)

  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcoopy_core/include)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcoopy_sqldiff/include)
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
  
  # We want Sqlite.  To avoid having two copies, we "steal" the
  # copy in fossil.  It would be trivial to replace this with
  # a standard sqlite amalgamation if needed.
  
  SET(SQLITE_DIR ${CMAKE_SOURCE_DIR}/src/ssfossil/fossil/src)
  INCLUDE_DIRECTORIES(${SQLITE_DIR})
  
  ADD_LIBRARY(coopy_sqlite SqliteTextBook.cpp SqliteSheet.cpp 
    ${SQLITE_DIR}/sqlite3.c)
  
  TARGET_LINK_LIBRARIES(coopy_sqlite coopy_core coopy_sqldiff)
  IF (UNIX)
    TARGET_LINK_LIBRARIES(coopy_sqlite dl pthread)
  ENDIF (UNIX)
  export(TARGETS coopy_sqlite APPEND FILE ${COOPY_DEPENDENCIES})
  install(TARGETS coopy_sqlite COMPONENT ${BASELIB} DESTINATION lib)
  
  ADD_EXECUTABLE(test_sqlite test_sqlite.cpp)
  TARGET_LINK_LIBRARIES(test_sqlite coopy_sqlite)
  
  if (HAVE_TR1)
    SET (TR1 "-std=c++0x -DHAVE_TR1")
    SET_PROPERTY(SOURCE SqliteTextBook.cpp APPEND PROPERTY COMPILE_FLAGS ${TR1})
    SET_PROPERTY(SOURCE SqliteSheet.cpp APPEND PROPERTY COMPILE_FLAGS ${TR1})
    SET_PROPERTY(SOURCE test_sqlite.cpp APPEND PROPERTY COMPILE_FLAGS ${TR1})
  endif ()
endif ()
