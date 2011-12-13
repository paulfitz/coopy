#ifndef CSVRENDER_INC
#define CSVRENDER_INC

#include <coopy/DataSheet.h>

class CsvRender {
private:
  bool full;
  bool decorate;
  bool header;
  bool dox;
public:
  CsvRender() {
    full = false;
    decorate = true;
    dox = false;
  }
  void setDecorate(bool flag) { decorate = flag; }
  void setFull(bool flag) { full = flag; }
  void setHeader(bool flag) { header = flag; }
  void setDox(bool flag) { dox = flag; }
  std::string renderHtml(const coopy::store::DataSheet& sheet,
			 const std::string& title = "");
};

#endif
