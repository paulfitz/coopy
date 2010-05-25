
# assumes a build directory one-level within the source directory

#VERBOSE="--verbose"
BASE=..
./bin/ssdiff $VERBOSE $BASE/tests/test005_base.csv $BASE/tests/test005_fix_typo_add.csv

echo " "
echo " ----- "
echo " "
./bin/ssdiff $VERBOSE --format-human $BASE/tests/test005_base.csv $BASE/tests/test005_replace_column_and_reorder.csv 

echo " "
echo " ----- "
echo " "
./bin/ssdiff $VERBOSE --format-human $BASE/tests/test005_base.csv $BASE/tests/test005_fix_typo_add.csv 

