
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
  if (core.size()>1) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }

  if (core.size()<1||help) {
    opt.beginHelp();
    opt.addUsage("ssresolve [options] FILE");
    opt.addDescription("Resolve a file with conflicts from ssmerge.");
    opt.showOptions(OPTION_FOR_RESOLVE);
    opt.addExample("ssresolve numbers_muddle.csv",
		   "Check if file is resolved.").require("_numbers_muddle.csv");
    opt.addExample("ssresolve --ours numbers_muddle.csv",
		   "Resolve conflicts in favor of local/left values.").require("_numbers_muddle.csv");
    opt.addExample("ssresolve --theirs numbers_muddle.csv",
		   "Resolve conflicts in favor of remote/right values.").require("_numbers_muddle.csv");
    opt.addExample("ssresolve --neither numbers_muddle.csv",
		   "Resolve conflicts in favor of ancestral values.").require("_numbers_muddle.csv");
    opt.endHelp();
    return help?0:1;
  }

  Diff diff;
  return diff.apply(opt);
}

