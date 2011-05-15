
#include <stdio.h>
#include <getopt.h>

#include <coopy/PolyBook.h>
#include <coopy/CsvFile.h>
#include <coopy/CsvTextBook.h>
#include <coopy/PatchParser.h>
#include <coopy/SheetPatcher.h>
#include <coopy/MergeOutputVerboseDiff.h>
#include <coopy/Dbg.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace coopy::format;

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

bool copy_file(const char *src, const char *dest) {
  return Patcher::copyFile(src,dest);
}

int main(int argc, char *argv[]) {
  bool verbose = false;
  bool inplace = false;
  bool help = false;
  string output = "-";
  string formatName = "apply";
  string tmp = "-";
  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"verbose", 0, 0, 'v'},
      {"output", 1, 0, 'o'},
      {"fake", 0, 0, 'k'},
      {"help", 0, 0, 'h'},
      {"inplace", 0, 0, 'i'},
      {"tmp", 1, 0, 't'},
      {"format", 1, 0, 'f'},
      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 'v':
      verbose = true;
      break;
    case 'i':
      inplace = true;
      break;
    case 'k':
      formatName = "raw";
      break;
    case 'h':
      help = true;
      break;
    case 'o':
      output = optarg;
      break;
    case 't':
      tmp = optarg;
      break;
    case 'f':
      formatName = optarg;
      break;
    default:
      fprintf(stderr, "Unrecognized option\n");
      return 1;
    }
  }

  if (optind<argc-2) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }
  argc -= optind;
  argv += optind;

  const char *local_name = "";
  const char *patch_name = "";

  if (argc==1&&(formatName!="apply"&&formatName!="color")) {
    patch_name = argv[0];
  } else if (argc<2 || help) {
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
    printf("  sspatch --format csv patch.txt    # render patch in csv format\n");
    printf("  sspatch --format hilite local.csv patch.txt # render patch in highlighter\n");
    printf("                                    # format (best used with --output out.xls)\n");
    return 1;
  } else {
    local_name = argv[0];
    patch_name = argv[1];
  }

  if (inplace) {
    output = local_name;
  }

  if (verbose) {
    coopy_set_verbose(verbose);
  }

  bool willMod = (formatName=="apply");

  Patcher *alt = alt = Patcher::createByName(formatName.c_str());
  if (alt==NULL) {
    fprintf(stderr,"Failed to create patch tool of type '%s'\n",
	    formatName.c_str());
    return 1;
  }

  PolyBook local;
  CsvSheet patch;
  if (!willMod&&!alt->outputStartsFromInput()) {
    local_name = "";
  } else {
    if (!local.read(local_name)) {
      fprintf(stderr,"Failed to read %s\n", local_name);
      delete alt; alt = NULL;
      return 1;
    }
  }

  if (willMod&&local.inplace()&&output!=local_name) {
    if (output=="-"&&tmp=="-") {
      fprintf(stderr,"Inplace operation; please confirm output or specify a 'tmp' location\n");
      delete alt; alt = NULL;
      return 1;
    }
    if (tmp=="-") {
      tmp = output;
    }
    if (!copy_file(local_name,tmp.c_str())) {
      fprintf(stderr,"Failed to write %s\n", output.c_str());
      delete alt; alt = NULL;
      return 1;
    }
    if (!local.read(tmp.c_str())) {
      fprintf(stderr,"Failed to switch to %s\n", output.c_str());
      delete alt; alt = NULL;
      return 1;
    }
  }
  

  //PolySheet sheet = local.readSheetByIndex(0);
  /*
  if (!sheet.isValid()) {
    fprintf(stderr, "No sheet found\n");
    return 1;
  }
  */

  PolyBook obook;
  if (alt->needOutputBook()) {
    if (!obook.attach(output.c_str())) {
      delete alt; alt = NULL;
      return 1;
    }
    alt->attachOutputBook(obook);
  }
  PolyBook tbook;
  if (alt->outputStartsFromInput()) {
    if (output!="-") {
      if (!local.write(output.c_str())) {
	delete alt; alt = NULL;
	return 1;
      }
      if (!tbook.read(output.c_str())) {
	fprintf(stderr,"Failed to read %s\n", output.c_str());
	return 1;
      }
    } else {
      tbook.take(new CsvTextBook(true));
      Property p;
      tbook.copy(local,p);
    }
    alt->attachBook(tbook);
  } else {
    alt->attachBook(local);
  }

  MergeOutputVerboseDiff fakePatcher;
  PatchParser parser(alt,patch_name);
  CompareFlags flags;
  alt->startOutput(output,flags);
  alt->setFlags(flags);
  bool ok = parser.apply();
  alt->stopOutput(output,flags);
  if (!ok) {
    fprintf(stderr,"Patch operation failed\n");
    delete alt; alt = NULL;
    return 1;
  }

  if (alt->needOutputBook()) {
    obook.flush();
  }
  if (alt->outputStartsFromInput()) {
    if (!tbook.write(output.c_str())) {
      fprintf(stderr,"Failed to write %s\n", output.c_str());
      delete alt; alt = NULL;
      return 1;
    }
  }  
  if (willMod) {
    if ((!local.inplace())||(tmp!=output)) {
      if (output!=local_name || !local.inplace()) {
	if (!local.write(output.c_str())) {
	  fprintf(stderr,"Failed to write %s\n", output.c_str());
	  delete alt; alt = NULL;
	  return 1;
	}
      }
    }
  }
  delete alt; alt = NULL;
  return 0;
}

