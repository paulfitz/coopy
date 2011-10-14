
#include <stdio.h>
#include <getopt.h>

#include <coopy/CsvFile.h>
#include <coopy/BookCompare.h>
#include <coopy/MergeOutputAccum.h>
#include <coopy/MergeOutputIndex.h>
#include <coopy/PolyBook.h>
#include <coopy/Dbg.h>
#include <coopy/Options.h>
#include <coopy/Diff.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;
using namespace coopy::app;

int main(int argc, char *argv[]) {
  Options opt("ssmerge");
  int r = opt.apply(argc,argv);
  if (r!=0) return r;

  std::string output = opt.checkString("output");
  bool verbose = opt.checkBool("verbose");
  bool help = opt.checkBool("help");
  bool indexed = opt.checkString("mode","merge")=="index";
  bool merged = opt.checkString("mode","merge")=="merge";

  CompareFlags flags = opt.getCompareFlags();

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

  /*
  coopy_set_verbose(verbose);
  PolyBook local, remote, parent;
  if (!parent.read(core[0].c_str())) {
    fprintf(stderr,"Failed to read %s\n", core[0].c_str());
    return 1;
  }
  if (!local.read(core[1].c_str())) {
    fprintf(stderr,"Failed to read %s\n", core[1].c_str());
    return 1;
  }
  if (!remote.read(core[2].c_str())) {
    fprintf(stderr,"Failed to read %s\n", core[2].c_str());
    return 1;
  }
  BookCompare cmp;
  cmp.setVerbose(verbose);

  if (!indexed) {
    MergeOutputAccum accum;
    cmp.compare(parent,local,remote,accum,flags);
    if (CsvFile::write(accum.getSheet(),output.c_str())!=0) {
      return 1;
    }
  } else {
    MergeOutputIndex accum;
    PolyBook book;
    book.attach(output.c_str());
    accum.attachBook(book);
    cmp.compare(parent,local,remote,accum,flags);
    book.flush();
  }
  return 0;
  */
}

