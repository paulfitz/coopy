#ifndef COOPY_DATABOOK
#define COOPY_DATABOOK

#include <coopy/PolySheet.h>
#include <coopy/Property.h>
#include <coopy/RefCount.h>

#include <vector>
#include <string>

namespace coopy {
  namespace store {
    class DataBook;
  }
}

class coopy::store::DataBook : public RefCount {
public:
  DataBook() {
    pool = 0/*NULL*/;
  }

  virtual ~DataBook() {}

  virtual std::vector<std::string> getNames() = 0;

  virtual bool open(const Property& config) {
    return false;
  }

  virtual PolySheet readSheet(const std::string& name) = 0;

  virtual PolySheet readSheetByIndex(int index) {
    std::vector<std::string> names = getNames();
    if (index>=(int)names.size()) return PolySheet();
    return readSheet(names[index]);
  }

  virtual bool save(const char *fname, const char *format) {
    return false;
  }

  virtual bool inplace() const {
    return false;
  }

  virtual bool canWrite() const { return true; }

  bool operator==(const DataBook& alt) const;

  virtual bool copy(const DataBook& alt, const Property& options);

  virtual bool addSheet(const SheetSchema& schema) {
    return false;
  }

  bool fixSchema(const SheetSchema& in,SimpleSheetSchema& out);

  virtual PolySheet provideSheet(const SheetSchema& schema);

  virtual std::string desc() const {
    return "DataBook";
  }

  virtual bool namedSheets() const {
    return true;
  }

  virtual DataBook& tail() {
    return *this;
  }

  virtual bool setPool(Pool *pool) {
    this->pool = pool;
    return true;
  }

  virtual Pool *getPool() const {
    return pool;
  }

  bool applyPool(DataSheet& sheet) {
    return sheet.setPool(getPool());
  }

  virtual bool isValid() const {
    return true;
  }

  static bool exists(const char *fname);

private:
  Pool *pool;
};

#endif
