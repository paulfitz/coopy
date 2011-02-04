#ifndef COOPY_COLUMNINFO
#define COOPY_COLUMNINFO

#include <string>
#include <coopy/Property.h>

namespace coopy {
  namespace store {
    class ColumnType;
    class ColumnInfo;
  }
}

class coopy::store::ColumnType {
public:
  enum {
    COLUMN_FAMILY_NONE,
    COLUMN_FAMILY_INTEGER,
    COLUMN_FAMILY_REAL,
    COLUMN_FAMILY_TEXT,
    COLUMN_FAMILY_DATETIME,
    COLUMN_FAMILY_BLOB,

    COLUMN_SOURCE_NONE,
    COLUMN_SOURCE_SQLITE,
    COLUMN_SOURCE_MYSQL,
    COLUMN_SOURCE_EXCEL,
    COLUMN_SOURCE_CSV,
    COLUMN_SOURCE_ACCESS,
  };
    
  std::string src_name;
  std::string src_lang;
  bool allowNull;
  int family;
  int src;
  int size;
  int size2;
  bool primaryKey;
  bool primaryKeySet;

  ColumnType() {
    reset();
  }

  ColumnType(const std::string& name) {
    setType(name);
  }

  void reset() {
    allowNull = false;
    family = COLUMN_FAMILY_NONE;
    src = COLUMN_SOURCE_NONE;
    size = 0;
    size2 = 0;
    primaryKey = false;
    primaryKeySet = false;
    src_lang = "";
    src_name = "";
  }

  // do guess-work
  bool setType(const std::string& name, const std::string& lang = "unknown");

  std::string asSqlite() const;
};


class coopy::store::ColumnInfo {
private:
  std::string name;
  bool nameSet;
  ColumnType columnType;
  bool typeSet;
public:
  ColumnInfo() : nameSet(false), typeSet(false) {}
  ColumnInfo(std::string name) : name(name), nameSet(true), typeSet(false) {}
  ColumnInfo(std::string name, const ColumnType& t) : name(name), nameSet(true), typeSet(true), columnType(t) {}
  
  virtual ~ColumnInfo() {}
  
  virtual bool hasName() const { return nameSet; }
  
  virtual std::string getName() const { return name; }

  virtual bool hasPrimaryKey() const { return columnType.primaryKeySet; }
  
  virtual bool isPrimaryKey() const { return columnType.primaryKey; }
  
  const ColumnType& getColumnType() const { return columnType; }

  virtual bool hasType() const { return typeSet; }
};

#endif
