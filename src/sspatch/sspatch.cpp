
#include <stdio.h>
#include <getopt.h>

#include <coopy/PolyBook.h>
#include <coopy/CsvFile.h>
#include <coopy/CsvPatch.h>
#include <coopy/FormatSniffer.h>
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
  FILE *fin = NULL;
  FILE *fout = NULL;
  fin = fopen(src,"rb");
  if (fin==NULL) return false;
  fout = fopen(dest,"wb");
  if (fout==NULL) {
    fclose(fin);
    return false;
  }
  char buf[32768];
  int bytes_read = 0;
  while ((bytes_read=fread(buf,1,sizeof(buf),fin))>0) {
    fwrite(buf,1,bytes_read,fout);
  }
  fclose(fout);
  fclose(fin);
  return true;
}

int main(int argc, char *argv[]) {
  bool verbose = false;
  bool fake = false;
  bool inplace = false;
  string output = "-";
  string formatName = "apply";
  string tmp = "-";
  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"verbose", 0, 0, 'v'},
      {"output", 1, 0, 'o'},
      {"fake", 0, 0, 'k'},
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
      fake = true;
      formatName = "raw";
      break;
    case 'o':
      output = optarg;
      break;
    case 't':
      tmp = optarg;
      break;
    case 'f':
      fake = true;
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

  if (argc==1&&fake) {
    patch_name = argv[0];
  } else if (argc<2) {
    printf("Apply patch to a table.\n");
    printf("  sspatch [--verbose] [--output output.csv] sheet.csv patch.txt\n");
    printf("  sspatch [--output output.sqlite] [--tmp tmp.sqlite] db.sqlite patch.txt\n");
    printf("  sspatch [--inplace] db.sqlite patch.txt\n");
    printf("  sspatch [--format FORMAT] patch.txt\n");
    printf("Output defaults to standard output.\n");
    printf("Make changes in original file by doing:\n");
    printf("  sspatch --inplace sheet.sqlite patch.txt\n");
    printf("or:\n");
    printf("  sspatch --output sheet.csv sheet.csv patch.txt\n");
    printf("Get debug info about what a patch would do:\n");
    printf("  sspatch --format raw patch.txt\n");
    printf("Convert a CSV format diff to a TDIFF format diff:\n");
    printf("  sspatch --format tdiff patch.csv\n");
    printf("Convert a TDIFF format diff to a CSV format diff:\n");
    printf("  sspatch --format csv patch.txt\n");
    printf("Sanitize a TDIFF format diff by regenerating it:\n");
    printf("  sspatch --format tdiff patch.txt\n");
    printf("Read from standard input:\n");
    printf("  sspatch local.csv -\n");
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

  fake = (formatName!="apply");

  PolyBook local;
  CsvSheet patch;
  if (fake) {
    local_name = "";
  } else {
    if (!local.read(local_name)) {
      fprintf(stderr,"Failed to read %s\n", local_name);
      return 1;
    }
  }

  if (local.inplace()&&output!=local_name) {
    if (output=="-"&&tmp=="-") {
      fprintf(stderr,"Inplace operation; please confirm output or specify a 'tmp' location\n");
      return 1;
    }
    if (tmp=="-") {
      tmp = output;
    }
    if (!copy_file(local_name,tmp.c_str())) {
      fprintf(stderr,"Failed to write %s\n", output.c_str());
      return 1;
    }
    if (!local.read(tmp.c_str())) {
      fprintf(stderr,"Failed to switch to %s\n", output.c_str());
      return 1;
    }
  }
  
  FormatSniffer sniffer;
  sniffer.open(patch_name);
  Format format = sniffer.getFormat();

  if (format.id!=FORMAT_PATCH_CSV && format.id!=FORMAT_PATCH_TDIFF) {
    fprintf(stderr,"Only DTBL CSV format and TDIFF format patches are supported\n");
    return 1;
  }

  PolySheet sheet = local.readSheetByIndex(0);
  /*
  if (!sheet.isValid()) {
    fprintf(stderr, "No sheet found\n");
    return 1;
  }
  */

  Patcher *alt = alt = Patcher::createByName(formatName.c_str());
  if (alt==NULL) {
    fprintf(stderr,"Failed to create patch tool of type '%s'\n",
	    formatName.c_str());
    return 1;
  }
  alt->attachSheet(sheet);
  alt->attachBook(local);
  MergeOutputVerboseDiff fakePatcher;
  PatchParser parser(alt,&sniffer);
  CompareFlags flags;
  if (fake) {
    start_output(output,flags);
    alt->setFlags(flags);
  }
  bool ok = parser.apply();
  if (fake) {
    stop_output(output,flags);
  }
  if (!ok) {
    fprintf(stderr,"Patch application failed\n");
    return 1;
  }

  if (fake) {
    return 0;
  }
  
  //if (CsvFile::write(local,output.c_str())!=0) {
  if ((!local.inplace())||(tmp!=output)) {
    if (output!=local_name || !local.inplace()) {
      if (!local.write(output.c_str())) {
	fprintf(stderr,"Failed to write %s\n", output.c_str());
	return 1;
      }
    }
  }
  return 0;
}

