SET(WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

IF (MINGW)
  # langinfo.h missing?
  MESSAGE(STATUS "Internationalization may not be working under MSYS right now")
  ADD_DEFINITIONS(-DFOSSIL_I18N=0)
ENDIF (MINGW)

FIND_PACKAGE(OpenSSL)
IF (OPENSSL_FOUND)
  ADD_DEFINITIONS(-DFOSSIL_ENABLE_SSL=1)
  INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
ENDIF ()

ADD_EXECUTABLE(makeheaders fossil/src/makeheaders.c)
ADD_EXECUTABLE(translate fossil/src/translate.c)
ADD_EXECUTABLE(mkindex fossil/src/mkindex.c)

GET_TARGET_PROPERTY(MAKEHEADERS_LOC makeheaders LOCATION)
GET_TARGET_PROPERTY(TRANSLATE_LOC translate LOCATION)
GET_TARGET_PROPERTY(MKINDEX_LOC mkindex LOCATION)

# The following is very minimal, more dependencies should be captured
# if actually modifying fossil code.

FILE(GLOB FOSSIL_SRCS RELATIVE ${WORKING_DIR}/fossil/src ${WORKING_DIR}/fossil/src/*.c)
SET(GEN_HDRS)
SET(MAKEHEADERS_ARGS)
SET(GEN_SRCS)
SET(ORG_SRCS)
FOREACH(src ${FOSSIL_SRCS})
  IF (NOT src MATCHES "(makeheaders.c)|(translate.c)|(mkindex.c)|(th.c)|(sqlite3.c)")
    STRING(REPLACE ".c" ".h" hdr ${src})
    SET(GEN_HDRS ${GEN_HDRS} ${OUTPUT_DIR}/${hdr})
    STRING(REPLACE ".c" "_.c" src_ ${src})
    SET(GEN_SRCS ${GEN_SRCS} ${OUTPUT_DIR}/${src_})
    SET(MAKEHEADERS_ARGS ${MAKEHEADERS_ARGS} "${src_}:${hdr}")
    SET(dir ${WORKING_DIR}/fossil/src)
    # If source code exists in src/fossil, override official code.
    IF (EXISTS ${WORKING_DIR}/src/fossil/${src})
      SET(dir ${WORKING_DIR}/src/fossil)
    ENDIF (EXISTS ${WORKING_DIR}/src/fossil/${src})
    SET(ORG_SRCS ${ORG_SRCS} ${dir}/${src})
    ADD_CUSTOM_COMMAND(OUTPUT ${OUTPUT_DIR}/${src_}
      COMMAND ${TRANSLATE_LOC} ${src} > ${OUTPUT_DIR}/${src_}
      WORKING_DIRECTORY ${dir}
      DEPENDS ${dir}/${src}
      DEPENDS ${TRANSLATE_LOC}
    )
  ENDIF (NOT src MATCHES "(makeheaders.c)|(translate.c)|(mkindex.c)|(th.c)|(sqlite3.c)")
ENDFOREACH(src)

ADD_CUSTOM_COMMAND(OUTPUT ${GEN_HDRS}
  COMMAND ${MAKEHEADERS_LOC} ${MAKEHEADERS_ARGS}  ${WORKING_DIR}/fossil/src/sqlite3.h ${WORKING_DIR}/fossil/src/th.h ${WORKING_DIR}/src/fossil/VERSION.h
  COMMAND touch ${GEN_HDRS}
  DEPENDS ${MAKEHEADERS_LOC}
#  DEPENDS ${ORG_SRCS}
)

ADD_CUSTOM_COMMAND(OUTPUT ${OUTPUT_DIR}/page_index.h
  COMMAND ${MKINDEX_LOC} ${GEN_SRCS} > ${OUTPUT_DIR}/page_index.h
  COMMAND cat ${WORKING_DIR}/src/fossil/main.h >> ${OUTPUT_DIR}/page_index.h
  DEPENDS ${GEN_SRCS}
  DEPENDS ${MKINDEX_LOC}
)

INCLUDE_DIRECTORIES(${OUTPUT_DIR})
INCLUDE_DIRECTORIES(${WORKING_DIR}/fossil/src)
INCLUDE_DIRECTORIES(${WORKING_DIR}/src/fossil)
INCLUDE_DIRECTORIES(${SHEET_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/ssrender/include)
ADD_LIBRARY(coopy_fossil ${GEN_SRCS} ${GEN_HDRS} 
  fossil/src/th.c 
  #fossil/src/sqlite3.c 
  ${OUTPUT_DIR}/page_index.h
  src/csv_hooks.cpp 
  src/ssfossil.h src/ssfossil.cpp
)
TARGET_LINK_LIBRARIES(coopy_fossil z)
IF (OPENSSL_FOUND)
  TARGET_LINK_LIBRARIES(coopy_fossil ${OPENSSL_LIBRARIES})
  if (MINGW)
    TARGET_LINK_LIBRARIES(coopy_fossil gdi32 crypt32)
  endif ()
ENDIF ()
IF (MINGW)
  TARGET_LINK_LIBRARIES(coopy_fossil ws2_32)
ELSE (MINGW)
  TARGET_LINK_LIBRARIES(coopy_fossil dl pthread)
ENDIF (MINGW)
TARGET_LINK_LIBRARIES(coopy_fossil sheet)
TARGET_LINK_LIBRARIES(coopy_fossil ssrender)
TARGET_LINK_LIBRARIES(coopy_fossil csv)

ADD_EXECUTABLE(ssfossil ${WORKING_DIR}/src/main.cpp)
TARGET_LINK_LIBRARIES(ssfossil coopy_fossil)
INSTALL(TARGETS ssfossil COMPONENT ${BASEBIN} DESTINATION bin)
