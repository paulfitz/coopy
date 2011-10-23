#ifndef COOPY_SHEETPATCHER
#define COOPY_SHEETPATCHER

#include <coopy/Patcher.h>
#include <coopy/DataSheet.h>
#include <coopy/PolySheet.h>
#include <coopy/TextBook.h>
#include <coopy/CsvSheet.h>
#include <coopy/NameSniffer.h>

#include <vector>

namespace coopy {
  namespace cmp {
    class SheetPatcher;
  }
}

class coopy::cmp::SheetPatcher : public Patcher {
private:
  ConfigChange config;
  coopy::store::CsvSheet activeRow;
  coopy::store::CsvSheet activeCol;
  coopy::store::CsvSheet statusCol;
  std::vector<int> columns;
  std::vector<std::string> column_names;
  std::map<std::string,int> name2col;
  std::map<std::string,std::string> syn2name;
  std::map<std::string,std::string> name2syn;
  int rowCursor;
  bool summary;
  Patcher *chain;
  bool chainOwned;
  int changeCount;
  bool descriptive;
  bool forReview;
  bool declaredNames;
  bool readyForConflicts;
  int conflictColumn;
  int xoff;
  int yoff;
  bool killNeutral;
  bool merging;
  coopy::store::NameSniffer *sniffer;
  coopy::store::DataSheet *sniffedSheet;

  int matchRow(const std::vector<int>& active_cond,
	       const std::vector<coopy::store::SheetCell>& cond,
	       int width,
	       bool show=false);

  int matchCol(const std::string& mover);

  int updateCols();

  bool moveColumn(int idx, int idx2);

  SheetPatcher(bool descriptive = false,
	       bool forReview = false,
	       bool forMerge = false) : descriptive(descriptive),
    forReview(forReview), merging(forMerge)
  {
    rowCursor = -1;
    summary = false;
    chain = 0/*NULL*/;
    chainOwned = false;
    changeCount = 0;
    xoff = 0;
    yoff = 0;
    sniffer = 0/*NULL*/;
    sniffedSheet = 0/*NULL*/;
    killNeutral = false;
    readyForConflicts = false;
    conflictColumn = -1;
    declaredNames = false;
  }

public:
  static SheetPatcher *createForApply() {
    return new SheetPatcher();
  }

  static SheetPatcher *createForMerge() {
    return new SheetPatcher(false,false,true);
  }

  static SheetPatcher *createForDescription() {
    return new SheetPatcher(true);
  }

  static SheetPatcher *createForReview() {
    return new SheetPatcher(true,true);
  }

  virtual ~SheetPatcher() {
    clearNames();
    if (chain!=0/*NULL*/) {
      if (chainOwned) {
	delete chain;
      }
      chain = 0/*NULL*/;
      chainOwned = false;
    }
  }

  void setNames();

  void clearNames() {
    if (sniffer!=0/*NULL*/) {
      delete sniffer;
      sniffer = 0/*NULL*/;
    }
    sniffedSheet = 0/*NULL*/;
  }

  void showSummary(Patcher *chain, bool chainOwned = true) {
    summary = true;
    this->chain = chain;
    this->chainOwned = chainOwned;
  }

  int getChangeCount() {
    return changeCount;
  }

  virtual bool changeName(const NameChange& change) { 
    if (chain) chain->changeName(change);
    return Patcher::changeName(change);
  }

  virtual bool changeConfig(const ConfigChange& change) { 
    if (chain) chain->changeConfig(change);
    this->config = change;
    return true; 
  }
  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);
  virtual bool declareNames(const std::vector<std::string>& names, bool final);

  virtual bool setSheet(const char *name);

  virtual bool mergeStart();

  virtual bool mergeDone();

  virtual bool mergeAllDone();

  virtual void setConflicted() {
    if (chain) chain->setConflicted();
    Patcher::setConflicted();
  }

  virtual bool outputStartsFromInput() {
    return descriptive||merging;
  }
  
  virtual bool needOutputBook() {
    return true;
  }

  bool markChanges(const RowChange& change, int r,int width,
		   std::vector<int>& active_val,
		   std::vector<coopy::store::SheetCell>& val,
		   std::vector<coopy::store::SheetCell>& cval,
		   std::vector<coopy::store::SheetCell>& pval);


  bool handleConflicts();
};

#endif

