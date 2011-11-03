#!/bin/bash

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

NO_AUTOSTART=true
source ../scripts/snap_coopy_base.sh

function snap_xls() {
    no_coopy
    gnumeric $1&
    sleep 2
    dlg=`$mark`
    xdotool getactivewindow windowsize $default_size
    $tick
    $focus $dlg
    xdotool key Down Down Down Down Right
    $snapshot $base $2
    no_coopy
}

C1=/tmp/coopy_clone1
C2=/tmp/coopy_clone2

rm -rf $C1; mkdir -p $C1
rm -rf $C2; mkdir -p $C2

cd $C1
coopy --clone=http://coopy:coopy@localhost:8080 || exit 1
coopy --export=numbers.xls --key=numbers || exit 1
sleep 1
sspatch --inplace numbers.xls --cmd "= |DIGIT=4->9|"
coopy --push -m "better starting point"

cd $C2
coopy --clone=http://coopy:coopy@localhost:8080 || exit 1
coopy --export=numbers2.xls --key=numbers || exit 1

snap_xls $C1/numbers.xls conflicted_start
sspatch --inplace $C1/numbers.xls --cmd "= |DIGIT=9->4|"
snap_xls $C1/numbers.xls conflicted_theirs
sspatch --inplace $C2/numbers2.xls --cmd "= |DIGIT=9->44|"
snap_xls $C2/numbers2.xls conflicted_ours

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

snap_xls numbers2.xls conflicted_excel
