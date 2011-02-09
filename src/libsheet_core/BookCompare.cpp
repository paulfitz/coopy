#include <coopy/BookCompare.h>
#include <coopy/SheetCompare.h>
#include <coopy/Dbg.h>
#include <coopy/CsvWrite.h>

#include <set>
#include <algorithm>

#include <stdio.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;

int BookCompare::compare(TextBook& pivot, TextBook& local, TextBook& remote, 
			 MergeOutput& output, const CompareFlags& flags) {
  // Merge currently based purely on names, no content comparison.
  // Hence a sheet rename cannot be guessed at yet.

  dbg_printf("BookCompare::compare begins\n");

  output.setFlags(flags);
  output.mergeStart();

  vector<string> pivot_names = pivot.getNames();
  vector<string> local_names = local.getNames();
  vector<string> remote_names = remote.getNames();
  set<string> pivot_name_set(pivot_names.begin(),pivot_names.end());
  set<string> local_name_set(local_names.begin(),local_names.end());
  set<string> remote_name_set(remote_names.begin(),remote_names.end());
  set<string> candidate_name_set;
  set<string> name_set;
  set_union(local_name_set.begin(),
	    local_name_set.end(),
	    remote_name_set.begin(),
	    remote_name_set.end(),
	    inserter(candidate_name_set, candidate_name_set.end()));
  for (set<string>::const_iterator it=candidate_name_set.begin();
       it!=candidate_name_set.end();
       it++) {
    string name = *it;
    bool in_pivot = (pivot_name_set.find(name)!=pivot_name_set.end());
    bool in_local = (local_name_set.find(name)!=local_name_set.end());
    bool in_remote = (remote_name_set.find(name)!=remote_name_set.end());
    if (!in_pivot) {
      // added in either local or remote
      name_set.insert(name);
    } else if (in_local&&in_remote) {
      // not removed
      name_set.insert(name);
    }
  }

  for (set<string>::const_iterator it=name_set.begin();
       it!=name_set.end();
       it++) {
    string name = *it;
    dbg_printf("\n\nBookCompare::compare - Working on \"%s\"\n", name.c_str());
    PolySheet pivot_sheet;
    PolySheet local_sheet;
    PolySheet remote_sheet;
    if (pivot_names.size()==1 && 
	local_names.size()==1 && 
	remote_names.size()==1) {
      pivot_sheet = pivot.readSheetByIndex(0);
      local_sheet = local.readSheetByIndex(0);
      remote_sheet = remote.readSheetByIndex(0);
    } else {
      pivot_sheet = pivot.readSheet(name.c_str());
      local_sheet = local.readSheet(name.c_str());
      remote_sheet = remote.readSheet(name.c_str());
    }
    if (!(pivot_sheet.isValid()&&
	  local_sheet.isValid()&&
	  remote_sheet.isValid())) {
      fprintf(stderr,"Could not find matching sheets - no big deal, but not ready for this yet\n");
      exit(1);
    }

    SheetCompare cmp;
    if (name_set.size()>1) {
      bool ok = output.setSheet(name.c_str());
      if (!ok) {
	fprintf(stderr,"Output format rejected sheet \"%s\"\n", name.c_str());
	return -1;
      }
    }
    cmp.compare(pivot_sheet,local_sheet,remote_sheet,output,flags);
    dbg_printf("BookCompare::compare - Done with \"%s\"\n", name.c_str());
  }

  return -1;
}
  
void BookCompare::setVerbose(bool verbose) {
  _csv_verbose = verbose;
}
