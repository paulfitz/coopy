#ifndef APPEARANCE_INC
#define APPEARANCE_INC

#include <coopy/RefCount.h>

namespace coopy {
  namespace store {
    class AppearanceRange;
    class Appearance;
  }
}

class coopy::store::AppearanceRange {
public:
  int start;
  int end;
  AppearanceRange() {
    start = end = -1;
  }

  static AppearanceRange full() {
    return AppearanceRange();
  }
};

// Appearance info is handled externally, e.g. by PangoAttrList
// in gnumeric.  We don't touch it apart from colorful diffs.
class coopy::store::Appearance : public RefCount {
public:
  virtual bool setForegroundRgb16(int r, int g, int b, 
				  const AppearanceRange& range) {
    return false;
  }

  virtual bool setBackgroundRgb16(int r, int g, int b, 
				  const AppearanceRange& range) {
    return false;
  }

  virtual bool setWeightBold(bool flag, const AppearanceRange& range) {
    return false;
  }

  virtual bool setStrikethrough(bool flag, const AppearanceRange& range) {
    return false;
  }

  virtual bool begin() {
    return true;
  }

  virtual bool end() {
    return true;
  }
};

#endif
