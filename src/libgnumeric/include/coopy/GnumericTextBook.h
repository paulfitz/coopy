#ifndef COOPY_GNUMERICTEXTBOOK
#define COOPY_GNUMERICTEXTBOOK

#include <coopy/TextBook.h>

#include <string>
#include <vector>

namespace coopy {
  namespace store {
    /**
     *
     * Gnumeric table plugin (for Excel files, OpenOffice, etc)
     *
     */
    namespace gnumeric {
      class GnumericTextBook;
      class GnumericTextBookFactory;
    }
  }
}

class coopy::store::gnumeric::GnumericTextBook : public TextBook {
private:
  void *implementation;
  std::vector<std::string> names;
  bool dirtyNames;

  void updateNames();

public:
  GnumericTextBook();

  virtual ~GnumericTextBook();

  virtual void clear();

  virtual bool create();

  virtual bool load(const char *fname);

  virtual bool save(const char *fname, const char *format);

  virtual std::vector<std::string> getNames();

  virtual PolySheet readSheet(const std::string& name);
  virtual PolySheet readSheetByIndex(int index);

  bool addSheet(const SheetSchema& schema);
};


#endif
