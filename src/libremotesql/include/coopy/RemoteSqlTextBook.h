#ifndef COOPY_REMOTESQLTEXTBOOK
#define COOPY_REMOTESQLTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/Property.h>
#include <coopy/TextBookFactory.h>

namespace coopy {
  namespace store {
    /**
     *
     * Remote SQL plugin (currently limited to MySQL).
     *
     */
    namespace remotesql {
      class RemoteSqlTextBook;
      class RemoteSqlTextBookFactory;
    }
  }
}

class coopy::store::remotesql::RemoteSqlTextBook : public TextBook {
public:
  RemoteSqlTextBook(const std::string& kind);
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

  std::string getTableSchema() {
    return table_schema;
  }

  std::string getTableCatalog() {
    return table_catalog;
  }

  bool hasFirstClassAutoIncrement() {
    return kind=="mysql";
  }

  bool isMysql() {
    return kind=="mysql";
  }

  bool isPg() {
    return kind=="pg";
  }

private:
  void *implementation;
  std::string database_name, table_schema, table_catalog;
  std::vector<std::string> names_cache;
  bool dirty;
  std::string kind;
};


class coopy::store::remotesql::RemoteSqlTextBookFactory : public TextBookFactory {
private:
  std::string name;
public:
  RemoteSqlTextBookFactory(const char *name = "mysql") : name(name) {
  }
  
  virtual std::string getName() {
    return name;
  }

  virtual TextBook *open(AttachConfig& config, AttachReport& report) {
    if (config.shouldCreate) {
      report.errorCreateNotImplemented(name.c_str());
      return NULL;
    }
    RemoteSqlTextBook *book = new RemoteSqlTextBook(name);
    if (book==NULL) return NULL;
    if (!book->open(config.options)) {
      delete book;
      book = NULL;
    }
    if (config.prevBook!=NULL) {
      if (config.shouldWrite) {
	book->copy(*config.prevBook,config.options);
      }
    }
    if (book!=NULL) {
      report.success = true;
    }
    return book;
  }
};

#endif
