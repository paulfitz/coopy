#!/bin/bash

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

source ../scripts/snap_coopy_base.sh

killall google-chrome 2> /dev/null
killall -9 google-chrome 2> /dev/null

# setup
$tick
app=`$mark`
xdotool key alt+s
$tick
xdotool key Return
$tick
xdotool key alt+o
$tick
xdotool type $lnk2
xdotool key alt+o
$tick
$focus $app
$tick

# select project code
$tick
xdotool key alt+s
$tick
xdotool key Down Down Down Down Down
$tick
$snapshot $base select_code

# see project code
xdotool key Return
$tick
$snapshot $base code

# select fork
xdotool key Escape
$tick
$focus $app
$tick
xdotool key alt+s
$tick
xdotool key Down Down Down
$tick
$snapshot $base select_fork

xdotool key Return
$tick
$snapshot $base fork_host

xdotool key Down Down Return
$tick
xdotool key alt+o
$tick
xdotool key ctrl+a
xdotool type "$lnk1"
$tick
$snapshot $base enter_fork

exit 0

# choose host

xdotool key Return
$tick
$snapshot $base host

# select repository

function select_link {
    xdotool key ctrl+f
    $tick
    xdotool type "$1"
    $tick
    xdotool key Escape
    $tick
}

function click_link {
    xdotool key Return
    $tick
}

xdotool key Return
$tick
$tick
xdotool key ctrl+shift+b ctrl+0 ctrl+minus ctrl+minus
select_link "Log out"
click_link
xdotool key ctrl+r
$tick
select_link "Chisel"

chrome=`$mark`
$restore
$tick
xdotool getactivewindow windowsize $long_size
$tick
$focus $chrome
$snapshot $base chisel_front

# show dashboard

select_link "Log in"
click_link
$tick
$tick
xdotool key Tab Return
$tick
$tick
xdotool getactivewindow windowsize $default_size
$tick
$focus $chrome
select_link "Create"
$snapshot $base chisel_dashboard

# show create

click_link
$tick
$tick
xdotool getactivewindow windowsize $mid_size
$tick
$focus $chrome
$snapshot $base chisel_create

# select a repo

xdotool key alt+Left
$tick
$tick
select_link "coopy_demo"
xdotool getactivewindow windowsize $default_size
$tick
$focus $chrome
$snapshot $base chisel_link

# show repo
click_link
$tick
$tick
xdotool key ctrl+l
$snapshot $base chisel_fossil

$tick
xdotool key ctrl+shift+b ctrl+0 ctrl+w
