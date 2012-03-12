#ifndef COOPY_MERGEOUTPUTNOVEL
#define COOPY_MERGEOUTPUTNOVEL

#include <coopy/MergeOutput.h>

#include <map>

namespace coopy {
  namespace cmp {
    class MergeOutputNovel;
  }
}


class coopy::cmp::MergeOutputNovel : public Patcher {
private:
  std::string sheet_name;
  std::map<std::string,int> noted;
public:
  MergeOutputNovel() {
    sheet_name = coopy_get_default_table_name();
  }
  
  virtual bool wantLinks() { 
    return true;
  }

  virtual bool setSheet(const char *name) {
    sheet_name = name;
    return true;
  }

  virtual bool declareLink(const LinkDeclare& decl);

  virtual bool outputStartsFromInput() {
    return true;
  }
  
  virtual bool needOutputBook() {
    return true;
  }

};


#endif
