#ifndef COOPY_COLUMNREF_INC
#define COOPY_COLUMNREF_INC

namespace coopy {
  namespace store {
    class ColumnRef;
  }
}

/**
 * Reference to a column.
 */
class coopy::store::ColumnRef {
private:
  int index;
public:
  ColumnRef(int index = -1) {
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
