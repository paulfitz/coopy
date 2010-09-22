#ifndef SSFOSSIL_CSVMERGE
#define SSFOSSIL_CSVMERGE

#include <coopy/CsvSheet.h>

namespace coopy {
  namespace cmp {
    class CsvMerge;
  }
}

class coopy::cmp::CsvMerge {
private:
  coopy::store::CsvSheet work;
public:
  int apply(coopy::store::CsvSheet& pivot, 
	    coopy::store::CsvSheet& v1, 
	    coopy::store::CsvSheet& v2);

  void dumb_conflict(coopy::store::CsvSheet& local, 
		     coopy::store::CsvSheet& remote);

  coopy::store::CsvSheet& get() { return work; }
};

#endif
