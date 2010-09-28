#!/bin/bash

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

SRC=`cat CMakeCache.txt | grep "coopy_SOURCE_DIR:" | sed "s/.*=//"`
echo "SOURCE in $SRC"

TEST="$SRC/tests"
echo "TESTS in $TEST"
OUT="diffs"
DIFF="./bin/ssdiff --format-human"

mkdir -p $OUT
echo "EXAMPLES in $OUT"

$DIFF $TEST/numbers.csv $TEST/numbers_flip_column.csv > $OUT/move_column.txt

