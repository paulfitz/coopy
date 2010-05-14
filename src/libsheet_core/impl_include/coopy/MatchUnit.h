#ifndef COOPY_MATCHUNIT
#define COOPY_MATCHUNIT

class MatchUnit {
public:
  int pivotUnit;
  int localUnit;
  int remoteUnit;
  bool deleted;

  MatchUnit() {
    remoteUnit = localUnit = pivotUnit = -1;
    deleted = false;
  }

  MatchUnit(int pivot, int local, int remote, bool deleted) {
    pivotUnit = pivot;
    localUnit = local;
    remoteUnit = remote;
    this->deleted = deleted;
  }
};

#endif

