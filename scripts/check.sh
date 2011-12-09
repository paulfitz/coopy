#!/bin/bash

if [ -e CMakeLists.txt ]; then
    exit 1
fi

if [ ! -e CMakeCache.txt ]; then
    exit 1
fi

rm -rf src/testing
cmake . || exit 1
make || exit 1
make test || exit 1
