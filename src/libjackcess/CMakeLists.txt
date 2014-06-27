if (USE_JACKCESS)

  add_definitions("-w") # just too verbose for java
  add_definitions("--classpath ${CMAKE_CURRENT_SOURCE_DIR}/jackcess") # :/usr/share/java/log4j.jar:/usr/share/java/commons-logging.jar:/usr/share/java/commons-lang.jar:/usr/share/java/commons-collections.jar")
  include_directories(${CMAKE_SOURCE_DIR}/src/libcoopy_core/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

  file(GLOB_RECURSE js jackcess/*.java)
  file(GLOB_RECURSE js_res RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/jackcess_res jackcess_res/*.*)
  file(GLOB_RECURSE js_res_abs ${CMAKE_CURRENT_SOURCE_DIR}/jackcess_res jackcess_res/*.*)
  # message(STATUS "${js_res}")

  foreach (r ${js_res}) 
    set(rabs ${CMAKE_CURRENT_SOURCE_DIR}/jackcess_res/${r})
    # string(REPLACE "/" "." base ${r})
    set(base ${r})
    # message(STATUS "==> ${r} ${base}")
    set_property(SOURCE ${rabs}
      PROPERTY LANGUAGE
      JavaC)
    set_property(SOURCE ${rabs}
      APPEND PROPERTY COMPILE_FLAGS
      "--resource ${base}")
  endforeach()

  make_directory(${CMAKE_BINARY_DIR}/gcjgen)
  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/gcjgen/JackBox.h
    COMMAND ${CMAKE_COMMAND} -E remove -f JackBox.class JackBox.h
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/JackBox.java .
    COMMAND gcj --classpath ${CMAKE_CURRENT_SOURCE_DIR}/jackcess -C JackBox.java
    COMMAND gcjh --classpath ${CMAKE_BINARY_DIR}/gcjgen JackBox
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gcjgen
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/JackBox.java
    )
  include_directories(${CMAKE_BINARY_DIR}/gcjgen)

  add_library(coopy_jackcess_main ${js})
  install(TARGETS coopy_jackcess_main COMPONENT ${BASELIB} ${DESTINATION_LIB})

  add_library(coopy_jackcess_res ${js_res_abs})
  install(TARGETS coopy_jackcess_res COMPONENT ${BASELIB} ${DESTINATION_LIB})

  #add_library(coopy_jackcess jackcess.java ${js} ${CMAKE_BINARY_DIR}/jackcess.h ${js_res_abs2})

  if (HAVE_TR1)
    ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
  endif()

  add_library(coopy_jackcess ${CMAKE_CURRENT_SOURCE_DIR}/JackBox.java ${CMAKE_BINARY_DIR}/gcjgen/JackBox.h
    include/coopy/JackSheet.h JackSheet.cpp
    include/coopy/JackTextBook.h JackTextBook.cpp
    )
  target_link_libraries(coopy_jackcess coopy_jackcess_main 
    coopy_jackcess_res coopy_core)
  install(TARGETS coopy_jackcess COMPONENT ${BASELIB} ${DESTINATION_LIB})

  add_executable(test_jackcess test_jackcess.cpp)
  target_link_libraries(test_jackcess coopy_jackcess)

  #install(TARGETS coopy_socialcalc COMPONENT ${BASELIB} ${DESTINATION_LIB})

endif ()

