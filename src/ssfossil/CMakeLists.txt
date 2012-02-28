set(WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

if (MINGW)
  # langinfo.h missing?
  message(STATUS "Internationalization may not be working under MSYS right now")
  add_definitions(-DFOSSIL_I18N=0)
endif (MINGW)

find_package(OpenSSL)
if (OPENSSL_FOUND)
  add_definitions(-DFOSSIL_ENABLE_SSL=1)
  include_directories(${OPENSSL_INCLUDE_DIR})
endif ()

add_executable(makeheaders fossil/src/makeheaders.c)
add_executable(translate fossil/src/translate.c)
add_executable(mkindex fossil/src/mkindex.c)

get_target_property(MAKEHEADERS_LOC makeheaders LOCATION)
get_target_property(TRANSLATE_LOC translate LOCATION)
get_target_property(MKINDEX_LOC mkindex LOCATION)

# The following is very minimal, more dependencies should be captured
# if actually modifying fossil code.

file(GLOB FOSSIL_SRCS RELATIVE ${WORKING_DIR}/fossil/src ${WORKING_DIR}/fossil/src/*.c)
set(GEN_HDRS)
set(MAKEHEADERS_ARGS)
set(GEN_SRCS)
set(ORG_SRCS)
foreach(src ${FOSSIL_SRCS})
  if (NOT src MATCHES "(makeheaders.c)|(translate.c)|(mkindex.c)|(th.c)|(sqlite3.c)")
    string(REPLACE ".c" ".h" hdr ${src})
    set(GEN_HDRS ${GEN_HDRS} ${OUTPUT_DIR}/${hdr})
    string(REPLACE ".c" "_.c" src_ ${src})
    set(GEN_SRCS ${GEN_SRCS} ${OUTPUT_DIR}/${src_})
    set(MAKEHEADERS_ARGS ${MAKEHEADERS_ARGS} "${src_}:${hdr}")
    set(dir ${WORKING_DIR}/fossil/src)
    # If source code exists in src/fossil, override official code.
    if (EXISTS ${WORKING_DIR}/src/fossil/${src})
      set(dir ${WORKING_DIR}/src/fossil)
    endif (EXISTS ${WORKING_DIR}/src/fossil/${src})
    set(ORG_SRCS ${ORG_SRCS} ${dir}/${src})
    add_custom_command(OUTPUT ${OUTPUT_DIR}/${src_}
      COMMAND ${TRANSLATE_LOC} ${src} > ${OUTPUT_DIR}/${src_}
      WORKING_DIRECTORY ${dir}
      DEPENDS ${dir}/${src}
      DEPENDS ${TRANSLATE_LOC}
    )
  endif (NOT src MATCHES "(makeheaders.c)|(translate.c)|(mkindex.c)|(th.c)|(sqlite3.c)")
endforeach(src)

add_custom_command(OUTPUT ${GEN_HDRS}
  COMMAND ${MAKEHEADERS_LOC} ${MAKEHEADERS_ARGS}  ${WORKING_DIR}/fossil/src/sqlite3.h ${WORKING_DIR}/fossil/src/th.h ${WORKING_DIR}/src/fossil/VERSION.h
  COMMAND touch ${GEN_HDRS}
  DEPENDS ${MAKEHEADERS_LOC}
)

add_custom_command(OUTPUT ${OUTPUT_DIR}/page_index.h
  COMMAND ${MKINDEX_LOC} ${GEN_SRCS} > ${OUTPUT_DIR}/page_index.h
  COMMAND cat ${WORKING_DIR}/src/fossil/main.h >> ${OUTPUT_DIR}/page_index.h
  DEPENDS ${GEN_SRCS}
  DEPENDS ${MKINDEX_LOC}
)

include_directories(${OUTPUT_DIR})
include_directories(${WORKING_DIR}/fossil/src)
include_directories(${WORKING_DIR}/src/fossil)
include_directories(${SHEET_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src/ssrender/include)
add_library(coopy_fossil ${GEN_SRCS} ${GEN_HDRS} 
  fossil/src/th.c 
  #fossil/src/sqlite3.c 
  ${OUTPUT_DIR}/page_index.h
  src/csv_hooks.cpp 
  src/ssfossil.h src/ssfossil.cpp
)
install(TARGETS coopy_fossil COMPONENT ${BASELIB} DESTINATION lib)

target_link_libraries(coopy_fossil z)
if (OPENSSL_FOUND)
  target_link_libraries(coopy_fossil ${OPENSSL_LIBRARIES})
  if (MINGW)
    target_link_libraries(coopy_fossil gdi32 crypt32)
  endif ()
endif ()
if (MINGW)
  target_link_libraries(coopy_fossil ws2_32)
else (MINGW)
  target_link_libraries(coopy_fossil dl pthread)
endif (MINGW)
target_link_libraries(coopy_fossil coopy_full)
target_link_libraries(coopy_fossil coopy_render)
target_link_libraries(coopy_fossil coopy_csv)

add_executable(ssfossil ${WORKING_DIR}/src/main.cpp)
target_link_libraries(ssfossil coopy_fossil)
install(TARGETS ssfossil COMPONENT ${BASEBIN} DESTINATION bin)
