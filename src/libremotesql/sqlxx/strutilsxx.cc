/*************************************************************************
* libstrutilsxx                                                          *
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
* Lesser General Public License for more details.                        *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with this library; if not, write to the Free Software    *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307    *
* USA                                                                    *
*************************************************************************/

#include "strutilsxx.h"
#include <cstdio>
#include <sys/types.h>
#ifdef HAVE_GNUREGEX_H
#include <gnuregex.h>
#else
//#include <regex.h>
#endif
#include <cstdarg>
#include <string.h>
#include <stdlib.h>

using namespace std;

namespace strutilsxx {

/** @name strutilsxx
    This library is a collection of useful additional C++ string processing
    functions like trimming, replacing, formating, etc.
    @version 0.7.0
    @author Klaus Reimer <k@ailis.de>
*/

// =======================================================================

/** Right trim a string.
    This function removes all spaces and tabs on the right side of the
    string <b>sString</b> and returns the result.
*/

string trimRight(const string &sString) {
  string::size_type iPos;
  
  iPos=sString.find_last_not_of("\t ");
  switch (iPos) {
    case string::npos:
      return "";
      break;
    case 0:
      return sString;
      break;
    default:
      return sString.substr(0,iPos+1);  
  }
}


/** Left trim a string.
    This function removes all spaces and tabs on the left side of the
    string <b>sString</b> and returns the result.
*/

string trimLeft(const string &sString) {
  string::size_type iPos;
  
  iPos=sString.find_first_not_of("\t ");
  switch (iPos) {
    case string::npos:
      return "";
      break;
    case 0:
      return sString;
      break;
    default:
      return sString.substr(iPos);  
  }
}


/** Trim a string.
    This function removes all spaces and tabs on the left side and on the
    right side of the string <b>sString</b> and returns the result.
*/

string trim(const string &sString) {
  return trimRight(trimLeft(sString));
}


/** Remove the enclosing quote characters from a string.
    This function removes the enclosing quote characters from the
    string <b>sString</b> and returns the result.
*/

string stripQuotes(const string &sString) {
  if (sString.length()>1) {
    if ((sString[sString.length()-1]=='"') && (sString[0]=='"'))
      return sString.substr(1,sString.length()-2);
    else
      return sString;
  } else return sString;
}


/** Replace parts in a string.
    This function can be used to replace all occurences of the sub string 
    <b>sOld</b> in the string <b>sString</b> with the new sub string
    <b>sNew</b>. The function returns the resulting string.
*/

string replace(const string &sString, const string &sOld, const string &sNew) {
  int iPos, iIndex;
  string sNewString;
  
  sNewString=sString;
  iIndex=0;
  while ((iPos=sNewString.find(sOld,iIndex))>=0) {
    sNewString.replace(iPos,sOld.length(),sNew);
    iIndex=iPos+sNew.length();
  }
  return sNewString;
}


/** Add Slashes to special characters.
    This function adds backslashes to all special characters in the
    string <b>sSource</b> that needs escaping. If <b>bEscape8Bit</b> is
    set to <i>true</i> then all characters greater than character number 127
    are escaped by backslashes, too. You can define more characters to be
    escaped in the string <b>sEscape</b>. The function returns the 
    resulting string.
*/

string addSlashes(const string &sSource, const bool bEscape8Bit, const string &sEscape) {
  char sOctal[4];
  string sDest;
  string::size_type iCount;
  
  sDest="";
  for (iCount=0;iCount<sSource.size();iCount++) {
    if (((unsigned char) sSource[iCount]<32) 
      || ((bEscape8Bit) && ((unsigned char) sSource[iCount]>126))
      || (sEscape.find(sSource[iCount])!=string::npos)) {
      sDest+='\\';
      switch (sSource[iCount]) {
        case '\n': sDest+='n'; break;
        case '\t': sDest+='t'; break;
        case '\r': sDest+='r'; break;
        case '\a': sDest+='a'; break;
        case '\v': sDest+='v'; break;
        case '\b': sDest+='b'; break;
        case '\f': sDest+='f'; break;
        default: 
          sprintf(sOctal,"%03o",(unsigned char) sSource[iCount]);
          sDest+=sOctal;
      }
    }
    else {
      switch ((unsigned char) sSource[iCount]) {
        case '\\':
          sDest+="\\\\";
          break;
        case '"':
          sDest+="\\\"";
          break;
        case '\'':
          sDest+="\\\'";
          break;
        default:
          sDest+=sSource[iCount];
      }
    }
  }  
  return sDest;
}


/** Parse string with escaped special characters.
    This function converts all escaped characters in the string
    <b>sSource</b> and returns the converted string.
*/

string stripSlashes(const string &sSource) {
  unsigned char cNum;
  string::size_type iCount;
  int iNum;
  string sDest;
  
  sDest="";
  for(iCount=0;iCount<sSource.size();iCount++) {
    if (sSource[iCount]=='\\') {
      iCount++;
      switch (sSource[iCount]) {
        case 'n': sDest+='\n'; break;
        case 't': sDest+='\t'; break;
        case 'r': sDest+='\r'; break;
        case 'a': sDest+='\a'; break;
        case 'v': sDest+='\v'; break;
        case 'b': sDest+='\b'; break;
        case 'f': sDest+='\f'; break;
        case '\\': sDest+='\\'; break;
        case '"': sDest+='"'; break;
        case '\'': sDest+='\''; break;
        default:
          cNum=0;
          iNum=0;
          while ((iNum<3) && (sSource[iCount]>='0') && (sSource[iCount]<='7')) {
            cNum<<=3;
            cNum+=sSource[iCount]-48;
            iCount++;
            iNum++;                          
          }
          iCount--;
          sDest+=cNum;          
      }
    }
    else
      sDest+=sSource[iCount];    
  }
  return sDest;
}


/** Remove carriage return and line feed characters.
    This function removes all carriage return and line feed characters from
    the string <b>sSource</b> and returns the resulting string.
*/

string stripNewLines(const string &sSource) {
  int iPos;
  string sDest;
  
  sDest=sSource;
  iPos=0;
  while ((iPos=sDest.find_first_of("\r\n",iPos))>=0)
    sDest.erase(iPos,1);
  return sDest;
}


/** Convert string to upper-case.
    This function converts all characters of the string <b>sString</b>
    to upper-case and returns the resulting string.
*/

string upperstr(const string &sString) {
  string sNewString;
  unsigned int iCount;
  
  sNewString=sString;
  for (iCount=0;iCount<sNewString.length();iCount++)
    sNewString[iCount]=toupper(sNewString[iCount]);
  return sNewString;
}



/** Convert string to lower-case.
    This function converts all characters of the string <b>sString</b>
    to lower-case and returns the resulting string.
*/

string lowerstr(const string &sString) {
  string sNewString;
  unsigned int iCount;
  
  sNewString=sString;
  for (iCount=0;iCount<sNewString.length();iCount++)
    sNewString[iCount]=tolower(sNewString[iCount]);
  return sNewString;
}


/** Convert interger to string.
    This function converts the integer <b>iValue</b> to a string and returns
    it. Optionally you can specify a different numeric base than 10 with the
    parameter <b>iRadix</b> and you can use the <b>iLen</b> Parameter to
    specify the length of the resulting string (filled with zeroes). For
    example: If you convert the value <i>254</i> with base <i>16</i> and
    length <i>4</i> you get a string value of <i>00fe</i>.
*/

string intstr(const long iValue, const int iRadix, const int iLen) {
  unsigned long iTempValue;
  int iCount;
  string sResult;
  
  if (iValue>=0) {
    sResult="";
    iCount=0;
    iTempValue=iValue;
  }
  else {
    iCount=1;
    iTempValue=-iValue;
  }
  while (iTempValue || iCount<iLen) {
    if ((iTempValue%iRadix)<10)
      sResult.insert(sResult.begin(),48+iTempValue%iRadix);
    else
      sResult.insert(sResult.begin(),55+iTempValue%iRadix);
    iTempValue=iTempValue/iRadix;
    iCount++;
  }
  if (iValue<0) sResult="-"+sResult;
  return sResult;
}


/** Convert unsigned interger to string.
    This function converts the unsigned integer <b>iValue</b> to a string
    and returns it. Optionally you can specify a different numeric base than
    10 with the parameter <b>iRadix</b> and you can use the <b>iLen</b>
    Parameter to specify the length of the resulting string (filled with
    zeroes). For example: If you convert the value <i>254</i> with base
    <i>16</i> and length <i>4</i> you get a string value of <i>00fe</i>.
*/

string uintstr(const unsigned long iValue, const int iRadix, const int iLen) {
  unsigned long iTempValue;
  int iCount;
  string sResult;
  
  sResult="";
  iCount=0;
  iTempValue=iValue;
  while (iTempValue || iCount<iLen) {
    if ((iTempValue%iRadix)<10)
      sResult.insert(sResult.begin(),48+iTempValue%iRadix);
    else
      sResult.insert(sResult.begin(),55+iTempValue%iRadix);
    iTempValue=iTempValue/iRadix;
    iCount++;
  }
  return sResult;
}


/** Format a string.
    This function is similiar to the printf() command in C. The first
    parameter is a format string and all other parameters are values to
    be included in the format string. The formated string is returned.
*/

string format(const char *sString, ...) {
  va_list args;
  string sResult;
  char *pBuffer;
  FILE *hFile;
  int iSize;
  
  va_start(args,sString);
  hFile=fopen("/dev/null","wt");
  iSize=vfprintf(hFile,sString,args);
  fclose(hFile);
  pBuffer=(char *) malloc(iSize+1);
  vsprintf(pBuffer,sString,args);
  va_end(args);
  sResult=pBuffer;
  free(pBuffer);
  return sResult;
}


/** Converts a custom separated string into a string vector.
    This function separates all items in <b>sString</b> which are separated
    by <b>sSeparator</b> and stores these items in a string vector which is
    returned. You can use this function to convert a comma-separated string
    into a list of strings.
*/

vector< string > explode(const string &sSeparator, const string &sString) {
  vector< string > slPieces;
  string::size_type iBegin, iEnd;
  
  iBegin=0;
  while ((iEnd=sString.find(sSeparator,iBegin))!=string::npos) {
    slPieces.insert(slPieces.end(),sString.substr(iBegin,iEnd-iBegin));
    iBegin=iEnd+sSeparator.size();
  }
  slPieces.insert(slPieces.end(),sString.substr(iBegin));
  return slPieces;
}


/** Converts a custom separated string into a string vector.
    This function is doing the same as the function <i>explode</i> but this
    function honors quotes and escape characters. Separators in quotes or
    escaped separator-characters are not identified as separators.
*/

vector< string > explodeQuoted(const string &sSeparator, const string &sString) {
  vector< string > slPieces;
  string sItem;
  string::size_type iBegin;
  string::size_type iSeparator, iDblQuote, iBackSlash;
  bool bComplete;
  
  iBegin=0;
  sItem="";
  while (iBegin<sString.size()) {
    iSeparator=sString.find(sSeparator,iBegin);  
    iDblQuote=sString.find('"',iBegin);
    iBackSlash=sString.find('\\',iBegin);
    if ((iSeparator<iDblQuote) && (iSeparator<iBackSlash)) {
      sItem+=sString.substr(iBegin,iSeparator-iBegin);
      slPieces.insert(slPieces.end(),sItem);
      sItem="";
      iBegin=iSeparator+sSeparator.size();
    }
    else if ((iDblQuote<iSeparator) && (iDblQuote<iBackSlash)) {
      sItem+=sString.substr(iBegin,iDblQuote-iBegin);
      iBegin=iDblQuote+1;
      bComplete=false;
      while (!bComplete) {
        iDblQuote=sString.find('"',iBegin);
        iBackSlash=sString.find('\\',iBegin);
        if (iBackSlash<iDblQuote) {
          sItem+=stripSlashes(sString.substr(iBegin,iBackSlash-iBegin+2));
          iBegin=iBackSlash+2;
        }
        else {
          bComplete=true;
          sItem+=sString.substr(iBegin,iDblQuote-iBegin);
          iBegin=iDblQuote+1;
        }
      }
    }
    else if ((iBackSlash<iSeparator) && (iBackSlash<iDblQuote)) {
      sItem+=stripSlashes(sString.substr(iBegin,iBackSlash-iBegin+2));
      iBegin=iBackSlash+2;
    }
    else {
      sItem+=sString.substr(iBegin);
//      if (sItem!="") slPieces.insert(slPieces.end(),sItem);
      iBegin=sString.size();
    }
  }
  slPieces.insert(slPieces.end(),sItem);
  return slPieces;
}


/** Converts a string vector into a custom separated string.
    This function takes all items from the string vector <b>slPieces</b>
    and glues them together with the string <b>sGlue</b>. The resulting
    string is returned.
*/

string implode(const string &sGlue, const vector< string > &slPieces) {
  string sResult;
  vector< string >::size_type iCount;
  
  for (iCount=0;iCount<slPieces.size();iCount++) {
    sResult+=slPieces[iCount];
    if (iCount<slPieces.size()-1) sResult+=sGlue;
  }
  return sResult;
}


/** Check string against regular expression.
    This function checks if the string <b>sString</b> matches the
    regular expression <b>sPattern</b> and returns the boolean result.
*/

bool matchRegex(const string &sString, const string &sPattern) {
#ifdef HAVE_GNUREGEX_H
  regex_t RBuffer;

  re_syntax_options=RE_SYNTAX_EGREP;
  RBuffer.buffer=0;
  RBuffer.allocated=0;
  RBuffer.translate=0;
  RBuffer.fastmap=0;
  re_compile_pattern(sPattern.c_str(),sPattern.size(),&RBuffer);
  return re_match(&RBuffer,sString.c_str(),sString.size(),0,NULL)>=0;
#else
  return false;
#endif
}


/** Read text file backward.
    This function reads a line from the file <b>IFile</b> and stores the
    line in <b>sLine</b>. <b>cDelimiter</b> defaults to a carriage return
    character and can be changed to use an other line delimiter. The
    different to the STL-function getline() is that this function is reading
    backward. If you open a file and seek to the end of the file you can use
    this function to read line by line from the end to the beginning.
*/

istream &rgetline (istream &IFile, string &sLine, char cDelimiter) {
  const int iMaxBufferSize=32;
  streampos iCur;
  int iBufferSize, iIndex;
  char pBuffer[iMaxBufferSize+1];

  sLine="";
  iCur=IFile.tellg();
  if (!iCur) {
    IFile.setstate(ios::failbit|ios::eofbit);  
    return IFile;
  }
  IFile.seekg(-1,ios::cur);
  IFile.read((char *) &pBuffer,1);
  if (pBuffer[0]==cDelimiter) IFile.seekg(-1,ios::cur);
  while ((int) IFile.tellg()) {
    if (IFile.tellg()>iMaxBufferSize) {
      IFile.seekg(-iMaxBufferSize,ios::cur);
      iBufferSize=iMaxBufferSize;
    }
    else {
      iBufferSize=IFile.tellg();
      IFile.seekg(0);
    }
    IFile.read((char *) &pBuffer,iBufferSize);
    pBuffer[iBufferSize]=0;
    iIndex=iBufferSize-1;
    while ((pBuffer[iIndex]!=cDelimiter) && (iIndex>=0)) iIndex--;
    if (iIndex>=0) {
      sLine=&pBuffer[iIndex+1]+sLine;
      IFile.seekg(-iBufferSize+iIndex+1,ios::cur);
      return IFile;
    }
    else {
      sLine=pBuffer+sLine;
      IFile.seekg(-iBufferSize,ios::cur);
    }
  }
  return IFile;
}

}
