if (USE_SOCIALCALC)

  include(FindPkgConfig)

  pkg_search_module(MOZJS REQUIRED mozilla-js xulrunner-js firefox-js seamonkey-js)
  
  include_directories(${MOZJS_INCLUDE_DIRS})
  link_directories(${MOZJS_LIBRARY_DIRS})

  include_directories(${CMAKE_SOURCE_DIR}/src/libcsv/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

  add_executable(codify codify.cpp)
  

  set(jscpp)
  foreach (js socialcalc-3
      socialcalcconstants 
      socialcalctableeditor
      socialcalcspreadsheetcontrol socialcalcwrap)
  
    string(REPLACE "-" "_" fname ${js})
    add_custom_command(
      OUTPUT ${fname}.cpp 
      COMMAND codify ${CMAKE_CURRENT_SOURCE_DIR}/js/${js}.js 
      ${fname}.cpp ${fname}
      DEPENDS codify ${CMAKE_CURRENT_SOURCE_DIR}/js/${js}.js
      )
    list(APPEND jscpp ${fname}.cpp)
  endforeach()

  include_directories(${CMAKE_SOURCE_DIR}/src/libcoopy_core/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

  add_library(coopy_socialcalc
    JsWrap.cpp 
    include/coopy/JsWrap.h
    ${jscpp}
    SocialCalcTextBook.cpp
    SocialCalcTextBookFactory.cpp
    )


  target_link_libraries(coopy_socialcalc ${MOZJS_LIBRARIES} coopy_core coopy_csv)

  add_executable(test_socialcalc SocialCalc.cpp)
  target_link_libraries(test_socialcalc coopy_socialcalc)

endif ()

