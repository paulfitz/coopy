#!/bin/bash

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

key="$1"

SRC=`cat CMakeCache.txt | grep "coopy_SOURCE_DIR:" | sed "s/.*=//"`
echo "SOURCE in $SRC"

TEST="$SRC/tests"
echo "TESTS in $TEST"
OUT="diffs"
DIFF="./bin/ssdiff"
PATCH="./bin/sspatch"
MERGE="./bin/ssmerge"
SSFORMAT="./bin/ssformat"
CSV2HTML="./bin/csv2html"
TMPF=/tmp/eg_coopy_tmp.csv
TMPF1=/tmp/eg_coopy_tmp1.csv
TMPF2=/tmp/eg_coopy_tmp2.csv

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

function show_file {
    local fname="$1"
    local format="$2"
    if [ "k$format" = "kxls" ]; then
	rm -f /tmp/tmp.html
	# bug in ssconvert :-(
	#PROB="1380,383"
	#echo "= |length=$PROB->FOO|" | ( sspatch --inplace $fname - > /dev/null )
	ssconvert --export-type=Gnumeric_html:html40frag $fname /tmp/tmp.html > /dev/null
	if [ ! -e /tmp/tmp.html ] ; then
	    echo "Failed to ssconvert $fname" 1>&2
	    exit 1
	fi
	cat /tmp/tmp.html | grep -v caption | grep -v exporter | sed "s/&rt;/>/g" | sed "s/\@/\\\\@/g" | sed "s/border=.1.//g" | sed "s|</*font[^<>]*>||g" | sed "s/FOO/$PROB/"
    elif [ "k$format" = "kcsv" ]; then
	$CSV2HTML $fname | sed "s/&rt;/>/g" | sed "s/\@/\\\\@/g"
    else
	echo "\verbatim"
	cat $fname || exit 1
	echo "\endverbatim"
    fi
}

function diff_base_apply {
    f1=$1
    f2=$2
    name=$3
    echo $name > /tmp/_gen_name.txt
    if grep -q "$key" /tmp/_gen_name.txt; then
    out=$OUT/diff_example_$name.txt

    # echo "### $DIFF --format-csv $TEST/$f1 $TEST/$f2"
    echo "### $DIFF --format-tdiff $TEST/$f1 $TEST/$f2"
    {
	echo "## SECTION command command"
	echo " "
	echo "In this example, we compare two tables as follows:"
	echo "\verbatim"
	echo " ssdiff $f1 $f2"
	echo " ssdiff --format hilite --output diff.xls $f1 $f2"
	echo "\endverbatim"
	echo "The two tables are:"
	echo "## LINK ref1 \"$f1\""
	echo "## LINK ref2 \"$f2\""
	echo " "
	echo "The result of comparing the tables are:"
	echo "## LINK tdiff_output \"tdiff output\"  (textual comparision)" 
	echo "## LINK highlighter_output \"highlighter output\"  (visual comparision)"
	echo " "
	echo "## SECTION ref1 $f1"
	$SSFORMAT $TEST/$f1 $TMPF.2.csv 2> /dev/null
	show_file $TMPF.2.csv csv
	echo " "
	echo "## SECTION ref2 $f2"
	$SSFORMAT $TEST/$f2 $TMPF.3.csv 2> /dev/null
	show_file $TMPF.3.csv csv
	echo "## SECTION tdiff_output tdiff output"
	$DIFF --output $TMPF1 --format tdiff $TEST/$f1 $TEST/$f2 || exit 1
	show_file $TMPF1 tdiff
	echo " "
	echo "## SECTION highlighter_output highlighter output"
	rm -f $TMPF2.xls
	{
	    $DIFF --output $TMPF2.xls --format color $TEST/$f1 $TEST/$f2 || exit 1
	} > /dev/null
	show_file $TMPF2.xls xls
	if [ ! -e $TMPF2.xls ]; then
	    exit 1
	fi
	echo " "
    } > $out
    echo "* Generated $out"
    n=`echo $name | sed "s/_/ /g" | sed "s/^named //"`
    dox diff_example $name "$n example for ssdiff"
    else
	echo "skipped $name"
    fi
}

