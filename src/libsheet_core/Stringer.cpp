#define WANT_MAP2STRING
#define WANT_VECTOR2STRING

#include <coopy/Stringer.h>
#include <coopy/SheetStyle.h>
#include <coopy/DataSheet.h>

#include <stdio.h>

using namespace std;
using namespace coopy::store;

string stringer_encoder(bool x) {
  char buf[256];
  sprintf(buf,"%s",x?"true":"false");
  return buf;
}

string stringer_encoder(int x) {
  char buf[256];
  sprintf(buf,"%d",x);
  return buf;
}

string stringer_encoder(const string& x) {
  SheetStyle style;
  string result = DataSheet::encodeCell(SheetCell(x,false),style);
  return (result!="")?result:"\"\"";
}

string stringer_encoder(const coopy::store::SheetCell& x) {
  SheetStyle style;
  string result = DataSheet::encodeCell(x,style);
  //printf("Encoding %s as %s\n", x.text.c_str(), result.c_str());
  return (result!="")?result:"\"\"";
}

std::string stringify(const std::string& x) {
  return x;
}

std::string stringify(const coopy::store::SheetCell& x) {
  return x.toString();
}



string quoteSql(string x, char del, bool alwaysQuote) {
  bool quote = false;
  bool easy = true;
  string result = x;
  for (int i=0; i<(int)x.length(); i++) {
    char ch = x[i];
    if ((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z')||(ch>='0'&&ch<='9')||
	ch=='_') {
      // ok
    } else {
      if (ch==del) {
	easy = false;
      }
      quote = true;
    }
  }
  if ((alwaysQuote||quote)&&easy) {
    result = string() + del + x + del;
  } else if (!easy) {
    result = del;
    for (int i=0; i<(int)x.length(); i++) {
      if (x[i]!=del) {
	result += x[i];
      } else {
	result += del;
	result += del;
      }
    }
    result += del;
  }
  return result;
}

std::string Stringer::getSpreadsheetColumnName(int x) {
  string current;
  int offset = 0;
  int rem = x;
  do {
    offset = rem%26;
    rem = rem/26;
    char A[2] = "A";
    A[0] += offset;
    current = string(A) + current;
  } while (rem!=0);
  return current;
}

std::string Stringer::nextSpreadsheetColumnName(std::string current) {
    int len = current.length();
    int cursor = len-1;
    while (cursor>=0) {
      if (current[cursor]!='Z') {
	current[cursor]++;
	break;
      }
      current[cursor] = 'A';
      cursor--;
      if (cursor<0) {
	current += 'A';
      }
    }  
    return current;
}


