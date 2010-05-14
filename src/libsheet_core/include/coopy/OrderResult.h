#ifndef ORDERRESULT_INC
#define ORDERRESULT_INC

#include <coopy/CsvSheet.h>

class OrderResult {
private:
  IntSheet _a2b, _b2a;
public:

  void setup(IntSheet& _a2b, IntSheet& _b2a) {
    this->_a2b = _a2b;
    this->_b2a = _b2a;
  }

  virtual int a2b(int x) const {
    return _a2b.cell(0,x);
  }

  virtual int b2a(int x) const {
    return _b2a.cell(0,x);
  }

  int alen() const { return _a2b.height(); }
  int blen() const { return _b2a.height(); }
};

class IdentityOrderResult : public OrderResult {
public:
  virtual int a2b(int x) const {
    return x;
  }

  virtual int b2a(int x) const {
    return x;
  }
};

#endif
