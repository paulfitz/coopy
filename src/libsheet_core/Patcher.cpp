#include <coopy/Patcher.h>

#include <algorithm>

using namespace std;
using namespace coopy::cmp;

int OrderChange::identityToIndex(int id) const {
  if (id==-1) return -1;
  vector<int>::const_iterator it = find(indicesBefore.begin(),
					indicesBefore.begin(),id);
  if (it == indicesBefore.end()) { return -1; }
  return it-indicesBefore.begin();
}
