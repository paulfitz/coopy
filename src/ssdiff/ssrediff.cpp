
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
    opt.addUsage("ssrediff [options] <patch>");
    opt.addDescription("Reformat a diff file.");
    opt.showOptions(OPTION_FOR_REDIFF);
    opt.endHelp();

    return 1;
  }

  Diff diff;
  return diff.apply(opt);
}

