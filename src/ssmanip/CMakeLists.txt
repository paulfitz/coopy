
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# need hash_map, but it is deprecated, and unordered_map not ready
ADD_DEFINITIONS(-Wno-deprecated)

ADD_LIBRARY(ssmanip CsvMerge.cpp CsvMerge.h CsvSheet.cpp CsvSheet.h CsvColumn.h CsvColumn.cpp CsvStat.h CsvStat.cpp CsvCompare.h CsvCompare.cpp CsvWrite.h CsvWrite.cpp OrderResult.h CsvPatch.cpp CsvPatch.h CsvStyle.cpp CsvStyle.h)

ADD_EXECUTABLE(merge_test main_test.cpp)
TARGET_LINK_LIBRARIES(merge_test ssmanip)
