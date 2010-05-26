#ifndef ORDERRESULT_INC
#define ORDERRESULT_INC

#include <coopy/IntSheet.h>

class OrderResult {
private:
  IntSheet _a2b, _b2a;
public:

  void setup(IntSheet& _a2b, IntSheet& _b2a) {
    this->_a2b = _a2b;
    this->_b2a = _b2a;
  }

  virtual int a2b(int x) const {
    if (x==-2) return -2;
    return _a2b.cell(0,x);
  }

  virtual int b2a(int x) const {
    if (x==-2) return -2;
    return _b2a.cell(0,x);
  }

  int alen() const { return _a2b.height(); }
  int blen() const { return _b2a.height(); }

  void trimHead(int pre, int post) {
    int ha = _a2b.height();
    int hb = _b2a.height();
    for (int i=0; i<ha; i++) {
      int& x = _a2b.cell(0,i);
      if (x==pre) x = post; else break;
    }
    for (int i=0; i<hb; i++) {
      int& x = _b2a.cell(0,i);
      if (x==pre) x = post; else break;
    }
  }

  void trimTail(int pre, int post) {
    int ha = _a2b.height();
    int hb = _b2a.height();
    for (int i=ha-1; i>=0; i--) {
      int& x = _a2b.cell(0,i);
      if (x==pre) x = post; else break;
    }
    for (int i=hb-1; i>=0; i--) {
      int& x = _b2a.cell(0,i);
      if (x==pre) x = post; else break;
    }
  }
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
