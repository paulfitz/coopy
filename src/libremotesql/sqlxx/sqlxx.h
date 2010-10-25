/*************************************************************************
* libsqlxx                                                               *
* ---------------------------------------------------------------------- *
* Copyright (C) 2000-2002  Klaus Reimer, k@ailis.de                      *
*                                                                        *
* This library is free software; you can redistribute it and/or          *
* modify it under the terms of the GNU Lesser General Public             *
* License as published by the Free Software Foundation; either           *
* version 2 of the License, or (at your option) any later version.       *
*                                                                        *
* This library is distributed in the hope that it will be useful,        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
* Library General Public License for more details.                       *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with this progam; if not, write to the                   *
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,           *
* Boston, MA 02111-1307, USA.                                            *
*************************************************************************/

#ifndef SQLXX_H
#define SQLXX_H

#include <string>
#include <stdexcept>
#include <strutilsxx.h>
#include <vector>

namespace sqlxx {

#define SQLXX_ODBC 0
#define SQLXX_MYSQL 1
#define SQLXX_POSTGRES 2

class sqlxx_error : public std::runtime_error {
public:
  sqlxx_error(const std::string &what_arg): std::runtime_error(what_arg) { }
};

struct SValue {
  std::string sValue;
  bool bNull;
};

class CSQL;

class CSQLResult {
private:
  int iBufferSize;
  friend class CSQL;
  void *Result;
  CSQL *Database;
  unsigned int iNumCols, iCurrent;
  void close(void);
  std::vector< SValue > vResult;
  std::vector< std::string > vFields;
  char *pBuffer;
  std::string getError(void);
  void fetchColNames(void);
public:
  CSQLResult(CSQL *Database, const int iBufferSize=1024);
  ~CSQLResult(void);
  void query(const std::string &sQuery);
  unsigned int getNumCols(void);
  unsigned long getNumRows(void);
  std::string getColName(const unsigned int iIndex);
  int getColType(const unsigned int iIndex);
  bool fetch(void);
  std::string get(const unsigned int iIndex);
  std::string get(const std::string sField);
  long getInt(const unsigned int iIndex);
  long getInt(const std::string sField);
  float getFloat(const unsigned int iIndex);
  float getFloat(const std::string sField);
  bool isNull(const unsigned int iIndex);
  bool isNull(const std::string sField);
};

class CSQL {
private:
  friend class CSQLResult;
  std::vector< CSQLResult * > vResults;
  void *Env, *DB, *Result;
  bool bConnected;
  unsigned short iType;
  std::string sDSN;
  std::string sHostname;
  std::string sDatabase;
  std::string sDriver;
  std::string sUsername;
  std::string sPassword;
  std::string sSocket;
  std::string sOptions;
  unsigned int iPort;
  std::string getError(void *Result=NULL);
  void addResult(CSQLResult *Result);
  void delResult(CSQLResult *Result);
public:
  CSQL(void);
  ~CSQL(void);
  void setDSN(const std::string &sNewDSN);
  void setHostname(const std::string &sNewHostname);
  void setSocket(const std::string &sNewSocket);
  void setPort(const std::string &sNewPort);
  void setPort(unsigned int iNewPort);
  void setDatabase(const std::string &sNewDatabase);
  void setDriver(const std::string &sNewDriver);
  void setUsername(const std::string &sNewUsername);
  void setPassword(const std::string &sNewPassword);
  void setType(const unsigned short iNewType);
  void setOptions(const std::string &sNewOptions);
  std::string getDSN(void);
  std::string getHostname(void);
  std::string getSocket(void);
  std::string getPort(void);
  unsigned int getPortN(void);
  std::string getDatabase(void);
  std::string getDriver(void);
  std::string getUsername(void);
  std::string getPassword(void);
  unsigned short getType(void);
  std::string getOptions(void);
  void connect(void);
  void disconnect(void);  
  bool isConnected(void);    
  CSQLResult *openQuery(const std::string &sQuery, const int iBufferSize=1024);
  void execQuery(const std::string &sQuery);
  void closeQuery(const CSQLResult *DBResult);
  long getNewID(const std::string &sTable, const std::string &sFieldname);
  std::vector< CSQLResult * > &getQueries(void);
  int countQueries(void);
};

std::string quote(const std::string &sSource, const bool bEscape8Bit=false,
  const std::string &sEscape="");
}

#endif
