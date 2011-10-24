
#include <stdio.h>
#include <coopy/Options.h>
#include <coopy/Diff.h>

using namespace std;
using namespace coopy::app;

int main(int argc, char *argv[]) {
  Options opt("ssmerge");
  int r = opt.apply(argc,argv);
  if (r!=0) return r;

  bool help = opt.checkBool("help");

  const vector<string>& core = opt.getCore();
  if (core.size()>3) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }

  if (core.size()<3||help) {
    opt.beginHelp();
    opt.addUsage("ssmerge [options] FILE1 FILE2 FILE3");
    opt.addDescription("Merge table/database/spreadsheets. The first file must be a common ancestor of the remaining two.");
    opt.showOptions(OPTION_FOR_MERGE);
    opt.addExample("ssmerge numbers_buggy.csv numbers.csv numbers_buggy_add.csv",
		   "Apply a patch to a table read from standard input.").require("numbers_buggy.csv").require("numbers.csv").require("numbers_buggy_add.csv");
    opt.endHelp();
    return 1;
  }

  Diff diff;
  return diff.apply(opt);
}

