
add_subdirectory(libsheet_core)
add_subdirectory(libcsv)
add_subdirectory(libsheet)
set(SHEET_INCLUDE_DIRS 
  ${CMAKE_SOURCE_DIR}/src/libsheet_core 
  ${CMAKE_SOURCE_DIR}/src/libcsv 
  ${CMAKE_SOURCE_DIR}/src/libsheet)

add_subdirectory(ssmerge)
add_subdirectory(ssrender)
add_subdirectory(coopy_scm)

add_subdirectory(testing)
