#ifndef COOPY_WIDESHEETMANAGER
#define COOPY_WIDESHEETMANAGER

#include <string>
#include <vector>

class sqlite3;

class WideSheetManager {
private:
  sqlite3 *db;
  std::string dir;
  std::string sep;

public:
  WideSheetManager() {
    db = 0/*NULL*/;
  }


  WideSheetManager(bool create, const char *fname = NULL) {
    db = 0/*NULL*/;
    connect(create,fname);
  }

  ~WideSheetManager();

  bool connect(bool create = false, const char *fname = NULL);
  bool disconnect();

  bool isValid() { 
    return db!=0/*NULL*/;
  }

  bool setProperty(const char *key, const char *val);

  std::string getProperty(const char *key);

  std::string getPropertyWithDefault(const char *key);

  bool exportSheet();

  bool acceptSheet();

  bool diffSheet();

  sqlite3 *get() {
    return db;
  }

  std::string linkTable() {
    return "widesheet_links";
  }

  bool setFile(const char *key, const char *val);

  std::string getFile(const char *key);

  bool exportSheet(const char *key, bool reverse = false);

  bool importSheet(const char *key) {
    return exportSheet(key, true);
  }

  bool setDirectory(const char *dir, const char *sep = "/");
};

#endif
