if (USE_SOCIALCALC)

  if (HAVE_TR1)
    add_definitions("-std=c++0x" -DHAVE_TR1)
  endif ()

  include_directories(${CMAKE_SOURCE_DIR}/src/libcsv/include)
  include_directories(${CMAKE_SOURCE_DIR}/src/libjs/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


  set(jscpp)
  foreach (js socialcalc-3
      socialcalcconstants 
      socialcalctableeditor
      socialcalcspreadsheetcontrol socialcalcwrap)
  
    string(REPLACE "-" "_" fname ${js})
    
    js_to_cpp(${CMAKE_CURRENT_SOURCE_DIR}/js/${js}.js 
      ${fname}.cpp
      ${fname})
    list(APPEND jscpp ${fname}.cpp)
  endforeach()

  include_directories(${CMAKE_SOURCE_DIR}/src/libcoopy_core/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

  add_library(coopy_socialcalc
    ${jscpp}
    SocialCalcTextBook.cpp
    include/coopy/SocialCalcTextBook.h
    SocialCalcTextBookFactory.cpp
    include/coopy/SocialCalcTextBookFactory.h
    SocialCalcSheet.cpp
    include/coopy/SocialCalcSheet.h
    )


  target_link_libraries(coopy_socialcalc ${MOZJS_LIBRARIES} coopy_core coopy_csv coopy_js)

  add_executable(test_socialcalc SocialCalc.cpp)
  target_link_libraries(test_socialcalc coopy_socialcalc)

  install(TARGETS coopy_socialcalc COMPONENT ${BASELIB} DESTINATION lib)

endif ()

