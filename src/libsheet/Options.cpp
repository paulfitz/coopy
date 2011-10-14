#include <stdio.h>
#include <getopt.h>

#include <coopy/Options.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;
using namespace coopy::app;

int Options::apply(int argc, char *argv[]) {
  core.clear();
  flags = CompareFlags();
  option_bool.clear();
  mapping.clear();

  option_string["output"] = "-";
  option_string["parent"] = "";
  option_string["version"] = "";

  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"format-csv", 0, 0, 'c'},
      {"format-sql", 0, 0, 's'},
      {"format-raw", 0, 0, 'r'},
      {"format-tdiff", 0, 0, 't'},
      {"format-index", 0, 0, 'i'},

      {"format", 1, 0, 'f'},

      {"apply", 0, 0, 'a'},

      {"equals", 0, 0, 'e'},
      {"index", 0, 0, 'i'},
      {"verbose", 0, 0, 'v'},

      {"id", 1, 0, 'k'},
      {"bid", 1, 0, 'b'},

      {"named", 0, 0, 'd'},

      {"fixed-columns", 0, 0, 'F'},
      {"ignore-case", 0, 0, 'A'},

      {"ordered", 0, 0, '1'},
      {"unordered", 0, 0, '0'},

      {"map", 1, 0, 'm'},

      {"omit-format-name", 0, 0, 'O'},
      {"omit-sheet-name", 0, 0, 'P'},

      {"table", 1, 0, 'T'},

      {"output", 1, 0, 'o'},
      {"version", 1, 0, 'V'},
      {"parent", 1, 0, 'p'},
      {"list-formats", 0, 0, 'l'},
      {"help", 0, 0, 'h'},

      {"head-trimmed", 0, 0, 0},
      {"tail-trimmed", 0, 0, 0},
      {"fake", 0, 0, 0},
      {"inplace", 0, 0, 0},
      {"tmp", 1, 0, 0},
      {"patch", 1, 0, 0},

      {"cmd", 1, 0, 'x'},

      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 0:
      {
	string k = long_options[option_index].name;
	if (k=="head-trimmed") {
	  flags.head_trimmed = true;
	} else if (k=="tail-trimmed") {
	  flags.tail_trimmed = true;
	} else if (k=="fake") {
	  option_string["mode"] = "raw";
	} else if (k=="inplace") {
	  option_bool["inplace"] = true;
	} else if (k=="tmp") {
	  option_string["tmp"] = optarg;
	} else if (k=="patch") {
	  option_string["patch"] = optarg;
	} else {
	  fprintf(stderr,"Unknown option %s\n", k.c_str());
	  return 1;
	}
      }
      break;
    case 'c':
      option_string["mode"] = "csv";
      break;
    case 's':
      option_string["mode"] = "sql";
      break;
    case 'h':
      option_bool["help"] = true;
      break;
    case 't':
      option_string["mode"] = "tdiff";
      break;
    case 'r':
      option_string["mode"] = "raw";
      break;
    case 'v':
      option_bool["verbose"] = true;
      break;
    case 'e':
      option_bool["equality"] = true;
      break;
    case 'i':
      option_string["mode"] = "index";
      break;
    case 'd':
      flags.trust_column_names = true;
      break;
    case '0':
      flags.use_order = false;
      break;
    case '1':
      flags.use_order = true;
      break;

    case 'k':
      flags.ids.push_back(optarg);
      flags.trust_ids = true;
      flags.bias_ids = false;
      break;

    case 'b':
      flags.ids.push_back(optarg);
      flags.trust_ids = false;
      flags.bias_ids = true;
      break;

    case 'm':
      {
	if (!mapping.read(optarg)) {
	  fprintf(stderr,"Failed to read %s\n", optarg);
	  return 1;
	}
	flags.mapping_book = &mapping;
      }
      break;

    case 'O':
      flags.omit_format_name = true;
      break;
    case 'P':
      flags.omit_sheet_name = true;
      break;

    case 'f':
      option_string["mode"] = optarg;
      break;
    case 'o':
      option_string["output"] = optarg;
      break;
    case 'p':
      option_string["parent"] = optarg;
      break;
    case 'V':
      option_string["version"] = optarg;
      break;
    case 'x':
      option_string["cmd"] = optarg;
      break;

    case 'l':
      PolyBook::showFormats();
      return 0;
      break;
    case 'a':
      option_bool["apply"] = true;
      break;

    case 'F':
      flags.fixed_columns = true;
      break;

    case 'A':
      flags.ignore_case = true;
      break;

    case 'T':
      flags.tables.insert(optarg);
      break;

    default:
      fprintf(stderr, "Unrecognized option\n");
      return 1;
    }
  }

  argc -= optind;
  argv += optind;

  for (int i=0; i<argc; i++) {
    core.push_back(argv[i]);
  }

  return 0;
}


