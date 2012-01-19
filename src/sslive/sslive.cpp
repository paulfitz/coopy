
#include <stdio.h>
#include <getopt.h>

#include <coopy/PolyBook.h>
#include <coopy/CsvFile.h>
#include <coopy/CsvTextBook.h>
#include <coopy/PatchParser.h>
#include <coopy/SheetPatcher.h>
#include <coopy/Dbg.h>

#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Network.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace coopy::format;

using namespace yarp::os;

using namespace std;

int main(int argc, char *argv[]) {
  bool verbose = false;
  bool help = false;
  string formatName = "apply";
  string cmd = "";
  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"verbose", 0, 0, 'v'},
      {"help", 0, 0, 'h'},
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
    case 'h':
      help = true;
      break;
    case 'f':
      formatName = optarg;
      break;
    default:
      fprintf(stderr, "Unrecognized option\n");
      return 1;
    }
  }

  if (optind<argc-1) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }
  argc -= optind;
  argv += optind;
  if (argc<1 || help) {
    printf("Welcome to sslive!\n");
    printf("It serves to useful purpose just yet.\n");
    printf("It may grow to support a rsync-like protocol.\n");
    return 1;
  }

  const char *local_name = argv[0];

  PolyBook obook;
  if (!obook.attach(local_name)) {
    return 1;
  }

  Network yarp;
  Port p;
  p.open("/sslive");
  while (true) {
    Bottle b;
    if (!p.read(b,true)) break;
    printf("Got %s\n", b.toString().c_str());
    Bottle reply;
    ConstString cmd = b.get(0).asString();
    if (cmd=="get") {
      PolySheet sheet = obook.readSheetByIndex(0);
      if (sheet.isValid()) {
	int x = b.get(1).asInt();
	int y = b.get(2).asInt();
	if (x<0 || 
	    y<0 ||
	    x >= sheet.width() || 
	    y>=sheet.height()) {
	    reply.addString("None");
	  } else {
	    SheetCell result = sheet.cellSummary(x,y);
	    reply.addString(result.toString().c_str());
	}
      } else {
	reply.addString("None");
      }
    } else if (cmd=="SELECT"||cmd=="Select"||cmd=="select") {
      reply.addString("No SQL - if you want SQL, use an SQL database");
    }

    printf("Reply %s\n", reply.toString().c_str());
    p.reply(reply);
  }
  
  return 0;
}

