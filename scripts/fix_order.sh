#!/bin/bash

if [ ! -e CMakeCache.txt ]; then
    echo "Call from build directory"
    exit 1
fi

BASE="$PWD"
SRC=`cat coopy_source.txt`
ORDER_FILE="$SRC/doc/pdf_order.txt"

if [ ! -e "$ORDER_FILE" ] ; then
    echo "Cannot find order file"
    exit 1
fi

LIST_FILE="/tmp/CoopyDocLatex.cmake"
LIST_FILE_FINAL="$SRC/conf/CoopyDocLatex.cmake"
echo "# created by scripts/fix_order.sh" > $LIST_FILE
echo  "# from doc/pdf_order.txt" >> $LIST_FILE
echo -n "set (LATEX_MANUAL_DOCS" >> $LIST_FILE
cd $SRC/doc || exit 1
for tag in `cat $ORDER_FILE`; do
    if [ "k$tag" = "kmain" ]; then
	fname="main.dox"
    else
	fname=`grep -l -H "@page $tag " *.dox`
    fi
    echo "$tag -> $fname"
    echo -n " $fname" >> $LIST_FILE
done
echo ")" >> $LIST_FILE
cmp $LIST_FILE $LIST_FILE_FINAL || cp $LIST_FILE $LIST_FILE_FINAL
