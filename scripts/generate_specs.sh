#!/bin/bash

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

if [ ! -e tdiff ]; then
    git clone git://gist.github.com/749949.git
    mv 749949 tdiff || exit 1
else
    cd tdiff
    git pull || exit 1
    cd ..
fi

cd tdiff
make || exit 1
TARG=../../doc/tdiff
mkdir -p $TARG
cp tdiff_spec_draft.html tdiff_spec_draft.txt $TARG


