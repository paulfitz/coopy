if (HAVE_TR1)
  ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
endif ()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libsheet_core/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libsheet/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

ADD_LIBRARY(sheet_fold FoldTool.cpp include/coopy/FoldTool.h)
TARGET_LINK_LIBRARIES(sheet_fold sheet_full)
export(TARGETS sheet_fold APPEND FILE ${COOPY_DEPENDENCIES})

