
#include <stdio.h>
#include <getopt.h>

#include <coopy/CsvFile.h>
#include <coopy/MergeOutputPatch.h>
#include <coopy/MergeOutputSqlDiff.h>
#include <coopy/MergeOutputHumanDiff.h>
#include <coopy/MergeOutputVerboseDiff.h>
#include <coopy/MergeOutputCsvDiff.h>
#include <coopy/MergeOutputTdiff.h>
#include <coopy/MergeOutputIndex.h>
#include <coopy/BookCompare.h>
#include <coopy/PolyBook.h>
#include <coopy/SheetPatcher.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace std;

static void start_output(string output, CompareFlags& flags) {
  if (output=="" || output=="-") {
    flags.out = stdout;
    return;
  }
  FILE *fout = fopen(output.c_str(),"wb");
  if (fout==NULL) {
    fprintf(stderr,"Could not open %s for writing\n", output.c_str());
    exit(1);
  }
  flags.out = fout;
}

static void stop_output(string output, CompareFlags& flags) {
  if (flags.out!=stdout) {
    fclose(flags.out);
    flags.out = stdout;
  }
}

Patcher *createTool(string mode, string version="") {
  Patcher *result = NULL;
  if (mode=="sql") {
    result = new MergeOutputSqlDiff;
  } else if (mode=="human") {
    result = new MergeOutputHumanDiff;
  } else if (mode=="raw") {
    result = new MergeOutputVerboseDiff;
  } else if (mode=="tdiff") {
    result = new MergeOutputTdiff;
  } else if (mode=="csv") {
    if (version=="0.2") {
      result = new MergeOutputCsvDiffV0p2;
    } else if (version=="0.4"||version=="0.5"||version=="") {
      result = new MergeOutputCsvDiff;
    }
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::string output = "-";
  std::string mode = "csv";
  std::string parent_file = "";
  std::string version = "";
  std::string map_file = "";
  std::vector<std::string> ids;
  bool verbose = false;
  bool equality = false;
  bool indexed = false;
  bool apply = false;

  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"format-csv", 0, 0, 'c'},
      {"format-csv-old", 0, 0, '0'},
      {"format-sql", 0, 0, 's'},
      {"format-human", 0, 0, 'h'},
      {"format-raw", 0, 0, 'r'},
      {"format-tdiff", 0, 0, 't'},
      {"format-index", 0, 0, 'i'},

      {"apply", 0, 0, 'a'},

      {"equals", 0, 0, 'e'},
      {"index", 0, 0, 'i'},
      {"verbose", 0, 0, 'v'},

      {"id", 1, 0, 'k'},

      {"map", 1, 0, 'm'},

      {"output", 1, 0, 'o'},
      {"version", 1, 0, 'V'},
      {"parent", 1, 0, 'p'},
      {"list-formats", 0, 0, 'l'},
      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 'c':
      mode = "csv";
      break;
    case '0':
      mode = "csv0";
      break;
    case 's':
      mode = "sql";
      break;
    case 'h':
      mode = "human";
      break;
    case 't':
      mode = "tdiff";
      break;
    case 'r':
      mode = "raw";
      break;
    case 'v':
      verbose = true;
      break;
    case 'e':
      equality = true;
      break;
    case 'i':
      indexed = true;
      break;

    case 'k':
      ids.push_back(optarg);
      break;

    case 'm':
      map_file = optarg;
      break;

    case 'o':
      output = optarg;
      break;
    case 'p':
      parent_file = optarg;
      break;
    case 'V':
      version = optarg;
      break;
    case 'l':
      PolyBook::showFormats();
      return 0;
      break;
    case 'a':
      apply = true;
      mode = "tdiff";
      break;
    default:
      fprintf(stderr, "Unrecognized option\n");
      return 1;
    }
  }

  if (optind<argc-3) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }
  argc -= optind;
  argv += optind;

  if (argc<2) {
    printf("Show the difference between two tables. Call as:\n");
    printf("  ssdiff [--output <filename>] local.csv modified.csv\n");
    printf("For differences in a classic-diff-like format, do:\n");
    printf("  ssdiff --format-tdiff local.csv modified.csv\n");
    printf("For differences in a verbose format, do:\n");
    printf("  ssdiff --format-human local.csv modified.csv\n");
    printf("To extract the mapping from local to modified in tabular form, do:\n");
    printf("  ssdiff --index local.csv modified.csv\n");
    printf("Output defaults to standard output.  To list supported formats:\n");
    printf("  ssdiff --list-formats\n");
    printf("It is possible to immediately apply a difference to the local file:\n");
    printf("  ssdiff --apply local.xls modified.csv\n");
    printf("This will modify local.xls to bring it into alignment with modified.csv\n");
    printf("Formatting information in local.xls will be left untouched.\n");
    return 1;
  }

  CompareFlags flags;
  BookCompare cmp;
  cmp.setVerbose(verbose);

  PolyBook _pivot;
  PolyBook *pivot;
  PolyBook _local;
  PolyBook *local = &_local;
  PolyBook _remote;
  PolyBook *remote = &_remote;
  
  if (!_local.read(argv[0])) {
    fprintf(stderr,"Failed to read %s\n", argv[0]);
    return 1;
  }
  if (!_remote.read(argv[1])) {
    fprintf(stderr,"Failed to read %s\n", argv[1]);
    return 1;
  }
  if (parent_file!="") {
    if (!_pivot.read(parent_file.c_str())) {
      fprintf(stderr,"Failed to read %s\n", parent_file.c_str());
      return 1;
    }
    pivot = &_pivot;
  } else {
    pivot = &_local;
    flags.pivot_sides_with_local = true;
  }

  if (equality) {
    if (*local == *remote) {
      return 0;
    }
    return 1;
  }

  PolyBook mapping;
  if (map_file!="") {
    if (!mapping.read(map_file.c_str())) {
	fprintf(stderr,"Failed to read %s\n", map_file.c_str());
	return 1;
      }
      flags.mapping_book = &mapping;
  }

  if (ids.size()>0) {
    flags.ids = ids;
    flags.trust_ids = true;
  }
  if (indexed) {
    MergeOutputIndex diff;
    PolyBook book;
    book.attach(output.c_str());
    diff.attachBook(book);
    cmp.compare(*pivot,*local,*remote,diff,flags);
    book.flush();
  } else if (!apply) {
    Patcher *diff = createTool(mode,version);
    if (diff!=NULL) {
      start_output(output,flags);
      cmp.compare(*pivot,*local,*remote,*diff,flags);
      stop_output(output,flags);
      delete diff;
      diff = NULL;
    } else if (mode=="csv0") {
      MergeOutputPatch patch;
      cmp.compare(*pivot,*local,*remote,patch,flags);
      const CsvSheet& result = patch.get();
      if (output!="") {
	if (CsvFile::write(result,output.c_str())!=0) {
	  return 1;
	}
      } else {
	SheetStyle style;
	std::string out = result.encode(style);
	printf("%s",out.c_str());
      }
    } else {
      fprintf(stderr,"Patch mode not recognized\n");
      return 1;
    }
  } else {
    Patcher *pdiff = createTool(mode,version);
    SheetPatcher diff(local);
    diff.showSummary(pdiff);
    start_output(output,flags);
    cmp.compare(*pivot,*local,*remote,diff,flags);
    stop_output(output,flags);
    if (pdiff!=NULL) {
      delete pdiff;
      pdiff = NULL;
    }
    if (diff.getChangeCount()>0) {
      if (!local->inplace()) {
	if (!local->write(argv[0])) {
	  fprintf(stderr,"Failed to write %s\n", argv[0]);
	  return 1;
	}
      }
    }
  }
  return 0;
}