function patch_base_apply {
    f1=$1
    f2=$2
    name=$3
    echo $name > /tmp/_gen_name.txt
    if grep -q "$key" /tmp/_gen_name.txt; then
    out=$OUT/patch_example_$name.txt
    DF1=$TMPF1
    DF2=$TMPF2

    $DIFF --format-csv --output $DF1 $TEST/$f1 $TEST/$f2
    $DIFF --output $DF2 $TEST/$f1 $TEST/$f2

    echo "### $PATCH $TEST/$f1 $DF1"
    {
	echo "## SECTION command command"
	echo " "
	echo "In this example, we apply a patch as follows:"
	echo "\verbatim"
	echo "sspatch $f1 patch.tdiff"
	echo "\endverbatim"
	echo "The output, input, and patch are:"
	echo "## LINK output \"output\""
	echo "## LINK ref1 \"$f1\""
	echo "## LINK ref2 \"patch.tdiff\""
	#echo "## LINK ref3 \"patch.csv\""
	echo " "
	echo "## SECTION output output"
	$PATCH --output $TMPF.1.csv $TEST/$f1 $DF1 || exit 1
	$PATCH --output $TMPF.2.csv $TEST/$f1 $DF2 || exit 1
	diff $TMPF.1.csv $TMPF.2.csv || exit 1
	show_file $TMPF.1.csv csv || exit 1
	echo " "
	echo "## SECTION ref1 $f1"
	$SSFORMAT $TEST/$f1 $TMPF.3.csv 2> /dev/null
	show_file $TMPF.3.csv csv
	echo " "
	echo "## SECTION ref2 patch.tdiff"
	show_file $DF2 tdiff
	#echo " "
	#echo "## SECTION ref3 patch.csv"
	#echo "Here is an equivalent patch in CSV format."
	#show_file $DF1 csv
	#echo "This patch can be applied with the following command:"
	#echo "\verbatim"
	#echo "sspatch $f1 patch.csv"
	#echo "\endverbatim"
    } > $out
    echo "* Generated $out"
    n=`echo $name | sed "s/_/ /g" | sed "s/^named //"`
    dox patch_example $name "$n example for sspatch"
    else
	echo "skipped $name"
    fi
}

function diff_apply {
    f1=$1
    f2=$2
    namer=$3
    #diff_base_apply human $f1 $f2 $namer

    patch_base_apply $f1 $f2 ${namer}

    diff_base_apply $f1 $f2 ${namer}

    #diff_base_apply raw $f1 $f2 ${namer}_raw
}

function merge_apply {
    f1=$1
    f2=$2
    f3=$3
    name=$4
    echo $name > /tmp/_gen_name.txt
    if grep -q "$key" /tmp/_gen_name.txt; then
    out=$OUT/merge_example_$name.txt
    {
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
	./bin/ssmerge --output $TMPF $TEST/$f1 $TEST/$f2 $TEST/$f3 || exit 1
	show_file $TMPF csv
	echo " "
	echo "## SECTION parent parent/pivot file: $f1"
	ssformat $TEST/$f1 $TMPF.1.csv 2> /dev/null
	show_file $TMPF.1.csv csv
	echo " "
	echo "## SECTION local local/left file: $f2"
	ssformat $TEST/$f2 $TMPF.2.csv 2> /dev/null
	show_file $TMPF.2.csv csv
	echo " "
	echo "## SECTION remote remote/right file: $f3"
	ssformat $TEST/$f3 $TMPF.3.csv 2> /dev/null
	show_file $TMPF.3.csv csv
    } > $out
    echo "* Generated $out"
    dox merge_example $name "$name example for ssmerge"
    else
	echo "skipped $name"
    fi
}

for f in `cd ../doc/tables; ls *.csv`; do
    echo $f
    show_file ../doc/tables/$f csv > ../doc/tables/$f.html
done

diff_apply broken_bridges.csv bridges.csv bridge
for pre in "named_"; do
  diff_apply ${pre}numbers.csv ${pre}numbers_flip_column.csv ${pre}move_column
  diff_apply ${pre}numbers.csv ${pre}numbers_change_five.csv update_cell
  diff_apply ${pre}numbers.csv ${pre}numbers_add_row.csv ${pre}insert_row
  diff_apply ${pre}numbers_add_row.csv ${pre}numbers.csv ${pre}delete_row
  diff_apply ${pre}numbers.csv ${pre}numbers_wide.csv ${pre}insert_column
  diff_apply ${pre}numbers_three_23.csv ${pre}numbers.csv ${pre}fix_a_cell
done
diff_apply numbers.xls numbers_change_five.xls xls_update_cell

merge_apply numbers.csv numbers_change_five.csv numbers_flip_column.csv change_cell_and_flip_columns
merge_apply numbers_wide.csv numbers_wide_flip_pair1.csv numbers_wide_flip_pair2.csv flip_columns_locally_and_remotely
# merge_apply test001_base.csv test001_spell.csv test001_col.csv big_merge_with_lots_of_changes # too confusing
merge_apply test005_base.csv test005_fix_typo_add.csv test005_replace_column_and_reorder.csv altitude_typo_fix_and_reorder

