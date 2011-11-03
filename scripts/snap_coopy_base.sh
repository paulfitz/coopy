#!/bin/bash

lnk1="https://chiselapp.com/user/coopy/repository/coopy_demo/index"
lnk2="http://localhost:8080"

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
    fossil user new coopy coopy coopy
    fossil user new admin admin admin
    fossil settings localauth 1
    fossil user capabilities nobody chgmnouz
    fossil user capabilities anonymous chgmnouz
    fossil user capabilities admin s
    killall fossil 2> /dev/null
    killall -9 fossil 2> /dev/null
    fossil server &
fi

which xdotool | exit 1
which wmctrl | exit 1
which scrot | exit 1

coopy="$root/bin/coopy"
# snapshot="$root/../scripts/snap_window.py"

mkdir -p /tmp/ss
base=/tmp/ss/coopy_
rm -f $base*.png

function snapper() {
    scrot -u "$1$2.png"
}
function no_coopy() {
    killall coopy 2> /dev/null
    killall -9 coopy 2> /dev/null
}
snapshot="snapper"

default_size="628 300"
mid_size="628 450"
long_size="628 600"
tick="sleep 1"
mark="xdotool getactivewindow"
focus="xdotool windowactivate"
restore="wmctrl -r :ACTIVE: -b remove,maximized_vert,maximized_horz"

cd /tmp
rm -rf coopy_demo
mkdir coopy_demo
cd coopy_demo

no_coopy
if [ "k$NO_AUTOSTART" = "k" ]; then
  $coopy &
fi

