
#include <stdio.h>
#include <coopy/Diff.h>

using namespace coopy::app;

int main(int argc, char *argv[]) {
  Options opt("sspatch");
  int r = opt.apply(argc,argv);
  if (r!=0) return r;

  bool help = opt.checkBool("help");
  if (argc==1 || help) {
    opt.beginHelp();
    opt.addUsage("sspatch [options] <dbase> <patch>");
    opt.addDescription("Modify a table/database/spreadsheet based on a pre-computed comparison.");
    opt.showOptions(OPTION_FOR_PATCH);
    opt.endHelp();
    return 1;
  }

  Diff diff;
  return diff.apply(opt);
}
