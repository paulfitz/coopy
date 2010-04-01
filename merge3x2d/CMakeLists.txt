
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# need hash_map, but it is deprecated, and unordered_map not ready
ADD_DEFINITIONS(-Wno-deprecated)

ADD_LIBRARY(merge3x2d CsvMerge.cpp CsvMerge.h CsvSheet.cpp CsvSheet.h CsvColumn.h CsvColumn.cpp CsvStat.h CsvStat.cpp CsvCompare.h CsvCompare.cpp CsvWrite.h CsvWrite.cpp OrderResult.h)

ADD_EXECUTABLE(merge_test main_test.cpp)
TARGET_LINK_LIBRARIES(merge_test merge3x2d)
