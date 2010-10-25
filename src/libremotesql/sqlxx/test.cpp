#include <iostream>
#include <sqlxx.h>
#include <strutilsxx.h>

using namespace std;
using namespace sqlxx;

int main(void) {
  CSQL SQL;
  CSQLResult *Result;
  int iCol, iCount;
  
  try {
    SQL.setUsername("root");
    SQL.setPassword("");
    SQL.setHostname("localhost");
    SQL.setPort(3128);
//    SQL.setSocket("/var/run/mysqld/mysqld.sock");
    SQL.setDatabase("mysql");
    SQL.setType(SQLXX_MYSQL);
    //SQL.setType(SQLXX_ODBC);
    //SQL.setDriver("/usr/lib/libmyodbc.so");
    SQL.connect();
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
    SQL.disconnect();
  }
  catch (sqlxx_error E) {
    cerr << E.what() << endl;
  }
  return 0;
}

