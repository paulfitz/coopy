
#include <stdio.h>
#include <coopy/Diff.h>

using namespace coopy::app;
using namespace std;

int main(int argc, char *argv[]) {
  Options opt("sspatch");
  int r = opt.apply(argc,argv);
  if (r!=0) return r;

  bool help = opt.checkBool("help");
  const vector<string>& core = opt.getCore();
  if (core.size()<1 || help) {
    opt.beginHelp();
    opt.addUsage("sspatch [options] DATAFILE PATCHFILE");
    opt.addUsage("sspatch [options] --cmd PATCHSTRING DATAFILE");
    opt.addDescription("Modify a table/database/spreadsheet to integrate the changes described in a pre-computed difference.");
    opt.showOptions(OPTION_FOR_PATCH);
    opt.addExample("sspatch numbers_buggy.csv numbers_patch.tdiff",
		   "Apply a patch to a table. Output goes to standard output. Input file is untouched.").require("numbers_buggy.csv").require("numbers_patch.tdiff");
    opt.addExample("sspatch --inplace numbers_buggy.csv numbers_patch.tdiff",
		   "Apply a patch to a table. Input file is modified.").require("numbers_buggy.csv").require("numbers_patch.tdiff");
    opt.addExample("sspatch --tmp tmp.sqlite numbers_buggy.sqlite numbers_patch.tdiff",
		   "Apply a patch to a sqlite database. Input file is not modified. Space for a temporary database is needed to do this. If not supplied, sspatch will ask for it.").require("numbers_buggy.sqlite").require("numbers_patch.tdiff");
    opt.addExample("sspatch - numbers_patch.tdiff < numbers_buggy.csv",
		   "Apply a patch to a table read from standard input.").require("numbers_buggy.csv").require("numbers_patch.tdiff");
    opt.addExample("sspatch numbers_buggy.csv - < numbers_patch.tdiff",
		   "Apply a patch read from standard input.").require("numbers_buggy.csv").require("numbers_patch.tdiff");
    opt.addExample("sspatch --cmd \"+ |two|2|\" numbers_buggy.csv",
		   "Add a new row to a table.").require("numbers_buggy.csv");
    opt.addExample("sspatch --cmd \"- |NAME=four|\" numbers.csv",
		   "Remove a row from a table").require("numbers.csv");
    opt.addExample("sspatch --cmd \"= |NAME=four|DIGIT:*->4|\" numbers_buggy.csv",
		   "Change the DIGIT column on a row with NAME=four.").require("numbers_buggy.csv");
    opt.endHelp();
    return 1;
  }

  Diff diff;
  return diff.apply(opt);
}
