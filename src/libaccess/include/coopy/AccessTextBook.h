#ifndef COOPY_ACCESSTEXTBOOK
#define COOPY_ACCESSTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/TextBookFactory.h>

namespace coopy {
  namespace store {
    /**
     *
     * MDB/Access plugin.
     *
     */
    namespace mdb {
      class AccessTextBook;
      class AccessTextBookFactory;
    }
  }
}

class coopy::store::mdb::AccessTextBook : public TextBook {
public:
  AccessTextBook();
  virtual ~AccessTextBook();

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
    return false; 
  }

  virtual bool addSheet(const SheetSchema& schema);


  virtual std::string desc() const {
    return "AccessBook";
  }

private:
  void *implementation;

  Property config;

  std::vector<std::string> names;

  std::vector<std::string> getNamesImpl();

  static int uses;

};


class coopy::store::mdb::AccessTextBookFactory : public TextBookFactory {
public:
  virtual std::string getName() {
    return "access";
  }

  virtual TextBook *open(AttachConfig& config, AttachReport& report) {
    AccessTextBook *book = new AccessTextBook();
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
