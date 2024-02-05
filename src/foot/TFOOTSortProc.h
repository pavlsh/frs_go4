// -*- mode:C++ -*-

#ifndef TFOOTSORTPROC_H
#define TFOOTSORTPROC_H


#include "Riostream.h"
#include "TObjString.h"
#include "TFOOTParameter.h"

struct TFootPtr;
class TFOOTSortEvent;
class TH1;

class TFOOTSortProc
{
public:
  TFOOTSortProc();
  virtual ~TFOOTSortProc();

  TH1 * h[8];  //!
  TFOOTParameter *par;
  void  FillEvent( TFOOTSortEvent* oev, TFootPtr * iev);
private:
  void  FillHist(TFOOTSortEvent* oev );
};

#endif //TFOOTSORTPROC_H
