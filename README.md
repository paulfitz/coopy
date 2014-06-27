The COOPY toolbox
=================

Diffing, patching, merging, and revision-control for spreadsheets and
databases.  Focused on keeping data in sync across different 
technologies (e.g. a MySQL table and an Excel spreedsheet).

* For a stripped-down js/py/rb/php version see http://paulfitz.github.io/daff/
* See BUILD.md for information on building the programs.
  - Summary: CMake
* See SERVE.txt for server-side information.
  - Summary: fossil
* See COPYING.txt for copyright and license information.
  - Summary: GPL.  Relicensing of library core planned for version 1.0.

Help!
-----

* OSX user? Would you be willing to test new versions of a homebrew formula for Coopy?
* Windows user? Would you be willing to test new releases of Coopy?

Please get in touch and help a lazy linux guy stay in his comfort zone. [Chime in on an issue](https://github.com/paulfitz/coopy/issues/2) about this, or I'm [@fitzyfitzyfitzy](https://twitter.com/) on twitter.

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

The main programs
-----------------
* ssdiff - generate diffs for spreadsheets and databases.
* sspatch - apply patches to spreadsheets and databases.
* ssmerge - merge tables with a common ancestor.
* ssfossil - the fossil DVCS, modified to use tabular diffs
  rather than line-based diffs.
* coopy - a graphical interface to ssfossil.

Supported data formats
----------------------
* CSV (comma separated values)
* SSV (semicolon separated values)
* TSV (tab separated values)
* Excel formats (via gnumeric's libspreadsheet)
* Other spreadsheet formats (via gnumeric's libspreadsheet)
* Sqlite
* MySQL
* Microsoft Access format (via mdbtools - READ ONLY)
* A JSON representation of tables.
* A custom "CSVS" format that is a minimal extension of CSV
  to handle multiple sheets in a single file, allow
  for unambiguous header rows, and have a clear representation
  of NULL.

Supported diff formats
----------------------
* TDIFF (format developed with Joe Panico of diffkit.org)
* DTBL (csv-compatible format, COOPY specific, may be dropped)
* SQL (Sqlite flavor)

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
