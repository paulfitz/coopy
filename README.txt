Coopy!
======

Coopy uses fossil (the SCM) to make distributed data collection
projects easier.  Coopy comes with command-line tools for merging
spreadsheets.

* See COPYING.txt for copyright and license information.
* See BUILD.txt for information on building the programs.
* See SERVE.txt for server-side information.

Here we describe Coopy and the tools it comes with, and how to
use them.  Generally command line tools work with the following
formats:
* CSV (comma separated values)
* SSV (semicolon separated values)
* TSV (tab separated values)

These programs can help you:
* Merge CSV files that came from the same source but have been
  modified independently (see ssmerge).
* Maintain a repository of CSV files with multiple active contributors
  (see coopy).

There are seeds of support for sqlite databases and Excel+OpenOffice,
but they haven't ripened just yet.  Excel/OpenOffice support is via
gnumeric.

ssmerge
-------

Merge spreadsheets, with sensible treatment of changes:
  ssmerge [--output output.csv] parent.csv local.csv remote.csv
Note: parent.csv should be a 'common ancestor' of the other two.
The merged or conflicting result will be placed in the specified output file.
If one or more inputs have been trimmed, let ssmerge know so it won't
treat trimming as implying deletion:
  ssmerge [--head-trimmed] [--tail-trimmed] parent.csv local.csv remote.csv
There is a --verbose flag to see debugging information.

coopy
-----

Coopy is a simple graphical interface for putting a directory
on your machine under version control.  CSV files in this
directory will be managed using the algorithm behind ssmerge.
So multiple contributors running Coopy can make changes to
offline to tables on their computer, and then have those
changes merge sensibly when they synchronize.

ssdiff
------

Show difference between two spreadsheets. Call as:
  ssdiff [--output <filename>] [--parent parent.csv] reference.csv modified.csv
  ssdiff --format-raw local.csv modified.csv   # full information
  ssdiff --format-human local.csv modified.csv # human readable output
  ssdiff --format-csv local.csv modified.csv   # format that sspatch can read
Output defaults to standard output.

ssformat
--------

Converts between some spreadsheet formats.
E.G. to convert from comma-separated to tab-separated format, call as:
  ssformat input.csv output.tsv
E.G. to convert from comma-separated to sqlite-readable sql, call as:
  ssformat input.csv output.sql
The sqlite conversion is not very polished, and makes some assumptions.

sspatch
-------

This is currently at an early stage of development, but will be very
important to how coopy supports spreadsheet formats with typing and
presentation information in them (i.e. all "real" formats like Excel's
formats or those of OpenOffice).  The idea is to take a description of
how the data in a spreadsheet should be modified, like those produced
by ssdiff, and apply them in a type/presentation-preserving way to the
full spreadsheet.  In other words, as columns or rows of data are
moved around, their type/presentation information should follow them.
