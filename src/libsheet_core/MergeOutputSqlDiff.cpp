#include <coopy/MergeOutputSqlDiff.h>

#include <stdio.h>
#include <stdlib.h>

#define WANT_MAP2STRING
#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

using namespace std;
using namespace coopy::cmp;

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

bool MergeOutputSqlDiff::changeRow(const RowChange& change) {
  printf("Got row change %s ==> %s\n",
	 map2string(change.cond).c_str(),
	 map2string(change.val).c_str());
  switch (change.mode) {
  case ROW_CHANGE_INSERT:
    printf("  Insert row\n");
    break;
  case ROW_CHANGE_DELETE:
    printf("  Delete row\n");
    break;
  case ROW_CHANGE_UPDATE:
    printf("  Update row\n");
    break;
  default:
    printf("  Unknown row operation\n");
    exit(1);
    break;
  }
  return true;
}


