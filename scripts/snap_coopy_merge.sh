#!/bin/bash

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

NO_AUTOSTART=true
source ../scripts/snap_coopy_base.sh

C1=/tmp/coopy_clone1
C2=/tmp/coopy_clone2

rm -rf $C1; mkdir -p $C1
rm -rf $C2; mkdir -p $C2

cd $C1
coopy --clone=http://coopy:coopy@localhost:8080 || exit 1
coopy --export=numbers.xls --key=numbers || exit 1
sleep 1
sspatch --inplace numbers.xls --cmd "+ |spam|42|" --cmd "- |DIGIT=3|" --cmd "= |DIGIT=5->55|"
coopy --push -m "better starting point"

cd $C2
coopy --clone=http://coopy:coopy@localhost:8080 || exit 1
coopy --export=numbers2.xls --key=numbers || exit 1

snap_xls $C1/numbers.xls merged_start
sspatch --inplace $C1/numbers.xls --cmd "* |NAME=*|DIGIT=2|" --cmd "+ |three|3|"
snap_xls $C1/numbers.xls merged_theirs
sspatch --inplace $C2/numbers2.xls --cmd "- |NAME=spam|" --cmd "= |DIGIT=55->5|"
snap_xls $C2/numbers2.xls merged_ours

cd $C1
coopy --push -m "mod1"

sleep 1
cd $C2
coopy --push -m "mod2" &
sleep 2
$snapshot $base would_fork
xdotool key Return
sleep 1

cd $C2
coopy --pull 

snap_xls numbers2.xls merged_excel
