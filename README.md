The COOPY toolbox
=================

Diffing, patching, merging, and revision-control for spreadsheets and
databases.  Focused on keeping data in sync across different 
technologies (e.g. a MySQL table and an Excel spreedsheet).

The main programs
-----------------
* [`ssdiff`](http://share.find.coop/doc/ssdiff.html) - generate diffs for spreadsheets and databases.
* [`sspatch`](http://share.find.coop/doc/sspatch.html) - apply patches to spreadsheets and databases.
* [`ssmerge`](http://share.find.coop/doc/ssmerge.html) - merge tables with a common ancestor.
* [`ssformat`](http://share.find.coop/doc/ssformat.html) - convert tables from one format to another.
* [`ssfossil`](http://share.find.coop/doc/ssfossil.html) - the fossil DVCS, modified to use tabular diffs
  rather than line-based diffs. You can also [work with git](http://share.find.coop/doc/tutorial_git.html).
  If you use github, you may want to check out [CSVHub](http://theodi.org/blog/csvhub-github-diffs-for-csv-files),
  which uses a simplified version of `ssdiff` called [`daff`](http://paulfitz.github.io/daff/) to show pretty
  data diffs on github.
* [`coopy`](http://share.find.coop/doc/coopy.html) - a graphical interface to ssfossil.

Supported data formats
----------------------
* CSV (comma separated values)
* SSV (semicolon separated values)
* TSV (tab separated values)
* Excel formats (via gnumeric's libspreadsheet)
* Other spreadsheet formats (via gnumeric's libspreadsheet)
* Sqlite
* MySQL
* Microsoft Access format (via mdbtools - READ ONLY, or via jackcess for read/write)
* A JSON representation of tables.
* A custom "CSVS" format that is a minimal extension of CSV
  to handle multiple sheets in a single file, allow
  for unambiguous header rows, and have a clear representation
  of NULL.

Supported diff formats
----------------------
* Highlighter diff format, see spec at http://dataprotocols.org/tabular-diff-format/
* TDIFF (format developed with Joe Panico of diffkit.org)
* DTBL (csv-compatible format, COOPY specific, may be dropped)
* SQL (Sqlite flavor)

Example uses
------------
* Enumerating differences between any pairwise combination of CSV files,
  database tables, or spreadsheets.
* Applying changes to a database or spreadsheet, without losing
  meta-data (formatting of spreadsheet, indexing/type information for
  database).  Particularly useful for applying changes in an
  exports CSV file back to the original source.
* Editing a MySQL/Sqlite database in gnumeric/openoffice/Excel/...
* Distributed editing of a spreadsheet/database using a DVCS.
  Benefits: revision history, offline editing in tool of choice,
  self-hosting possible.

Features
--------
* By default, when comparing tables, no initial assumption is
  made about schema similarity.  Column names are not required 
  to exist, or to be preserved between tables.  The number and
  order of columns may also differ.
* If schema changes are not expected, COOPY can be directed
  to use certain columns as a trusted identity for rows (a key).
* Respects row order for table representations for which row
  order is meaningful (spreadsheets, csv).
* By default, COOPY assumes your data is very messy.  If it is clean, you can get much faster results by tweaking some options.

Algorithm
---------
The core of the COOPY toolbox is a 3-way comparision between an
ancestor and two descendents.  First, rows are compared using bags of
substrings drawn from across all columns.  Once corresponding rows are
known, columns are compared, again using bags of substrings.  Row and
column assignments are optimized and ordered using a Viterbi lattice.
Once the pairwise relationships between each descendent and its
ancestor are known, differences are computed, and a good merged
ordering is determined (again using the Viterbi algorithm).

Installing on OSX
-----------------
* Use homebrew.
* Do `brew tap paulfitz/data` to get a formula for coopy.
* Then `brew install coopy` should work fine.

Installing on Windows
---------------------
* Get an installer from the [releases page](https://github.com/paulfitz/coopy/releases).

Installing on Linux
-------------------
* Sorry, this is where I develop myself, but I don't have an installer. Building is easy though!

Building
--------
* For a stripped-down js/py/rb/php version see http://paulfitz.github.io/daff/
* See BUILD.md for information on building the programs.
  - Summary: CMake
* See SERVE.txt for server-side information.
  - Summary: fossil
* See COPYING.txt for copyright and license information.
  - Summary: GPL.  Relicensing of library core planned for version 1.0.

Status
------
COOPY targets a stable, fully-documented release at version 1.0.  At 
the time of writing, the version number is just beyond 0.5.  It is 
about half way there.

Apparently COOPY is the closest thing right now to git for data:

 * http://opendata.stackexchange.com/questions/748/is-there-a-git-for-data

But if you deal with big data sets and don't care so much about diffs
and patches and whatnot, you may want to look at `dat`:

 * https://github.com/maxogden/dat
