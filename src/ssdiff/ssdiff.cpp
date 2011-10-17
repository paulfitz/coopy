
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
    opt.addUsage("ssdiff [options] <file1> <file2>");
    opt.addDescription("Show the difference between two tables/databases/spreadsheets.");
    opt.showOptions(OPTION_FOR_DIFF);
    opt.endHelp();

    return 1;
  }

  Diff diff;
  return diff.apply(opt);
}

