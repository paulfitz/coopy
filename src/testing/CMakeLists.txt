INCLUDE_DIRECTORIES(${SHEET_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# This is a utility primarily for testing
ADD_EXECUTABLE(test_sheet test_sheet.cpp)
TARGET_LINK_LIBRARIES(test_sheet sheet)

SET(TESTS ${CMAKE_SOURCE_DIR}/tests)

############################################################################
# check detection of header line

# path may need modification on windows
SET(testprg ${EXECUTABLE_OUTPUT_PATH}/test_sheet)

ADD_TEST(directory_header ${testprg} --read ${TESTS}/test001_base.csv 
  --prop hdr --assert 0)
ADD_TEST(directory_header_missing ${testprg} --read ${TESTS}/test001_base.csv 
  --remove_row 0 --prop hdr --assert -1)

ADD_TEST(latlong_header ${testprg} --read ${TESTS}/test002_base.csv 
  --prop hdr --assert 0)
ADD_TEST(latlong_header_missing ${testprg} --read ${TESTS}/test002_base.csv 
  --remove_row 0 --prop hdr --assert -1)

ADD_TEST(company_header ${testprg} --read ${TESTS}/test003_base.csv 
  --prop hdr --assert 0)
ADD_TEST(company_header_missing ${testprg} --read ${TESTS}/test003_base.csv 
  --remove_row 0 --prop hdr --assert -1)

ADD_TEST(car_headerless ${testprg} --read ${TESTS}/test004_base.csv 
  --prop hdr --assert -1)
ADD_TEST(car_headerless2 ${testprg} --read ${TESTS}/test004_base.csv 
  --remove_row 0 --prop hdr --assert -1)

# Parent   1 2 3
# Local  0 1 2 3
# Remote     2 3
# MERGE: 0   2 3
ADD_TEST(latlong_compare1 ${testprg}
  --local --read ${TESTS}/test002_base.csv 
  --remote --read ${TESTS}/test002_base.csv --remove_row 0 --remove_row 1
  --parent --read ${TESTS}/test002_base.csv --remove_row 0
  --compare
  --prop height --assert 3)

ADD_TEST(company_merge_conflict ${testprg}
  --parent --read ${TESTS}/test003_base.csv 
  --local --read ${TESTS}/test003_change.csv
  --remote --read ${TESTS}/test003_conflicting_change.csv
  --compare
#  --write /tmp/check.csv
  --prop height --assert 5)

ADD_TEST(company_merge_add_col ${testprg}
  --parent --read ${TESTS}/test003_base.csv 
  --local --read ${TESTS}/test003_add.csv
  --remote --read ${TESTS}/test003_insert_col.csv
  --compare
#  --local --write ${TESTS}/result_company_merge_add_col.csv
  --remote --read ${TESTS}/result_company_merge_add_col.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(directory_merge_add_col ${testprg}
  --parent --read ${TESTS}/test001_base.csv 
  --local --read ${TESTS}/test001_add.csv
  --remote --read ${TESTS}/test001_col.csv
  --compare
#  --local --write ${TESTS}/result_directory_merge_add_col.csv
  --remote --read ${TESTS}/result_directory_merge_add_col.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(directory_merge_spelling ${testprg}
  --parent --read ${TESTS}/test001_base.csv 
  --local --read ${TESTS}/test001_add.csv
  --remote --read ${TESTS}/test001_spell.csv
  --compare
#  --local --write ${TESTS}/result_directory_merge_spelling.csv
  --remote --read ${TESTS}/result_directory_merge_spelling.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(altitude_merge ${testprg}
  --parent --read ${TESTS}/test005_base.csv 
  --local --read ${TESTS}/test005_fix_typo_add.csv
  --remote --read ${TESTS}/test005_replace_column.csv
  --compare
#  --local --write ${TESTS}/result_altitude_merge.csv
  --remote --read ${TESTS}/result_altitude_merge.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(altitude_merge_without_header ${testprg}
  --parent --read ${TESTS}/test005_base.csv --remove_row 0
  --local --read ${TESTS}/test005_fix_typo_add.csv --remove_row 0
  --remote --read ${TESTS}/test005_replace_column.csv --remove_row 0
  --compare
  --remote --read ${TESTS}/result_altitude_merge.csv --remove_row 0
  --diff
  --prop diffs --assert 0)

ADD_TEST(altitude_merge_conflict ${testprg}
  --parent --read ${TESTS}/test005_base.csv 
  --local --read ${TESTS}/test005_fix_typo_add.csv
  --remote --read ${TESTS}/test005_fix_typo_conflict.csv
  --compare
#  --local --write ${TESTS}/result_altitude_merge_conflict.csv
  --remote --read ${TESTS}/result_altitude_merge_conflict.csv
  --diff
  --prop diffs --assert 0)


ADD_TEST(altitude_diff_typo ${testprg}
  --parent --read ${TESTS}/test005_base.csv 
  --local --read ${TESTS}/test005_base.csv
  --remote --read ${TESTS}/test005_fix_typo_add.csv
  --compare=1
#  --local --write ${TESTS}/result_altitude_diff_typo.csv
  --remote --read ${TESTS}/result_altitude_diff_typo.csv
  --diff
  --prop diffs --assert 0)


# Check that producing a diff against remote, then applying it as a patch, 
# correctly gives remote
ADD_TEST(altitude_patch_typo ${testprg}
  --parent --read ${TESTS}/test005_base.csv 
  --local --read ${TESTS}/test005_base.csv
  --remote --read ${TESTS}/test005_fix_typo_add.csv
  --compare=1
  --patch
  --diff
  --prop diffs --assert 0)


ADD_TEST(altitude_diff_remove_row ${testprg}
  --parent --read ${TESTS}/test005_base.csv 
  --local --read ${TESTS}/test005_base.csv
  --remote --read ${TESTS}/test005_remove_row.csv
  --compare=1
#  --local --write ${TESTS}/result_altitude_diff_remove_row.csv
  --remote --read ${TESTS}/result_altitude_diff_remove_row.csv
  --diff
  --prop diffs --assert 0)


ADD_TEST(altitude_patch_remove_row ${testprg}
  --parent --read ${TESTS}/test005_base.csv 
  --local --read ${TESTS}/test005_base.csv
  --remote --read ${TESTS}/test005_remove_row.csv
  --compare=1
  --patch
  --diff
  --prop diffs --assert 0)


# ADD_TEST(altitude_diff_replace_col ${testprg}
#   --parent --read ${TESTS}/test005_base.csv 
#   --local --read ${TESTS}/test005_base.csv
#   --remote --read ${TESTS}/test005_replace_column.csv
#   --compare=1
#   --local --write ${TESTS}/result_altitude_diff_replace_col.csv
#   --remote --read ${TESTS}/result_altitude_diff_replace_col.csv
#   --diff
#   --prop diffs --assert 0)


