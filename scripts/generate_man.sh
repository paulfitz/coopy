#!/bin/bash

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

for f in ssdiff sspatch ssrediff ssmerge ssformat; do
    echo $f
    rm -f test.dox
    ./bin/$f --help-doxygen > test.dox
    cp test.dox ../doc/cmd_$f.dox
done
