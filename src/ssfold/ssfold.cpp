
#include <stdio.h>
#include <getopt.h>

#include <coopy/PolyBook.h>
#include <coopy/Dbg.h>
#include <coopy/FoldTool.h>
#include <coopy/CsvTextBook.h>

using namespace coopy::store;
using namespace coopy::fold;
using namespace std;

int main(int argc, char *argv[]) {
  std::string recipe_file = "";
  std::string table_name = "";
  bool verbose = false;
  bool unfold = false;
  bool fold = false;
  FoldOptions options;

  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"fold", 0, 0, 'f'},
      {"unfold", 0, 0, 'u'},
      {"verbose", 0, 0, 'v'},

      {"recipe", 1, 0, 'r'},
      {"table", 1, 0, 't'},

      {"drop", 1, 0, 'd'},

      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 'v':
      verbose = true;
      break;
    case 'f':
      fold = true;
      break;
    case 'u':
      unfold = true;
      break;

    case 'r':
      recipe_file = optarg;
      break;
    case 't':
      table_name = optarg;
      break;

    case 'd':
      options.drops.insert(optarg);
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
    printf("Fold or flatten tables:\n");
    printf("  ssfold [--table TABLE] [--recipe RECIPE] [--unfold|--fold] SOURCE DESTINATION\n");
    printf("  ssfold --recipe folds.csv --table main --unfold tables.sqlite sheet.csv\n");
    printf("  ssfold --drop length bridges.csv partial.csv  # removes length column\n");
    return 1;
  }

  if (verbose) {
    coopy_set_verbose(verbose);
  }

  if (fold&&unfold) {
    fprintf(stderr,"Cannot both fold and unfold\n");
    return 1;
  }
  fold = !unfold;
  
  PolyBook local;
  PolyBook remote;
  PolyBook recipe;
  
  if (!local.read(argv[0])) {
    fprintf(stderr,"Failed to read %s\n", argv[0]);
    return 1;
  }
  if (recipe_file!="") {
    if (!recipe.read(recipe_file.c_str())) {
      fprintf(stderr,"Failed to read %s\n", recipe_file.c_str());
      return 1;
    }
  }

  dbg_printf("ssfold: %s\n", fold?"folding":"unfolding");
  options.tableName = table_name;
  options.fold = fold;
  options.recipe = recipe;
  FoldTool tool;
  bool ok = tool.fold(local,remote,options);
  if (!ok) {
    fprintf(stderr,"Fold failed - probably because it is not yet implemented\n");
    return 1;
  }
  //if (!CsvTextBook::write(argv[1],&remote,true)) {
  if (!remote.write(argv[1])) {
    fprintf(stderr,"Failed to write %s\n", argv[1]);
    return 1;
  }
  return 0;
}

