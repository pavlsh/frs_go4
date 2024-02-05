// -*- mode:C++ -*-

#ifndef TFOOTPARAMETER_H
#define TFOOTPARAMETER_H


#include "TGo4Parameter.h"
#include "Riostream.h"
#include "TObjString.h"
//#include "TArrayI.h"

class TFOOTParameter : public TGo4Parameter
{
public:
  TFOOTParameter();
  TFOOTParameter(const char* name);
  virtual ~TFOOTParameter();
  virtual void Print(Option_t* t="") const;
  virtual Bool_t UpdateFrom(TGo4Parameter *);

  Int_t order[8];
  Bool_t flip[8];

  ClassDef(TFOOTParameter,1)
};

#endif //TFOOTPARAMETER_H
