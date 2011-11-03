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

cd $C2
coopy --clone=http://coopy:coopy@localhost:8080 || exit 1
coopy --export=numbers2.xls --key=numbers || exit 1

sleep 1
sspatch --inplace $C1/numbers.xls --cmd "= |DIGIT=4->44|"
sspatch --inplace $C2/numbers2.xls --cmd "= |DIGIT=4->555|"
sleep 1

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

coopy . &
sleep 2
xdotool key Tab alt+t Down Down Down Return
sleep 2
dlg=`$mark`
xdotool getactivewindow windowsize $default_size
$tick
$focus $dlg
xdotool key Down Down Down Down Right
$snapshot $base conflicted_excel
