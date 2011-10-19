
#include <stdio.h>
#include <coopy/Options.h>
#include <coopy/Diff.h>

using namespace std;
using namespace coopy::app;

int main(int argc, char *argv[]) {
  Options opt("ssresolve");
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
    opt.addUsage("ssresolve [options] FILE");
    opt.addDescription("Resolve a file with conflicts from ssmerge.");
    opt.showOptions(OPTION_FOR_RESOLVE);
    opt.endHelp();
    return 1;
  }

  Diff diff;
  return diff.apply(opt);
}

