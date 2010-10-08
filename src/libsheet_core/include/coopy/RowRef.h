#ifndef COOPY_ROWREF_INC
#define COOPY_ROWREF_INC

namespace coopy {
  namespace store {
    class RowRef;
  }
}

class coopy::store::RowRef {
private:
  int index;
public:

  RowRef(int index = -1) {
    this->index = index;
  }

  bool isValid() const {
    return index>=0;
  }

  int getIndex() const {
    return index;
  }
};

#endif
