if (HAVE_TR1)
  ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
endif ()

INCLUDE_DIRECTORIES(${SHEET_INCLUDE_DIRS})

ADD_EXECUTABLE(test_sheet test_sheet.cpp)
TARGET_LINK_LIBRARIES(test_sheet sheet)

# need hash_map or unordered_map or something for efficient hashing.
# hash_map is deprecated under newer gcc versions.
ADD_DEFINITIONS(-Wno-deprecated)

ADD_EXECUTABLE(test_viterbi test_viterbi.cpp)
TARGET_LINK_LIBRARIES(test_viterbi sheet)

ADD_EXECUTABLE(make_sheet make_sheet.cpp)
TARGET_LINK_LIBRARIES(make_sheet sheet)

SET(TESTS ${CMAKE_SOURCE_DIR}/tests)

# path may need modification on windows
IF (MINGW)
  SET(EXE_EXT ".exe")
ELSE ()
  SET(EXE_EXT "")
ENDIF()
SET(testprg ${EXECUTABLE_OUTPUT_PATH}/test_sheet${EXE_EXT})
SET(testprg_viterbi ${EXECUTABLE_OUTPUT_PATH}/test_viterbi${EXE_EXT})

############################################################################
# check detection of header line - not actually a used feature in the end

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

############################################################################
# check merging

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


IF (FALSE)
# not ready for this yet
ADD_TEST(altitude_merge_with_reorder ${testprg}
  --parent --read ${TESTS}/test005_base.csv 
  --local  --read ${TESTS}/test005_base.csv
  --remote --read ${TESTS}/test005_replace_column_and_reorder.csv
  --compare
#  --local --write ${TESTS}/result_altitude_merge_with_reorder.csv
  --remote --read ${TESTS}/result_altitude_merge_with_reorder.csv
  --diff
  --prop diffs --assert 0)
ENDIF(FALSE)



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

#######################################################################
#######################################################################

# test very basic merge properties

ADD_TEST(basic_merge_garbled ${testprg}
  --parent --read ${TESTS}/numbers_spurious_comma.csv 
  --local --read ${TESTS}/numbers.csv
  --remote --read ${TESTS}/numbers_change_five.csv
  --compare
  --remote --read ${TESTS}/numbers_change_five.csv
  --diff
  --prop diffs --assert 0)
SET_PROPERTY(TEST basic_merge_garbled PROPERTY WILL_FAIL TRUE)


