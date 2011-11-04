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
		need_update=true
		if [ -e $IMAGE_OUT/$img.png.mark.png ]; then
		    if [ $IMAGE_OUT/$img.png.mark.png -nt $IMAGE/$img.png ]; then
			need_update=false
			# echo "SKIPPY $img" 1>&2
		    fi
		fi
		let cm=1
		if $need_update; then
		    echo "UPDATING $img" 1>&2
		    convert $IMAGE/$img.png $IMAGE_OUT/$img.pdf || exit 1
		    cp $IMAGE/$img.png $IMAGE_OUT/$img.png.mark.png
		    width=`identify -format "%w" $IMAGE/$img.png`
		    let cm=$width/60
		    echo $cm > $IMAGE_OUT/$img.png.width
		else
		    cm=`cat $IMAGE_OUT/$img.png.width`
		fi
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
