#include <coopy/MergeOutputSqlDiff.h>

using namespace std;

string vector2string(const vector<string>& src) {
  string result = "";
  for (vector<string>::const_iterator it=src.begin(); it!=src.end(); it++) {
    if (it!=src.begin()) {
      result += " ";
    }
    result += *it;
  }
  return result;
}

bool MergeOutputSqlDiff::changeColumn(const OrderChange& change) {
  printf("Got order change %s -> %s\n",
	 vector2string(change.namesBefore).c_str(),
	 vector2string(change.namesAfter).c_str());
  if (change.mode==ORDER_CHANGE_DELETE) {
    printf("  Delete column %d\n", change.subject);
  } else {
    printf("  Insert column before %d\n", change.subject);
  }
  return true;
}


