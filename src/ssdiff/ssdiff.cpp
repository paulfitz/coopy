
#include <stdio.h>
#include <coopy/Diff.h>

using namespace coopy::store;
using namespace coopy::cmp;
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

  if (core.size()<2||help) {
    printf("Show the difference between two tables/databases/spreadsheets. Call as:\n");
    printf("  ssdiff [--output <filename>] local.csv remote.csv\n");
    printf("\n");

    printf("To list supported input formats:\n");
    printf("  ssdiff --list-formats\n");
    printf("\n");

    printf("The difference can be shown in different output formats:\n");
    printf("  ssdiff --format tdiff local.csv modified.csv  # default format\n");
    printf("  ssdiff --format raw local.csv modified.csv    # verbose format for debugging\n");
    printf("  ssdiff --format sql local.csv modified.csv    # SQL format\n");
    printf("  ssdiff --format hilite local.csv modified.csv # colorful format, best used\n");
    printf("                                                # with --output out.xls\n");
    printf("  ssdiff --format review local.csv modified.csv # hilite, with column for\n");
    printf("                                                # selecting/rejecting changes\n");
    printf("  ssdiff --format csv local.csv modified.csv    # CSV-compatible diff format\n");
    printf("  ssdiff --format index local.csv modified.csv  # table showing matching rows\n");
    printf("\n");

    printf("The default matching algorithm is very general.  Generality can be traded for\n");
    printf("speed with the following options:\n");
    printf("  ssdiff --id COL1 --id COL2 local.csv modified.csv # declare primary key\n");
    printf("  ssdiff --bid COL1 --bid COL2 local.csv modified.csv # concentrate comparison\n");
    printf("  ssdiff --named local.csv modified.csv   # trust names of columns\n");
    printf("  ssdiff --unordered local.csv modified.csv   # do not worry about row order\n");
    printf("  ssdiff --fixed-columns local.csv modified.csv   # no column modifications\n");
    printf("\n");

    printf("For multi-table input, you can narrow the diff down using:\n");
    printf("  ssdiff --table TABLE1 --table TABLE2 local.mdb modified.mdb\n");
    printf("\n");

    printf("It is possible to immediately apply a difference as a patch:\n");
    printf("  ssdiff --apply local.xls modified.csv   # modifies local.xls\n");
    return 1;
  }

  Diff diff;
  return diff.apply(opt);
}

