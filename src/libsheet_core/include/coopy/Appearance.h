#ifndef APPEARANCE_INC
#define APPEARANCE_INC

#include <coopy/RefCount.h>

namespace coopy {
  namespace store {
    class AppearanceRange;
    class Appearance;
  }
}

/**
 *
 * A range for controlling appearance within part of a single cell.
 *
 */
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

/**
 *
 * Store the appearance of a cell, row, or column.  Coopy doesn't
 * concern itself with appearance much, but it is needed
 * for making colorful diffs.
 *
 */
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
