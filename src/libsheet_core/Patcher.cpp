#include <coopy/Patcher.h>
#include <coopy/MergeOutputSqlDiff.h>
#include <coopy/MergeOutputHumanDiff.h>
#include <coopy/MergeOutputVerboseDiff.h>
#include <coopy/MergeOutputCsvDiff.h>
#include <coopy/MergeOutputTdiff.h>

#include <algorithm>

using namespace std;
using namespace coopy::cmp;

int OrderChange::identityToIndex(int id) const {
  //if (id==-1) return -1;
  vector<int>::const_iterator it = find(indicesBefore.begin(),
					indicesBefore.end(),id);
  if (it == indicesBefore.end()) { return -1; }
  return it-indicesBefore.begin();
}

int OrderChange::identityToIndexAfter(int id) const {
  //if (id==-1) return -1;
  vector<int>::const_iterator it = find(indicesAfter.begin(),
					indicesAfter.end(),id);
  if (it == indicesAfter.end()) { return -1; }
  return it-indicesAfter.begin();
}

Patcher *Patcher::createByName(const char *name, const char *version) {
  string mode(name);
  string _version;
  if (version!=NULL) _version = version;
  Patcher *result = NULL;
  if (mode=="sql") {
    result = new MergeOutputSqlDiff;
  } else if (mode=="human") {
    result = new MergeOutputHumanDiff;
  } else if (mode=="raw") {
    result = new MergeOutputVerboseDiff;
  } else if (mode=="tdiff") {
    result = new MergeOutputTdiff;
  } else if (mode=="csv") {
    if (_version=="0.2") {
      result = new MergeOutputCsvDiffV0p2;
    } else if (_version=="0.4"||_version=="0.5"||_version=="") {
      result = new MergeOutputCsvDiff;
    }
  }
  return result;
}


