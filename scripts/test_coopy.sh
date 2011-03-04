#!/bin/bash

BUILD_DIR=$PWD
BIN_DIR=$PWD/bin
SSFORMAT=$BIN_DIR/ssformat 
COOPY=$BIN_DIR/coopy 

BASE=$BUILD_DIR/test_coopy
rm -rf $BASE
mkdir -p $BASE
cd $BASE || exit 1

# make an empty repository in "store"
mkdir -p store
cd store || exit 1
echo "* Creating empty repository in $PWD"
fossil new repository.coopy || exit 1

# fossil gets upset if ancestor isn't clearly older
sleep 1

# clone and view repository in "clone1"
cd $BASE
mkdir -p clone1
cd clone1 || exit 1
CLONE1=$PWD
echo "* Cloning repository in $PWD"
fossil clone ../store/repository.coopy repository.coopy || exit 1
fossil open repository.coopy || exit 1

# clone and view repository in "clone2"
cd $BASE
mkdir -p clone2
cd clone2 || exit 1
CLONE2=$PWD
echo "* Cloning repository in $PWD"
fossil clone ../store/repository.coopy repository.coopy || exit 1
fossil open repository.coopy || exit 1

# okay, we are ready to start pushing data
cd $CLONE1 || exit 1
(
cat<<EOF
== People ==
Name,Age,Zip
------------
Paul,36,02139
Leo,3,99999
Noemi,33,07028
EOF
) > people.sheets
ssformat people.sheets people.xls
echo "* Add people.xls to clone1"
$COOPY --add=people.xls || exit 1
echo "* Push clone1"
$COOPY -m "add people" --push || exit 1

# okay, we are ready to start pulling data
cd $CLONE2 || exit 1
$COOPY --pull || exit 1
ls people.csvs || exit 1
$COOPY --key=people --export=people.xls || exit 1

# modify data
sleep 1
echo "* Modify data"
ssformat people.xls > tmp.sheet
sed -i "s/Noemi/Nomers/" tmp.sheet
ssdiff --apply people.xls tmp.sheet
$COOPY -m "change Noemi's name" --push || exit 1

# check change propagated
echo "* Check modification propagated"
cd $CLONE1 || exit 1
$COOPY --pull || exit 1
ssformat people.xls | grep Nomers || exit 1
