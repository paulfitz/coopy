#ifndef COOPY_CSVTEXTBOOK
#define COOPY_CSVTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/TextBookFactory.h>
#include <coopy/CsvSheet.h>

#include <vector>
#include <map>

namespace coopy {
  namespace store {
    class CsvTextBook;
    class CsvTextBookFactory;
  }
}

class coopy::store::CsvTextBook : public TextBook {
public:
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
  
  bool read(const char *fname);

  virtual bool open(const Property& config);
};


class coopy::store::CsvTextBookFactory : public TextBookFactory {
public:
  virtual std::string getName() {
    return "book";
  }

  virtual TextBook *open(AttachConfig& config, AttachReport& report) {
    if (config.shouldWrite) {
      if (config.prevBook!=NULL) {
	dbg_printf("writing book file %s\n", config.options.get("file").asString().c_str());
	//int r = CsvFile::write(config.prevBook->readSheetByIndex(0),
	//config.options);
	int r = -1;
	if (r==0) {
	  report.success = true;
	  return config.prevBook;
	}
      }
      return NULL;
    }

    CsvTextBook *book = new CsvTextBook;
    if (book==NULL) return NULL;

    if (config.shouldRead) {
      if (!config.options.check("should_attach")) {
	dbg_printf("reading csv file %s\n", config.options.get("file").asString().c_str());
	bool r = book->read(config.fname.c_str());
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
};

#endif
