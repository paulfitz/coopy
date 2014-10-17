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

#include "sqlxx.h"
#include <iostream>
#include <algorithm>
#ifdef USE_MYSQL
#ifdef _WIN32
#include <winsock.h>
#endif
#include <mysql.h>
#endif
#ifdef USE_POSTGRES
#include <libpq-fe.h>
#endif
#ifdef USE_ODBC
#include <isql.h>
#include <isqlext.h>
#endif

using namespace std;
using namespace strutilsxx;

namespace sqlxx {

#ifdef DEBUG
int ICDI=0;
#define TRACEINPARAM(c,m) for(int IDI=0;IDI<ICDI;IDI++) cerr << "  "; cerr << "-> sqlxx: " << this << " = " << c << "(" << m << ")" << endl; ICDI++; fflush(stderr);
#define FTRACEINPARAM(c,m) for(int IDI=0;IDI<ICDI;IDI++) cerr << "  "; cerr << "-> sqlxx: " << c << "(" << m << ")" << endl; ICDI++; fflush(stderr);
#define TRACEIN(c) for(int IDI=0;IDI<ICDI;IDI++) cerr << "  "; cerr << "-> sqlxx: " << this << " = " << c << "()" << endl; ICDI++; fflush(stderr);
#define TRACEOUTRET(c,r) ICDI--; for(int IDI=0;IDI<ICDI;IDI++) cerr << "  "; cerr << "<- sqlxx: " << this << " = " << c << "=" << r << endl; fflush(stderr);
#define FTRACEOUTRET(c,r) ICDI--; for(int IDI=0;IDI<ICDI;IDI++) cerr << "  "; cerr << "<- sqlxx: " << " = " << c << "=" << r << endl; fflush(stderr);
#define TRACEOUT(c) ICDI--; for(int IDI=0;IDI<ICDI;IDI++) cerr << "  "; cerr << "<- sqlxx: " << this << " = " << c << endl; fflush(stderr);
#else
#define TRACEINPARAM(c,m)
#define FTRACEINPARAM(c,m)
#define TRACEIN(c)
#define TRACEOUTRET(c,r)
#define FTRACEOUTRET(c,r)
#define TRACEOUT(c)
#endif

/** @name sqlxx
    This library contains C++ classes for easy database access via ODBC using
    libiodbc2 or via native access. Currently the native access to mysql and
    postgresql databases is implemented.
    @version 2.1.0
    @author Klaus Reimer <k@ailis.de>
*/


/** SQL Result Constructor.
    This is the constructor of the SQL Result. You can specify a custom
    buffer size in bytes with the parameter <b>iBufferSize</b>. This size 
    is used by ODBC access and defaults to 1024 bytes.
    Normally you don't need to instantiate a CSQLResult by your own.
    CSQL::openQuery returns an instance of this class.
*/

CSQLResult::CSQLResult(CSQL *Database, const int iBufferSize) {
  TRACEINPARAM("CSQLResult::CSQLResult",format("%p,%i",Database,iBufferSize))
  this->Database=Database;
  this->iBufferSize=iBufferSize;
  Result=NULL;
  pBuffer=NULL;
  Database->addResult(this);
  TRACEOUT("CSQLResult::CSQLResult")
}


/** SQL Result Destructor.
    This is the destructor of the SQL Result. It calls CSQL::close() to
    close the database connection.
*/

CSQLResult::~CSQLResult(void) {
  TRACEIN("CSQLResult::~CSQLResult")
  close();
  TRACEOUT("CSQLResult::~CSQLResult")
}


/** Closes database connection.
    This method closes the database connection and frees all allocated
    memory.
*/

void CSQLResult::close(void) {
  TRACEIN("CSQLResult::close")
  if (Database && Database->bConnected) {
    switch(Database->iType) {
      #ifdef USE_POSTGRES
      case SQLXX_POSTGRES:
        if (Result)
          PQclear((PGresult *) Result);
          iCurrent=0;
        break;
      #endif
      #ifdef USE_MYSQL
      case SQLXX_MYSQL:
        if (Result)
          mysql_free_result((MYSQL_RES *) Result);
        break;
      #endif
      #ifdef USE_ODBC
      case SQLXX_ODBC:
        if (Result) {
          switch (SQLFreeStmt((SQLHSTMT) Result,SQL_DROP)) {
            case SQL_ERROR:
              cerr << "SQLFreeStmt: Error" << endl;
              abort();
              break;
            case  SQL_INVALID_HANDLE:
              cerr << "SQLFreeStmt: Invalid statement handle" << endl;
              abort();
              break;
          }
          Result=NULL;
        }
        free(pBuffer);
        break;
      #endif
      default:
        break;
    }
  }
  if (Database) Database->delResult(this);
  TRACEOUT("CSQLResult::close")
}


/** Returns last error message.
    This method returns the last error message of the SQL Result.
*/

string CSQLResult::getError(void) {
  string sResult;
  
  TRACEIN("CSQLResult::getError")
  if (Database) {
    sResult=Database->getError(Result);
  }
  else
    sResult="Database object has died";
  TRACEOUTRET("CSQLResult::getError",sResult)
  return sResult;
};

void CSQLResult::fetchColNames(void) {
  #ifdef USE_ODBC
  char pBuffer[256];
  #endif
  string sColName;
  unsigned int iCol;
  
  TRACEIN("CSQLResult::fetchColNames")
  vFields.erase(vFields.begin(),vFields.end());
  for (iCol=0;iCol<iNumCols;iCol++) {
    switch(Database->iType) {
      #ifdef USE_POSTGRES
      case SQLXX_POSTGRES:
        vFields.push_back(PQfname((PGresult *) Result,iCol));
        break;
      #endif
      #ifdef USE_MYSQL
      case SQLXX_MYSQL:
        vFields.push_back(mysql_fetch_fields((MYSQL_RES *) Result)[iCol].name);
        break;
      #endif
      #ifdef USE_ODBC
      case SQLXX_ODBC:
        switch (SQLDescribeCol((SQLHSTMT) Result,iCol+1,(UCHAR *) pBuffer,sizeof(pBuffer),
                NULL,NULL,NULL,NULL,NULL)) {
          case SQL_ERROR:
            throw sqlxx_error("SQLDescribeCol: "+getError());
            break;
          case SQL_INVALID_HANDLE:
            throw sqlxx_error("SQLDescribeCol: Invalid statement handle");
            break;
        }
        vFields.push_back(pBuffer);
        break;
      #endif
      default:
        break;
    }
  }
  TRACEOUT("CSQLResult::fetchColNames")
}


/** Query Database.
    This method sends the SQL query <b>sQuery</b> to the database.  You
    don't need to use this method on your own. Use CSQL::openQuery()
    instead.
*/

void CSQLResult::query(const string &sQuery) {
  #ifdef USE_ODBC
  SQLSMALLINT iODBCNumCols;
  #endif
  
  TRACEINPARAM("CSQLResult::query",sQuery)
  if (!Database->isConnected()) throw sqlxx_error("Not connected to database");
  switch (Database->iType) {
    #ifdef USE_POSTGRES
    case SQLXX_POSTGRES:
      Result=(void *) PQexec((PGconn *) Database->DB,sQuery.c_str());
      if (!Result)
        throw sqlxx_error("PGexec: Fatal error: Returned NULL pointer");
      if (PQresultStatus((PGresult *) Result)==PGRES_TUPLES_OK) {
        iNumCols=PQnfields((PGresult *) Result);
      } 
      else {
        iNumCols=0;
        if (PQresultStatus((PGresult *) Result)!=PGRES_COMMAND_OK) 
          throw sqlxx_error("PGexec: "+getError());
      }
      iCurrent=0;
      break;
    #endif
    #ifdef USE_MYSQL
    case SQLXX_MYSQL:
      if (mysql_query((MYSQL *) Database->DB,sQuery.c_str()))
        throw sqlxx_error("mysql_query: Error"+getError());
      Result=(void *) mysql_store_result((MYSQL *) Database->DB);
      if (!Result && mysql_field_count((MYSQL *) Database->DB))
        throw sqlxx_error("mysql_store_result:"+getError());
      if (Result)
        iNumCols=mysql_num_fields((MYSQL_RES *) Result);
      else
        iNumCols=0;
      break;
    #endif
    #ifdef USE_ODBC
    case SQLXX_ODBC:
      pBuffer=(char *) malloc(iBufferSize);
      if (!pBuffer)
        throw sqlxx_error("CSQLResult::CSQLResult(): Unable to allocate memory ("+intstr(iBufferSize)+")");
      switch (SQLAllocStmt((SQLHDBC) Database->DB,&Result)) {
        case SQL_ERROR: 
          throw sqlxx_error("SQLAllocStmt: Error"+getError());
          break;
        case SQL_INVALID_HANDLE:
          throw sqlxx_error("SQLAllocStmt: Invalid database handle");
          break;
      }
      switch (SQLPrepare((SQLHSTMT) Result, (UCHAR *) sQuery.c_str(),SQL_NTS)) {
        case SQL_ERROR:
          throw sqlxx_error("SQLPrepare: "+getError());
          break;
        case SQL_INVALID_HANDLE:
          throw sqlxx_error("SQLPrepare: Invalid statement handle");
          break;
      }
      switch (SQLExecute((SQLHSTMT) Result)) {
        case SQL_ERROR:
          throw sqlxx_error("SQLExecute: "+getError());
          break;
        case SQL_INVALID_HANDLE:
          throw sqlxx_error("SQLExecute: Invalid statement handle");
          break;
      }
      switch (SQLNumResultCols((SQLHSTMT) Result,&iODBCNumCols)) {
        case SQL_ERROR:
          throw sqlxx_error("SQLNumResultCols: "+getError());
          break;
        case SQL_INVALID_HANDLE:
          throw sqlxx_error("SQLNumResultCols: Invalid statement handle");
          break;
      }
      iNumCols=iODBCNumCols;
      break;
    #endif
    default:
      iNumCols=0;
      break;
  }
  if (iNumCols) fetchColNames();
  TRACEOUT("CSQLResult::query")
}


/** Return number of columns.
    This method returns the number of columns of the current SQL Result.
*/

unsigned int CSQLResult::getNumCols(void) {
  TRACEIN("CSQLResult::getNumCols")
  TRACEOUTRET("CSQLResult::getNumCols",iNumCols)
  return iNumCols;
}


/** Return number of rows.
    This method returns the number of rows of the current SQL Result.
*/

unsigned long CSQLResult::getNumRows(void) {
  long iNumRows;
  
  TRACEIN("CSQLResult::getNumRows")
  switch(Database->iType) {
    #ifdef USE_POSTGRES
    case SQLXX_POSTGRES:
      iNumRows=PQntuples((PGresult *) Result);
      break;
    #endif
    #ifdef USE_MYSQL
    case SQLXX_MYSQL:
      iNumRows=mysql_num_rows((MYSQL_RES *) Result);
      break;
    #endif
    #ifdef USE_ODBC
    case SQLXX_ODBC:
      switch (SQLRowCount((SQLHSTMT) Result,&iNumRows)) {
        case SQL_ERROR:
          throw sqlxx_error("SQLRowCount: "+getError());
          break;
        case SQL_INVALID_HANDLE:
          throw sqlxx_error("SQLRowCount: Invalid statement handle");
          break;
      }
      break;
    #endif
    default:
      iNumRows=0;
      break;
  }
  TRACEOUTRET("CSQLResult::getNumRows",iNumRows)
  return iNumRows;
}


/** Returns column name.
    Return the name of the column with the index <b>iIndex</b>.
*/

string CSQLResult::getColName(const unsigned int iIndex) {
  if (iIndex>=iNumCols)
    throw sqlxx_error("CSQLResult::getColName: Invalid index: "+intstr(iIndex));
  return vFields[iIndex];
}


/** Returns column type.
    Returns the type of the column with index <b>iIndex</b>.
    The meaning of the type is database dependent.
*/

int CSQLResult::getColType(const unsigned int iIndex) {
  int iType;
  #ifdef USE_ODBC
  unsigned char cColName[50];
  short iName;
  short iODBCType;
  unsigned long iDef;
  short iScale;
  short iNullable;
  #endif
  
  TRACEINPARAM("CSQLResult::getColType",iIndex)
  if (iIndex>=iNumCols)
    throw sqlxx_error("CSQLResult::getColType: Invalid index: "+intstr(iIndex));
  switch(Database->iType) {
    #ifdef USE_POSTGRES
    case SQLXX_POSTGRES:
      iType=PQftype((PGresult *) Result,iIndex);
      break;
    #endif
    #ifdef USE_MYSQL
    case SQLXX_MYSQL:
      iType=mysql_fetch_fields((MYSQL_RES *) Result)[iIndex].type;
      break;
    #endif
    #ifdef USE_ODBC
    case SQLXX_ODBC:
      switch (SQLDescribeCol((SQLHSTMT) Result,iIndex+1,cColName,sizeof(cColName),
          &iName,&iODBCType,&iDef,&iScale,&iNullable)) {
        case SQL_ERROR:
          throw sqlxx_error("SQLDescribeCol: "+getError());
          break;
        case SQL_INVALID_HANDLE:
          throw sqlxx_error("SQLDescribeCol: Invalid statement handle");
          break;
      }
      iType=iODBCType;
    #endif
    default:
      iType=-1;
      break;
  }
  TRACEOUTRET("CSQLResult::getColType",iType)
  return iType;  
}


/** Fetch a row of data.
    This method fetches a row of data from the current SQL Result.
*/

bool CSQLResult::fetch(void) {
  bool bResult;
  string sResult;
  unsigned int iCol;
  SValue Value;
  #ifdef USE_ODBC
  SDWORD iIndicator;
  SQLRETURN iRet;
  #endif
  #ifdef USE_MYSQL
  MYSQL_ROW hMYSQLROW;
  #endif
  
  TRACEIN("CSQLResult::fetch")
  bResult=true;
  vResult.erase(vResult.begin(),vResult.end());
  switch (Database->iType) {
    #ifdef USE_POSTGRES
    case SQLXX_POSTGRES:
      if (iCurrent<getNumRows()) {
        bResult=true;
        for (iCol=0;iCol<iNumCols;iCol++) {
          Value.bNull=PQgetisnull((PGresult *) Result,iCurrent,iCol);
          if (Value.bNull)
            Value.sValue="";
          else
            Value.sValue=PQgetvalue((PGresult *) Result,iCurrent,iCol);
          vResult.insert(vResult.end(),Value);
        }
        iCurrent++;
      } else bResult=false;
      break;
    #endif
    #ifdef USE_MYSQL
    case SQLXX_MYSQL:
      hMYSQLROW=mysql_fetch_row((MYSQL_RES *) Result);
      bResult=hMYSQLROW!=NULL;
      if (bResult) {
        for (iCol=0;iCol<iNumCols;iCol++) {
          Value.bNull=hMYSQLROW[iCol]==NULL;
          if (Value.bNull)
            Value.sValue="";
          else
            Value.sValue=hMYSQLROW[iCol];
          vResult.insert(vResult.end(),Value);
        }
      }
      break;
    #endif
    #ifdef USE_ODBC
    case SQLXX_ODBC:
      switch (SQLFetch((SQLHSTMT) Result)) {
        case SQL_ERROR:
          throw sqlxx_error("SQLFetch: "+getError());
          break;
        case SQL_INVALID_HANDLE:
          throw sqlxx_error("SQLFetch: Invalid statement handle");
          break;
        case SQL_NO_DATA_FOUND:
          bResult=false;
          break;
      }  
      if (bResult) {
        for (iCol=1;iCol<=iNumCols;iCol++) {
          sResult="";
          do {
            switch (iRet=SQLGetData((SQLHSTMT) Result,iCol,SQL_CHAR,pBuffer,
                iBufferSize,&iIndicator)) {
              case SQL_ERROR:
                throw sqlxx_error("SQLGetData: "+getError());
                break;
              case SQL_INVALID_HANDLE:
                throw sqlxx_error("SQLGetData: Invalid statement handle");
                break;
            }        
            sResult+=pBuffer;
          } while (iRet==SQL_SUCCESS_WITH_INFO);
          Value.bNull=iIndicator==SQL_NULL_DATA;
          if (Value.bNull)
            Value.sValue="";
          else
            Value.sValue=sResult;
          vResult.insert(vResult.end(),Value);
        }
      }
      break;
    #endif
    default:
      iCol=0;
      break;
  }
  TRACEOUTRET("CSQLResult::fetch",bResult)
  return bResult;
}


/** Returns column value as string.
    Returns the value of the column with index <b>iIndex</b> as string.
*/

string CSQLResult::get(const unsigned int iIndex) {
  TRACEINPARAM("CSQLResult::get",iIndex)
  if (!vResult.size())
    throw sqlxx_error("CSQLResult::get: No data available");
  if (iIndex>=iNumCols)
    throw sqlxx_error("CSQLResult::get: Invalid index: "+intstr(iIndex));
  TRACEOUTRET("CSQLResult::get",vResult[iIndex].sValue)
  return vResult[iIndex].sValue;
}


/** Returns column value as string.
    Returns the value of the column named <b>sField</b> as string.
*/

string CSQLResult::get(const string sField) {
  TRACEINPARAM("CSQLResult::get",sField)
  unsigned int iIndex;
  
  if (!vResult.size())
    throw sqlxx_error("CSQLResult::get: No data available");
  iIndex=0;
  while ((iIndex<iNumCols) && (vFields[iIndex]!=sField)) iIndex++;
  if (iIndex>=iNumCols)
    throw sqlxx_error("CSQLResult::get: Unknown field name: "+sField);
  TRACEOUTRET("CSQLResult::get",vResult[iIndex].sValue)
  return vResult[iIndex].sValue;
}


/** Returns column value as long integer.
    Returns the value of the column with index <b>iIndex</b> as long integer.
*/

long CSQLResult::getInt(const unsigned int iIndex) {
  int iValue;
  
  TRACEINPARAM("CSQLResult::getInt",iIndex)
  if (!vResult.size())
    throw sqlxx_error("CSQLResult::getInt: No data available");
  if (iIndex>=iNumCols)
    throw sqlxx_error("CSQLResult::getInt: Invalid index: "+intstr(iIndex));
  iValue=atoi(vResult[iIndex].sValue.c_str());
  TRACEOUTRET("CSQLResult::getInt",iValue)
  return iValue;
}


/** Returns column value as long integer.
    Returns the value of the column named <b>sField</b> as long integer.
*/

long CSQLResult::getInt(const string sField) {
  int iValue;
  unsigned int iIndex;
  
  TRACEINPARAM("CSQLResult::getInt",sField)
  if (!vResult.size())
    throw sqlxx_error("CSQLResult::getInt: No data available");
  iIndex=0;
  while ((iIndex<iNumCols) && (vFields[iIndex]!=sField)) iIndex++;
  if (iIndex>=iNumCols)
    throw sqlxx_error("CSQLResult::getInt: Unknown field name: "+sField);
  iValue=atoi(vResult[iIndex].sValue.c_str());
  TRACEOUTRET("CSQLResult::getInt",iValue)
  return iValue;
}


/** Returns column value as float.
    Returns the value of the column with index <b>iIndex</b> as long float.
*/

float CSQLResult::getFloat(const unsigned int iIndex) {
  float fValue;
  
  TRACEINPARAM("CSQLResult::getFloat",iIndex)
  if (!vResult.size())
    throw sqlxx_error("CSQLResult::getFloat: No data available");
  if (iIndex>=iNumCols)
    throw sqlxx_error("CSQLResult::getFloat: Invalid index: "+intstr(iIndex));
  fValue=atof(vResult[iIndex].sValue.c_str());
  TRACEOUTRET("CSQLResult::getFloat",fValue)
  return fValue;
}


/** Returns column value as float.
    Returns the value of the column named <b>sField</b> as long float.
*/

float CSQLResult::getFloat(const string sField) {
  float fValue;
  unsigned int iIndex;
  
  TRACEINPARAM("CSQLResult::getFloat",sField)
  if (!vResult.size())
    throw sqlxx_error("CSQLResult::getFloat: No data available");
  iIndex=0;
  while ((iIndex<iNumCols) && (vFields[iIndex]!=sField)) iIndex++;
  if (iIndex>=iNumCols)
    throw sqlxx_error("CSQLResult::getFloat: Unknown field name: "+sField);
  fValue=atof(vResult[iIndex].sValue.c_str());
  TRACEOUTRET("CSQLResult::getFloat",fValue)
  return fValue;
}


/** Determine if column value is NULL.
    Returns true if the value of the column with index <b>iIndex</b> is NULL.
    Returns false if not.
*/

bool CSQLResult::isNull(const unsigned int iIndex) {
  TRACEINPARAM("CSQLResult::isNull",iIndex)
  if (!vResult.size())
    throw sqlxx_error("CSQLResult::isNull: No data available");
  if (iIndex>=iNumCols)
    throw sqlxx_error("CSQLResult::isNull: Invalid index: "+intstr(iIndex));
  TRACEOUTRET("CSQLResult::isNull",vResult[iIndex].bNull)
  return vResult[iIndex].bNull;
}


/** Determine if column value is NULL.
    Returns true if the value of the column named <b>sField</b> is NULL.
    Returns false if not.
*/

bool CSQLResult::isNull(const string sField) {
  unsigned int iIndex;
  
  TRACEINPARAM("CSQLResult::isNull",sField)
  if (!vResult.size())
    throw sqlxx_error("CSQLResult::isNull: No data available");
  iIndex=0;
  while ((iIndex<iNumCols) && (vFields[iIndex]!=sField)) iIndex++;
  if (iIndex>=iNumCols)
    throw sqlxx_error("CSQLResult::isNull: Unknown field name: "+sField);
  TRACEOUTRET("CSQLResult::isNull",vResult[iIndex].bNull)
  return vResult[iIndex].bNull;
}


// === class CSQL ===================================================


/** SQL Constructor.
    This is the constructor of the class CSQL. It initializes the SQL
    object with empty values.
*/ 

CSQL::CSQL(void) {
  TRACEIN("CSQL::CSQL")
  sDSN="";
  sHostname="";
  sDatabase="";
  sDriver="";
  sUsername="";
  sPassword="";
  sOptions="";
  iPort=0;
  bConnected=false;
  #ifdef USE_ODBC
  iType=SQLXX_ODBC;
  #elif USE_MYSQL
  iType=SQLXX_MYSQL;
  #elif USE_POSTGRES
  iType=SQLXX_POSTGRES;
  #endif
  vResults.erase(vResults.begin(),vResults.end());
  TRACEOUT("CSQL::CSQL")
}


/** SQL Destructor.
    This is the destructor of the CSQL class. It automatically disconnects
    all open database connections and frees all allocated memories of all
    connected CSQLResult objects.
*/

CSQL::~CSQL(void) {
  TRACEIN("CSQL::~CSQL")
  disconnect();
  while (vResults.size()) {
    vResults[0]->close();
  }
  TRACEOUT("CSQL::~CSQL")
}


/** Get last error message.
    This method returns the last error message. If no parameter is given the
    last database related error message is returned. If a Pointer to a CSQLResult 
    object is specified, the last error message of this SQLResult-Object is
    returned.
    Don't use this method if possible. Catch the sqlxx_error exceptions which
    already contains the complete error message. 
*/

string CSQL::getError(void *Result) { 
  #ifdef USE_ODBC
  char cSQLState[15];
  SDWORD iNativeError;
  char cErrorMsg[256];
  SWORD iMsgLen;
  #endif
  string sResult;
    
  TRACEINPARAM("CSQL::getError",Result)
  switch(iType) {
    #ifdef USE_POSTGRES
    case SQLXX_POSTGRES:
      sResult="PostgreSQL error message: ";
      if (Result)
        sResult+=PQresultErrorMessage((PGresult *) Result);
      else
        sResult+=PQerrorMessage((PGconn *) DB);
      break;
    #endif
    #ifdef USE_MYSQL
    case SQLXX_MYSQL:
      sResult="MySQL error code: ";
      sResult+=intstr(mysql_errno((MYSQL *) DB));
      sResult+="\nMySQL error message: ";
      sResult+=mysql_error((MYSQL *) DB);
      break;
    #endif
    #ifdef USE_ODBC
    case SQLXX_ODBC:
      switch (SQLError((SQLHENV) Env,(SQLHDBC) DB,(SQLHSTMT) Result,
          (UCHAR *) cSQLState,&iNativeError,(UCHAR *) cErrorMsg,
          sizeof(cErrorMsg),&iMsgLen)) {
        case SQL_INVALID_HANDLE:
          throw sqlxx_error("SQLError: Invalid handle");
          break;
        case SQL_NO_DATA_FOUND:
          sResult="Empty";
          break;
        case SQL_SUCCESS:
          sResult="SQL error state code: ";
          sResult+=cSQLState;
          sResult+="\nError message: ";
          sResult+=cErrorMsg;
          sResult+="\nInternal error code: ";
          sResult+=intstr(iNativeError);
          break;
      }
      break;
    #endif
    default:
      sResult="Unknown database type";
      break;
  }
  TRACEOUTRET("CSQL::getError",sResult)
  return sResult;
}


/** Connects a CSQLResult to this CSQL object.
    This method is used internally to connect a CSQLResult object to this
    CSQL object. THe connected CSQLResult objects are freed automatically
    by the CSQL destructor.
*/

void CSQL::addResult(CSQLResult *Result) {
  TRACEINPARAM("CSQL::addResult",Result)
  vResults.push_back(Result);
  TRACEOUT("CSQL::addResult")
}


/** Disconnects a CSQLResult from this CSQL object.
    This method is used internally to disconnect a CSQLResult object from 
    this CSQL object. THe connected CSQLResult objects are freed 
    automatically by the CSQL destructor.
*/

void CSQL::delResult(CSQLResult *Result) {
  vector< CSQLResult *>::iterator IPos;

  TRACEINPARAM("CSQL::delResult",Result)  
  IPos=find(vResults.begin(),vResults.end(),Result);
  if (IPos!=vResults.end()) vResults.erase(IPos);
  TRACEOUT("CSQL::delResult")
}


/** Sets DSN.
    Sets the DSN to <b>sNewDSN</b>. The DSN is only used by ODBC connections.
*/

void CSQL::setDSN(const string &sNewDSN) {
  TRACEINPARAM("CSQL::setDSN",sNewDSN)
  sDSN=sNewDSN;
  TRACEOUT("CSQL::setDSN")
}


/** Sets Hostname.
    Sets Hostname to <b>sNewHostname</b>.
*/

void CSQL::setHostname(const string &sNewHostname) {
  TRACEINPARAM("CSQL::setHostname",sNewHostname)
  sHostname=sNewHostname;
  TRACEOUT("CSQL::setHostname")
}


/** Sets Socket.
    Sets Socket to <b>sNewSocket</b>.
*/

void CSQL::setSocket(const string &sNewSocket) {
  TRACEINPARAM("CSQL::setSocket",sNewSocket)
  sSocket=sNewSocket;
  TRACEOUT("CSQL::setSocket")
}


/** Sets Port.
    Sets Ports to <b>sNewPort</b>. Use this method if you want to
    specify the port address as a string.
*/

void CSQL::setPort(const string &sNewPort) {
  TRACEINPARAM("CSQL::setPort",sNewPort)
  iPort=atoi(sNewPort.c_str());
  TRACEOUT("CSQL::setPort")
}


/** Sets Port.
    Sets Ports to <b>sNewPort</b>. Use this method if you want to
    specify the port address as an integer.
*/

void CSQL::setPort(const unsigned int iNewPort) {
  TRACEINPARAM("CSQL::setPort",iNewPort)
  iPort=iNewPort;
  TRACEOUT("CSQL::setPort")
}


/** Sets database name.
    Sets database name to <b>sNewDatabase</b>.
*/

void CSQL::setDatabase(const string &sNewDatabase) {
  TRACEINPARAM("CSQL::setDatabase",sNewDatabase)
  sDatabase=sNewDatabase;
  TRACEOUT("CSQL::setDatabase")
}


/** Sets ODBC driver.
    Sets odbc driver to <b>sNewDriver</b>. This value is only used for
    ODBC connections
*/

void CSQL::setDriver(const string &sNewDriver) {
  TRACEINPARAM("CSQL::setDriver",sNewDriver)
  sDriver=sNewDriver;
  TRACEOUT("CSQL::setDriver")
}


/** Sets database username.
    Sets database username to <b>sNewUsername</b>.
*/

void CSQL::setUsername(const string &sNewUsername) {
  TRACEINPARAM("CSQL::setUsername",sNewUsername)
  sUsername=sNewUsername;
  TRACEOUT("CSQL::setUsername")
}


/** Sets database password.
    Sets database password to <b>sNewPassword</b>.
*/

void CSQL::setPassword(const string &sNewPassword) {
  TRACEINPARAM("CSQL::setPassword",sNewPassword)
  sPassword=sNewPassword;
  TRACEOUT("CSQL::setPassword")
}


/** Sets database options.
    Sets database options to <b>sNewOptions</b>. These options are only
    used by ODBC connections. Your ODBC driver documentation will tell you
    what options are possible.
*/

void CSQL::setOptions(const string &sNewOptions) {
  TRACEINPARAM("CSQL::setOptions",sNewOptions)
  sOptions=sNewOptions;
  TRACEOUT("CSQL::setOptions")
}


/** Sets database type.
    Sets database type to <b>iNewType</b>. This can be SQLXX_MYSQL, 
    SQLXX_POSTGRES or SQLXX_ODBC. But this is dependent on how you have
    compiled the sqlxx library.
*/
    
void CSQL::setType(const unsigned short iNewType) {
  TRACEINPARAM("CSQL::setType",iNewType)
  #ifndef USE_MYSQL
  if (iNewType==SQLXX_MYSQL)
    throw sqlxx_error("Native mysql support is not compiled in");
  #endif
  #ifndef USE_POSTGRES
  if (iNewType==SQLXX_POSTGRES)
    throw sqlxx_error("Native postgres support is not compiled in");
  #endif
  #ifndef USE_ODBC
  if (iNewType==SQLXX_ODBC)
    throw sqlxx_error("ODBC support is not compiled in");
  #endif
  iType=iNewType;
  TRACEOUT("CSQL::setType")
}


/** Returns DSN.
    Returns the DSN.
*/

string CSQL::getDSN(void) {
  TRACEIN("CSQL::getDSN")
  TRACEOUTRET("CSQL::getDSN",sDSN);
  return sDSN;
}


/** Returns Hostname.
    Returns the Hostname.
*/

string CSQL::getHostname(void) {
  TRACEIN("CSQL::getHostname")
  TRACEOUTRET("CSQL::getHostname",sHostname);
  return sHostname;
}


/** Returns Socket.
    Returns the Socket.
*/

string CSQL::getSocket(void) {
  TRACEIN("CSQL::getSocket")
  TRACEOUTRET("CSQL::getSocket",sSocket);
  return sSocket;
}


/** Returns Port.
    Returns the port address as string.
*/

string CSQL::getPort(void) {
  TRACEIN("CSQL::getPort")
  TRACEOUTRET("CSQL::getPort",iPort);
  return intstr(iPort);
}


/** Returns Port.
    Returns the port address as unsigned integer.
*/

unsigned int CSQL::getPortN(void) {
  TRACEIN("CSQL::getPort")
  TRACEOUTRET("CSQL::getPort",iPort);
  return iPort;
}


/** Returns database name.
    Returns the database name.
*/

string CSQL::getDatabase(void) {
  TRACEIN("CSQL::getDatabase")
  TRACEOUTRET("CSQL::getDatabase",sDatabase);
  return sDatabase;
}


/** Returns ODBC driver.
    Returns the filename to the ODBC driver.
*/

string CSQL::getDriver(void) {
  TRACEIN("CSQL::getDriver")
  TRACEOUTRET("CSQL::getDriver",sDriver);
  return sDriver;
}


/** Returns database username.
    Returns the database username.
*/

string CSQL::getUsername(void) {
  TRACEIN("CSQL::getUsername")
  TRACEOUTRET("CSQL::getUsername",sUsername);
  return sUsername;
}


/** Returns database password.
    Returns the database password.
*/

string CSQL::getPassword(void) {
  TRACEIN("CSQL::getPassword")
  TRACEOUTRET("CSQL::getPassword",sPassword);
  return sPassword;
}


/** Returns ODBC options.
    Returns the ODBC options.
*/

string CSQL::getOptions(void) {
  TRACEIN("CSQL::getOptions")
  TRACEOUTRET("CSQL::getOptions",sOptions);
  return sOptions;
}


/** Returns database type.
    Returns the database type.
*/

unsigned short CSQL::getType(void) {
  TRACEIN("CSQL::getType")
  TRACEOUTRET("CSQL::getType",iType);
  return iType;
}


/** Connect to database.
    This method inititates the connection to the database.
*/

void CSQL::connect(void) {
  #ifdef USE_ODBC
  string sDataSource;
  short iBufLen;
  char cBuffer[257];
  #endif
  #ifdef USE_MYSQL
  char *cHostname;
  char *cPassword;
  char *cSocket;
  #endif
  #if defined(USE_MYSQL) || defined(USE_POSTGRES)
  char *cDatabase;
  #endif

  TRACEIN("CSQL::connect")
  if (bConnected) throw sqlxx_error("Already connected to database");
  switch(iType) {
    #ifdef USE_POSTGRES
    case SQLXX_POSTGRES:
      if (sDatabase!="")
        cDatabase=(char *) sDatabase.c_str();
      else
        cDatabase=NULL;
      if (sSocket!="" || sHostname=="" || !iPort)
        DB=(void *) PQsetdbLogin(NULL,0,
          NULL,NULL,cDatabase,sUsername.c_str(),sPassword.c_str());
      else
        DB=(void *) PQsetdbLogin(sHostname.c_str(),intstr(iPort).c_str(),
          NULL,NULL,cDatabase,sUsername.c_str(),sPassword.c_str());
      if (PQstatus((PGconn *) DB)==CONNECTION_BAD)
        throw sqlxx_error("PQsetdbLogin: "+getError());
      break;
    #endif
    #ifdef USE_MYSQL
    case SQLXX_MYSQL:
      DB=(void *) mysql_init(NULL);
      if (!DB)
        throw sqlxx_error("mysql_init: Unable to initialize mysql handle");
      if (sHostname!="")
        cHostname=(char *) sHostname.c_str();
      else
        cHostname=NULL;
      if (sPassword!="")
        cPassword=(char *) sPassword.c_str();
      else
        cPassword=NULL;
      if (sSocket!="")
        cSocket=(char *) sSocket.c_str();
      else
        cSocket=NULL;
      if (sDatabase!="")
        cDatabase=(char *) sDatabase.c_str();
      else
        cDatabase=NULL;
      if (!mysql_real_connect((MYSQL *) DB,cHostname,sUsername.c_str(),
          cPassword,cDatabase,iPort,
          cSocket,0))
        throw sqlxx_error("mysql_real_connect: "+getError());
      break;
    #endif
    #ifdef USE_ODBC
    case SQLXX_ODBC:
      switch (SQLAllocEnv(&Env)) {
        case SQL_ERROR:
          throw sqlxx_error("SQLAllocEnv: Internal error: "+getError());
          break;
        case SQL_INVALID_HANDLE:
          throw sqlxx_error("SQLAllocEnv: Invalid environment handle");
          break;
      }
      switch (SQLAllocConnect((SQLHENV) Env,&DB)) {
        case SQL_ERROR:
          throw sqlxx_error("SQLAllocConnect: "+getError());
          break;
        case SQL_INVALID_HANDLE:
          throw sqlxx_error("SQLAllocConnect: Invalid environment handle");
          break;
      }
      sDataSource="";
      if (sDriver=="")
        if (sDSN!="")
          sDataSource+="DSN="+sDSN+";";
        else
          sDataSource+="DSN="+sDatabase+";";
      else
        sDataSource+="DRIVER="+sDriver+";";
      if (sSocket!="")
        sDataSource+="SOCKET="+sSocket+";";
      else {
        if (sHostname!="") sDataSource+="SERVER="+sHostname+";";
        if (iPort) sDataSource+="PORT="+intstr(iPort)+";";
      }
      if (sUsername!="") sDataSource+="UID="+sUsername+";";
      if (sPassword!="") sDataSource+="PWD="+sPassword+";";
      if (sDatabase!="") sDataSource+="DATABASE="+sDatabase+";";
      if (sDriver!="") sDataSource+="DSN="+sDatabase+";";
      sDataSource+="ReadOnly=0;";
      if (sOptions!="") sDataSource+=sOptions;
      switch (SQLDriverConnect((SQLHDBC) DB,0,(UCHAR *) sDataSource.c_str(), SQL_NTS,
              (UCHAR *) cBuffer,sizeof(cBuffer),&iBufLen,SQL_DRIVER_COMPLETE)) {
        case SQL_ERROR:
          throw sqlxx_error("SQLDriverConnect: "+getError());
          break;
        case SQL_INVALID_HANDLE:
          throw sqlxx_error("SQLDriverConnect: Invalid database handle");
          break;
      }
      break;
    #endif
    default:
      break;
  }
  bConnected=true;
  TRACEOUT("CSQL::connect")
}


/** Disconnects from database.
    This methode closes the connection to the database.
*/

void CSQL::disconnect(void) {
  TRACEIN("CSQL::disconnect")
  if (bConnected) {
    switch(iType) {
      #ifdef USE_POSTGRES
      case SQLXX_POSTGRES:
        PQfinish((PGconn *) DB);
        break;
      #endif
      #ifdef USE_MYSQL
      case SQLXX_MYSQL:
        mysql_close((MYSQL *) DB);
        break;
      #endif
      #ifdef USE_ODBC
      case SQLXX_ODBC:
        SQLDisconnect((SQLHDBC) DB);
        if (DB) SQLFreeConnect((SQLHDBC) DB);
        if (Env) SQLFreeEnv((SQLHENV) Env);
        break;
      #endif
      default:
        break;
    }
    bConnected=false;
  }
  TRACEOUT("CSQL::disconnect")
}


/** Determine if connected.
    This method returns true if the database connection is established.
    False if not.
*/

bool CSQL::isConnected(void) {
  TRACEIN("CSQL::isConnected")
  TRACEOUTRET("CSQL::isConnected",bConnected)
  return bConnected;
}


/** Opens an SQL Query.
    This method opens a new SQL query <b>sQuery</b> and returns a pointer to
    a CSQLResult object.
*/

CSQLResult *CSQL::openQuery(const string &sQuery, const int iBufferSize) {
  CSQLResult *DBResult;
  
  TRACEINPARAM("CSQL::openQuery",format("%s,%i",sQuery.c_str(),iBufferSize))
  DBResult=new CSQLResult(this,iBufferSize);
  DBResult->query(sQuery);
  TRACEOUTRET("CSQL::openQuery",DBResult)
  return DBResult;
}


/** Execute SQL query.
    This method executes the SQL query <b>sQuery</b>.
*/

void CSQL::execQuery(const string &sQuery) {
  CSQLResult *DBResult;
  
  TRACEINPARAM("CSQL::execQuery",sQuery);
  DBResult=new CSQLResult(this);
  DBResult->query(sQuery);
  TRACEOUTRET("CSQL::execQuery",DBResult)
  delete DBResult;
}


/** Closes SQL query.
    Closes the SQL query <b>DBResult</b>.
*/

void CSQL::closeQuery(const CSQLResult *DBResult) {
  TRACEINPARAM("CSQL::closeQuery",DBResult)
  delete DBResult;
  TRACEOUT("CSQL::closeQuery")
}


/** Returns next available Primary key.
    This method returns the next available primary key value for the field
    <b>sIDField</b> in table <b>sTable</b>
*/

long CSQL::getNewID(const string &sTable, const string &sIDField) {
  CSQLResult *DBResult;
  long iID;
  
  TRACEINPARAM("CSQL::getNewID",format("%s,%s",sTable.c_str(),sIDField.c_str()))
  DBResult=openQuery(format(
    "SELECT MAX(%s) "
    "FROM %s",sIDField.c_str(),sTable.c_str()
  ));
  DBResult->fetch();
  iID=DBResult->getInt(0)+1;
  closeQuery(DBResult);
  TRACEOUTRET("CSQL::getNewID",iID)
  return iID;
}


/** Returns vector with active queries.
    This method returns a vector with pointers to all active queries of 
    this CSQL object.
*/

vector< CSQLResult * > &CSQL::getQueries(void) {
  TRACEIN("CSQL::getQueries")
  TRACEOUTRET("CSQL::getQueries",&vResults)
  return vResults;
}


/** Count active queries.
    Returns the number of active SQL queries of this CSQL object.
*/

int CSQL::countQueries(void) {
  int iResult;
  
  TRACEIN("CSQL::countQueries")
  iResult=vResults.size();
  TRACEOUTRET("CSQL::countQueries",iResult)
  return iResult;
}


/** Quotes a string.
    Quotes the string sSource and returns this new quoted string.  This
    method uses the addSlashes function of the strutilsxx library so there
    are another two optional parameters: bEscape8Bit, sEscape. They are
    relayed to the addSlashes function and are explained in the strutilsxx
    documentation.
*/

string quote(const string &sSource, const bool bEscape8Bit, 
    const string &sEscape) {
  string sResult;
  
  FTRACEINPARAM("quote",format("%s,%i,%s",sSource.c_str(),bEscape8Bit,sEscape.c_str()))
  sResult = "'" + addSlashes(sSource, bEscape8Bit, sEscape) + "'";
  FTRACEOUTRET("CSQL::quote",sResult)
  return sResult;
}

}
