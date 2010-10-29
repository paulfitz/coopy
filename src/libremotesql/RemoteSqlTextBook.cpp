#include <coopy/Dbg.h>
#include <coopy/RemoteSqlTextBook.h>
#include <stdio.h>

#include <sqlxx.h>
#include <strutilsxx.h>

using namespace coopy::store;
using namespace std;
using namespace sqlxx;

#define HELPER(x) (*((CSQL*)(x)))

RemoteSqlTextBook::RemoteSqlTextBook() {
  implementation = NULL;
}

RemoteSqlTextBook::~RemoteSqlTextBook() {
  clear();
}

void RemoteSqlTextBook::clear() {
  if (implementation!=NULL) {
    HELPER(implmentation).disconnect();
    delete &HELPER(implementation);
    implementation = NULL;
  }
}

bool RemoteSqlTextBook::read(const Property& config) {
  try {
    implementation = new CSQL();
    COOPY_ASSERT(implementation!=NULL);
    CSQL& SQL = HELPER(implementation);
    SQL.setUsername(config.get("username").asString().c_str());
    SQL.setPassword(config.get("password").asString().c_str());
    SQL.setHostname(config.get("host").asString().c_str());
    SQL.setPort(config.get("port",PolyValue::makeInt(3128)).asInt());
//    SQL.setSocket("/var/run/mysqld/mysqld.sock");
    SQL.setDatabase(config.get("database").asString().c_str());
    SQL.setType(SQLXX_MYSQL);
    //SQL.setType(SQLXX_ODBC);
    //SQL.setDriver("/usr/lib/libmyodbc.so");
    SQL.connect();

    /*
    Result=SQL.openQuery("SELECT * FROM user");
    cout << "Columns: " << Result->getNumCols() << endl;
    cout << "Rows: " << Result->getNumRows() << endl;
    while (Result->fetch()) {
      for(iCol=0;iCol<Result->getNumCols();iCol++) {
        cout << Result->getColName(iCol) << ": " << Result->get(iCol);
        if (Result->isNull(iCol)) cout << " (is NULL)";
        cout << endl;
      }
      cout << endl;
    }
    SQL.closeQuery(Result);
    */
    //SQL.disconnect();
    printf("Connected ok!\n");
    return true;
  }
  catch (sqlxx_error E) {
    cerr << E.what() << endl;
    if (implementation!=NULL) {
      delete &HELPER(implementation);
      implementation = NULL;
    }
    return false;
  }
  printf("RemoteSqlTextBook is a stub\n");
  return false;
}

std::vector<std::string> RemoteSqlTextBook::getNames() {
  return vector<string>();
}

PolySheet RemoteSqlTextBook::readSheet(const std::string& name) {
  return PolySheet();
}
