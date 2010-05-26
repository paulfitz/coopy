#ifndef COOPY_MATCHUNIT
#define COOPY_MATCHUNIT

enum {
  // status, from local perspective
  MATCH_UNIT_NONE,
  MATCH_UNIT_INSERT,
  MATCH_UNIT_DELETE,
  MATCH_UNIT_PRESERVE,
};

class MatchUnit {
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

