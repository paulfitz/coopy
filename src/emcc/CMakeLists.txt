if (USE_EMSCRIPTEN)

  if (NOT LINUX)
    message(STATUS "This will probably only work on Linux")
  endif ()

  message(STATUS "Be sure to run cmake/ccmake as 'emconfigure cmake ...")

  if (HAVE_TR1)
    ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
  endif ()
  
  INCLUDE_DIRECTORIES(${SHEET_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})


  ADD_EXECUTABLE(emdiff emdiff.cpp)
  TARGET_LINK_LIBRARIES(emdiff coopy_full)

  set(ODIR ${CMAKE_BINARY_DIR}/jsgen)
  set(ONAME ssdiff.js)

  add_custom_command(OUTPUT ${ODIR}/${ONAME}
    COMMAND mkdir -p ${ODIR}
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/emscripten/prep_js.sh ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${ODIR}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${CMAKE_SOURCE_DIR}/scripts/emscripten/prep_js.sh
    emdiff)
  add_custom_target(js ALL SOURCES ${ODIR}/${ONAME})

endif ()
