if (HAVE_TR1)
  ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
endif ()

INCLUDE_DIRECTORIES(${SHEET_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SQLITE_DIR ${CMAKE_SOURCE_DIR}/src/ssfossil/fossil/src)
INCLUDE_DIRECTORIES(${SQLITE_DIR})

ADD_LIBRARY(coopy_widesheet WideSheetManager.cpp WideSheetManager.h)
TARGET_LINK_LIBRARIES(coopy_widesheet coopy_full coopy_sqlite)

ADD_EXECUTABLE(widesheet widesheet.cpp)
TARGET_LINK_LIBRARIES(widesheet coopy_widesheet)

INSTALL(TARGETS widesheet COMPONENT ${BIGBIN} DESTINATION bin)
