
#include <stdio.h>
#include <coopy/Diff.h>

using namespace coopy::app;

int main(int argc, char *argv[]) {
  Options opt("sspatch");
  int r = opt.apply(argc,argv);
  if (r!=0) return r;

  bool help = opt.checkBool("help");
  if (argc==1 || help) {
    printf("Modify a table/database/spreadsheet based on a pre-computed comparison.\n");
    printf("  sspatch [--output output.csv] local.csv patch.txt\n");
    printf("\n");
    printf("Output by default goes to standard output.  For inplace modification:\n");
    printf("  sspatch --inplace db.sqlite patch.txt\n");
    printf("\n");
    printf("The patch may be read from standard input as follows:\n");
    printf("  sspatch local.csv -\n");
    printf("\n");
    
    printf("The sspatch tool can also be used to convert between patch formats\n");
    printf("  sspatch --format raw patch.txt    # show debug information about patch\n");
    printf("  sspatch --format tdiff patch.csv  # render patch in tdiff format\n");
    printf("  sspatch --format sql patch.csv    # render patch in SQL format\n");
    printf("  sspatch --format csv patch.txt    # render patch in csv format\n");
    printf("  sspatch --format hilite local.csv patch.txt # render patch in highlighter\n");
    printf("                                    # format (best used with --output out.xls)\n");
    return 1;
  }

  Diff diff;
  return diff.apply(opt);
}
