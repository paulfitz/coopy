#ifndef COOPY_TEXTBOOK
#define COOPY_TEXTBOOK

#include <coopy/PolySheet.h>
#include <coopy/Property.h>
#include <coopy/RefCount.h>
#include <coopy/CompareFlags.h>

#include <vector>
#include <string>

namespace coopy {
  namespace store {
    class TextBook;
  }
}

class coopy::store::TextBook : public RefCount {
public:
  TextBook() {
    pool = 0/*NULL*/;
  }

  virtual ~TextBook() {}

  virtual std::vector<std::string> getNames() = 0;

  virtual int getSheetCount() {
    return (int)getNames().size();
  }

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

  bool operator==(const TextBook& alt) const {
    coopy::cmp::CompareFlags flags;
    return equals(alt,flags);
  }

  bool equals(const TextBook& alt, const coopy::cmp::CompareFlags& flags) const;

  virtual bool copy(const TextBook& alt, const Property& options);

  virtual bool addSheet(const SheetSchema& schema) {
    return false;
  }

  bool fixSchema(const SheetSchema& in,SimpleSheetSchema& out);

  virtual PolySheet provideSheet(const SheetSchema& schema);

  virtual std::string desc() const {
    return "TextBook";
  }

  virtual bool namedSheets() const {
    return true;
  }

  virtual TextBook& tail() {
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

  virtual bool writtenToFuture() const {
    return false;
  }

  static bool exists(const char *fname);

  std::string toString() {
    std::string result;
    std::vector<std::string> names = getNames();
    for (int i=0; i<(int)names.size(); i++) {
      result += names[i];
      result += ":\n";
      result += readSheet(names[i]).toString();
      result += "\n";
    }
    return result;
  }

private:
  Pool *pool;
};

#endif
