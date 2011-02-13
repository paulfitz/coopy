#include <stdio.h>
#include <getopt.h>

#include <coopy/Dbg.h>

#include <string>

#include "WideSheetManager.h"

using namespace std;

void showStatus(WideSheetManager& ws, const char *key) {
  string link = ws.getPropertyWithDefault(key);
  if (link=="") {
    printf("No %s setting.\n", key);
  } else {
    printf("%s %s\n", key, link.c_str());
  }
}

int main(int argc, char *argv[]) {
  bool verbose = false;

  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"verbose", 0, 0, 'v'},
      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 'v':
      verbose = true;
      coopy_set_verbose(true);
      break;
    default:
      fprintf(stderr, "Unrecognized option\n");
      return 1;
    }
  }

  argc -= optind;
  argv += optind;

  if (argc<1) {
    fprintf(stderr,"Welcome to widesheet. Usage:\n");
    fprintf(stderr,"  widesheet                    # this help message\n");
    fprintf(stderr,"  widesheet init               # create workspace, a widesheet.sqlite file\n");
    fprintf(stderr,"  widesheet status             # see workspace settings\n");
    fprintf(stderr,"  widesheet local <filename>   # user file, .xls etc, any directory\n");
    fprintf(stderr,"  widesheet remote <filename>  # controlled file, .csv[s], current directory\n");
    fprintf(stderr,"  widesheet pivot <filename>   # checkpoint file, will copy controlled file\n");
    fprintf(stderr,"  offer                        # update controlled file from user file\n");
    fprintf(stderr,"  accepted                     # copy pivot file from remote file\n");
    fprintf(stderr,"  diff                         # see difference between local and remote\n");
    return 1;
  }

  string arg = argv[0];
  argc--;
  argv++;

  if (arg=="init") {
    WideSheetManager ws(true);
    if (!ws.isValid()) {
      return 1;
    }
    return 0;
  }

  WideSheetManager ws(false);
  if (!ws.isValid()) {
    return 1;
  }

  if (arg=="local") {
    if (argc==1) {
      return ws.setProperty("local",argv[0])?0:1;
    }
    if (argc!=0) {
      fprintf(stderr,"Wrong number of arguments\n");
      return 1;
    }
    showStatus(ws,"local");
  }

  if (arg=="remote") {
    if (argc==1) {
      return ws.setProperty("remote",argv[0])?0:1;
    }
    if (argc!=0) {
      fprintf(stderr,"Wrong number of arguments\n");
      return 1;
    }
    showStatus(ws,"remote");
  }

  if (arg=="pivot") {
    if (argc==1) {
      return ws.setProperty("pivot",argv[0])?0:1;
    }
    if (argc!=0) {
      fprintf(stderr,"Wrong number of arguments\n");
      return 1;
    }
    showStatus(ws,"pivot");
  }

  if (arg=="status") {
    showStatus(ws,"local");
    showStatus(ws,"remote");
    showStatus(ws,"pivot");
    return 0;
  }

  if (arg=="offer") {
    return ws.exportSheet()?0:1;
  }

  if (arg=="accepted") {
    return ws.acceptSheet()?0:1;
  }

  if (arg=="diff") {
    return ws.diffSheet()?0:1;
  }

  fprintf(stderr, "Unknown operation.\n");
  return 1;
}