ADD_TEST(basic_merge_modify_one_cell ${testprg}
  --parent --read ${TESTS}/numbers.csv 
  --local --read ${TESTS}/numbers.csv
  --remote --read ${TESTS}/numbers_change_five.csv
  --compare
  --remote --read ${TESTS}/numbers_change_five.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(basic_merge_modify_one_cell2 ${testprg}
  --parent --read ${TESTS}/numbers.csv 
  --local --read ${TESTS}/numbers_change_five.csv
  --remote --read ${TESTS}/numbers.csv
  --compare
  --remote --read ${TESTS}/numbers_change_five.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(basic_merge_flip_column ${testprg}
  --parent --read ${TESTS}/numbers.csv 
  --local --read ${TESTS}/numbers.csv
  --remote --read ${TESTS}/numbers_flip_column.csv
  --compare
  --remote --read ${TESTS}/numbers_flip_column.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(basic_merge_flip_column2 ${testprg}
  --parent --read ${TESTS}/numbers.csv 
  --local --read ${TESTS}/numbers_flip_column.csv
  --remote --read ${TESTS}/numbers.csv
  --compare
  --remote --read ${TESTS}/numbers_flip_column.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(basic_merge_change_cell_flip_column ${testprg}
  --parent --read ${TESTS}/numbers.csv 
  --local --read ${TESTS}/numbers_change_five.csv
  --remote --read ${TESTS}/numbers_flip_column.csv
  --compare
  --remote --read ${TESTS}/numbers_flip_column_change_five.csv
  --diff
  --prop diffs --assert 0)


ADD_TEST(basic_merge_flip_column_pairs ${testprg}
  --parent --read ${TESTS}/numbers_wide.csv 
  --local --read ${TESTS}/numbers_wide_flip_pair1.csv
  --remote --read ${TESTS}/numbers_wide_flip_pair2.csv
  --compare
  --remote --read ${TESTS}/numbers_wide_flip_pairs.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(basic_merge_add_row ${testprg}
  --parent --read ${TESTS}/numbers.csv 
  --local --read ${TESTS}/numbers.csv
  --remote --read ${TESTS}/numbers_add_row.csv
  --compare
  --remote --read ${TESTS}/numbers_add_row.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(basic_merge_add_row2 ${testprg}
  --parent --read ${TESTS}/numbers.csv 
  --local --read ${TESTS}/numbers_add_row.csv
  --remote --read ${TESTS}/numbers.csv
  --compare
  --remote --read ${TESTS}/numbers_add_row.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(basic_merge_typo_and_replace_column ${testprg}
  --parent --read ${TESTS}/test005_base.csv
  --local --read ${TESTS}/test005_fix_typo_add.csv
  --remote --read ${TESTS}/test005_replace_column_and_reorder.csv
  --compare
  --remote --read ${TESTS}/test005_fix_typo_add_and_new_column.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(basic_merge_unrelated ${testprg}
  --parent --read ${TESTS}/blank.csv
  --local --read ${TESTS}/trimmer_base.csv
  --remote --read ${TESTS}/numbers.csv
  --compare
#  --local --write ${TESTS}/result_merge_unrelated.csv
  --remote --read ${TESTS}/result_merge_unrelated.csv
  --diff
  --prop diffs --assert 0)

#######################################################################
#######################################################################

# Basic CSV tests

ADD_TEST(csv_check_blanks ${testprg}
  --local --read ${TESTS}/csv_empty_cell.csv
  --remote --read ${TESTS}/csv_empty_cell_variant.csv
  --diff
  --prop diffs --assert 0)

#######################################################################
#######################################################################

# Test for some sensible behavior in face of redundancy

ADD_TEST(redundant_numbers ${testprg}
  --parent --read ${TESTS}/numbers_dup.csv
  --local --read ${TESTS}/numbers_dup.csv
  --remote --read ${TESTS}/numbers_dup_five.csv
  --compare=csv
#  --local --write ${TESTS}/result_redundant_numbers.csv
  --remote --read ${TESTS}/result_redundant_numbers.csv
  --diff
  --prop diffs --assert 0)

#######################################################################
#######################################################################

# Basic viterbi check

ADD_TEST(viterbi_check ${testprg_viterbi})

#######################################################################
#######################################################################

MACRO(ADD_TEST2 name ref target)
  ADD_TEST(${name} ${EXECUTABLE_OUTPUT_PATH}/${target}${EXE_EXT} 
    --output test_out_${name}
    ${ARGN})
  ADD_TEST(${name}Compare ${CMAKE_COMMAND} -E compare_files 
    test_out_${name} ${ref})
ENDMACRO()

ADD_TEST2(tail_merge ${TESTS}/result_trimmer_merge.csv ssmerge --tail-trim
  ${TESTS}/trimmer_base.csv ${TESTS}/trimmer_base.csv ${TESTS}/trimmer_more.csv)

foreach(patcher patch_001_col_move patch_002_col_insert patch_003_col_insert patch_004_col_delete patch_005_row_update patch_006_row_insert patch_007_row_delete)
  ADD_TEST2(${patcher}_v02 ${TESTS}/result_${patcher}.csv sspatch ${TESTS}/numbers.csv ${TESTS}/patch_v_0_2/${patcher}.txt)
  ADD_TEST2(${patcher}_v04 ${TESTS}/result_${patcher}.csv sspatch ${TESTS}/numbers.csv ${TESTS}/patch_v_0_4/${patcher}.txt)
endforeach()

#######################################################################
#######################################################################

# SQLITE testing

foreach(patcher patch_001_col_move patch_002_col_insert patch_003_col_insert patch_004_col_delete patch_005_row_update patch_006_row_insert patch_007_row_delete)
  ADD_TEST2(sqlite_${patcher} ${TESTS}/result_${patcher}.csv sspatch --tmp tmp.sqlite ${TESTS}/numbers.sqlite ${TESTS}/patch_v_0_2/${patcher}.txt)
endforeach()


#######################################################################
#######################################################################

# EXCEL testing

IF (USE_GNUMERIC)

  ADD_TEST2(excel_diff ${TESTS}/result_excel_diff.csv
    ssdiff --verbose --version 0.2 --format-csv ${TESTS}/numbers.xls ${TESTS}/numbers_change_five.xls)

  MACRO(TEST_XLS name ref target)
    ADD_TEST(${name} ${EXECUTABLE_OUTPUT_PATH}/${target}${EXE_EXT} 
      --output test_out_${name}.xls
      ${ARGN})
    ADD_TEST(${name}Compare ${EXECUTABLE_OUTPUT_PATH}/ss2html${EXE_EXT} 
      --compare
      #    test_out_${name}.xls ${ref} ${ref})
      test_out_${name}.xls test_out_${name}.html ${ref})
  ENDMACRO()
  
  # Blank columns don't get exported by gnumeric, so some tests are not
  # that useful
  foreach(patcher patch_000_nothing patch_001_col_move patch_002_col_insert patch_003_col_insert patch_004_col_delete patch_005_row_update patch_006_row_insert patch_007_row_delete)
    TEST_XLS(excel_${patcher} ${TESTS}/result_excel_${patcher}.html sspatch ${TESTS}/numbers.xls ${TESTS}/patch_v_0_2/${patcher}.txt)
    
  endforeach()

ENDIF()


#######################################################################
#######################################################################

# diff testing

MACRO(ADD_DIFF_TEST name f1 f2)
  ADD_TEST2(${name} ${TESTS}/results/${name}.csv
            ssdiff --verbose --format-csv ${f1} ${f2})
ENDMACRO ()

ADD_DIFF_TEST(ssdiff_loc ${TESTS}/loc/loc.csv ${TESTS}/loc/loc_perturb.csv)
