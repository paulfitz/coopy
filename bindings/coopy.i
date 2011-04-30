%module coopy
%{
#include <coopy/PolySheet.h>
#include <coopy/PolyBook.h>
#include <coopy/BookCompare.h>
%}
%include <coopy/RefCount.h>
%include <coopy/Property.h>
%include <coopy/SheetStyle.h>
%include <coopy/SheetCell.h>
%include <coopy/ColumnInfo.h>
%include <coopy/ColumnRef.h>
%include <coopy/RowRef.h>
%include <coopy/SheetSchema.h>
%include <coopy/DataSheet.h>
%include <coopy/PolySheet.h>

%include <coopy/DataBook.h>
%include <coopy/TextBook.h>
%include <coopy/PolyBook.h>

%include <coopy/CompareFlags.h>
%include <coopy/Patcher.h>
%include <coopy/BookCompare.h>
