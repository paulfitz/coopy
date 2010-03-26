cmake_minimum_required(VERSION 2.6)

project(coopy)

enable_testing()

# three-way merges of a 2D array
add_subdirectory(merge3x2d)

# probing properties of sheets
add_subdirectory(scanss)

# fossil integration
add_subdirectory(sheet_scm)
