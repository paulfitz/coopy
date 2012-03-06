#!/bin/bash

# generate lots of permutations of bridges.csv

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

BASE="$PWD"
SRC=`cat coopy_source.txt`
TESTS="$SRC/tests"
B="$TESTS/bridges.csv"
C="$TESTS/perms/bridges_"
#C="bridges_"


sspatch $B --cmd "@@ |b|d|l|" > ${C}null.csv
sspatch $B --cmd "@@ |b|d|l|" --cmd "@= b |ponts|d|l|" > ${C}pont.csv
sspatch $B --cmd "@@ |b|d|l|" --cmd "@= d |b|architect|l|" > ${C}arch.csv
sspatch $B --cmd "@@ |b|d|l|" --cmd "@= l |b|d|measure|" > ${C}meas.csv

for u in null pont arch meas; do
    B2="$C$u.csv"
    C2="$C${u}_"
    ssformat $B2 ${C2}bdl.csv
    sspatch $B2 --cmd "@@ |b|d|l|" --cmd "@: b |d|b|l|" > ${C2}dbl.csv
    sspatch $B2 --cmd "@@ |b|d|l|" --cmd "@: b |d|l|b|" > ${C2}dlb.csv
    sspatch $B2 --cmd "@@ |b|d|l|" --cmd "@: b |d|l|b|" --cmd "@: l |l|d|b|" > ${C2}ldb.csv
done

