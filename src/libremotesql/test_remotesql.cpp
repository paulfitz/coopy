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
  p.put("database","mysql");
  if (!book.read(p)) {
    printf("Could not open database\n");
    return 1;
  }
  vector<string> names = book.getNames();
  printf("got %d names\n", (int)names.size());
  for (int i=0; i<(int)names.size(); i++) {
    printf(" %d -> %s\n", i, names[i].c_str());
  }
  return 0;
}

