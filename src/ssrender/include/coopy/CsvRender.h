#ifndef CSVRENDER_INC
#define CSVRENDER_INC

#include <coopy/CsvSheet.h>

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
  std::string renderHtml(const CsvSheet& sheet);
};

#endif
