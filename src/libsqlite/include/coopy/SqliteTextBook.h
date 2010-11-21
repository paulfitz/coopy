#ifndef COOPY_SQLITETEXTBOOK
#define COOPY_SQLITETEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/TextBookFactory.h>

namespace coopy {
  namespace store {
    class SqliteTextBook;
    class SqliteTextBookFactory;
  }
}

class coopy::store::SqliteTextBook : public TextBook {
public:
  SqliteTextBook();
  virtual ~SqliteTextBook();

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

  virtual bool addSheet(const SheetSchema& schema);


  virtual std::string desc() const {
    return "SqliteBook";
  }

private:
  void *implementation;
  std::vector<std::string> names;

  std::vector<std::string> getNamesSql();

};


class coopy::store::SqliteTextBookFactory : public TextBookFactory {
public:
  virtual std::string getName() {
    return "sqlite";
  }

  virtual TextBook *open(AttachConfig& config, AttachReport& report) {
    SqliteTextBook *book = new SqliteTextBook();
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
