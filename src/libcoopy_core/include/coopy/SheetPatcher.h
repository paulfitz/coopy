#ifndef COOPY_SHEETPATCHER
#define COOPY_SHEETPATCHER

#include <coopy/Patcher.h>
#include <coopy/DataSheet.h>
#include <coopy/PolySheet.h>
#include <coopy/TextBook.h>
#include <coopy/CsvSheet.h>
#include <coopy/NameSniffer.h>
#include <coopy/MergeOutputFilter.h>

#include <vector>
#include <list>

namespace coopy {
  namespace cmp {
    class SheetPatcher;
  }
}

class coopy::cmp::SheetPatcher : public Patcher {
private:
  //coopy::store::PolySheet active_sheet;
  ConfigChange config;
  coopy::store::CsvSheet activeRow;
  coopy::store::CsvSheet activeCol;
  coopy::store::CsvSheet statusCol;
  std::vector<int> columns;
  std::vector<std::string> column_names;
  std::map<std::string,int> name2col;
  std::map<int,std::string> col2name;
  std::map<std::string,int> original_index;
  std::map<std::string,coopy::store::PoolColumnLink> name2pool;
  std::map<std::string,std::string> syn2name;
  std::map<std::string,std::string> name2syn;
  std::list<RowUnit> deferred_rows;
  int rowCursor;
  bool summary;
  Patcher *chain;
  bool chainOwned;
  int changeCount;
  bool descriptive;
  bool forReview;
  bool declaredNames;
  bool readyForConflicts;
  bool checkedHeader;
  int conflictColumn;
  int xoff;
  bool killNeutral;
  bool merging;
  bool sheetChange;
  std::string sheetName;
  bool sheetUpdateNeeded;
  coopy::store::NameSniffer *sniffer;
  coopy::store::DataSheet *sniffedSheet;

  int matchRow(const std::vector<int>& active_cond,
	       const std::vector<std::string>& active_name,
	       const std::vector<coopy::store::SheetCell>& cond,
	       int width,
	       bool show=false);

  void checkHeader();

  int matchCol(const std::string& mover);

  int updateCols();

  void updatePool();

  bool moveColumn(int idx, int idx2);
  
  bool renameColumn(int idx, const std::string& name, 
		    const std::string& oldName);

public:
  SheetPatcher(bool descriptive = false,
	       bool forReview = false,
	       bool forMerge = false) : descriptive(descriptive),
    forReview(forReview), merging(forMerge)
  {
    sheetUpdateNeeded = false;
    rowCursor = -1;
    summary = false;
    chain = 0/*NULL*/;
    chainOwned = false;
    changeCount = 0;
    xoff = 0;
    sniffer = 0/*NULL*/;
    sniffedSheet = 0/*NULL*/;
    killNeutral = false;
    readyForConflicts = false;
    conflictColumn = -1;
    declaredNames = false;
    checkedHeader = false;
  }

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

  virtual bool setNames(bool forceSheetChange = false);

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

  virtual bool changeName(const NameChange& change);

  virtual bool changeConfig(const ConfigChange& change) { 
    if (chain) chain->changeConfig(change);
    this->config = change;
    return true; 
  }

  virtual bool setFlags(const CompareFlags& flags) {
    if (chain) chain->setFlags(flags);
    return Patcher::setFlags(flags);
  }

  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);
  virtual bool declareNames(const std::vector<std::string>& names, bool final);

  virtual bool changePool(const PoolChange& change) {
    if (chain) chain->changePool(change);
    bool ok = applyPool(change);
    updatePool();
    return ok;
  }

  virtual bool setSheet(const char *name);

  virtual bool mergeStart();

  virtual bool mergeDone();

  virtual bool mergeAllDone();

  virtual void setConflicted() {
    if (chain) chain->setConflicted();
    Patcher::setConflicted();
  }

  virtual bool wantLinks() { 
    if (chain) return chain->wantLinks();
    return false; 
  }

  virtual bool declareLink(const LinkDeclare& decl) {
    if (chain) return chain->declareLink(decl);
    return false;
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

  bool updateSheet();

  //virtual coopy::store::PolySheet getSheet();

};

#endif

