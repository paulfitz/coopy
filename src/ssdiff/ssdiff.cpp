
#include <stdio.h>
#include <getopt.h>

#include <coopy/CsvFile.h>
#include <coopy/CsvTextBook.h>
#include <coopy/MergeOutputIndex.h>
#include <coopy/BookCompare.h>
#include <coopy/PolyBook.h>
#include <coopy/SheetPatcher.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace std;

Patcher *createTool(string mode, string version="") {
  return Patcher::createByName(mode.c_str(),version.c_str());
}

int main(int argc, char *argv[]) {
  std::string output = "-";
  std::string mode = "tdiff";
  std::string parent_file = "";
  std::string version = "";
  std::string map_file = "";

  std::vector<std::string> ids;
  bool verbose = false;
  bool equality = false;
  bool apply = false;
  bool named = false;

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
      {"format", 1, 0, 'f'},

      {"apply", 0, 0, 'a'},

      {"equals", 0, 0, 'e'},
      {"index", 0, 0, 'i'},
      {"verbose", 0, 0, 'v'},

      {"id", 1, 0, 'k'},

      {"named", 0, 0, 'd'},

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
      mode = "index";
      break;
    case 'd':
      named = true;
      break;

    case 'k':
      ids.push_back(optarg);
      break;


    case 'm':
      map_file = optarg;
      break;

    case 'f':
      mode = optarg;
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
    printf("For differences in a CSV-compatible format, do:\n");
    printf("  ssdiff --format csv local.csv modified.csv\n");
    printf("For differences in a classic-diff-like format (now the default), do:\n");
    printf("  ssdiff --format tdiff local.csv modified.csv\n");
    printf("For differences in a verbose format, do:\n");
    printf("  ssdiff --format raw local.csv modified.csv\n");
    printf("For differences in an SQL format, do:\n");
    printf("  ssdiff --format sql local.csv modified.csv\n");
    printf("To declare a key that should be trusted, do:\n");
    printf("  ssdiff --id COLUMN1 --id COLUMN2 local.csv modified.csv\n");
    printf("To declare that columns are named, and those names can be trusted:\n");
    printf("  ssdiff --named local.csv modified.csv\n");
    printf("To extract the mapping from local to modified in tabular form, do:\n");
    printf("  ssdiff --index local.csv modified.csv\n");
    printf("Output defaults to standard output.\n");
    printf("To list supported *input* formats:\n");
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
  flags.trust_column_names = named;

  Patcher *diff = createTool(mode,version);
  if (diff==NULL) {
    fprintf(stderr,"Patch mode not recognized\n");
    return 1;
  }
  PolyBook obook;
  if (diff->needOutputBook()) {
    if (!obook.attach(output.c_str())) {
      delete diff; diff = NULL;
      return 1;
    }
    diff->attachOutputBook(obook);
  }
  PolyBook tbook;
  if (diff->outputStartsFromInput()) {
    if (output!="-") {
      if (!_local.write(output.c_str())) {
	delete diff; diff = NULL;
	return 1;
      }
      if (!_local.read(argv[0])) {
	fprintf(stderr,"Failed to read %s\n", argv[0]);
	return 1;
      }
      if (!tbook.read(output.c_str())) {
	fprintf(stderr,"Failed to read %s\n", output.c_str());
	return 1;
      }
    } else {
      tbook.take(new CsvTextBook(true));
      Property p;
      tbook.copy(_local,p);
    }
    diff->attachBook(tbook);
  } else {
    diff->attachBook(*local);
  }

  if (apply) {
    SheetPatcher *apply_diff = new SheetPatcher(false);
    COOPY_ASSERT(apply_diff!=NULL);
    apply_diff->showSummary(diff);
    diff = apply_diff;
    diff->attachBook(*local);
  }
  if (!diff->startOutput(output,flags)) {
    fprintf(stderr,"Patch output failed\n");
    delete diff;
    diff = NULL;
    return 1;
  }
  cmp.compare(*pivot,*local,*remote,*diff,flags);
  diff->stopOutput(output,flags);
  if (diff->needOutputBook()) {
    obook.flush();
  }
  if (diff->outputStartsFromInput()) {
    if (!tbook.write(output.c_str())) {
      fprintf(stderr,"Failed to write %s\n", output.c_str());
      return 1;
    }
  }
  if (apply) {
    if (diff->getChangeCount()>0) {
      if (!local->inplace()) {
	if (!local->write(argv[0])) {
	  fprintf(stderr,"Failed to write %s\n", argv[0]);
	  return 1;
	}
      }
    }
  }
  delete diff;
  diff = NULL;

  return 0;
}

