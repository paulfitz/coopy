#define WANT_MAP2STRING
#define WANT_VECTOR2STRING

#include <coopy/Stringer.h>
#include <coopy/SheetStyle.h>
#include <coopy/DataSheet.h>

#include <stdio.h>

using namespace std;
using namespace coopy::store;

string stringer_encoder(int x) {
  char buf[256];
  sprintf(buf,"%d",x);
  return buf;
}

string stringer_encoder(const string& x) {
  SheetStyle style;
  string result = DataSheet::encodeCell(x,style);
  return (result!="")?result:"\"\"";
}

