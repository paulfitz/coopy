
# run from build directory

make make_sheet ssmerge
./bin/make_sheet 4000 10 > test.csv
tac test.csv > test_reverse.csv
rm -f gmon.out junk
time ./bin/ssmerge test.csv test.csv test_reverse.csv out.csv
gprof ./bin/ssmerge > junk
