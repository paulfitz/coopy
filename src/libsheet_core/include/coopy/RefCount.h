#ifndef COOPY_REFCOUNT
#define COOPY_REFCOUNT

namespace coopy {
  namespace store {
    class RefCount;
  }
}

class coopy::store::RefCount {
private:
  int ref_ct;
public:
  RefCount() {
    ref_ct = 0;
  }

  int addReference() {
    ref_ct++;
    return ref_ct;
  }

  int removeReference() {
    ref_ct--;
    return ref_ct;
  }

  int getReferenceCount() {
    return ref_ct;
  }
};

#endif
