#!/bin/bash

fname="$1"
osrc="$2"
bin="$3"
target="$4"

echo target is $target 1>&2

CSV2HTML="$bin/bin/ss2html"
SSDIFF="$bin/bin/ssdiff"
SSREDIFF="$bin/bin/ssrediff"
SSFORMAT="$bin/bin/ssformat"
SSMERGE="$bin/bin/ssmerge"

BASE=`dirname $fname`
IMG_DIR="images/screenshot"
mkdir -p $BASE/$IMG_DIR
SRC="$fname"

ROOT="$osrc"

{
which html2ps || exit 1
which ssconvert || exit 1
which ps2pdf || exit 1
which pdfcrop || exit 1
} 1>&2

width=""
x=0
function show_file {
    local fname="$1"
    local format="$2"
    local mode="$3"
#    if [ "k$mode" = "kalt" ] ; then
#	$CSV2HTML --header --dox $fname
#	return
#    fi
    if [ "k$format" = "ktxt" ]; then
	cat $fname
	return
    fi
    if [ ! "k$target" = "kpdf" ] ; then
	$CSV2HTML --header --dox $fname
	return
    fi
    if [ ! "k$format" = "knonexistent" ]; then
	rm -f /tmp/tmp.html
	# bug in ssconvert :-(
	#PROB="1380,383"
	#echo "= |length=$PROB->FOO|" | ( sspatch --inplace $fname - > /dev/null )
	#ssconvert --export-type=Gnumeric_html:html40frag $fname /tmp/tmp.html > /dev/null
	$CSV2HTML --header $fname > /tmp/tmp.html
	if [ ! -e /tmp/tmp.html ] ; then
	    echo "Failed to ssconvert $fname" 1>&2
	    exit 1
	fi
	cat /tmp/tmp.html | sed "s/<th/<th bgcolor='#000088'/g" | sed 's|>\([^<]*\)</th>|><font color="#ffffff">\1</font></th>|g' | sed "s|<table|<table border=1|" | sed "s/&rt;/>/g" > /tmp/tmp2.html
	# cp /tmp/tmp.html /tmp/tmp2.html
	# cat /tmp/tmp.html | grep -v exporter | sed "s/&rt;/>/g" | sed "s/rborder=.1.//g" | sed "s|</*font[^<>]*>||g" | sed "s/FOO/$PROB/" > /tmp/tmp2.html
	# cat /tmp/tmp.html | grep -v caption | grep -v exporter | sed "s/&rt;/>/g" | sed "s/\@/\\\\@/g" | sed "s/border=.1.//g" | sed "s|</*font[^<>]*>||g" | sed "s/FOO/$PROB/" > /tmp/tmp2.html
	echo "\\htmlonly"
	cat /tmp/tmp2.html
	echo "\\endhtmlonly"
	(
	# cp /tmp/tmp2.html /tmp/tmp2.txt
	rm -rf /tmp/tmp2.ps /tmp/tmp.pdf /tmp/tmp2.pdf
	html2ps --colour /tmp/tmp2.html > /tmp/tmp2.ps
	ps2pdf -dEPSCrop /tmp/tmp2.ps /tmp/tmp.pdf
	pdfcrop --margins 10 /tmp/tmp.pdf /tmp/tmp2.pdf
	) > /dev/null
	bname=`basename $SRC .paradox`
	iname="$IMG_DIR/${bname}_${x}.pdf"
	echo cmp /tmp/tmp2.pdf $BASE/$iname 1>&2
	f=/tmp/tmp2.pdf
	g="$BASE/$iname"
	{
	if [ -e $g ]; then
	    ModDate="`grep -a '^/ModDate' $g | head -n1`"
	    CreationDate="`grep -a '^/CreationDate' $g | head -n1`"
	    ID="`grep -a '^/ID' $g | head -n1`"
	    echo "ModDate $ModDate"
	    echo "CreationDate $CreationDate"
	    echo "ID $ID"  1>&2
	    sed -i "s|^/ModDate .*|$ModDate|g" $f
	    sed -i "s|^/CreationDate .*|$CreationDate|g" $f
	    sed -i "s|^/ID .*|$ID|g" $f
	fi

	cmp $f $g || (
	    echo cp /tmp/tmp2.pdf $BASE/$iname
	    cp /tmp/tmp2.pdf $BASE/$iname
	)
	} 1>& 2
	if [ ! "k$width" = "k" ]; then
	    echo "\\image latex $iname \"\" width=$width"
	else
	    echo "\\latexonly"
	    echo "\\begin{center}"
	    echo "\\endlatexonly"
	    echo "\\image latex $iname"
	    echo "\\latexonly"
	    echo "\\end{center}"
	    echo "\\endlatexonly"
	fi
	let x=x+1
    elif [ "k$format" = "kcsv" ]; then
	$CSV2HTML $fname | sed "s/&rt;/>/g" | sed "s/\@/\\\\@/g"
    else
	echo "\verbatim"
	cat $fname || exit 1
	echo "\endverbatim"
    fi
    width=""
}


