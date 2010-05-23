#ifndef COOPY_DATASHEET
#define COOPY_DATASHEET

#include <coopy/SheetStyle.h>

#include <string>

class DataSheet {
public:
  virtual int width() const = 0;
  virtual int height() const = 0;

  virtual std::string cellString(int x, int y) const = 0;

  std::string encode(const std::string& delim = ",") const;

  static std::string encodeCell(const std::string& str, 
				const std::string& delim);

};


#endif
