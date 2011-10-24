
#include <stdio.h>
#include <coopy/Diff.h>

using namespace coopy::app;
using namespace std;

int main(int argc, char *argv[]) {
  Options opt("ssdiff");
  int r = opt.apply(argc,argv);
  if (r!=0) return r;

  bool help = opt.checkBool("help");

  const vector<string>& core = opt.getCore();
  if (core.size()>2) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }

  if ((core.size()<2&&opt.checkString("patch")=="")||help) {
    opt.beginHelp();
    opt.addUsage("ssdiff [options] FILE1 FILE2");
    opt.addDescription("Show the difference between two tables/databases/spreadsheets.");
    opt.showOptions(OPTION_FOR_DIFF);
    opt.addExample("ssdiff numbers_buggy.csv numbers.csv",
		   "Compare two tables. Output goes to standard output.").require("numbers_buggy.csv").require("numbers.csv");
    opt.addExample("ssdiff --unordered numbers_buggy.csv numbers.csv",
		   "Compare two tables, neglecting row order.").require("numbers_buggy.csv").require("numbers.csv");
    opt.addExample("ssdiff --format sql numbers_buggy.sqlite numbers.sqlite",
		   "Compare two databases, with output in SQL format.").require("numbers_buggy.sqlite").require("numbers.sqlite");
    opt.endHelp();

    return 1;
  }

  Diff diff;
  return diff.apply(opt);
}

