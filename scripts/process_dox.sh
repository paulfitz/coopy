#!/bin/bash

fname="$1"
src="$2"
bin="$3"

CSV2HTML="$bin/bin/ss2html"
SSDIFF="$bin/bin/ssdiff"
SSREDIFF="$bin/bin/ssrediff"

BASE=`dirname $fname`
IMG_DIR="images/screenshot"
mkdir -p $BASE/$IMG_DIR
SRC="$fname"

which html2ps || exit 1
which ssconvert || exit 1
which ps2pdf || exit 1
which pdfcrop || exit 1

width=""
x=0
function show_file {
    local fname="$1"
    local format="$2"
    if [ "k$format" = "kxls" ]; then
	rm -f /tmp/tmp.html
	# bug in ssconvert :-(
	#PROB="1380,383"
	#echo "= |length=$PROB->FOO|" | ( sspatch --inplace $fname - > /dev/null )
	ssconvert --export-type=Gnumeric_html:html40frag $fname /tmp/tmp.html > /dev/null
	if [ ! -e /tmp/tmp.html ] ; then
	    echo "Failed to ssconvert $fname" 1>&2
	    exit 1
	fi
	cat /tmp/tmp.html | grep -v caption | grep -v exporter | sed "s/&rt;/>/g" | sed "s/border=.1.//g" | sed "s|</*font[^<>]*>||g" | sed "s/FOO/$PROB/" > /tmp/tmp2.html
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
	a="$prefix$2.csv"
	b="$prefix$3.csv"
	fmt=$4
	shift
	shift
	shift
	shift
	if [ "k$fmt" = "khilite" ] ; then
	    $SSDIFF --output ${prefix}result.xls --omit-format-name --format $fmt $a $b > /dev/null 2> /dev/null
	    show_file ${prefix}result.xls xls
	else
	    $SSDIFF $@ --omit-format-name --format $fmt $a $b
	fi
	continue
    fi
    m=`expr match "$line" "@rediff"`
    if [ "$m" = "7" ]; then
	set -- $line
	a="$prefix$2.csv"
	b="$prefix$3.csv"
	fmt=$4
	fmt2=$5
	shift
	shift
	shift
	shift
	shift
	fname=""
	if [ "k$fmt" = "khilite" ] ; then
	    fname="${prefix}result.xls"
	    $SSDIFF --output "$fname"  --omit-format-name --format $fmt $a $b > /dev/null 2> /dev/null
	    # show_file ${prefix}result.xls xls
	else
	    fname="${prefix}result.tdiff"
	    $SSDIFF --output "$fname" $@ --omit-format-name --format $fmt $a $b
	fi
	if [ "k$fmt2" = "kops" ] ; then
	    fname2="${prefix}result.csv"
	    $SSREDIFF --output $fname2 --omit-format-name $@ --format $fmt2 $fname
	    show_file ${prefix}result.csv csv
	else
	    $SSREDIFF --omit-format-name $@ --format $fmt2 $fname
	fi
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

