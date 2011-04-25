#include <coopy/Property.h>
#include <stdlib.h>

using namespace coopy::store;
using namespace std;

namespace coopy {
  namespace store {
    class IntValue;
    class BooleanValue;
    class StringValue;
  }
}

Property Property::nullProperty;

Property& Value::asMap() {
  return Property::getNullProperty();
}

const Property& Value::asMap() const {
  return Property::getNullProperty();
}

class coopy::store::IntValue : public Value {
public:
  int x;
  IntValue(int x): x(x) {}

  virtual bool isInt() const { return true; }
  virtual bool isNull() const { return false; }
  virtual int asInt() const { return x; }  
  virtual bool asBoolean() const { return (x!=0); }  

  virtual std::string toString() const {
    char buf[256];
    sprintf(buf,"%d",x);
    return buf;
  }
};

class coopy::store::BooleanValue : public Value {
public:

  bool x;
  BooleanValue(bool x): x(x) {}

  virtual bool isBoolean() const { return true; }
  virtual bool isNull() const { return false; }
  virtual int asInt() const { return x?1:0; }  
  virtual bool asBoolean() const { return x; }  

  virtual std::string toString() const {
    return x?"True":"False";
  }
};

class coopy::store::StringValue : public Value {
public:
  string x;
  StringValue(string x): x(x) {}

  virtual bool isString() const { return true; }
  virtual bool isNull() const { return false; }
  virtual std::string asString() const { return x; }  
  virtual int asInt() const { 
    if (x=="true") return 1;
    if (x=="True") return 1;
    if (x=="TRUE") return 1;
    return atoi(x.c_str());
  }  

  virtual bool asBoolean() const { 
    return asInt()!=0;
  }

  virtual std::string toString() const {
    return "[" + x + "]";
  }
};

bool PolyValue::setInt(int x) {
  setNull();
  value = new IntValue(x);
  if (value!=NULL) value->addReference();
  return value!=NULL;
}

bool PolyValue::setBoolean(bool x) {
  setNull();
  value = new BooleanValue(x);
  if (value!=NULL) value->addReference();
  return value!=NULL;
}

bool PolyValue::setString(const char *str) {
  setNull();
  value = new StringValue(str);
  if (value!=NULL) value->addReference();
  return value!=NULL;
}

bool PolyValue::setString(const std::string& str) {
  setNull();
  value = new StringValue(str);
  if (value!=NULL) value->addReference();
  return value!=NULL;
}

bool PolyValue::setNull() {
  clear();
}

bool PolyValue::setMap() {
  setNull();
  value = new Property();
  if (value!=NULL) value->addReference();
  return value!=NULL;
}

std::string Property::toString() const {
  std::string result = "";
  for (std::map<std::string,PolyValue>::const_iterator it = data.begin();
       it != data.end(); 
       it++) {
    result += "(";
    result += it->first;
    result += " ";
    result += it->second.toString();
    result += ") ";
  }
  return result;
}

