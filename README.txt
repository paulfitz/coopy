Coopy!
======

Coopy uses fossil (the SCM) to make distributed data collection
projects easier.  Coopy comes with command-line tools for merging
spreadsheets.

To build, use CMake (if you have cmake, you can just run ./autogen.sh).
+ http://www.cmake.org

Coopy is licensed under the GPLv2.  

Coopy uses "fossil" under the GPL:
+ See src/ssfossil/fossil

Coopy uses "libcsv" under the GPL:
+ See src/libcsv

The remaining code is:
  Copyright: (c) 2010 Paul Fitzpatrick <paulfitz@alum.mit.edu>
  CopyPolicy: Released under the GPLv2 (see GPL.TXT)

To use Coopy, you need to be able to host a fossil repository.
Known sites for hosting repositories:
  http://share.find.coop/
  http://www.chiselapp.com/

It is easy to do by yourself:
  http://www.fossil-scm.org/index.html/doc/tip/www/quickstart.wiki#serversetup