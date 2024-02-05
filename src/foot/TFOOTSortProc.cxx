#include "TFOOTSortProc.h"
#include "foot_unpack_event.hh"
#include <algorithm>
#include "TFOOTSortEvent.h"
#include "TH2I.h"
#include <TGo4AnalysisImp.h>

TFOOTSortProc::TFOOTSortProc()
{
  par=  dynamic_cast<TFOOTParameter*>
    (TGo4Analysis::Instance()->GetParameter("FOOTPar"));
  char dir[]="FOOT/";
  for(int i=0; i<8;i++)
    {
      h[i]=new TH2I (Form("rawamp-ch-%1d",i),
		     Form("FOOT Raw Amp. vs. ch. layer #%1d",i),
		     FOOT_CHN,0,FOOT_CHN,
		     FOOT_ADC_BINS, 0,FOOT_ADC_MAX);
      h[i]->SetMarkerColor(1);
      h[i]->SetXTitle("channel");
      h[i]->SetYTitle("ADC val.");
      TGo4Analysis::Instance()->AddHistogram(h[i], dir);
    }
}

 TFOOTSortProc::~TFOOTSortProc()
{;}
void TFOOTSortProc::FillEvent(TFOOTSortEvent* oev,
			      TFootPtr * iev)
{
  for (int i =0 ;i<8;i++)
    {
      const int id= par->order[i];
      const bool flip=par->flip[i];
      if(flip)
	oev->SetFlip(i,iev[id].foot_e);
      else
	oev->Set1(i,iev[id].foot_e);

    }
  
  FillHist( oev)  ;
}


void TFOOTSortProc::FillHist(TFOOTSortEvent* oev)
{
  UInt_t *val;
  for (int i =0;i<8;i++)
    {
      val=oev->Get1(i);
      for (int j=0;j<FOOT_CHN;j++)
	h[i]->Fill(j,val[j]);
    }
}
ClassImp(TFOOTSortProc)
