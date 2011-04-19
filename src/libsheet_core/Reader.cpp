#include <coopy/Reader.h>

using namespace coopy::format;
using namespace std;

string Reader::readLine(bool& eof) {
  bool more = false;
  string result = "";
  eof = false;
  do {
    string::size_type at = unread.find("\n");
    if (at==string::npos) {
      string x = read();
      if (x.length()==0) {
	eof = true;
	return "";
      }
      unread += x;
      more = true;
    } else {
      result = unread.substr(0,at);
      unread = unread.substr(at+1,unread.length());
      more = false;
    }
  } while (more);
  if (result.length()>0) {
    if (result[0]=='\r') {
      result = result.substr(1,result.length());
    }
  }
  if (result.length()>0) {
    if (result[result.length()-1]=='\r') {
      result = result.substr(0,result.length()-1);
    }
  }
  return result;
}
