#include <coopy/Dbg.h>
#include <coopy/RemoteSqlTextBook.h>
#include <coopy/RemoteSqlSheet.h>
#include <stdio.h>

#include <sqlxx.h>
#include <strutilsxx.h>

#include <algorithm>

using namespace coopy::store;
using namespace coopy::store::remotesql;
using namespace std;
using namespace sqlxx;

#define HELPER(x) (*((CSQL*)(x)))

RemoteSqlTextBook::RemoteSqlTextBook() {
  implementation = NULL;
  dirty = false;
}

RemoteSqlTextBook::~RemoteSqlTextBook() {
  clear();
}

void RemoteSqlTextBook::clear() {
  if (implementation!=NULL) {
    HELPER(implementation).disconnect();
    delete &HELPER(implementation);
    implementation = NULL;
  }
}

bool RemoteSqlTextBook::open(const Property& config) {
  dirty = true;
  try {
    implementation = new CSQL();
    COOPY_ASSERT(implementation!=NULL);
    CSQL& SQL = HELPER(implementation);
    SQL.setUsername(config.get("username").asString().c_str());
    SQL.setPassword(config.get("password").asString().c_str());
    SQL.setHostname(config.get("host").asString().c_str());
    SQL.setPort(config.get("port",PolyValue::makeInt(3128)).asInt());
//    SQL.setSocket("/var/run/mysqld/mysqld.sock");
    database_name = config.get("database").asString().c_str();
    SQL.setDatabase(database_name);
    SQL.setType(SQLXX_MYSQL);
    //SQL.setType(SQLXX_ODBC);
    //SQL.setDriver("/usr/lib/libmyodbc.so");
    SQL.connect();
    if (config.check("table")) {
      names_cache.clear();
      names_cache.push_back(config.get("table").asString());
      dirty = false;
    }
    return true;
  }
  catch (sqlxx_error E) {
    cerr << "remotesql: " << E.what() << endl;
    if (implementation!=NULL) {
      CSQL& SQL = HELPER(implementation);
      fprintf(stderr,"hostname %s database %s port %d username %s\n", 
	      SQL.getHostname().c_str(),
	      SQL.getDatabase().c_str(),
	      SQL.getPortN(),
	      SQL.getUsername().c_str());
      fprintf(stderr,"password [%s]\n", SQL.getPassword().c_str());
      delete &HELPER(implementation);
      implementation = NULL;
    }
    return false;
  }
  printf("RemoteSqlTextBook failed\n");
  return false;
}

std::vector<std::string> RemoteSqlTextBook::getNames() {
  if (!dirty) return names_cache;
  vector<string>& names = names_cache;
  names.clear();
  dirty = false;
  if (implementation==NULL) return names;
  CSQL& SQL = HELPER(implementation);
  string query = string("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE = 'BASE TABLE' AND TABLE_SCHEMA=")+quote(database_name);
  //printf("Query is %s\n", query.c_str());
  CSQLResult *result = SQL.openQuery(query);
  if (result==NULL) return names;
  while (result->fetch()) {
    //printf("Got %s\n", result->get(0).c_str());
    names.push_back(result->get(0));
  }
  SQL.closeQuery(result);
  return names;
}

PolySheet RemoteSqlTextBook::readSheet(const std::string& name) {
  vector<string> names = getNames();
  if (std::find(names.begin(), names.end(), name)==names.end()) {
    return PolySheet();
  }
  RemoteSqlSheet *sheet = new RemoteSqlSheet(this,name.c_str());
  return PolySheet(sheet,true);
}
