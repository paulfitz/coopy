#ifndef COOPY_COOPYHXTABLE_INC
#define COOPY_COOPYHXTABLE_INC

#include <string>

class CoopyhxTable {
public:
  virtual int width() = 0;
  virtual int height() = 0;
  virtual std::string getCell(int x, int y, bool& isNull) = 0;
  virtual int getOffset() = 0;
};

#endif
