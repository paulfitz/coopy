INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/merge3x2d)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libcsv)
ADD_LIBRARY(csv libcsv/csv.h libcsv/libcsv.c)
ADD_LIBRARY(scancsv CsvFile.h CsvFile.cpp)
TARGET_LINK_LIBRARIES(scancsv merge3x2d csv)
ADD_EXECUTABLE(scanss scanss.cpp)
TARGET_LINK_LIBRARIES(scanss scancsv)

ADD_EXECUTABLE(csvmerge csvmerge.cpp)
TARGET_LINK_LIBRARIES(csvmerge scancsv)
INSTALL(TARGETS csvmerge COMPONENT utilities DESTINATION bin)

ADD_EXECUTABLE(csvdiff csvdiff.cpp)
TARGET_LINK_LIBRARIES(csvdiff scancsv)
INSTALL(TARGETS csvdiff COMPONENT utilities DESTINATION bin)

ADD_EXECUTABLE(csvpatch csvpatch.cpp)
TARGET_LINK_LIBRARIES(csvpatch scancsv)
INSTALL(TARGETS csvpatch COMPONENT utilities DESTINATION bin)

SET(TESTS ${CMAKE_SOURCE_DIR}/tests)

############################################################################
# check detection of header line

# path may need modification on windows
SET(scanss ${EXECUTABLE_OUTPUT_PATH}/scanss)

ADD_TEST(directory_header ${scanss} --read ${TESTS}/test001_base.csv 
  --prop hdr --assert 0)
ADD_TEST(directory_header_missing ${scanss} --read ${TESTS}/test001_base.csv 
  --remove_row 0 --prop hdr --assert -1)

ADD_TEST(latlong_header ${scanss} --read ${TESTS}/test002_base.csv 
  --prop hdr --assert 0)
ADD_TEST(latlong_header_missing ${scanss} --read ${TESTS}/test002_base.csv 
  --remove_row 0 --prop hdr --assert -1)

ADD_TEST(company_header ${scanss} --read ${TESTS}/test003_base.csv 
  --prop hdr --assert 0)
ADD_TEST(company_header_missing ${scanss} --read ${TESTS}/test003_base.csv 
  --remove_row 0 --prop hdr --assert -1)

ADD_TEST(car_headerless ${scanss} --read ${TESTS}/test004_base.csv 
  --prop hdr --assert -1)
ADD_TEST(car_headerless2 ${scanss} --read ${TESTS}/test004_base.csv 
  --remove_row 0 --prop hdr --assert -1)

# Parent   1 2 3
# Local  0 1 2 3
# Remote     2 3
# MERGE: 0   2 3
ADD_TEST(latlong_compare1 ${scanss}
  --local --read ${TESTS}/test002_base.csv 
  --remote --read ${TESTS}/test002_base.csv --remove_row 0 --remove_row 1
  --parent --read ${TESTS}/test002_base.csv --remove_row 0
  --compare
  --prop height --assert 3)

ADD_TEST(company_merge_conflict ${scanss}
  --parent --read ${TESTS}/test003_base.csv 
  --local --read ${TESTS}/test003_change.csv
  --remote --read ${TESTS}/test003_conflicting_change.csv
  --compare
#  --write /tmp/check.csv
  --prop height --assert 5)

ADD_TEST(company_merge_add_col ${scanss}
  --parent --read ${TESTS}/test003_base.csv 
  --local --read ${TESTS}/test003_add.csv
  --remote --read ${TESTS}/test003_insert_col.csv
  --compare
#  --local --write ${TESTS}/result_company_merge_add_col.csv
  --remote --read ${TESTS}/result_company_merge_add_col.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(directory_merge_add_col ${scanss}
  --parent --read ${TESTS}/test001_base.csv 
  --local --read ${TESTS}/test001_add.csv
  --remote --read ${TESTS}/test001_col.csv
  --compare
#  --local --write ${TESTS}/result_directory_merge_add_col.csv
  --remote --read ${TESTS}/result_directory_merge_add_col.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(directory_merge_spelling ${scanss}
  --parent --read ${TESTS}/test001_base.csv 
  --local --read ${TESTS}/test001_add.csv
  --remote --read ${TESTS}/test001_spell.csv
  --compare
#  --local --write ${TESTS}/result_directory_merge_spelling.csv
  --remote --read ${TESTS}/result_directory_merge_spelling.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(altitude_merge ${scanss}
  --parent --read ${TESTS}/test005_base.csv 
  --local --read ${TESTS}/test005_fix_typo_add.csv
  --remote --read ${TESTS}/test005_replace_column.csv
  --compare
#  --local --write ${TESTS}/result_altitude_merge.csv
  --remote --read ${TESTS}/result_altitude_merge.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(altitude_merge_without_header ${scanss}
  --parent --read ${TESTS}/test005_base.csv --remove_row 0
  --local --read ${TESTS}/test005_fix_typo_add.csv --remove_row 0
  --remote --read ${TESTS}/test005_replace_column.csv --remove_row 0
  --compare
  --remote --read ${TESTS}/result_altitude_merge.csv --remove_row 0
  --diff
  --prop diffs --assert 0)

ADD_TEST(altitude_merge_conflict ${scanss}
  --parent --read ${TESTS}/test005_base.csv 
  --local --read ${TESTS}/test005_fix_typo_add.csv
  --remote --read ${TESTS}/test005_fix_typo_conflict.csv
  --compare
#  --local --write ${TESTS}/result_altitude_merge_conflict.csv
  --remote --read ${TESTS}/result_altitude_merge_conflict.csv
  --diff
  --prop diffs --assert 0)


ADD_TEST(altitude_diff_typo ${scanss}
  --parent --read ${TESTS}/test005_base.csv 
  --local --read ${TESTS}/test005_base.csv
  --remote --read ${TESTS}/test005_fix_typo_add.csv
  --compare=1
#  --local --write ${TESTS}/result_altitude_diff_typo.csv
  --remote --read ${TESTS}/result_altitude_diff_typo.csv
  --diff
  --prop diffs --assert 0)

