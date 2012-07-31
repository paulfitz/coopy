#ifndef COOPY_SQLCOMPARE_INC
#define COOPY_SQLCOMPARE_INC

#include <coopy/DbiSqlWrapper.h>
#include <coopy/Patcher.h>

namespace coopy {
  namespace cmp {
    class SqlCompare;
  }
}

class coopy::cmp::SqlCompare {
public:
  coopy::store::DbiSqlWrapper *db;
  coopy::store::SqlTable local;
  coopy::store::SqlTable remote;
  coopy::cmp::Patcher& output;

  SqlCompare(coopy::store::DbiSqlWrapper *db, 
	     const coopy::store::SqlTableName& table1, 
	     const coopy::store::SqlTableName& table2,
	     coopy::cmp::Patcher& output) :
  db(db), local(db,table1), remote(db,table2), output(output)
  {
  }

  bool validateSchema();

  bool apply();
};


#endif
