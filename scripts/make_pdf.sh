#/bin/bash

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

BASE="$PWD"
SRC=`cat coopy_source.txt`

$SRC/scripts/fix_images.sh || exit 1
$SRC/scripts/fix_order.sh || exit 1
make latex || exit 1
cd gendoc/latex || exit 1
make clean && make pdf && xpdf refman.pdf
