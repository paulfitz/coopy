#include <coopy/NameSniffer.h>

using namespace coopy::store;
using namespace std;

std::string NameSniffer::suggestColumnName(int col) {
  char buf[256];
  sprintf(buf,"%d",col);
  return buf;
}
