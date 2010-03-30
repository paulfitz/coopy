INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/merge3x2d)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libcsv)
ADD_LIBRARY(csv libcsv/csv.h libcsv/libcsv.c)
ADD_LIBRARY(scancsv CsvFile.h CsvFile.cpp)
TARGET_LINK_LIBRARIES(scancsv merge3x2d csv)
ADD_EXECUTABLE(scanss scanss.cpp)
TARGET_LINK_LIBRARIES(scanss scancsv)


SET(TESTS ${CMAKE_SOURCE_DIR}/tests)

############################################################################
# check detection of header line

ADD_TEST(directory_header scanss --read ${TESTS}/test001_base.csv 
  --prop hdr --assert 0)
ADD_TEST(directory_header_missing scanss --read ${TESTS}/test001_base.csv 
  --remove_row 0 --prop hdr --assert -1)

ADD_TEST(latlong_header scanss --read ${TESTS}/test002_base.csv 
  --prop hdr --assert 0)
ADD_TEST(latlong_header_missing scanss --read ${TESTS}/test002_base.csv 
  --remove_row 0 --prop hdr --assert -1)

ADD_TEST(company_header scanss --read ${TESTS}/test003_base.csv 
  --prop hdr --assert 0)
ADD_TEST(company_header_missing scanss --read ${TESTS}/test003_base.csv 
  --remove_row 0 --prop hdr --assert -1)

ADD_TEST(car_headerless scanss --read ${TESTS}/test004_base.csv 
  --prop hdr --assert -1)
ADD_TEST(car_headerless2 scanss --read ${TESTS}/test004_base.csv 
  --remove_row 0 --prop hdr --assert -1)

# Parent   1 2 3
# Local  0 1 2 3
# Remote     2 3
# MERGE: 0   2 3
ADD_TEST(latlong_compare1 scanss
  --local --read ${TESTS}/test002_base.csv 
  --remote --read ${TESTS}/test002_base.csv --remove_row 0 --remove_row 1
  --parent --read ${TESTS}/test002_base.csv --remove_row 0
  --compare
  --prop height --assert 3)
