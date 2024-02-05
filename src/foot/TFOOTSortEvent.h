// -*- mode:C++ -*-

#ifndef TFOOTSORTEVENT_H
#define TFOOTSORTEVENT_H


#include "Riostream.h"
#include "TObjString.h"
//#include "TArrayI.h"

class TFOOTSortEvent 
{
public:
   TFOOTSortEvent();
  virtual ~TFOOTSortEvent();

  Int_t FOOTRawCh[8*640];
  
  void Set1(Int_t i,Int_t * p);
  void SetFlip(Int_t i,Int_t * p);
  Int_t * Get1(Int_t i);
  
  ClassDef(TFOOTSortEvent,1)
};

#endif //TFOOTSORTEVENT_H
