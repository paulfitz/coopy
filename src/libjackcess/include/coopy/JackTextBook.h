#ifndef COOPY_JACKTEXTBOOK
#define COOPY_JACKTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/TextBookFactory.h>

namespace coopy {
  namespace store {
    namespace mdb {
      class JackTextBook;
      class JackTextBookFactory;
    }
  }
}

class coopy::store::mdb::JackTextBook : public TextBook {
public:
  JackTextBook();
  virtual ~JackTextBook();

  void clear();

  bool read(const char *fname);

  virtual bool open(const Property& config);
  
  std::vector<std::string> getNames() {
    return names;
  }

  PolySheet readSheet(const std::string& name);

  virtual bool inplace() const {
    return true;
  }

  virtual bool canWrite() const { 
    return true; 
  }

  virtual bool addSheet(const SheetSchema& schema);


  virtual std::string desc() const {
    return "JackAccessBook";
  }

private:
  void *implementation;

  Property config;

  std::vector<std::string> names;

  std::vector<std::string> getNamesImpl();

  static int uses;

};


class coopy::store::mdb::JackTextBookFactory : public TextBookFactory {
public:
  virtual std::string getName() {
    return "jackcess";
  }

  virtual TextBook *open(AttachConfig& config, AttachReport& report) {
    JackTextBook *book = new JackTextBook();
    if (book==NULL) return NULL;
    if (!book->open(config.options)) {
      delete book;
      book = NULL;
    }
    if (config.shouldWrite) {
      if (config.prevBook!=NULL) {
	if (!book->copy(*config.prevBook,config.options)) {
	  delete book;
	  book = NULL;
	  report.msg = "data transfer failed";
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
