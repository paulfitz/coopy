
#include <stdio.h>
#include <coopy/Diff.h>

using namespace coopy::app;
using namespace std;

#include <coopy/unimain.h>
int main(int argc, char *argv[]) {
  Options opt("ssdiff");
  int r = opt.apply(argc,argv);
  if (r!=0) return r;

  bool help = opt.checkBool("help");

  const vector<string>& core = opt.getCore();
  if (opt.checkBool("git")) {
    if (core.size()!=7) {
      fprintf(stderr, "Expected 7 parameters from git, but got %d\n",
	      core.size());
      return 1;
    }
    
    string path = core[0];
    string old_file = core[1];
    string old_hex = core[2];
    string old_mode = core[3];
    string new_file = core[4];
    string new_hex = core[5];
    string new_mode = core[6];
    vector<string>& coreMod = opt.getCoreMod();
    coreMod.clear();
    coreMod.push_back(old_file);
    coreMod.push_back(new_file);
    printf("--- a/%s\n", path.c_str());
    printf("+++ b/%s\n", path.c_str());
  }
  if (core.size()>2) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }

  if ((core.size()<1&&opt.checkString("patch")=="")||help) {
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
    opt.addExample("ssdiff --format hilite --output review.csv numbers_buggy.csv numbers.csv","Generate tabular diff for eyeballing. If ssdiff is compiled with gnumeric support, and output format is *.xls, color highlighting is added.").require("numbers_buggy.csv").require("numbers_buggy.csv").require("numbers.csv");
    opt.endHelp();

    return help?0:1;
  }

  Diff diff;
  return diff.apply(opt);
}

