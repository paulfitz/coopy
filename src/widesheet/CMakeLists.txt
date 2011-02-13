INCLUDE_DIRECTORIES(${SHEET_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SQLITE_DIR ${CMAKE_SOURCE_DIR}/src/ssfossil/fossil/src)
INCLUDE_DIRECTORIES(${SQLITE_DIR})

ADD_LIBRARY(widesheet_management WideSheetManager.cpp WideSheetManager.h)
TARGET_LINK_LIBRARIES(widesheet_management sheet_full sqlite)

ADD_EXECUTABLE(widesheet widesheet.cpp)
TARGET_LINK_LIBRARIES(widesheet widesheet_management)

INSTALL(TARGETS widesheet COMPONENT ${BIGBIN} DESTINATION bin)
