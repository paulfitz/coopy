#ifndef COOPY_WIDESHEET
#define COOPY_WIDESHEET

#include <string>

class sqlite3;

class WideSheet {
private:
  sqlite3 *db;
  bool init;

  void connect(bool create = false);
public:
  WideSheet(bool create) {
    db = 0/*NULL*/;
    init = false;
    connect(create);
  }

  ~WideSheet();

  bool isValid() { 
    return db!=0/*NULL*/;
  }

  bool setProperty(const char *key, const char *val);

  std::string getProperty(const char *key);

  std::string getPropertyWithDefault(const char *key);

  bool exportSheet();

  bool acceptSheet();

  bool diffSheet();
};

#endif
