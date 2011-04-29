#ifndef CSVRENDER_INC
#define CSVRENDER_INC

#include <coopy/DataSheet.h>

class CsvRender {
private:
  bool full;
  bool decorate;
public:
  CsvRender() {
    full = false;
    decorate = true;
  }
  void setDecorate(bool flag) { decorate = flag; }
  void setFull(bool flag) { full = flag; }
  std::string renderHtml(const coopy::store::DataSheet& sheet);
};

#endif
