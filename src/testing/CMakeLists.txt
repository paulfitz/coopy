if (HAVE_TR1)
  ADD_DEFINITIONS("-std=c++0x" -DHAVE_TR1)
else ()
  # need hash_map or unordered_map or something for efficient hashing.
  # hash_map is deprecated under newer gcc versions.
  ADD_DEFINITIONS(-Wno-deprecated)
endif ()

INCLUDE_DIRECTORIES(${SHEET_INCLUDE_DIRS})

ADD_EXECUTABLE(fix_eol eol.cpp)

ADD_EXECUTABLE(test_sheet test_sheet.cpp)
TARGET_LINK_LIBRARIES(test_sheet coopy_full)

ADD_EXECUTABLE(test_viterbi test_viterbi.cpp)
TARGET_LINK_LIBRARIES(test_viterbi coopy_full)

ADD_EXECUTABLE(make_sheet make_sheet.cpp)
TARGET_LINK_LIBRARIES(make_sheet coopy_full)

SET(TESTS ${CMAKE_SOURCE_DIR}/tests)

get_target_property(testprg test_sheet LOCATION)
get_target_property(testprg_viterbi test_viterbi LOCATION)
get_target_property(ssformat ssformat LOCATION)
get_target_property(ssdiff ssdiff LOCATION)
get_target_property(sspatch sspatch LOCATION)
get_target_property(ssfold ssfold LOCATION)
get_target_property(ssmerge ssmerge LOCATION)
get_target_property(ssresolve ssresolve LOCATION)
get_target_property(ssrediff ssrediff LOCATION)
get_target_property(fix_eol fix_eol LOCATION)
if (USE_GNUMERIC)
  get_target_property(gnumeric_ss2html gnumeric_ss2html LOCATION)
endif ()
if (COMPILE_GUI)
  get_target_property(coopy coopy LOCATION)
  set(coopy ${coopy} --delay)
endif ()
set(SSCOMPARE ${ssdiff} --equal)


set(XLS_OR_CSV csv)
set(XLS)
if (USE_GNUMERIC)
  set(XLS_OR_CSV xls)
  set(XLS xls)
endif ()
set(SOCIALCALC)
if (USE_SOCIALCALC)
  set(SOCIALCALC socialcalc)
endif ()

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
#  --local --write ${TESTS}/result_company_merge_conflicting.csv
  --remote --read ${TESTS}/result_company_merge_conflicting.csv
  --diff
  --prop diffs --assert 0)

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
#  --local --write /tmp/out.csv
  --remote --read ${TESTS}/result_directory_merge_spelling.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(altitude_merge ${testprg}
  --parent --read ${TESTS}/test005_base.csv 
  --local --read ${TESTS}/test005_fix_typo_add.csv
  --remote --read ${TESTS}/test005_replace_column.csv
  --compare
#  --local --write ${TESTS}/result_altitude_merge.csv
  --local --write /tmp/out.csv
  --remote --read ${TESTS}/result_altitude_merge.csv
  --diff
  --prop diffs --assert 0)

