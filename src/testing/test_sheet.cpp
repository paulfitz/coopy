
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>

#include <coopy/CsvMerge.h>
#include <coopy/CsvFile.h>
#include <coopy/DataStat.h>
#include <coopy/SheetCompare.h>
#include <coopy/CsvPatch.h>
#include <coopy/MergeOutputAccum.h>
#include <coopy/MergeOutputPatch.h>
#include <coopy/MergeOutputCsvDiff.h>

using namespace coopy::store;
using namespace coopy::cmp;

int main(int argc, char *argv[]) {
  int c;
  int result = 0;

  bool parented = false;
  CsvSheet local;
  CsvSheet remote;
  CsvSheet parent;
  CsvSheet *ss = &local;
  DataStat stat;
  bool dirty = true;
  int diffs = 0;
  
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
      {"read", 1, 0, 'r'},
      {"write", 1, 0, 'w'},
      {"save", 1, 0, 's'},
      {"prop", 1, 0, 'p'},
      {"patch", 0, 0, 't'},
      {"assert", 1, 0, 'a'},
      {"remove_row", 1, 0, 'd'},
      {"local", 0, 0, 'L'},
      {"remote", 0, 0, 'R'},
      {"parent", 0, 0, 'P'},
      {"dumb", 0, 0, 'D'},
      {"compare", 2, 0, 'c'},
      {"diff", 0, 0, 'f'},
      {0, 0, 0, 0}
    };

    c = getopt_long(argc, argv, "",
		    long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 'L':
      printf("Switching to local sheet\n");
      ss = &local;
      break;
    case 'R':
      printf("Switching to remote sheet\n");
      ss = &remote;
      break;
    case 'P':
      printf("Switching to parent sheet\n");
      parented = true;
      ss = &parent;
      break;
    case 'r':
      if (optarg) {
	CsvFile::read(optarg,*ss);
	printf("Read %s (%dx%d)\n", optarg, ss->width(), ss->height());
	dirty = true;
      }
      break;
    case 'w':
      if (optarg) {
	CsvFile::write(*ss,optarg);
	printf("Wrote %s (%dx%d)\n", optarg, ss->width(), ss->height());
      }
      break;
    case 'p':
      if (optarg) {
	std::string prop = optarg;
	result = -1;
	if (prop=="hdr") {
	  if (dirty) {
	    printf("Evaluating...\n");
	    stat.evaluate(*ss);
	    dirty = false;
	  }
	  result = stat.getRowDivider();
	  printf("hdr is %d\n", result);
	} else if (prop=="height") {
	  result = local.height();
	  printf("height is %d\n", result);
	} else if (prop=="diffs") {
	  result = diffs;
	  printf("diffs is %d\n", result);
	}
      }
      break;
    case 'a':
      if (optarg) {
	int v = atoi(optarg);
	if (v==result) {
	  printf("Success, %d==%d\n", result, v);
	} else {
	  printf("Failure, %d!=%d\n", result, v);
	  printf("Local:\n");
	  CsvFile::write(local,"-");
	  printf("Remote:\n");
	  CsvFile::write(remote,"-");
	  printf("Failure, %d!=%d\n", result, v);
	  return 1;
	}
      }
      break;
    case 'd':
      if (optarg) {
	int row = atoi(optarg);
	ss->removeRow(row);
	printf("Removed row %d\n", row);
	dirty = true;
      }
      break;
    case 'D':
      printf("Making dumb conflict sheet\n");
      {
	CsvMerge merge;
	merge.dumb_conflict(local,remote);
	local = merge.get();
	ss = &local;
      }
      break;
    case 'f': 
      {
	diffs = 0;
	printf("Check if remote and local are similar\n");
	printf("remote %d %d\n", remote.width(), remote.height());
	printf("local %d %d\n", local.width(), local.height());
	if (remote.width()!=local.width() ||
	    remote.height()!=local.height()) {
	  diffs = 1000;
	} else {
	  for (int x=0; x<remote.width(); x++) {
	    for (int y=0; y<remote.height(); y++) {
	      if (remote.cellString(x,y)!=local.cellString(x,y)) {
		diffs++;
	      }
	    }
	  }
	}
      }
      break;
    case 't':
      {
	printf("Applying patch ('local') to parent, result goes to local\n");
	CsvPatch patch;
	patch.apply(parent,local);
	local = patch.get();
	ss = &local;
      }
      break;
    case 'c':
      {
	std::string diffMode = "";
	if (optarg) {
	  diffMode = optarg;
	  printf("Diff mode is [%s]\n", diffMode.c_str());
	}
	if (parented) {
	  printf("Three way compare...\n");
	}


	SheetCompare cmp;
	cmp.setVerbose(true);
	if (!parented) {
	  parent = local;
	}
	CompareFlags flags;
	if (diffMode!="") {
	  if (diffMode=="csv") {
	    MergeOutputCsvDiffStable output;
	    cmp.compare(parent,local,remote,output,flags);
	    local = output.get();
	  } else {
	    MergeOutputPatch output;
	    cmp.compare(parent,local,remote,output,flags);
	    local = output.get();
	  }
	} else {
	  MergeOutputAccum output;
	  cmp.compare(parent,local,remote,output,flags);
	  local.copyData(output.getSheet());
	}
	ss = &local;
      }
      break;

    default:
      fprintf(stderr, "Usage: %s [--read fname] [--eval]\n",
	      argv[0]);
      return -1;
    }
  }

  if (optind<argc) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }

  return 0;
}

