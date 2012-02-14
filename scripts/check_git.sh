#!/bin/bash

rm -rf coopy_test
mkdir -p coopy_test/repo
cd coopy_test/repo
git init

ssformat --test-file numbers.sqlite

(
cat<<XXX
*.csv merge=coopy-merge-csv
*.sqlite filter=coopy-filter-sqlite
*.sqlite merge=coopy-merge-sqlite
XXX
) > .gitattributes

git add .gitattributes
git commit -m "add coopy sqlite rule"

git add numbers.sqlite
git commit -m "add sqlite example"

cd ..   # should be in coopy_test directory now
git clone repo repo2
cd repo2
ls -a   # should see numbers.sqlite and .gitattributes

sspatch numbers.sqlite --inplace --cmd "= |NAME:three->threepio|"
ssformat numbers.sqlite
git commit -m "scramble three" numbers.sqlite

cd ../repo
sspatch numbers.sqlite --inplace --cmd "= |three|3->33|"
ssformat numbers.sqlite  # check change
git commit -m "scramble 3" numbers.sqlite

git pull ../repo2

echo "***"
echo "***"
echo "***"
ssformat numbers.sqlite
echo "***"
echo "***"
echo "***"

cd ../repo2; git pull ../repo master

sspatch numbers.sqlite --inplace --cmd "= |two|2->22|"
git commit -m "conflict 22" numbers.sqlite

cd ../repo
sspatch numbers.sqlite --inplace --cmd "= |two|2->222|"
git commit -m "conflict 222" numbers.sqlite

git pull ../repo2

echo "***"
echo "***"
echo "***"
ssformat numbers.sqlite
echo "***"
echo "***"
echo "***"

ssresolve --ours numbers.sqlite

echo "***"
echo "***"
echo "***"
ssformat numbers.sqlite
echo "***"
echo "***"
echo "***"

git add numbers.sqlite
git commit -m "resolved conflict"
