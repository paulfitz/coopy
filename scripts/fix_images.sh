#!/bin/bash

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

BASE="$PWD"
SRC=`cat coopy_source.txt`

which identify || exit 1
which convert || exit 1

IMAGE="$SRC/doc/images"
IMAGE_OUT="$BASE/doc/images"
mkdir -p $IMAGE_OUT/screenshot
DOC="$SRC/doc"
for f in `cd $DOC; ls *.dox`; do
    if grep -q "image html" $DOC/$f; then
	echo $f
	awk 1 $DOC/$f | {
	while IFS='' read -r line; do
	    m=`expr match "$line" ".*image html"`
	    if [ ! "k$m" = "k0" ] ; then
		echo "$line"
		img=`echo "$line" | sed "s/.* html //" | sed "s/.png .*//"`
		conv="convert $IMAGE/$img.png $IMAGE_OUT/$img.pdf"
		width=`identify -format "%w" $IMAGE/$img.png`
		let cm=$width/60
		# echo "$conv" 1>&2
		$conv
		echo "$line" | sed "s/ html / latex /" | sed "s/\.png/.pdf/" | sed "s/$/ width=${cm}cm/"
	    else
		m=`expr match "$line" ".*image latex"`
		if [ "k$m" = "k0" ] ; then
		    echo "$line"
		fi
	    fi
	done
	} > /tmp/coopy_rewrite.dox
	diff -u $DOC/$f /tmp/coopy_rewrite.dox
	cmp $DOC/$f /tmp/coopy_rewrite.dox || cp /tmp/coopy_rewrite.dox $DOC/$f 
    fi
done
