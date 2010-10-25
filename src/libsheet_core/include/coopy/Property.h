#ifndef COOPY_PROPERTY
#define COOPY_PROPERTY

#include <map>
#include <string>

namespace coopy {
  namespace store {
    class Value;
    class PolyValue;
    class Property;
  }
}

class coopy::store::Value {
public:
  virtual ~Value() {}

  virtual int asInt() const { return 0; }
  virtual std::string asString() const { return ""; }

  virtual bool isInt() const { return false; }
  virtual bool isString() const { return false; }    

  virtual bool isNull() const { return true; }
};

class coopy::store::PolyValue : public Value {
public:
  Value *value;

  PolyValue() {
    value = 0/*NULL*/;
  }

  virtual ~PolyValue() {
    if (value!=0/*NULL*/) {
      delete value;
      value = 0/*NULL*/;
    }
  }

  bool setInt(int x);
  bool setString(const char *str);
  bool setNull();

  virtual int asInt() const { 
    if (value!=0/*NULL*/) return value->asInt();
    return 0; 
  }

  virtual std::string asString() const { 
    if (value!=0/*NULL*/) return value->asString();
    return ""; 
  }

  virtual bool isInt() const { 
    if (value!=0/*NULL*/) return value->isInt();
    return false; 
  }

  virtual bool isString() const { 
    if (value!=0/*NULL*/) return value->isString();
    return false; 
  }    

  virtual bool isNull() const { 
    if (value!=0/*NULL*/) return value->isNull();
    return true; 
  }

};

class coopy::store::Property {
private:
  std::map<std::string,PolyValue> data;
  PolyValue nullValue;

public:
  bool put(const char *key, int val) {
    data[key] = PolyValue();
    return data[key].setInt(val);
  }

  bool put(const char *key, const char *str) {
    data[key] = PolyValue();
    return data[key].setString(str);
  }

  bool check(const char *key) const {
    std::map<std::string,PolyValue>::const_iterator it = data.find(key);    
    return (it!=data.end());
  }

  const Value& get(const char *key) const {
    std::map<std::string,PolyValue>::const_iterator it = data.find(key);    
    if (it==data.end()) return nullValue;
    return it->second;
  }
};

#endif

