#/bin/bash

FMT="tdiff"
if [ ! "k$1" = "k" ]; then
    FMT=$1
fi

TDIR=$PWD/../tests
BIN="$PWD/bin/ssdiff --format-$FMT"

function apply {
    local=$1
    remote=$2
    echo " "
    echo " "
    echo "========================================================="
    echo "Working on $local -> $remote"
    $BIN $local $remote
}

apply $TDIR/named_numbers.csv $TDIR/named_numbers_change_five.csv 
apply $TDIR/named_numbers_change_five.csv $TDIR/named_numbers.csv 
apply $TDIR/named_numbers.csv $TDIR/named_numbers_add_row.csv 
apply $TDIR/named_numbers_add_row.csv $TDIR/named_numbers.csv 

