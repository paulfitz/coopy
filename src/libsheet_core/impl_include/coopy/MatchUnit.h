#ifndef COOPY_MATCHUNIT
#define COOPY_MATCHUNIT

namespace coopy {
  namespace cmp {
    class MatchUnit;

    enum {
      // status, from local perspective
      MATCH_UNIT_NONE,
      MATCH_UNIT_INSERT,
      MATCH_UNIT_DELETE,
      MATCH_UNIT_PRESERVE,
    };
  }
}

class coopy::cmp::MatchUnit {
public:
  int pivotUnit;
  int localUnit;
  int remoteUnit;
  bool deleted;
  int status;

  MatchUnit() {
    remoteUnit = localUnit = pivotUnit = -1;
    deleted = false;
    status = MATCH_UNIT_NONE;
  }

  MatchUnit(int pivot, int local, int remote, bool deleted, int status) {
    pivotUnit = pivot;
    localUnit = local;
    remoteUnit = remote;
    this->deleted = deleted;
    this->status = status;
  }
};

#endif

