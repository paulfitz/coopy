
# assumes a build directory one-level within the source directory

BASE=..
./bin/ssmerge $BASE/tests/test005_base.csv $BASE/tests/test005_base.csv $BASE/tests/test005_fix_typo_add.csv