table_mode=false
cname="none"
prefix="/tmp/process_dox."
cache="$prefix$cname.csv"
(
while read -r line; do
    m=`expr match "$line" "@table"`
    if [ "$m" = "6" ]; then
	set -- $line
	cname="$2"
	cache="/tmp/process_dox.$cname.csv"
	table_mode=true
	echo -n > $cache
	continue
    fi
    m=`expr match "$line" "@endtable"`
    if [ "$m" = "9" ]; then
	table_mode=false
	show_file $cache csv
	continue
    fi
    m=`expr match "$line" "@width"`
    if [ "$m" = "6" ]; then
	set -- $line
	width="$2"
    fi
    m=`expr match "$line" "@diff"`
    if [ "$m" = "5" ]; then
	set -- $line
	a="$prefix$2"
	b="$prefix$3"
	out="$prefix$4"
	fmt=$5
	shift 5
	rm -f $out
	$SSDIFF --output $out $@ --omit-format-name --format $fmt $a $b > /dev/null 2> /dev/null
	continue
    fi
    m=`expr match "$line" "@merge"`
    if [ "$m" = "6" ]; then
	set -- $line
	a="$prefix$2"
	b="$prefix$3"
	c="$prefix$4"
	d="$prefix$5"
	rm -f $d
	$SSMERGE --output $d $a $b $c > /dev/null 2> /dev/null
	continue
    fi
    m=`expr match "$line" "@rediff"`
    if [ "$m" = "7" ]; then
	set -- $line
	a="$prefix$2"
	out="$prefix$3"
	fmt=$4
	shift 4
	rm -f $out
	$SSREDIFF --output $out --omit-format-name --format $fmt $a > /dev/null 2> /dev/null
	continue
    fi
    m=`expr match "$line" "@show"`
    if [ "$m" = "5" ]; then
	set -- $line
	fname=$2
	fmt=$3
	if [ "k$fmt" = "k" ]; then
	    fmt="hilite"
	fi
	if [ -e $prefix$fname ]; then
	    fname="$prefix$fname"
	elif [ -e $ROOT/tests/$fname ]; then
	    fname="$ROOT/tests/$fname"
	elif [ -e $ROOT/doc/$fname ]; then
	    fname="$ROOT/doc/$fname"
	else
	    echo "CANNOT FIND $fname" 1>&2
	    exit 1
	fi
	show_file $fname $fmt || exit 1
	cp $fname $prefix`basename $fname`
	continue
    fi
    m=`expr match "$line" "@load"`
    if [ "$m" = "5" ]; then
	set -- $line
	fname=$2
	if [ -e $ROOT/tests/$fname ]; then
	    fname="$ROOT/tests/$fname"
	elif [ -e $ROOT/doc/$fname ]; then
	    fname="$ROOT/doc/$fname"
	else
	    echo "CANNOT FIND $fname" 1>&2
	    exit 1
	fi
	cp $fname $prefix`basename $fname`
	continue
    fi
    m=`expr match "$line" "@autodoc"`
    if [ "$m" = "8" ]; then
	set -- $line
	cmd=$2
	$bin/bin/$cmd --help-dox || exit 1	
	continue
    fi
    if $table_mode ; then
	echo "$line" >> $cache
    else
	echo "$line"
    fi
done
) < $fname

