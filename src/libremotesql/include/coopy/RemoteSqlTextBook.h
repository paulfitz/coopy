#ifndef COOPY_REMOTESQLTEXTBOOK
#define COOPY_REMOTESQLTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/Property.h>
#include <coopy/TextBookFactory.h>

namespace coopy {
  namespace store {
    class RemoteSqlTextBook;
    class RemoteSqlTextBookFactory;
  }
}

class coopy::store::RemoteSqlTextBook : public TextBook {
public:
  RemoteSqlTextBook();
  virtual ~RemoteSqlTextBook();

  void clear();

  bool open(const Property& config);

  std::vector<std::string> getNames();

  PolySheet readSheet(const std::string& name);

  virtual bool inplace() const {
    return true;
  }

  void *getSqlInterface() {
    return implementation;
  }

  std::string getDatabaseName() {
    return database_name;
  }

private:
  void *implementation;
  std::string database_name;
  std::vector<std::string> names_cache;
  bool dirty;
};


class coopy::store::RemoteSqlTextBookFactory : public TextBookFactory {
private:
  std::string name;
public:
  RemoteSqlTextBookFactory(const char *name = "mysql") : name(name) {
  }
  
  virtual std::string getName() {
    return name;
  }

  virtual TextBook *open(AttachConfig& config, AttachReport& report) {
    if (config.shouldCreate||config.shouldWrite) {
      return NULL;
    }
    RemoteSqlTextBook *book = new RemoteSqlTextBook();
    if (book==NULL) return NULL;
    if (!book->open(config.options)) {
      delete book;
      book = NULL;
    }
    if (book!=NULL) {
      report.success = true;
    }
    return book;
  }
};

#endif
