#ifndef COOPY_POOL
#define COOPY_POOL

#include <coopy/SheetCell.h>

#include <map>

namespace coopy {
  namespace store {
    class Pool;
    class PoolColumnLink;
    class PoolColumn;
  }
}

class coopy::store::PoolColumn {
public:
  virtual ~PoolColumn() {}
  virtual bool isValid() const = 0;
  virtual SheetCell lookup(const SheetCell& val, bool& match) = 0;
  virtual bool put(const SheetCell& src, const SheetCell& dest) = 0;
};

class coopy::store::PoolColumnLink {
private:
  PoolColumn *column;
  bool invent;
  std::string table_name;
  std::string column_name;
  std::string pool_name;
public:
  PoolColumnLink() {
    column = 0 /*NULL*/;
    invent = false;
  }

  PoolColumnLink(PoolColumn& column, bool invent) : column(&column),
    invent(invent) {
  }

  PoolColumnLink(PoolColumn& column, bool invent,
		 const std::string& table_name,
		 const std::string& column_name,
		 const std::string& pool_name) : column(&column),
    invent(invent),
    table_name(table_name),
    column_name(column_name),
    pool_name(pool_name) {
  }

  PoolColumnLink(const PoolColumnLink& alt) {
      this->column = alt.column;
      this->invent = alt.invent;
      this->table_name = table_name;
      this->column_name = column_name;
      this->pool_name = pool_name;
  }

  const PoolColumnLink& operator = (const PoolColumnLink& alt) {
      this->column = alt.column;
      this->invent = alt.invent;
      this->table_name = alt.table_name;
      this->column_name = alt.column_name;
      this->pool_name = alt.pool_name;
      return *this;
  }
  
  bool isInventor() { return invent; }

  PoolColumn& getColumn() { return *column; }

  bool isValid() const {
    if (!column) return false;
    return column->isValid();
  }

  std::string getTableName() const {
    return table_name;
  }

  std::string getColumnName() const {
    return column_name;
  }

  std::string getPoolName() const {
    return pool_name;
  }
};

class coopy::store::Pool {
public:
  virtual ~Pool() {}

  virtual bool create(const std::string& key,
		      const std::string& table_name,
		      const std::string& column_name,
		      bool invent) = 0;

  virtual PoolColumnLink lookup(const std::string& table_name,
				const std::string& column_name) = 0;

  virtual SheetCell lookup(const std::string& table_name,
			   const std::string& column_name,
			   const SheetCell& val,
			   bool& match) = 0;
};

#endif
