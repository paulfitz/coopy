#ifndef CSVSHEET_COMPARE_INC
#define CSVSHEET_COMPARE_INC

#include "CsvSheet.h"


class CsvCompare {
private:
  CsvSheet cmp;
public:
  void compare(CsvSheet& a, CsvSheet& b);

  const CsvSheet& get() { return cmp; }
};


class CsvCompare3 {
private:
  CsvSheet cmp;
public:
  void compare(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote);

  const CsvSheet& get() { return cmp; }
};

#endif
