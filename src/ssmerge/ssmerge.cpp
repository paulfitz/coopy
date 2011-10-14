
#include <stdio.h>
#include <coopy/Options.h>
#include <coopy/Diff.h>

using namespace std;
using namespace coopy::app;

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
    printf("Merge spreadsheets, with sensible treatment of changes:\n");
    printf("  ssmerge [--output output.csv] parent.csv local.csv remote.csv\n");
    printf("Note: parent.csv should be a 'common ancestor' of the other two.\n");
    printf("The merged or conflicting result will be placed in the specified output file.\n");
    printf("If one or more inputs have been trimmed, let ssmerge know so it won't\ntreat trimming as implying deletion:\n");
    printf("  ssmerge [--head-trimmed] [--tail-trimmed] parent.csv local.csv remote.csv\n");
    return 1;
  }

  Diff diff;
  return diff.apply(opt);
}

