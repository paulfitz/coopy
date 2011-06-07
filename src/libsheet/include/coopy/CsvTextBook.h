#ifndef COOPY_CSVTEXTBOOK
#define COOPY_CSVTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/TextBookFactory.h>
#include <coopy/CsvSheet.h>
#include <coopy/CsvFile.h>

#include <vector>
#include <map>

namespace coopy {
  namespace store {
    class CsvTextBook;
    class CsvTextBookFactory;
  }
}

class coopy::store::CsvTextBook : public TextBook, public CsvSheetReader {
public:
  CsvTextBook(bool compact) : compact(compact) {
    named = true;
  }

  std::vector<PolySheet> sheets;
  std::vector<std::string> names;
  std::map<std::string,int> name2index;

  virtual std::vector<std::string> getNames() {
    return names;
  }

  virtual PolySheet readSheet(const std::string& name) {
    if (name2index.find(name)!=name2index.end()) {
      return sheets[name2index[name]];
    }
    return PolySheet();
  }

  bool clear() {
    sheets.clear();
    names.clear();
    name2index.clear();
    return true;
  }
  
  bool readCsvs(const char *fname);

  bool readCsvsData(const char *data, int len);

  bool writeCsvs(const char *fname) {
    return write(fname,this,compact);
  }

  static bool write(const char *fname, TextBook *book, bool compact);

  //  virtual bool open(const Property& config);

  bool addSheet(const SheetSchema& schema);

  virtual CsvSheet *nextSheet(const char *name, bool named);

  virtual bool namedSheets() const {
    return named;
  }

private:
  bool compact;
  bool named;
};


class coopy::store::CsvTextBookFactory : public TextBookFactory {
private:
  bool compact;
public:
  CsvTextBookFactory(bool compact) : compact(compact) {
  }

  virtual std::string getName() {
    return compact?"csvs":"book";
  }

  virtual TextBook *open(AttachConfig& config, AttachReport& report) {
    if (config.shouldWrite) {
      if (config.prevBook!=NULL) {
	dbg_printf("writing csvs book file %s\n", config.options.get("file").asString().c_str());
	//int r = CsvFile::write(config.prevBook->readSheetByIndex(0),
	//config.options);
	if (CsvTextBook::write(config.options.get("file").asString().c_str(),
			       config.prevBook,
			       compact)) {
	  report.success = true;
	  return config.prevBook;
	}
      }
      return NULL;
    }

    CsvTextBook *book = new CsvTextBook(compact);
    if (book==NULL) return NULL;

    if (config.shouldRead) {
      if (!config.options.check("should_attach")) {
	dbg_printf("reading csv file %s\n", config.options.get("file").asString().c_str());
	bool r = book->readCsvs(config.fname.c_str());
	if (!r) {
	  delete book;
	  book = NULL;
	}
      }
    }
    
    if (book!=NULL) {
      report.success = true;
    }

    return book;
  }

  static CsvTextBookFactory *makeFactory() {
    return new CsvTextBookFactory(false);
  }

  static CsvTextBookFactory *makeCompactFactory() {
    return new CsvTextBookFactory(true);
  }
};


#endif
