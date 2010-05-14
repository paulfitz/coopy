
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# need hash_map or unordered_map or something for efficient hashing.
# hash_map is deprecated under newer gcc versions.
ADD_DEFINITIONS(-Wno-deprecated)

ADD_LIBRARY(sheet_core CsvMerge.cpp CsvMerge.h CsvSheet.cpp CsvSheet.h CsvColumn.h CsvColumn.cpp CsvStat.h CsvStat.cpp CsvCompare.h CsvCompare.cpp CsvWrite.h CsvWrite.cpp OrderResult.h CsvPatch.cpp CsvPatch.h CsvStyle.cpp CsvStyle.h)

ADD_EXECUTABLE(merge_test main_test.cpp)
TARGET_LINK_LIBRARIES(merge_test sheet_core)
