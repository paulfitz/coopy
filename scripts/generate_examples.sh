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
DIFF="./bin/ssdiff"
MERGE="./bin/ssmerge"

mkdir -p $OUT
echo "EXAMPLES in $OUT"

function dox {
    grp=$1
    name=$2
    title=$3
    mkdir -p $SRC/doc/generated_examples
    (
	cat<<EOF
/**
 *

\ingroup $grp

\defgroup ${grp}_$name $title

EOF

	raw="$OUT/${grp}_$name.txt"
	if [ ! -e $raw ]; then
	    echo "Cannot find $raw"
	    exit 1
	fi
	cat $raw | sed "s/## SECTION /\\\\section ${grp}_${name}_/" | sed "s/## LINK /\\\\li \\\\ref ${grp}_${name}_/"
	cat<<EOF

 *
 */
EOF

    ) > $SRC/doc/generated_examples/${grp}_$name.dox
    echo "* Generated $SRC/doc/generated_examples/${grp}_$name.dox"
}

function diff_apply {
    f1=$1
    f2=$2
    name=$3
    out=$OUT/diff_example_$name.txt
    (
	echo "## SECTION command command"
	echo "\verbatim"
	echo " ssdiff $f1 $f2"
	echo "\endverbatim"
	echo "## LINK output \"output\""
	echo "## LINK ref1 \"$f1\""
	echo "## LINK ref2 \"$f2\""
	echo " "
	echo "## SECTION output output"
	echo "\verbatim"
	$DIFF $TEST/$f1 $TEST/$f2
	echo "\endverbatim"
	echo " "
	echo "## SECTION ref1 $f1"
	echo "\verbatim"
	cat $TEST/$f1
	echo "\endverbatim"
	echo " "
	echo "## SECTION ref2 $f2"
	echo "\verbatim"
	cat $TEST/$f2
	echo "\endverbatim"
    ) > $out
    echo "* Generated $out"
    dox diff_example $name "$name example for ssdiff"
}

function merge_apply {
    f1=$1
    f2=$2
    f3=$3
    name=$4
    out=$OUT/merge_example_$name.txt
    (
	echo "## SECTION command command"
	echo "\verbatim"
	echo "ssmerge $f1 $f2 $f3"
	echo "\endverbatim"
	echo "## LINK output \"output\""
	echo "## LINK parent \"input: $f1\""
	echo "## LINK local \"input: $f2\""
	echo "## LINK remote \"input: $f3\""
	echo " "
	echo "## SECTION output output"
	echo "\verbatim"
	./bin/ssmerge $TEST/$f1 $TEST/$f2 $TEST/$f3
	echo "\endverbatim"
	echo " "
	echo "## SECTION parent parent/pivot file: $f1"
	echo "\verbatim"
	cat $TEST/$f1
	echo "\endverbatim"
	echo " "
	echo "## SECTION local local/left file: $f2"
	echo "\verbatim"
	cat $TEST/$f2
	echo "\endverbatim"
	echo " "
	echo "## SECTION remote remote/right file: $f3"
	echo "\verbatim"
	cat $TEST/$f3
	echo "\endverbatim"
    ) > $out
    echo "* Generated $out"
    dox merge_example $name "$name example for ssmerge"
}

diff_apply numbers.csv numbers_flip_column.csv move_column
diff_apply numbers.csv numbers_change_five.csv update_cell
diff_apply numbers.csv numbers_add_row.csv insert_row
diff_apply numbers_add_row.csv numbers.csv delete_row
diff_apply numbers.csv numbers_wide.csv insert_column
diff_apply numbers_three_23.csv numbers.csv fix_a_cell
merge_apply numbers.csv numbers_change_five.csv numbers_flip_column.csv change_cell_and_flip_columns
merge_apply numbers_wide.csv numbers_wide_flip_pair1.csv numbers_wide_flip_pair2.csv flip_columns_locally_and_remotely
merge_apply test001_base.csv test001_spell.csv test001_col.csv big_merge_with_lots_of_changes
merge_apply test005_base.csv test005_fix_typo_add.csv test005_replace_column_and_reorder.csv altitude_typo_fix_and_reorder
