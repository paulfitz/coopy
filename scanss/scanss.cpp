
#include "CsvMerge.h"
#include "CsvFile.h"
#include "CsvStat.h"
#include "CsvCompare.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>

int main(int argc, char *argv[]) {
  int c;
  int digit_optind = 0;
  int result = 0;

  CsvSheet local;
  CsvSheet remote;
  CsvSheet parent;
  CsvSheet *ss = &local;
  CsvStat stat;
  bool dirty = true;
  
  while (1) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
      {"read", 1, 0, 'r'},
      {"write", 1, 0, 'w'},
      {"save", 1, 0, 's'},
      {"prop", 1, 0, 'p'},
      {"assert", 1, 0, 'a'},
      {"remove_row", 1, 0, 'd'},
      {"local", 0, 0, 'L'},
      {"remote", 0, 0, 'R'},
      {"parent", 0, 0, 'P'},
      {"dumb", 0, 0, 'D'},
      {"compare", 0, 0, 'c'},
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
	if (dirty) {
	  printf("Evaluating...\n");
	  stat.evaluate(*ss);
	  dirty = false;
	}
	std::string prop = optarg;
	if (prop=="hdr") {
	  result = stat.getRowDivider();
	  printf("hdr is %d\n", result);
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
	  return 1;
	}
      }
      break;
    case 'd':
      if (optarg) {
	int row = atoi(optarg);
	ss->removeRow(row);
	printf("Removed row %d\n");
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
    case 'c':
      {
	CsvCompare cmp;
	cmp.compare(local,remote);
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

