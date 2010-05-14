
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

# need hash_map or unordered_map or something for efficient hashing.
# hash_map is deprecated under newer gcc versions.
ADD_DEFINITIONS(-Wno-deprecated)

ADD_LIBRARY(sheet_core CsvMerge.cpp include/coopy/CsvMerge.h CsvSheet.cpp include/coopy/CsvSheet.h include/coopy/CsvColumn.h CsvColumn.cpp include/coopy/CsvStat.h CsvStat.cpp include/coopy/CsvCompare.h CsvCompare.cpp include/coopy/CsvWrite.h CsvWrite.cpp include/coopy/OrderResult.h CsvPatch.cpp include/coopy/CsvPatch.h CsvStyle.cpp include/coopy/CsvStyle.h)

ADD_EXECUTABLE(merge_test main_test.cpp)
TARGET_LINK_LIBRARIES(merge_test sheet_core)
