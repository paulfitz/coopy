
if (NOT Mdbtools_FOUND)

  FIND_LIBRARY(Mdbtools_LIBRARIES NAMES mdb PATH /usr/lib /usr/local/lib) 
  FIND_PATH(Mdbtools_INCLUDE_DIRS mdbtools.h /usr/include /usr/local/include DOC "directory containing mdbtools.h")

  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(Mdbtools "Mdbtools not found" Mdbtools_LIBRARIES Mdbtools_INCLUDE_DIRS)

endif ()

