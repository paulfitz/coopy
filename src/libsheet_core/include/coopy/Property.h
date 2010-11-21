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
  virtual bool asBoolean() const { return false; }
  virtual std::string asString() const { return ""; }
  virtual Property& asMap();
  virtual const Property& asMap() const;

  virtual bool isInt() const { return false; }
  virtual bool isBoolean() const { return false; }
  virtual bool isString() const { return false; }    
  virtual bool isMap() const { return false; }    

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
  bool setBoolean(bool x);
  bool setString(const char *str);
  bool setString(const std::string& str);
  bool setNull();
  bool setMap();

  virtual int asInt() const { 
    if (value!=0/*NULL*/) return value->asInt();
    return 0; 
  }

  virtual bool asBoolean() const { 
    if (value!=0/*NULL*/) return value->asBoolean();
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

  virtual bool isBoolean() const { 
    if (value!=0/*NULL*/) return value->isBoolean();
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

  static PolyValue makeBoolean(bool x) {
    PolyValue v;
    v.setBoolean(x);
    return v;
  }

  static PolyValue makeString(const char *str) {
    PolyValue v;
    v.setString(str);
    return v;
  }

  static PolyValue makeString(const std::string& str) {
    PolyValue v;
    v.setString(str);
    return v;
  }

  static PolyValue makeMap() {
    PolyValue v;
    v.setMap();
    return v;
  }
};

class coopy::store::Property : public Value {
private:
  std::map<std::string,PolyValue> data;
  PolyValue nullValue;
  static Property nullProperty;

public:
  virtual bool isMap() const { return true; }
  virtual bool isNull() const { return false; }
  virtual Property& asMap() { return *this; }
  virtual const Property& asMap() const { return *this; }

  bool put(const char *key, int val) {
    data[key] = PolyValue();
    return data[key].setInt(val);
  }

  bool put(const char *key, bool val) {
    data[key] = PolyValue();
    return data[key].setBoolean(val);
  }

  bool put(const char *key, const char *str) {
    data[key] = PolyValue();
    return data[key].setString(str);
  }

  bool put(const char *key, const std::string& str) {
    data[key] = PolyValue();
    return data[key].setString(str);
  }

  Property& nest(const char *key) {
    data[key] = PolyValue();
    if (!data[key].setMap()) {
      return nullProperty;
    }
    return data[key].asMap();
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

  static Property& getNullProperty() {
    return nullProperty;
  }
};

#endif

