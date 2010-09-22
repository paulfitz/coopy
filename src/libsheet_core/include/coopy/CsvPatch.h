#ifndef CSVSHEET_PATCH_INC
#define CSVSHEET_PATCH_INC

#include <coopy/CsvSheet.h>

class CsvPatch {
private:
  coopy::store::CsvSheet result;
public:
  void apply(coopy::store::CsvSheet& original, 
	     coopy::store::CsvSheet& patch);

  const coopy::store::CsvSheet& get() { return result; }
};

#endif
