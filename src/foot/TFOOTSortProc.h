// -*- mode:C++ -*-

#ifndef TFOOTSORTPROC_H
#define TFOOTSORTPROC_H


#include "Riostream.h"
#include "TObjString.h"
#include "TFOOTParameter.h"

struct TFootPtr;
class TFOOTSortEvent;
class TH2;

class TFOOTSortProc
{
public:
  TFOOTSortProc(){;};
  ~TFOOTSortProc(){;};
  void  FillEvent(TFOOTParameter* fp, TFOOTSortEvent* oev, TFootPtr * iev);
  void  FillHist(TFOOTSortEvent* oev, TH2 *h);
};

#endif //TFOOTSORTPROC_H
