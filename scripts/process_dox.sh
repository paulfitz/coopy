#!/bin/bash

fname="$1"
src="$2"
bin="$3"

CSV2HTML="$bin/bin/csv2html"
SSDIFF="$bin/bin/ssdiff"

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
	cat /tmp/tmp.html | grep -v caption | grep -v exporter | sed "s/&rt;/>/g" | sed "s/\@/\\\\@/g" | sed "s/border=.1.//g" | sed "s|</*font[^<>]*>||g" | sed "s/FOO/$PROB/"
    elif [ "k$format" = "kcsv" ]; then
	$CSV2HTML $fname | sed "s/&rt;/>/g" | sed "s/\@/\\\\@/g"
    else
	echo "\verbatim"
	cat $fname || exit 1
	echo "\endverbatim"
    fi
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

