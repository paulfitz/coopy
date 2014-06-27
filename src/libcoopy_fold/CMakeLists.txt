if (HAVE_TR1)
  ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
endif ()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcoopy_core/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcsv/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcoopy_full/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

ADD_LIBRARY(coopy_fold FoldTool.cpp include/coopy/FoldTool.h)
TARGET_LINK_LIBRARIES(coopy_fold coopy_full)
export(TARGETS coopy_fold APPEND FILE ${COOPY_DEPENDENCIES})
install(TARGETS coopy_fold COMPONENT ${BASELIB} ${DESTINATION_LIB})

