#!/bin/bash

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

BASE="$PWD"
SRC=`cat coopy_source.txt`

echo "I keep forgetting how to release"
echo "  ../scripts/release.sh package"
echo "  ../scripts/release.sh tag"

if [ "k$1" = "kpackage" ]; then
    $SRC/scripts/make_packages.sh linux || exit 1
    $SRC/scripts/make_packages.sh mingw || exit 1
    $SRC/scripts/make_packages.sh osx || exit 1
    $SRC/scripts/make_packages.sh doc || exit 1
fi

if [ "k$1" = "ktag" ]; then
  cmake . || exit 1
  tag=`cat coopy_version.txt`
  echo "tag is $tag, ok?"
  read x
  echo "git tag -a v$tag -m \"version $tag\""
  git tag -a v$tag -m "version $tag"
  echo "tag push, ok?"
  read x
  git push --tags
fi
