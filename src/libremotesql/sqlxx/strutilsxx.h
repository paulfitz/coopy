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

#ifndef STRUTILSXX_H
#define STRUTILSXX_H

#include <string>
#include <vector>
#include <iostream>

namespace strutilsxx {

std::string trimRight(const std::string &sString);
std::string trimLeft(const std::string &sString);
std::string trim(const std::string &sString);
std::string stripQuotes(const std::string &sString);
std::string replace(const std::string &sString, const std::string &sOld, const std::string &sNew);
std::string addSlashes(const std::string &sSource, const bool bEscape8Bit=false, const std::string &sEscape="");
std::string stripSlashes(const std::string &sSource);
std::string stripNewLines(const std::string &sSource);
std::string upperstr(const std::string &sString);
std::string lowerstr(const std::string &sString);
std::string intstr(const long iValue, const int iRadix=10, const int iLen=1);
std::string uintstr(const unsigned long iValue, const int iRadix=10, const int iLen=1);
std::string format(const char *sString, ...);
std::vector< std::string > explode(const std::string &sSeparator, const std::string &sString);
std::vector< std::string > explodeQuoted(const std::string &sSeparator, const std::string &sString);
std::string implode(const std::string &sGlue, const std::vector< std::string > &slPieces);
bool matchRegex(const std::string &sString, const std::string &sPattern);
std::istream &rgetline (std::istream &IFile, std::string &sLine, char cDelimiter='\n');

}

#endif
