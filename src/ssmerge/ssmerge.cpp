
#include <stdio.h>
#include <coopy/Options.h>
#include <coopy/Diff.h>

using namespace std;
using namespace coopy::app;

#include <coopy/unimain.h>
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
		   "Merge two CSV tables (numbers.csv and numbers_buggy_add.csv) with a common ancestor (numbers_buggy.csv).").require("numbers_buggy.csv").require("numbers.csv").require("numbers_buggy_add.csv");
    opt.addExample("ssmerge --theirs numbers_buggy.csv numbers.csv numbers_conflict.csv",
		   "Merge numbers.csv and numbers_conflict.csv (with common ancestor numbers_buggy.csv), deferring to numbers_conflict.csv in the case of conflict.").require("numbers_buggy.csv").require("numbers.csv").require("numbers_conflict.csv");
    opt.addExample("ssmerge --ours numbers_buggy.csv numbers.csv numbers_conflict.csv",
		   "Merge numbers.csv and numbers_conflict.csv (with common ancestor numbers_buggy.csv), deferring to numbers.csv in the case of conflict.").require("numbers_buggy.csv").require("numbers.csv").require("numbers_conflict.csv");
    opt.addExample("ssmerge --inplace --theirs numbers_buggy.csv numbers.csv numbers_conflict.csv",
		   "Merge directly into numbers.csv.  Without --inplace, output goes to standard output.").require("numbers_buggy.csv").require("numbers.csv").require("numbers_conflict.csv");
    opt.endHelp();
    return help?0:1;
  }

  Diff diff;
  return diff.apply(opt);
}

