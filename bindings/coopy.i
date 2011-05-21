%module(directors="1") coopy
%feature("director") coopy::cmp::Patcher;

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

// common name clashes
 //%rename(toString_c) *::toString() const;
%rename(clone_c) *::clone() const;

#if defined(SWIGJAVA)
%rename(equals) *::operator==;
%rename(not_equals) *::operator!=;
%ignore *::setString(const std::string &);
%ignore *::makeString(const std::string &);
%ignore *::SheetCell(const std::string &,bool);
%rename(isFinal) *::final;
%ignore coopy::store::Property::put(char const *,std::string const &);
#endif
#if defined(SWIGRUBY)
%rename(not_equals) *::operator!=;
#endif
%ignore *::asMap() const;
%rename(assign) *::operator=;

%{
#include <coopy/Appearance.h>
#include <coopy/PolySheet.h>
#include <coopy/PolyBook.h>
#include <coopy/BookCompare.h>
%}
%include <coopy/RefCount.h>
%include <coopy/Appearance.h>
%include <coopy/Property.h>
%include <coopy/SheetStyle.h>
%include <coopy/SheetCell.h>

%template(CellMap) std::map<std::string,coopy::store::SheetCell>;
%template(StringList) std::vector<std::string>;

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