ADD_TEST(altitude_merge_without_header ${testprg}
  --parent --read ${TESTS}/test005_base.csv --remove_row 0
  --local --read ${TESTS}/test005_fix_typo_add.csv --remove_row 0
  --remote --read ${TESTS}/test005_replace_column.csv --remove_row 0
  --compare
# --local --write /tmp/ref.csv
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
#  --local --write /tmp/ref.csv
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
# no longer fails, now CSV loading is Excel-style-forgiving
# SET_PROPERTY(TEST basic_merge_garbled PROPERTY WILL_FAIL TRUE)


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
  ADD_TEST(${name} ${${target}}
    --output=test_out_${name}.csv
    ${ARGN})
  ADD_TEST(${name}Compare ${ssdiff}
    --equals test_out_${name}.csv ${ref})
  #${CMAKE_COMMAND} -E compare_files 
  # test_out_${name} ${ref})
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
  ADD_TEST2(sqlite_${patcher} ${TESTS}/db/hdr_result_${patcher}.csv sspatch --tmp tmp.sqlite ${TESTS}/numbers.sqlite ${TESTS}/patch_v_0_2/${patcher}.txt)
endforeach()

#######################################################################
#######################################################################

# CSV <-> TDIFF testing

foreach(patcher patch_001_col_move patch_002_col_insert patch_003_col_insert patch_004_col_delete patch_005_row_update patch_006_row_insert patch_007_row_delete)
  SET(patch_file_csv1 ${TESTS}/patch_v_0_4/${patcher}.txt)
  SET(patch_file_tdiff test_out_tdiff_${patcher}.tdiff)
  SET(patch_file_csv2 test_out_tdiff_${patcher}_regenerated.csv)
  SET(result_csv1 test_out_tdiff_${patcher}_result_csv1.csv)
  SET(result_tdiff test_out_tdiff_${patcher}_result_tdiff.csv)
  SET(result_csv2 test_out_tdiff_${patcher}_result_csv2.csv)

  ADD_TEST(tdiff_${patcher} ${sspatch}
    --format tdiff --output ${patch_file_tdiff} ${patch_file_csv1})
  ADD_TEST(tdiff_${patcher}Reverse ${sspatch} 
    --format csv --output ${patch_file_csv2} ${patch_file_tdiff})

  ADD_TEST(tdiff_${patcher}_csv1 ${sspatch} 
    --output ${result_csv1} ${TESTS}/numbers.csv ${patch_file_csv1})
  ADD_TEST(tdiff_${patcher}_tdiff ${sspatch} 
    --output ${result_tdiff} ${TESTS}/numbers.csv ${patch_file_tdiff})
  ADD_TEST(tdiff_${patcher}_csv1 ${sspatch} 
    --output ${result_csv2} ${TESTS}/numbers.csv ${patch_file_csv2})

  ADD_TEST(tdiff_${patcher}Compare1 ${ssdiff} 
    --equals ${result_csv1} ${result_tdiff})
  ADD_TEST(tdiff_${patcher}Compare1 ${ssdiff} 
    --equals ${result_csv2} ${result_tdiff})
endforeach()

#######################################################################
#######################################################################

# EXCEL testing

set(EXCEL)

IF (USE_GNUMERIC)

  set(EXCEL xls)

  ADD_TEST2(excel_diff ${TESTS}/result_excel_diff.csv
    ssdiff --verbose --format-version=0.2 --format-csv ${TESTS}/numbers.xls ${TESTS}/numbers_change_five.xls)

  # tickles getopt_long bug on mingw, punt for now.
  # ADD_TEST2(excel_diff ${TESTS}/result_excel_diff.csv
  #  ssdiff --verbose --format-version 0.2 --format csv ${TESTS}/numbers.xls ${TESTS}/numbers_change_five.xls)

  MACRO(TEST_XLS name ref target)
    ADD_TEST(${name} ${${target}} 
      --output=test_out_${name}.xls
      ${ARGN})
    ADD_TEST(${name}Compare ${gnumeric_ss2html} 
      --compare
        #test_out_${name}.xls ${ref} ${ref})
	test_out_${name}.xls test_out_${name}.html ${ref})
  ENDMACRO()
  
  # Blank columns don't get exported by gnumeric, so some tests are not
  # that useful
  foreach(patcher patch_000_nothing patch_001_col_move patch_002_col_insert patch_003_col_insert patch_004_col_delete patch_005_row_update patch_006_row_insert patch_007_row_delete)
    TEST_XLS(excel_${patcher} ${TESTS}/color/result_excel_${patcher}.html sspatch ${TESTS}/numbers.xls ${TESTS}/patch_v_0_2/${patcher}.txt)
    
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

MACRO(ADD_ROUND_TRIP_TEST_BASE name P1 P2 INTERFORMAT FORMAT DIFF_EXTRA)
    set(ext ${FORMAT})
    if ("${ext}" STREQUAL "color")
      set(ext "csvs")
    endif()
    set(fname "${name}_patch.${ext}")
    set(fname2 "${name}_result.${INTERFORMAT}")
    ADD_TEST(${name}_diff ${ssdiff} 
      --output ${fname} --format ${FORMAT} ${ARGN} ${P1} ${P2} ${DIFF_EXTRA})
    ADD_TEST(${name}_patch ${sspatch} 
      --output ${fname2} ${P1} ${fname} ${DIFF_EXTRA})
    ADD_TEST(${name}Compare ${ssdiff}
      --equals ${fname2} ${P2})
ENDMACRO ()

MACRO(ADD_ROUND_TRIP_TEST name P1 P2 FORMAT)
  ADD_ROUND_TRIP_TEST_BASE(${name} ${P1} ${P2} csv ${FORMAT} "")
ENDMACRO ()

MACRO(ADD_ROUND_TRIP_TEST_CSVS name P1 P2 FORMAT)
  ADD_ROUND_TRIP_TEST_BASE(${name} ${P1} ${P2} csvs ${FORMAT} "")
ENDMACRO ()

MACRO(ADD_ROUND_TRIP_TEST_HEADERLESS name P1 P2 FORMAT)
  ADD_ROUND_TRIP_TEST_BASE(${name} ${P1} ${P2} csv ${FORMAT} "--headerless")
ENDMACRO ()


MACRO(ADD_ROUND_TRIP_TEST_BI name P1 P2 FORMAT)
  ADD_ROUND_TRIP_TEST(${name}_forward ${P1} ${P2} ${FORMAT})
  ADD_ROUND_TRIP_TEST(${name}_reverse ${P2} ${P1} ${FORMAT})
ENDMACRO()

MACRO(ADD_ROUND_TRIP_TEST_BI_CSVS name P1 P2 FORMAT)
  ADD_ROUND_TRIP_TEST_CSVS(${name}_forward ${P1} ${P2} ${FORMAT})
  ADD_ROUND_TRIP_TEST_CSVS(${name}_reverse ${P2} ${P1} ${FORMAT})
ENDMACRO()

MACRO(ADD_ROUND_TRIP_TEST_BI_BASE name P1 P2 INTER FORMAT)
  ADD_ROUND_TRIP_TEST_BASE(${name}_forward ${P1} ${P2} ${INTER} ${FORMAT} "")
  ADD_ROUND_TRIP_TEST_BASE(${name}_reverse ${P2} ${P1} ${INTER} ${FORMAT} "")
ENDMACRO()

foreach(FORMAT csv tdiff color)
  ADD_ROUND_TRIP_TEST_BI(joe_0000_${FORMAT} ${TESTS}/bug/joe_0000/local.csv ${TESTS}/bug/joe_0000/remote.csv ${FORMAT})
endforeach()

foreach(FORMAT csv tdiff color)
  SET(TARGETS named_numbers.csv named_numbers_wide.csv named_numbers_three_23.csv named_numbers_flip_column.csv named_numbers_add_row.csv test003_conflicting_change.csv bridges.csv)
  foreach(T1 ${TARGETS})
    foreach(T2 ${TARGETS})
      if (NOT ${T1} STREQUAL ${T2})
	get_filename_component(F1 ${T1} NAME_WE)
	get_filename_component(F2 ${T2} NAME_WE)
	string(REPLACE "named_numbers_" "" F1 ${F1})
	string(REPLACE "named_numbers_" "" F2 ${F2})
	  # tests need to be headerless since header nobbled
	ADD_ROUND_TRIP_TEST_HEADERLESS(rtnn_${F1}_${F2}_${FORMAT} ${TESTS}/${T1} ${TESTS}/${T2} ${FORMAT})
	if (NOT "${FORMAT}" STREQUAL "color") # column name sensitivity
	  ADD_ROUND_TRIP_TEST_HEADERLESS(rtfn_${F1}_${F2}_${FORMAT} dbi:csv:flip_vertical=1:file=${TESTS}/${T1} ${TESTS}/${T2} ${FORMAT})
	  ADD_ROUND_TRIP_TEST_HEADERLESS(rtnf_${F1}_${F2}_${FORMAT} ${TESTS}/${T1} dbi:csv:flip_vertical=1:file=${TESTS}/${T2} ${FORMAT})
	endif ()
      endif()
    endforeach()
  endforeach()
endforeach()

#######################################################################
#######################################################################

# quote testing

ADD_ROUND_TRIP_TEST(tdiff_quote_me ${TESTS}/quote_me.csv ${TESTS}/quote_me2.csv tdiff)
ADD_ROUND_TRIP_TEST(tdiff_quote_me2 ${TESTS}/quote_me2.csv ${TESTS}/quote_me.csv tdiff)
ADD_ROUND_TRIP_TEST(color_quote_me ${TESTS}/quote_me.csv ${TESTS}/quote_me2.csv color)
ADD_ROUND_TRIP_TEST(color_quote_me2 ${TESTS}/quote_me2.csv ${TESTS}/quote_me.csv color)
ADD_ROUND_TRIP_TEST_BI(column_names_with_spaces ${TESTS}/column_names_with_spaces_v1.csvs ${TESTS}/column_names_with_spaces_v2.csvs tdiff)

#######################################################################
#######################################################################

# conversion testing

ADD_TEST(format_book_sqlite ${ssformat} ${TESTS}/book/000/index.book format_book_sqlite.sqlite)


#######################################################################
#######################################################################

# header testing

foreach(prefix applicants capital argonaut)
  ADD_TEST(header_${prefix} ${ssformat} --header ${TESTS}/header/${prefix}.csv ${prefix}_header.csv)
  ADD_TEST(header_${prefix}_check ${ssdiff} --equals ${TESTS}/header/${prefix}_header.csv ${prefix}_header.csv)
endforeach()

#######################################################################
#######################################################################

# trust testing

ADD_ROUND_TRIP_TEST(trust_name_applicants ${TESTS}/header/applicants.csv ${TESTS}/header/applicants_fix.csv tdiff --id first --id last --named)

ADD_TEST(trust_name_applications_format ${ssdiff} --output trust_name_applications_check.tdiff --id first --id last --named --omit-format-name ${TESTS}/header/applicants.csv ${TESTS}/header/applicants_fix.csv)

ADD_TEST(trust_name_applications_check ${CMAKE_COMMAND} -E compare_files 
  trust_name_applications_check.tdiff 
  ${TESTS}/header/applicants_base_fix.tdiff)

#######################################################################
#######################################################################

# check insertions throughout range

FOREACH(x 1 2 3 4 5)
  ADD_TEST(insertion_omit_line_${x} ${sspatch} --output insertion_omit_line_${x}.csv --cmd "- |B=${x}|" ${TESTS}/numbers.csv)
  ADD_ROUND_TRIP_TEST(insertion_omit_line_${x}_rt insertion_omit_line_${x}.csv  ${TESTS}/numbers.csv tdiff)
  ADD_ROUND_TRIP_TEST(deletion_add_line_${x}_rt ${TESTS}/numbers.csv insertion_omit_line_${x}.csv tdiff)
ENDFOREACH()


#######################################################################
#######################################################################

# check csvs header
ADD_TEST(csvs_header_sqlite ${ssformat} ${TESTS}/numbers.${XLS_OR_CSV} csvs_header.sqlite)
ADD_TEST(csvs_header_csvs ${ssformat} csvs_header.sqlite csvs_header.csvs)
ADD_TEST(csvs_header_equal_sqlite ${ssdiff} --equal ${TESTS}/numbers.${XLS_OR_CSV} csvs_header.sqlite)
ADD_TEST(csvs_header_equal_csvs ${ssdiff} --equal ${TESTS}/numbers.${XLS_OR_CSV} csvs_header.csvs)
ADD_TEST(csvs_header_diff ${ssdiff} --verbose --output csvs_header_diff.tdiff --omit-format-name ${TESTS}/numbers.${XLS_OR_CSV} csvs_header.csvs)
ADD_TEST(csvs_header_diff_ok ${CMAKE_COMMAND} -E compare_files 
  csvs_header_diff.tdiff ${TESTS}/results/blank.tdiff)

ADD_ROUND_TRIP_TEST_BI_BASE(csvs_ez_diff ${TESTS}/header/broken_bridges.csvs ${TESTS}/header/bridges.csvs csvs tdiff)

ADD_ROUND_TRIP_TEST_BI_BASE(csvs_col_diff ${TESTS}/header/bridges.csvs ${TESTS}/header/bridges_quark.csvs csvs tdiff)

#######################################################################
#######################################################################

# check reviewed diff
FOREACH(rev all all_but_add just_add)
  ADD_TEST(review_${rev}_patch ${sspatch} --output review_${rev}_out.csv ${TESTS}/broken_bridges.csv ${TESTS}/review/review_${rev}.csv)
  ADD_TEST(review_${rev}_compare ${ssdiff} --equal ${TESTS}/review/review_${rev}_result.csv review_${rev}_out.csv)
ENDFOREACH()

ADD_TEST(review_convert ${sspatch} --output review_convert.tdiff --omit-format-name --omit-sheet-name --format tdiff ${TESTS}/review/review_all.csv)
ADD_TEST(review_convert_compare ${CMAKE_COMMAND} -E compare_files 
  review_convert.tdiff ${TESTS}/review/review_all.tdiff)


#######################################################################
#######################################################################

# check lots of moves
ADD_ROUND_TRIP_TEST(wii_move_tdiff ${TESTS}/bug/wii_0001/wii1.csv ${TESTS}/bug/wii_0001/wii2.csv tdiff)
ADD_ROUND_TRIP_TEST(wii_move_csv ${TESTS}/bug/wii_0001/wii1.csv ${TESTS}/bug/wii_0001/wii2.csv csv)


option(USE_PRIVATE_TESTS "Use non-public tests" OFF)
if (USE_PRIVATE_TESTS)
  set(PRIVATE_TESTS_ROOT "" CACHE PATH "Location of private tests") 
  if (PRIVATE_TESTS_ROOT)
    add_subdirectory(${PRIVATE_TESTS_ROOT} private_tests)
  endif()
endif ()

#######################################################################
#######################################################################

# Ignore case

ADD_TEST(ignore_case ${ssdiff} --output ignore_case.tdiff --ignore-case --omit-format-name ${TESTS}/bridges.csv ${TESTS}/case/bridges_case.csv)

ADD_TEST(ignore_case_check ${CMAKE_COMMAND} -E compare_files 
  ignore_case.tdiff 
  ${TESTS}/case/ignore_case.tdiff)

#######################################################################
#######################################################################

# folding isn't ready, but at least make sure a test can run

ADD_TEST(folding_base ${ssfold} --recipe ${TESTS}/fold/fold_org_loc.csvs --table organizations ${TESTS}/fold/contacts.csvs -)

ADD_TEST(folding_json ${ssfold} --recipe ${TESTS}/fold/fold_org_loc.csvs --table organizations ${TESTS}/fold/contacts.csvs folding_json_result.jsonbook)


#######################################################################
#######################################################################

# check conflicts

ADD_TEST(conflict_with_parent_tdiff ${ssdiff}  --omit-format-name --output conflict_with_parent.tdiff --parent ${TESTS}/conflict/bridges.csv ${TESTS}/conflict/bridges_v1.csv ${TESTS}/conflict/bridges_v2.csv)
ADD_TEST(conflict_with_parent_tdiff_Compare 
  ${CMAKE_COMMAND} -E compare_files conflict_with_parent.tdiff ${TESTS}/conflict/bridges_v1_v2_parent.tdiff)

ADD_TEST(conflict_with_parent_bang_tdiff ${ssdiff}  --omit-format-name --output conflict_with_parent_bang_tdiff.tdiff --parent ${TESTS}/conflict/bridges.csv ${TESTS}/conflict/bridges_v3.csv ${TESTS}/conflict/bridges_v4.csv)
ADD_TEST(conflict_with_parent_bang_tdiff_rediff ${ssrediff}  --omit-format-name --output conflict_with_parent_bang_tdiff_rediff.tdiff conflict_with_parent_bang_tdiff.tdiff)
ADD_TEST(conflict_with_parent_bang_tdiff_Compare 
  ${CMAKE_COMMAND} -E compare_files conflict_with_parent_bang_tdiff_rediff.tdiff conflict_with_parent_bang_tdiff.tdiff)


# Pending - hilite support
ADD_TEST(conflict_with_parent_hilite ${ssdiff}  --format hilite --output conflict_with_parent_hilite.csv --parent ${TESTS}/conflict/bridges.csv ${TESTS}/conflict/bridges_v1.csv ${TESTS}/conflict/bridges_v2.csv)
ADD_TEST(conflict_with_parent_hilite_theirs ${sspatch} --theirs --output conflict_with_parent_hilite_theirs.csv ${TESTS}/conflict/bridges_v1.csv conflict_with_parent_hilite.csv)
ADD_TEST(conflict_with_parent_hilite_ours ${sspatch} --ours --output conflict_with_parent_hilite_ours.csv ${TESTS}/conflict/bridges_v1.csv conflict_with_parent_hilite.csv)
ADD_TEST(conflict_with_parent_hilite_neither ${sspatch} --neither --output conflict_with_parent_hilite_neither.csv ${TESTS}/conflict/bridges_v1.csv conflict_with_parent_hilite.csv)
ADD_TEST(conflict_with_parent_hilite_check_ours ${SSCOMPARE}
  conflict_with_parent_hilite_ours.csv
  ${TESTS}/conflict/bridges_v1.csv)
ADD_TEST(conflict_with_parent_hilite_check_theirs ${SSCOMPARE}
  conflict_with_parent_hilite_theirs.csv
  ${TESTS}/conflict/bridges_v2.csv)
ADD_TEST(conflict_with_parent_hilite_check_neither ${SSCOMPARE}
  conflict_with_parent_hilite_neither.csv
  ${TESTS}/conflict/bridges.csv)


ADD_TEST(conflict_without_parent ${ssdiff}  --output conflict_without_parent.tdiff --omit-format-name ${TESTS}/conflict/bridges_v1.csv ${TESTS}/conflict/bridges_v2.csv)
ADD_TEST(conflict_without_parent_Compare 
  ${CMAKE_COMMAND} -E compare_files conflict_without_parent.tdiff ${TESTS}/conflict/bridges_v1_v2_no_parent.tdiff)

# not really conflicts, but related

ADD_TEST(patch_merge ${ssdiff}  --omit-format-name --unordered --output patch_merge.tdiff --parent ${TESTS}/conflict/indexed_bridges.csv ${TESTS}/conflict/indexed_bridges_v1.csv ${TESTS}/conflict/indexed_bridges_v2.csv)
ADD_TEST(patch_merge_Compare
  ${CMAKE_COMMAND} -E compare_files patch_merge.tdiff ${TESTS}/conflict/indexed_bridges_v1_v2.tdiff)

# check results
ADD_TEST(ssmerge_conflict_fails ${ssmerge} ${TESTS}/conflict/bridges.csv ${TESTS}/conflict/bridges_v1.csv ${TESTS}/conflict/bridges_v2.csv)
SET_PROPERTY(TEST ssmerge_conflict_fails PROPERTY WILL_FAIL TRUE)

ADD_TEST(ssmerge_conflict2_fails ${ssmerge} --output ssmerge_conflict2.csv ${TESTS}/named_numbers.csv ${TESTS}/named_numbers_change_five.csv ${TESTS}/named_numbers_change_five_alt.csv)
SET_PROPERTY(TEST ssmerge_conflict2_fails PROPERTY WILL_FAIL TRUE)

ADD_TEST(ssmerge_conflict2_header ${ssformat} --header ssmerge_conflict2.csv ssmerge_conflict2_header.csv)

ADD_TEST(ssmerge_conflict2_header_check ${ssdiff} --equal --verbose ssmerge_conflict2_header.csv ${TESTS}/named_numbers_merge_header.csv)


#######################################################################
#######################################################################

# sundry bug reports

ADD_TEST(insert_col_bug1 ${ssdiff} --parent ${TESTS}/header/insert/base.csvs ${TESTS}/header/insert/ver1.csvs ${TESTS}/header/insert/ver2.csvs)
ADD_TEST(insert_col_bug2 ${ssdiff} --parent ${TESTS}/header/insert/base.csvs ${TESTS}/header/insert/ver2.csvs ${TESTS}/header/insert/ver1.csvs)


IF (USE_GNUMERIC)

  ADD_TEST(peeps_to_xls ${ssformat} ${TESTS}/bug/peeps_0002/people.csvs peeps_to_xls.xls)
  ADD_TEST(peeps_to_xls_Compare ${ssdiff} --equal ${TESTS}/bug/peeps_0002/people.csvs peeps_to_xls.xls)

ENDIF ()

ADD_TEST(csvs_preserve_pk_index_pre ${ssformat} --index ${TESTS}/numbers.sqlite csvs_preserve_pk.1.csv)
ADD_TEST(csvs_preserve_pk ${ssformat} --table numbers ${TESTS}/numbers.sqlite csvs_preserve_pk.sqlite)
ADD_TEST(csvs_preserve_pk_index_post ${ssformat} --index csvs_preserve_pk.sqlite csvs_preserve_pk.2.csv)
ADD_TEST(csvs_preserve_pk_check ${ssdiff} --equal csvs_preserve_pk.1.csv csvs_preserve_pk.2.csv)

ADD_TEST(wide_format ${ssformat} ${TESTS}/bug/wide_0004/wide.csv wide_format.csvs)
ADD_TEST(wide_format_check ${ssdiff} ${TESTS}/bug/wide_0004/wide.csv wide_format.csvs)

ADD_ROUND_TRIP_TEST_BI(highlighter_small_file ${TESTS}/bug/highlighter_0005/orig.csv ${TESTS}/bug/highlighter_0005/mod.csv tdiff)
ADD_ROUND_TRIP_TEST_BI(highlighter_small_file_hilite ${TESTS}/bug/highlighter_0005/orig.csv ${TESTS}/bug/highlighter_0005/mod.csv color)


ADD_TEST(patch_with_table_parameter1 ${sspatch} 
  --table organizations
  --cmd "= |id=4|title=*->Dreamer Stealer|"
  --output patch_with_table_parameter1.csvs
  ${TESTS}/book/contact_base.csvs)

ADD_TEST(patch_with_table_parameter1_compare ${ssdiff} 
  --equals
  patch_with_table_parameter1.csvs
  ${TESTS}/book/contact_dreamer.csvs)

ADD_TEST(format_fossil_repo ${ssformat} 
  ${TESTS}/fossil/empty_fossil_repo.fossil
  format_fossil_repo.csvs)

ADD_TEST(format_fossil_repo_strict ${ssformat} 
  --strict
  ${TESTS}/fossil/empty_fossil_repo.fossil
  format_fossil_repo.csvs)


ADD_TEST(runon_tdiff_direct ${ssrediff} --format ops  --output runon_tdiff1.csvs ${TESTS}/bug/rediff/patch.tdiff)
ADD_TEST(runon_tdiff_regen ${ssrediff} --format tdiff --output runon_regen.tdiff ${TESTS}/bug/rediff/patch.tdiff)
ADD_TEST(runon_tdiff_indirect ${ssrediff} --format ops  --output runon_tdiff2.csvs runon_regen.tdiff)
ADD_TEST(runon_tdiff_match ${ssdiff} --equals runon_tdiff1.csvs runon_tdiff2.csvs)

#######################################################################
#######################################################################

# Command line interface

set(NEAT --omit-format-name)
set(TARG ${TESTS}/numbers_three_23.csv)

foreach(format1 sqlite csv sqlitext jsonbook ${EXCEL})
  foreach(format2 sqlite csv sqlitext jsonbook ${EXCEL})
    foreach(input numbers numbers_change_five numbers_add_row)
      SET(f ${format1}_${format2}_${input})
      ADD_TEST(cli_format_${f} ${ssformat} ${TESTS}/${input}.csv cli_format_${f}.${format1})
      ADD_TEST(cli_format_${f}_diff ${ssdiff} ${NEAT} --output cli_format_${f}_diff.tdiff cli_format_${f}.${format1} ${TARG})
      ADD_TEST(cli_format_${f}_patch ${sspatch} --output cli_format_${f}_patch.${format1} cli_format_${f}.${format1} cli_format_${f}_diff.tdiff)
      ADD_TEST(cli_format_${f}_check ${ssdiff} ${NEAT} --equal cli_format_${f}_patch.${format1} ${TARG})
      ADD_TEST(cli_format_${f}_merge1 ${ssmerge} --output cli_format_${f}_merge1.${format1} ${TESTS}/numbers.csv cli_format_${f}.${format1} ${TARG})
      ADD_TEST(cli_format_${f}_merge2 ${ssmerge} --output cli_format_${f}_merge2.${format2} ${TESTS}/numbers.csv cli_format_${f}.${format1} ${TARG})
      ADD_TEST(cli_format_${f}_check2 ${ssdiff} ${NEAT} --equal cli_format_${f}_merge1.${format1} cli_format_${f}_merge2.${format2})
    endforeach()
  endforeach()
endforeach()

#######################################################################
#######################################################################

# REDIFF FILTERING

ADD_TEST(ops_format ${ssrediff} --format ops --output ops_format.csvs ${TESTS}/review/review_all.tdiff)
ADD_TEST(ops_format_compare ${ssdiff} --equal ops_format.csvs ${TESTS}/review/review_all.ops)

ADD_TEST(ssrediff_contacts ${ssdiff} --output ssrediff_contacts.tdiff ${TESTS}/fold/contacts.csvs ${TESTS}/fold/contacts_add_multi.csvs)
ADD_TEST(ssrediff_contacts_org ${ssrediff} --table organizations --output ssrediff_contacts_org.tdiff ssrediff_contacts.tdiff)
ADD_TEST(ssrediff_contacts_org_apply ${sspatch} --output ssrediff_contacts_org_apply.csvs ${TESTS}/fold/contacts.csvs ssrediff_contacts_org.tdiff)
ADD_TEST(ssrediff_contacts_org_check ${ssdiff} --equal ssrediff_contacts_org_apply.csvs ${TESTS}/fold/contacts_add.csvs)

#######################################################################
#######################################################################

if (COMPILE_GUI)

  IF (USE_GNUMERIC)

    set(TC test_coopy)

    add_test(coopy_space_prep0 ${CMAKE_COMMAND} -E remove_directory ${TC})
    add_test(coopy_space_prep1 ${CMAKE_COMMAND} -E make_directory ${TC})
    add_test(coopy_space_prep3 ${CMAKE_COMMAND} -E make_directory ${TC}/store)
    add_test(coopy_space_prep4 ${CMAKE_COMMAND} -E make_directory ${TC}/clone1)
    add_test(coopy_space_prep5 ${CMAKE_COMMAND} -E make_directory ${TC}/clone2)
    
    add_test(coopy_space_new ${coopy} --new ${TC}/store)
    add_test(coopy_space_clone1 ${coopy} -v --clone=${TC}/store ${TC}/clone1)
    add_test(coopy_space_clone2 ${coopy} --clone=${TC}/store ${TC}/clone2)
    
    add_test(coopy_space_data ${ssformat} ${TESTS}/coopy/mini-people.csvs
      ${TC}/people.xls)
    
    add_test(coopy_space_data_pause ${coopy} --delay --silent ${TC}/store)
    
    add_test(coopy_space_data_add ${coopy} --key=ppl --add=${TC}/people.xls ${TC}/clone1)
    add_test(coopy_space_data_push ${coopy} -m "add people" --push ${TC}/clone1)
    add_test(coopy_space_data_pull ${coopy} --pull ${TC}/clone2)
    add_test(coopy_space_data_export ${coopy} --key=ppl --export=${TC}/people2.xls ${TC}/clone2)
 
    add_test(coopy_space_data_pause2 ${coopy} --delay --silent ${TC}/store)
    
    add_test(coopy_space_data_mod ${sspatch} --inplace ${TC}/people2.xls --cmd "= |Name=Noemi->Nomers|")
    add_test(coopy_space_data_push2 ${coopy} -m "change Noemi's name" --push ${TC}/clone2)
    add_test(coopy_space_data_changed1 ${ssdiff} --equal ${TC}/people.xls ${TC}/people2.xls)
    set_property(TEST coopy_space_data_changed1 PROPERTY WILL_FAIL TRUE)

    add_test(coopy_space_data_pause3 ${coopy} --delay --silent ${TC}/store)

    add_test(coopy_space_data_pull2 ${coopy} --pull ${TC}/clone1)
    add_test(coopy_space_data_changed2 ${ssdiff} --equal ${TC}/people.xls ${TC}/people2.xls)

    add_test(coopy_space_age_pause_prechange ${coopy} --delay --silent ${TC}/store)
    add_test(coopy_space_age_change1 ${sspatch} --inplace ${TC}/people.xls --cmd "= |Name=Paul|Age:*->37|")
    add_test(coopy_space_age_change2 ${sspatch} --inplace ${TC}/people2.xls --cmd "= |Name=Leo|Age:*->4|")

    add_test(coopy_space_age_push1 ${coopy} -m "change Paul's age" --push ${TC}/clone1)
    add_test(coopy_space_age_pull1 ${coopy} --pull ${TC}/clone2)
    add_test(coopy_space_age_push2 ${coopy} -m "change Leo's age" --push ${TC}/clone2)
    add_test(coopy_space_age_pull2 ${coopy} --pull ${TC}/clone1)

    add_test(coopy_space_age_match  ${ssdiff} --equal ${TC}/people.xls ${TC}/people2.xls)

    add_test(coopy_space_conflict_change1 ${sspatch} --inplace ${TC}/people.xls --cmd "= |Name=Paul|Zip:*->07028|")
    add_test(coopy_space_conflict_change2 ${sspatch} --inplace ${TC}/people2.xls --cmd "= |Name=Paul|Zip:*->99999|")

    add_test(coopy_space_conflict_pause3 ${coopy} --delay --silent ${TC}/store)

    add_test(coopy_space_conflict_push1 ${coopy} -m "change ZIP" --push ${TC}/clone1)
    add_test(coopy_space_conflict_pull1 ${coopy} --pull ${TC}/clone2)

    add_test(coopy_space_conflict_unresolved ${ssresolve} ${TC}/people2.xls)
    set_property(TEST coopy_space_conflict_unresolved PROPERTY WILL_FAIL TRUE)

    #add_test(coopy_space_conflict_resolve ${ssresolve} --theirs ${TC}/people2.xls)
     #add_test(coopy_space_conflict_resolved ${ssresolve} ${TC}/people2.xls)
 
  endif ()

endif ()

#######################################################################
#######################################################################

# Index format

add_test(index_format_prep ${ssdiff} --format index --output index_format.csvs ${TESTS}/named_numbers.csv ${TESTS}/named_numbers_wide.csv)
add_test(index_format_compare ${ssdiff} --equals index_format.csvs ${TESTS}/link_nn_nn_wide.csvs)

add_test(index_format_contacts_prep ${sspatch} --output index_format_contacts.csvs ${TESTS}/fold/contacts.csvs --cmd "@@@ people" --cmd "= |first=Tom|last=*->Thumb|" --cmd "- |first=Frank|" --cmd "@@@ ppl2org" --cmd "- |ppl_id=3|" --cmd "- |ppl_id=3|")

add_test(index_format_contacts_compare ${ssdiff} --equals index_format_contacts.csvs ${TESTS}/fold/thumb.csvs)

add_test(index_format_contacts_index ${ssdiff} --format index --output index_format_contacts_index.sqlite ${TESTS}/fold/contacts.csvs index_format_contacts.csvs)

add_test(index_format_contacts_index_compare ${ssdiff} --equals index_format_contacts_index.sqlite ${TESTS}/fold/contacts_to_thumb.csvs)


#######################################################################
#######################################################################

# Keep examples working

ADD_TEST(example_ssmerge1 ${ssmerge} 
  --output example_ssmerge1.csv
  ${TESTS}/examples/numbers_buggy.csv
  ${TESTS}/examples/numbers.csv
  ${TESTS}/examples/numbers_buggy_add.csv)

ADD_TEST(example_ssmerge1_compare ${ssdiff} --equals 
  example_ssmerge1.csv
  ${TESTS}/examples/numbers_more.csv)

ADD_TEST(example_ssmerge2 ${ssmerge} 
  --output example_ssmerge2.csv
  ${TESTS}/examples/numbers_buggy.csv 
  ${TESTS}/examples/numbers.csv 
  ${TESTS}/examples/numbers_conflict.csv)
SET_PROPERTY(TEST example_ssmerge2 PROPERTY WILL_FAIL TRUE)

foreach (RESOLVER ours theirs neither)

  ADD_TEST(example_ssmerge_${RESOLVER} ${ssmerge} 
    --output example_ssmerge_${RESOLVER}.csv
    --${RESOLVER}
    ${TESTS}/examples/numbers_buggy.csv 
    ${TESTS}/examples/numbers.csv 
    ${TESTS}/examples/numbers_conflict.csv)

  ADD_TEST(example_ssmerge_${RESOLVER}_compare ${ssdiff} --equals 
    example_ssmerge_${RESOLVER}.csv
    ${TESTS}/examples/numbers_merge_${RESOLVER}.csv)

endforeach()


#######################################################################
#######################################################################

# Keep GUT examples working

if (USE_GNUMERIC)
  ADD_TEST(gut_example_bob_diff ${ssdiff} 
    --output gut_example_bob_diff.tdiff
    --omit-format-name
    ${TESTS}/gut/root.xls
    ${TESTS}/gut/bob.xls)
  
  ADD_TEST(gut_example_bob_diff_ref ${ssrediff} 
    --output gut_example_bob_diff_ref.tdiff
    --omit-format-name
    ${TESTS}/gut/bobs_mods.tdiff)
  
  ADD_TEST(gut_example_bob_diff_cmp ${CMAKE_COMMAND} -E compare_files 
    gut_example_bob_diff.tdiff
    gut_example_bob_diff_ref.tdiff)
  
  ADD_TEST(gut_example_alice_bob ${sspatch} 
    --output gut_example_alice_bob.xls
    ${TESTS}/gut/alice.xls
    gut_example_bob_diff.tdiff)
  
  ADD_TEST(gut_example_alice_bob_cmp ${ssdiff} --equal
    gut_example_alice_bob.xls
    ${TESTS}/gut/alice_and_bob.xls)
endif ()

#######################################################################
#######################################################################

# Applying to foreign database

ADD_TEST(foreign_contacts_diff ${ssdiff} --output foreign_contacts_diff.tdiff --meta ${TESTS}/fold/contacts_meta.tdiff ${TESTS}/fold/contacts.csvs ${TESTS}/fold/contacts_add.csvs)
ADD_TEST(foreign_contacts_apply ${sspatch} --output foreign_contacts_apply.csvs ${TESTS}/fold/contacts.csvs foreign_contacts_diff.tdiff)
ADD_TEST(foreign_contacts_compare ${ssdiff} --equal foreign_contacts_apply.csvs ${TESTS}/fold/contacts_add_foreign.csvs)

ADD_TEST(foreign_contacts_multi_diff ${ssdiff} --output foreign_contacts_multi_diff.tdiff --meta ${TESTS}/fold/contacts_meta.tdiff ${TESTS}/fold/contacts.csvs ${TESTS}/fold/contacts_add_multi.csvs)
ADD_TEST(foreign_contacts_multi_apply ${sspatch} --output foreign_contacts_multi_apply.csvs ${TESTS}/fold/contacts.csvs foreign_contacts_multi_diff.tdiff)
ADD_TEST(foreign_contacts_multi_compare ${ssdiff} --equal foreign_contacts_multi_apply.csvs ${TESTS}/fold/contacts_add_multi_foreign.csvs)

foreach (D directory directory_alice directory_bob)
  ADD_TEST(foreign_directory_${D}_create ${ssdiff} --omit-format --output foreign_directory_${D}.tdiff ${TESTS}/directory/${D}.sqlite)
  ADD_TEST(foreign_directory_${D}_compare ${CMAKE_COMMAND} -E compare_files foreign_directory_${D}.tdiff ${TESTS}/directory/${D}.tdiff)
endforeach()

ADD_TEST(foreign_directory_ra_diff ${ssdiff} --output foreign_directory_ra_diff.tdiff ${TESTS}/directory/directory.sqlite ${TESTS}/directory/directory_alice.sqlite)
ADD_TEST(foreign_directory_rab_patch ${sspatch} --output foreign_directory_rab.sqlite ${TESTS}/directory/directory_bob.sqlite foreign_directory_ra_diff.tdiff)
ADD_TEST(foreign_directory_rab_compare ${ssdiff} --equal ${TESTS}/directory/directory_bob_add_alice.sqlite foreign_directory_rab.sqlite)

ADD_TEST(foreign_directory_rb_diff ${ssdiff} --output foreign_directory_rb_diff.tdiff ${TESTS}/directory/directory.sqlite ${TESTS}/directory/directory_bob.sqlite)
ADD_TEST(foreign_directory_rba_patch ${sspatch} --output foreign_directory_rba.sqlite ${TESTS}/directory/directory_alice.sqlite foreign_directory_rb_diff.tdiff)
ADD_TEST(foreign_directory_rba_compare ${ssdiff} --equal ${TESTS}/directory/directory_alice_add_bob.sqlite foreign_directory_rba.sqlite)

ADD_TEST(foreign_directory_merge_rab ${ssmerge} --output foreign_directory_merge_rab.sqlite ${TESTS}/directory/directory.sqlite ${TESTS}/directory/directory_alice.sqlite ${TESTS}/directory/directory_bob.sqlite)
ADD_TEST(foreign_directory_merge_rab_compare ${ssdiff} --equal ${TESTS}/directory/directory_alice_add_bob.sqlite foreign_directory_merge_rab.sqlite)

ADD_TEST(foreign_directory_merge_rba ${ssmerge} --output foreign_directory_merge_rba.sqlite ${TESTS}/directory/directory.sqlite ${TESTS}/directory/directory_bob.sqlite ${TESTS}/directory/directory_alice.sqlite)
ADD_TEST(foreign_directory_merge_rba_compare ${ssdiff} --equal ${TESTS}/directory/directory_bob_add_alice.sqlite foreign_directory_merge_rba.sqlite)


#######################################################################
#######################################################################

# sqlite <-> sqlitext conversions

ADD_TEST(sqlite_sqlitext_base1 ${ssformat} ${TESTS}/bridges.csv sqlite_sqlitext_base.sqlite)
ADD_TEST(sqlite_sqlitext_base2 ${ssformat} ${TESTS}/bridges.csv sqlite_sqlitext_base.sqlitext)
ADD_TEST(sqlite_sqlitext_fwd ${ssformat} sqlite_sqlitext_base.sqlite sqlite_sqlitext_base.2.sqlitext)
ADD_TEST(sqlite_sqlitext_rev ${ssformat} sqlite_sqlitext_base.sqlitext sqlite_sqlitext_base.2.sqlite)
ADD_TEST(sqlite_sqlitext_fwd_chk ${SSCOMPARE} ${TESTS}/bridges.csv sqlite_sqlitext_base.2.sqlite)
ADD_TEST(sqlite_sqlitext_rev_chk ${SSCOMPARE} ${TESTS}/bridges.csv sqlite_sqlitext_base.2.sqlitext)


#######################################################################
#######################################################################

# stream delimitation test

MACRO(ADD_STREAM_TEST name in out)
  ADD_TEST(${name} ${CMAKE_COMMAND} -Dname=${name} "-Din:STRING=${in}" "-Dout:STRING=${out}" "-Dprocess:STRING=${ARGN}" -P ${CMAKE_CURRENT_SOURCE_DIR}/harness.cmake)
ENDMACRO()

MACRO(ADD_STREAM_OUT_TEST name out)
  ADD_TEST(${name} ${CMAKE_COMMAND} -Dname=${name} "-Dout:STRING=${out}" "-Dprocess:STRING=${ARGN}" -P ${CMAKE_CURRENT_SOURCE_DIR}/harness.cmake)
ENDMACRO()

ADD_STREAM_TEST(delimit_stdin_base ${TESTS}/format/bi_bridge.txt delimit_stdin_base.tdiff ${ssdiff} --omit-format-name --omit-sheet-name dbi:csv:length=header:file=- dbi:csv:length=header:file=-)
ADD_TEST(delimit_stdin_fix ${fix_eol} delimit_stdin_base.tdiff delimit_stdin_fix.tdiff)
ADD_TEST(delimit_stdin_check ${CMAKE_COMMAND} -E compare_files delimit_stdin_fix.tdiff ${TESTS}/format/bi_bridge_diff.tdiff)

ADD_STREAM_TEST(delimit_stdin_json_base ${TESTS}/format/bi_bridge.txt delimit_stdin_json_base.jsonbook ${ssdiff} --format hilite --omit-format-name --omit-sheet-name --output dbi:jsonbook:file=- dbi:csv:length=header:file=- dbi:csv:length=header:file=-)
ADD_TEST(delimit_stdin_json_fix ${fix_eol} delimit_stdin_json_base.jsonbook delimit_stdin_json_fix.jsonbook)
ADD_TEST(delimit_stdin_json_check ${CMAKE_COMMAND} -E compare_files delimit_stdin_json_fix.jsonbook ${TESTS}/format/bi_bridge_diff.jsonbook)

#######################################################################
#######################################################################

# Column renaming
# Used to be handled in same way as cell changes, but now separated
# as a schema change.

foreach(FORMAT csv tdiff color)
  ADD_ROUND_TRIP_TEST_BI(column_rename_base_${FORMAT} ${TESTS}/bridges.csv ${TESTS}/column_name/bridges_column_name.csv ${FORMAT})
  ADD_ROUND_TRIP_TEST_BI(column_rename_cool_${FORMAT} ${TESTS}/column_name/bridges_cool.csv ${TESTS}/column_name/bridges_cool_exclaim.csv ${FORMAT})
endforeach()

ADD_STREAM_OUT_TEST(column_rename_stdout column_rename_stdout.csv ${ssdiff} --format hilite --output dbi:csv:header=-1:file=- ${TESTS}/column_name/bridges_cool.csv ${TESTS}/column_name/bridges_cool_exclaim.csv)
ADD_TEST(column_rename_stdout_ref ${ssdiff} --output column_rename_stdout_ref.csv --format hilite ${TESTS}/column_name/bridges_cool.csv ${TESTS}/column_name/bridges_cool_exclaim.csv)
ADD_TEST(column_rename_stdout_equal ${ssdiff} --equal column_rename_stdout.csv column_rename_stdout_ref.csv)

#######################################################################
#######################################################################

# Small tables edge cases

foreach(FORMAT csv tdiff color)
  ADD_ROUND_TRIP_TEST_BI(edge_name_age_${FORMAT} ${TESTS}/edge/name_age_1.csv ${TESTS}/edge/name_age_2.csv ${FORMAT})
  ADD_ROUND_TRIP_TEST_BI(edge_bridge_${FORMAT} ${TESTS}/edge/designer_1.csv ${TESTS}/bridges.csv ${FORMAT})
  ADD_ROUND_TRIP_TEST_BI(edge_user5_${FORMAT} ${TESTS}/edge/user_2.csv ${TESTS}/edge/user_5.csv ${FORMAT})
endforeach()

add_test(edge_user ${ssdiff} ${TESTS}/edge/user_2.csv ${TESTS}/edge/user_0.csv)

add_test(edge_user_2_5_stats ${ssdiff} --output edge_user_2_5_stats.csv
  --format stats --act schema
  ${TESTS}/edge/user_2.csv ${TESTS}/edge/user_5.csv)
add_test(edge_user_2_5_equal ${ssdiff} --equal edge_user_2_5_stats.csv ${TESTS}/edge/user_2_5_schema_stats.csv)

add_test(edge_user_2_5_row_stats ${ssdiff} --output edge_user_2_5_row_stats.csv
  --format stats --act insert --act update --act delete
  ${TESTS}/edge/user_2.csv ${TESTS}/edge/user_5.csv)
add_test(edge_user_2_5_row_equal ${ssdiff} --equal edge_user_2_5_row_stats.csv ${TESTS}/edge/user_2_5_row_stats.csv)

ADD_STREAM_OUT_TEST(edge_bridge_stdout edge_bridge_stdout.csv ${ssdiff} --format hilite --output dbi:csv:header=-1:file=- ${TESTS}/edge/designer_1.csv ${TESTS}/bridges.csv)
ADD_TEST(edge_bridge_stdout_ref ${ssdiff} --output edge_bridge_stdout_ref.csv --format hilite ${TESTS}/edge/designer_1.csv ${TESTS}/bridges.csv)
ADD_TEST(edge_bridge_stdout_equal ${ssdiff} --equal edge_bridge_stdout.csv edge_bridge_stdout_ref.csv)


#######################################################################
#######################################################################

# conflict _MERGE_ column

set(FORMATS ${XLS} csvs csv)

foreach(FORMAT0 ${FORMATS})
  foreach(FORMAT1 ${FORMATS})
    foreach(FORMAT2 ${FORMATS})
      set (FORMAT ${FORMAT0}_${FORMAT1}_${FORMAT2})
      add_test(people_header_${FORMAT}_base0 ${ssformat} ${TESTS}/coopy/mini-people.csvs
	people_header0.${FORMAT0})
      add_test(people_header_${FORMAT}_base1 ${ssformat} ${TESTS}/coopy/mini-people.csvs
	people_header1.${FORMAT1})
      add_test(people_header_${FORMAT}_base2 ${ssformat} ${TESTS}/coopy/mini-people.csvs
	people_header2.${FORMAT2})
      add_test(people_header_${FORMAT}_change1 ${sspatch} --inplace people_header1.${FORMAT1} --cmd "= |Name=Paul|Zip:*->07028|")
      add_test(people_header_${FORMAT}_change2 ${sspatch} --inplace people_header2.${FORMAT2} --cmd "= |Name=Paul|Zip:*->07042|")
      add_test(people_header_${FORMAT}_merge ${ssmerge} people_header0.${FORMAT0} people_header1.${FORMAT1} people_header2.${FORMAT2})
      set_property(TEST people_header_${FORMAT}_merge PROPERTY WILL_FAIL TRUE)
      add_test(people_header_${FORMAT}_merge_inplace ${ssmerge} --inplace people_header0.${FORMAT0} people_header1.${FORMAT1} people_header2.${FORMAT2})
      set_property(TEST people_header_${FORMAT}_merge_inplace PROPERTY WILL_FAIL TRUE)
    endforeach()
  endforeach()
endforeach()


#######################################################################
#######################################################################

# multiple-sheet comparison with hilite format

foreach(FORMAT tdiff csv color)
  ADD_ROUND_TRIP_TEST_BI_CSVS(multisheet_diff_directory_alice_${FORMAT} ${TESTS}/directory/directory.sqlite ${TESTS}/directory/directory_alice.sqlite ${FORMAT})
  ADD_ROUND_TRIP_TEST_BI_CSVS(multisheet_diff_directory_blank_${FORMAT} ${TESTS}/directory/directory.sqlite ${TESTS}/directory/directory_blank.sqlite ${FORMAT})
  ADD_ROUND_TRIP_TEST_BI_CSVS(multisheet_diff_directory_blank2_${FORMAT} ${TESTS}/directory/directory_blank.sqlite ${TESTS}/directory/directory_blank.sqlite ${FORMAT})
endforeach ()

#######################################################################
#######################################################################

# index diff

add_test(index_diff_base ${ssmerge} --format index 
  --output index_diff.base.csvs
  ${TESTS}/test005_base.csv 
  ${TESTS}/test005_fix_typo_add.csv
  ${TESTS}/test005_replace_column.csv)
add_test(index_diff_compare ${ssdiff} --equal index_diff.base.csvs
  ${TESTS}/results/altitude_index.csvs)

#######################################################################
#######################################################################

# bug related to row rematching

foreach(FORMAT tdiff csv color)
  ADD_ROUND_TRIP_TEST_BI_CSVS(row_rematch_bug_${FORMAT} ${TESTS}/directory/directory.sqlite ${TESTS}/directory/directory_alice.sqlite ${FORMAT})
endforeach()


#######################################################################
#######################################################################

# socialcalc

if (USE_SOCIALCALC)

  add_test(socialcalc_convert_to ${ssformat} ${TESTS}/numbers.csv socialcalc_convert_to.sc)
  add_test(socialcalc_convert_to_check ${ssdiff} --equal socialcalc_convert_to.sc ${TESTS}/numbers.csv)
  add_test(socialcalc_convert_from ${ssformat} socialcalc_convert_to.sc socialcalc_convert_from.csv)
  add_test(socialcalc_convert_from_check ${ssdiff} --equal socialcalc_convert_from.csv ${TESTS}/numbers.csv)


  # NOTE: socialcalc has an issue with exporting a "foo,bar" cell 
  # in ${TESTS}/bridges.csv

  foreach(patcher patch_001_col_move patch_002_col_insert patch_003_col_insert patch_004_col_delete patch_005_row_update patch_006_row_insert patch_007_row_delete)
    ADD_TEST2(socialcalc_${patcher} ${TESTS}/db/hdr_result_${patcher}.csv sspatch ${TESTS}/numbers.socialcalc ${TESTS}/patch_v_0_2/${patcher}.txt)
  endforeach()


endif ()


#######################################################################
#######################################################################

# Try lots of permutations, see ./scripts/bridge_permutations

foreach(FORMAT tdiff csv color)
  file(GLOB perm ${TESTS}/perms/bridges_*.csv)
  foreach(p2 ${perm})
    string(REGEX REPLACE ".*bridges_" "" s2 ${p2})
    string(REPLACE ".csv" "" s2 ${s2})
    ADD_ROUND_TRIP_TEST_BI(bridge_perm_${FORMAT}_${s2} ${TESTS}/bridges.csv ${p2} ${FORMAT})
  endforeach()
endforeach()

#######################################################################
#######################################################################

# standard input / standard output tests

IF (USE_GNUMERIC)
  ADD_STREAM_TEST(stdout_excel ${TESTS}/bridges.csv stdout_excel.xls ${ssformat} - dbi:gnumeric:format=.xls::file=-)
  ADD_TEST(stdout_excel_check ${ssdiff} --equal ${TESTS}/bridges.csv stdout_excel.xls)
ENDIF ()
ADD_STREAM_TEST(stdout_jsonbook ${TESTS}/bridges.csv stdout_jsonbook.jsonbook ${ssformat} - dbi:jsonbook::file=-)

# PENDING
# ADD_TEST(stdout_jsonbook_check ${ssdiff} --equal dbi:csv:header=-1::file=${TESTS}/bridges.csv stdout_jsonbook.jsonbook)

#######################################################################
#######################################################################

if (USE_JAVASCRIPT)

  set(book1 ${TESTS}/format/bi_bridge_diff.jsonbook)

  get_target_property(test_light test_light LOCATION)
  get_target_property(test_light_js test_light_js LOCATION)

  add_test(js_parse_cpp ${test_light} ${book1} js_parse_cpp.log)
  add_test(js_parse_js ${test_light_js} ${book1} js_parse_js.log)
  add_test(js_parse_match ${CMAKE_COMMAND} -E compare_files js_parse_cpp.log js_parse_js.log)

endif()
