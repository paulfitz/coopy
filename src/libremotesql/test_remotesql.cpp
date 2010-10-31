#include <coopy/RemoteSqlTextBook.h>
#include <coopy/Property.h>

using namespace coopy::store;
using namespace std;

int main(int argc, char *argv[]) {
  RemoteSqlTextBook book;
  Property p;
  p.put("username","root");
  p.put("password","");
  p.put("lostname","localhost");
  p.put("port",3128);
  p.put("database",(argc>1)?argv[1]:"mysql");
  if (!book.open(p)) {
    printf("Could not open database\n");
    return 1;
  }
  vector<string> names = book.getNames();
  printf("got %d names\n", (int)names.size());
  for (int i=0; i<(int)names.size(); i++) {
    printf(" %d -> %s\n", i, names[i].c_str());
  }
  if (names.size()==0) {
    return 0;
  }
  for (int i=0; i<(int)names.size(); i++) {
    bool force = false;
    if (argc>2) {
      force = true;
      if (names[i]!=argv[2]) continue;
    }
    PolySheet sheet = book.readSheet(names[i]);
    printf("Sheet %s has dimensions %dx%d\n", names[i].c_str(),
	   sheet.width(), sheet.height());
    if (argc==6) {
      int x = atoi(argv[3]);
      int y = atoi(argv[4]);
      const char *str = argv[5];
      printf("Setting %d %d => %s\n", x, y, str);
      sheet.cellString(x,y,str);
    }
    if (force || (sheet.height()>0 && sheet.height()<20)) {
      for (int y=0; y<sheet.height(); y++) {
	printf("%d: ", y);
	for (int x=0; x<sheet.width(); x++) {
	  printf(" [%s]", sheet.cellString(x,y).c_str());
	}
	printf("\n");
      }
    }
  }
  return 0;
}

