
#include <stdio.h>
#include <coopy/Diff.h>

using namespace coopy::app;
using namespace std;

int main(int argc, char *argv[]) {
  Options opt("ssrediff");
  int r = opt.apply(argc,argv);
  if (r!=0) return r;

  bool help = opt.checkBool("help");

  const vector<string>& core = opt.getCore();
  if (core.size()>2) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }

  if (core.size()<1||help) {
    opt.beginHelp();
    opt.addUsage("ssrediff [options] PATCHFILE");
    opt.addUsage("ssrediff [options] DATAFILE PATCHFILE");
    opt.addDescription("Reformat a tabular diff file. Converting to hilite/review formats will require supplying the original spreadsheet/database. Not every diff format supported as output by ssdiff can be read as input by ssrediff.");
    opt.showOptions(OPTION_FOR_REDIFF);
    opt.addExample("ssrediff --format sql numbers_patch.tdiff",
		   "Convert tdiff format file to SQL").require("_numbers_patch.tdiff");
    opt.addExample("ssrediff --format csv numbers_patch.tdiff",
		   "Convert tdiff format file to a CSV-readable diff format").require("_numbers_patch.tdiff");
    opt.addExample("ssrediff --format hilite --output review.csv numbers_buggy.csv numbers_patch.tdiff",
		   "Generate tabular form of diff for eyeballing. If ssrediff is compiled with gnumeric support, and output format is *.xls, color highlighting is added.").require("numbers_buggy.csv").require("_numbers_patch.tdiff");
    opt.endHelp();

    return help?0:1;
  }

  Diff diff;
  return diff.apply(opt);
}

