// -*- mode:C++ -*-

#ifndef TFOOTSORTEVENT_H
#define TFOOTSORTEVENT_H


#include "Riostream.h"
#include "TObjString.h"
#include "foot_common.hh"

class TFOOTSortEvent 
{
public:
   TFOOTSortEvent();
  virtual ~TFOOTSortEvent();

  UInt_t FOOTRawCh[8*FOOT_CHN];
  
  void Set1(Int_t i,UInt_t * p);
  void SetFlip(Int_t i,UInt_t * p);
  UInt_t * Get1(Int_t i);
  
  ClassDef(TFOOTSortEvent,1)
};

#endif //TFOOTSORTEVENT_H
