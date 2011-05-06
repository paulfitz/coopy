The COOPY toolbox
=================

Diffing, patching, merging, and revision-control for spreadsheets and
databases.  Focused on keeping data in sync across different 
technologies (e.g. a MySQL table and an Excel spreedsheet).

* See BUILD.txt for information on building the programs.
  - Summary: CMake
* See SERVE.txt for server-side information.
  - Summary: fossil
* See COPYING.txt for copyright and license information.
  - Summary: GPL.  Relicensing of library core planned for version 1.0.

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

Status
------
COOPY targets a stable release at version 1.0.  At the time of
writing, it is just beyond 0.5.  It is about half way there.
