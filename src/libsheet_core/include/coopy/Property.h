#ifndef COOPY_PROPERTY
#define COOPY_PROPERTY

#include <coopy/RefCount.h>

#include <map>
#include <string>

#include <stdio.h>

namespace coopy {
  namespace store {
    class Value;
    class PolyValue;
    class Property;
  }
}

class coopy::store::Value : public RefCount {
public:
  virtual ~Value() {}

  virtual int asInt() const { return 0; }
  virtual std::string asString() const { return ""; }

  virtual bool isInt() const { return false; }
  virtual bool isString() const { return false; }    

  virtual bool isNull() const { return true; }
};

class coopy::store::PolyValue : public Value {
private:
  Value *value;
public:

  PolyValue() {
    value = 0/*NULL*/;
  }

  PolyValue(const PolyValue& alt) {
    value = alt.value;
    if (value!=0/*NULL*/) {
      value->addReference();
    }
  }

  virtual ~PolyValue() {
    clear();
  }

  const PolyValue& operator=(const PolyValue& alt) {
    clear();
    value = alt.value;
    if (value!=0/*NULL*/) {
      value->addReference();
    }
    return *this;
  }

  void clear() {
    if (value==0/*NULL*/) return;
    int ref = value->removeReference();
    if (ref==0) {
      delete value;
    }
    value = 0/*NULL*/;
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

  static PolyValue makeInt(int x) {
    PolyValue v;
    v.setInt(x);
    return v;
  }

  static PolyValue makeString(const char *str) {
    PolyValue v;
    v.setString(str);
    return v;
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

  PolyValue get(const char *key, PolyValue default_value) const {
    std::map<std::string,PolyValue>::const_iterator it = data.find(key);    
    if (it==data.end()) return default_value;
    return it->second;
  }
};

#endif

