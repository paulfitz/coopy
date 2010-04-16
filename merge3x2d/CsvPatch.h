#ifndef CSVSHEET_PATCH_INC
#define CSVSHEET_PATCH_INC

#include "CsvSheet.h"

class CsvPatch {
private:
  CsvSheet result;
public:
  void apply(CsvSheet& original, CsvSheet& patch);

  const CsvSheet& get() { return result; }
};

#endif
