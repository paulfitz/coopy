#ifndef CSVSHEET_COMPARE_INC
#define CSVSHEET_COMPARE_INC

#include "CsvSheet.h"

class CsvCompare {
private:
  IntSheet rows_b2a, rows_a2b;
public:
  void compare(CsvSheet& a, CsvSheet& b);

  int b2a(int y) {
    return rows_b2a.cell(0,y);
  }

  int a2b(int y) {
    return rows_a2b.cell(0,y);
  }
};


class CsvCompare3 {
private:
  CsvSheet cmp;
public:
  void compare(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote);

  const CsvSheet& get() { return cmp; }
};

#endif
