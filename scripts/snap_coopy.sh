#!/bin/bash

lnk="https://chiselapp.com/user/coopy/repository/coopy_demo/index"
# lnk="/tmp/coopy_base"

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

root="$PWD"

if true; then
    cd /tmp || exit 1
    rm -rf coopy_base
    mkdir coopy_base || exit 1
    cd coopy_base || exit 1
    fossil new test.fossil || exit1
    fossil open test.fossil || exit 1
    (
	cat<<EOF
== numbers ==
NAME,DIGIT
----------
one,1
two,2
three,3
four,4
five,5
EOF
    ) > numbers.csvs
    sleep 1
    fossil add numbers.csvs || exit 1
    fossil commit -m "add numbers"
fi

which xdotool | exit 1
which sniff | exit 1  # check for dogtail

coopy="$root/bin/coopy"
snapshot="$root/../scripts/snap_window.py"
base=/tmp/coopy_snap_
default_size="628 300"
tick="sleep 1"
mark="xdotool getactivewindow"
focus="xdotool windowactivate"

cd /tmp
rm -rf coopy_demo
mkdir coopy_demo
cd coopy_demo

$coopy &

$tick
$snapshot $base main
app=`$mark`
$focus $app || exit 1

xdotool key alt+i
$tick
dlg=`$mark`
xdotool getactivewindow windowsize $default_size
$tick
$focus $dlg
$snapshot $base pull
xdotool key alt+o
$tick
$snapshot $base open
xdotool type $lnk
$tick
$snapshot $base open2
xdotool key alt+o
sleep 2
$focus $app
xdotool key Tab alt+s Down Down Down Return
$tick
dlg=`$mark`
xdotool getactivewindow windowsize $default_size
$tick
$focus $dlg
$snapshot $base zig
xdotool type "/tmp/numbers.xls"
xdotool key alt+s
sleep 3
gnumeric=`$mark`
xdotool getactivewindow windowsize $default_size
$focus $gnumeric
$snapshot $base number
xdotool key Down Down Down
xdotool type trois
$snapshot $base number2
xdotool key Return
xdotool key ctrl+s
$tick
xdotool getactivewindow windowkill
$tick
$focus $app || exit 1
xdotool key alt+o
$tick
dlg=`$mark`
$snapshot $base commit
xdotool type "randomly frenchified cell"
$tick
$snapshot $base commit2
xdotool key alt+o
$tick
$tick
dlg=`$mark`
$snapshot $base username
sleep 5

# kill %1
