#include <coopy/Property.h>

using namespace coopy::store;
using namespace std;

namespace coopy {
  namespace store {
    class IntValue;
    class StringValue;
  }
}

class coopy::store::IntValue : public Value {
public:
  int x;
  IntValue(int x): x(x) {}

  virtual bool isInt() const { return true; }
  virtual bool isNull() const { return false; }
  virtual int asInt() const { return x; }  
};

class coopy::store::StringValue : public Value {
public:
  string x;
  StringValue(string x): x(x) {}

  virtual bool isString() const { return true; }
  virtual bool isNull() const { return false; }
  virtual std::string asString() const { return x; }  
};

bool PolyValue::setInt(int x) {
  setNull();
  value = new IntValue(x);
  return value!=NULL;
}

bool PolyValue::setString(const char *str) {
  setNull();
  value = new StringValue(str);
  return value!=NULL;
}

bool PolyValue::setNull() {
  if (value!=0/*NULL*/) {
    delete value;
    value = 0/*NULL*/;
  }
}
