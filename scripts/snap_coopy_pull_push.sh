#!/bin/bash

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

source ../scripts/snap_coopy_base.sh

# main screen

$tick
xdotool key Tab Tab Tab
$snapshot $base main
app=`$mark`
$focus $app || exit 1

# set up repository
xdotool key alt+s
$tick
$snapshot $base setup

# select directory

xdotool key Return
$tick
dlg=`$mark`
xdotool getactivewindow windowsize $default_size
$tick
$focus $dlg
$snapshot $base pull

# select repository

xdotool key alt+o
$tick
$snapshot $base open

# selected repository

xdotool type $lnk1
$tick
$snapshot $base open2
xdotool key ctrl+a Del
xdotool type $lnk2

# repository loaded

xdotool key alt+o
sleep 2
$focus $app
xdotool key Tab alt+t Down Down Down
$snapshot $base after_pull

# save database as

xdotool key Return
$tick
dlg=`$mark`
xdotool getactivewindow windowsize $default_size
$tick
$focus $dlg
$snapshot $base save_as

# see spreadsheet

xdotool type "/tmp/numbers.xls"
xdotool key alt+s
sleep 3
gnumeric=`$mark`
xdotool getactivewindow windowsize $default_size
$focus $gnumeric
$snapshot $base xls

# modify spreadsheet

xdotool key Down Down Down
xdotool type trois
$snapshot $base xls2

# ... save changes and return to coopy ...

xdotool key Return
xdotool key ctrl+s
$tick
xdotool getactivewindow windowkill
$tick
$focus $app || exit 1

# commit message to add

xdotool key alt+o
$tick
dlg=`$mark`
$snapshot $base commit

# commit message added

xdotool type "randomly frenchified cell"
$tick
$snapshot $base commit2

# write denied

xdotool key alt+o
$tick
$tick
dlg=`$mark`
xdotool key Down

$snapshot $base access_denied_username

# set username

xdotool key Return
$tick
xdotool type coopy
$snapshot $base username

# set password

xdotool key alt+o
$tick
xdotool type coopy
$snapshot $base pw

# write done
xdotool key alt+o
$tick
$tick
$focus $app
$snapshot $base write_done

# kill %1
