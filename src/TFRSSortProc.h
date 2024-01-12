#ifndef TFRSSORTPROCESSOR_H
#define TFRSSORTPROCESSOR_H

#include "TFRSBasicProc.h"

#include "TFRSVftxSetting.h"

#include "define.hh"

class TFRSSortEvent;

class TFRSSortProc : public TFRSBasicProc {
public:
  TFRSSortProc() ;
  TFRSSortProc(const char* name);
  virtual ~TFRSSortProc() ;

  Bool_t BuildEvent(TGo4EventElement* output);
  //Bool_t FillHistograms(TFRSSortEvent* event);

  //void FRSSort(TFRSSortEvent* tgt);

  /* *************************************** */
  /*         VFTX : RAW TIME IN PS           */
  /* *************************************** */
  
protected:
  Long64_t StartOfSpilTime;
  Long64_t StartOfSpilTime2; //does not reset at end of extraction
  Long64_t PreviousTS;

  Int_t counter;

private:
  void v1190_channel_init();
  Int_t v1190_channel_dt[7][4];
  Int_t v1190_channel_lt[7][2];
  Int_t v1190_channel_rt[7][2];
  Int_t v1190_channel_timeref[8];
  Int_t v1190_channel_calibgrid[7];

  ClassDef(TFRSSortProc,1)
};

#endif //TFRSSORTPROCESSOR_H
