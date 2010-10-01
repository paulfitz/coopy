#!/bin/bash

echo "I keep forgetting how to release"
echo "  ./scripts/release.sh test"

if [ "k$1" = "ktest" ]; then
    ./scripts/make_packages.sh linux || exit 1
    ./scripts/make_packages.sh mingw || exit 1
    ./scripts/make_packages.sh osx || exit 1
    ./scripts/make_packages.sh doc || exit 1
fi
