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
echo Creating empty repository in $PWD
fossil new repository.coopy || exit 1

# clone and view repository in "clone1"
cd $BASE
mkdir -p clone1
cd clone1 || exit 1
CLONE1=$PWD
echo Cloning repository in $PWD
fossil clone ../store/repository.coopy repository.coopy || exit 1
fossil open repository.coopy || exit 1

# clone and view repository in "clone2"
cd $BASE
mkdir -p clone2
cd clone2 || exit 1
CLONE2=$PWD
echo Cloning repository in $PWD
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
) > test.csvs
ssformat test.csvs test.xls
#rm -f test.csvs
