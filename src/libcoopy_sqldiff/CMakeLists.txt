if (HAVE_TR1)
  ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
endif ()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/libcoopy_core/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

ADD_LIBRARY(coopy_sqldiff SqlCompare.cpp include/coopy/SqlCompare.h include/coopy/DbiSqlWrapper.h)
TARGET_LINK_LIBRARIES(coopy_sqldiff coopy_core)
export(TARGETS coopy_sqldiff APPEND FILE ${COOPY_DEPENDENCIES})
install(TARGETS coopy_sqldiff COMPONENT ${BASELIB} ${DESTINATION_LIB})

