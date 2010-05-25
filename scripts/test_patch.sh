
# assumes a build directory one-level within the source directory

#VERBOSE="--verbose"
BASE=..
./bin/ssdiff $VERBOSE $BASE/tests/test005_base.csv $BASE/tests/test005_fix_typo_add.csv

echo " "
echo " ----- "
echo " "
#cat $BASE/tests/test005_base.csv
#cat $BASE/tests/test005_replace_column_and_reorder.csv
./bin/ssdiff $VERBOSE --format-sql $BASE/tests/test005_base.csv $BASE/tests/test005_replace_column_and_reorder.csv 

