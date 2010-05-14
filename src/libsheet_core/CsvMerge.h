#ifndef SSFOSSIL_CSVMERGE
#define SSFOSSIL_CSVMERGE

#include "CsvSheet.h"

class CsvMerge {
private:
  CsvSheet work;
public:
  int apply(CsvSheet& pivot, CsvSheet& v1, CsvSheet& v2);

  void dumb_conflict(CsvSheet& local, CsvSheet& remote);

  CsvSheet& get() { return work; }

  static int run_tests();
};

#endif
