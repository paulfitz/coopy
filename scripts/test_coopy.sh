#!/bin/bash

BUILD_DIR=$PWD
BIN_DIR=$PWD/bin
SSFORMAT=$BIN_DIR/ssformat 
COOPY="$BIN_DIR/coopy --delay"

BASE=$BUILD_DIR/test_coopy
rm -rf $BASE
mkdir -p $BASE
cd $BASE || exit 1

# make an empty repository in "store"
mkdir -p store
cd store || exit 1
echo "* Creating empty repository in $PWD"
coopy --new || exit 1
echo " "
# fossil new repository.coopy || exit 1

# fossil gets upset if ancestor isn't clearly older
sleep 1

# clone and view repository in "clone1"
cd $BASE
mkdir -p clone1
cd clone1 || exit 1
CLONE1=$PWD
echo "* Cloning repository in $PWD"
coopy --clone=../store
#fossil clone ../store/repository.coopy repository.coopy || exit 1
#fossil open repository.coopy || exit 1
echo " "

# clone and view repository in "clone2"
cd $BASE
mkdir -p clone2
cd clone2 || exit 1
CLONE2=$PWD
echo "* Cloning repository in $PWD"
coopy --clone=../store
#fossil clone ../store/repository.coopy repository.coopy || exit 1
#fossil open repository.coopy || exit 1
echo " "

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
ssformat people.sheets ../people.xls
echo "* Add people.xls to clone1"
$COOPY --add=../people.xls || exit 1
echo "* Push clone1"
$COOPY -m "add people" --push || exit 1

# okay, we are ready to start pulling data
cd $CLONE2 || exit 1
$COOPY --pull || exit 1
ls people.csvs || exit 1
$COOPY --key=people --export=../people2.xls || exit 1

# modify data
sleep 1
echo "* Modify data"
ssformat ../people2.xls > tmp.sheet
sed -i "s/Noemi/Nomers/" tmp.sheet
ssdiff --apply ../people2.xls tmp.sheet
$COOPY -m "change Noemi's name" --push || exit 1

# check change propagated
echo "* Check modification propagated"
cd $CLONE1 || exit 1
$COOPY --pull || exit 1
ssformat ../people.xls | grep Nomers || exit 1

echo Success

# pair of changes
cd $BASE
sleep 1
sspatch --inplace people.xls --cmd "= |Name=Paul|Age:*->37|"
sspatch --inplace people2.xls --cmd "= |Name=Leo|Age:*->4|"

# commit 1
echo "CHANGE PAUL"
cd $CLONE1
$COOPY -m "change Paul" --push

# commit 2
echo "CHANGE LEO"
cd $CLONE2
$COOPY --pull
$COOPY -m "change Leo" --push

# pull 2
cd $CLONE1
$COOPY --pull

cd $BASE
ssdiff --equal people.xls people2.xls || exit 1

echo Success

# pair of incompatible changes
cd $BASE
sleep 1
sspatch --inplace people.xls --cmd "= |Name=Paul|Zip:*->07028|"
sspatch --inplace people2.xls --cmd "= |Name=Paul|Zip:*->99999|"

# commit 1
echo "CHANGE PAUL"
cd $CLONE1
$COOPY -m "change Paul" --push

# commit 2
echo "CHANGE LEO"
cd $CLONE2
$COOPY --pull

# $COOPY -m "change Leo" --push

# pull 2
# cd $CLONE1
# $COOPY --pull

# cd $BASE
# ssdiff --equal people.xls people2.xls || exit 1
