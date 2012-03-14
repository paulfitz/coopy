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
  int yoffset;
  std::map<int,int> col_map;
  std::map<int,std::string> remote_name;
  std::map<std::string,int> noted;
  std::string base_col_name;
public:
  MergeOutputNovel() {
    sheet_name = coopy_get_default_table_name();
    yoffset = 0;
    base_col_name = "_ROW_";
  }
  
  virtual bool wantLinks() { 
    return true;
  }

  virtual bool setSheet(const char *name) {
    sheet_name = name;
    col_map.clear();
    remote_name.clear();
    return true;
  }

  virtual bool declareLink(const LinkDeclare& decl);

  virtual bool outputStartsFromInput() {
    return true;
  }
  
  virtual bool needOutputBook() {
    return true;
  }

  bool formatSheet();
};


#endif
