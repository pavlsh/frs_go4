#include "TFOOTSortEvent.h"
#include <algorithm>

 TFOOTSortEvent::TFOOTSortEvent()
{;}

 TFOOTSortEvent::~TFOOTSortEvent()
{;}


void TFOOTSortEvent::Set1(Int_t i,Int_t * p)
{
  std::copy_n(p,640,FOOTRawCh + 640*i);
}

void TFOOTSortEvent::SetFlip(Int_t i,Int_t * p)
{
  std::copy_n(p,320,FOOTRawCh + 320 + 640*i);
  std::copy_n(p+320,320,FOOTRawCh + 640*i);
}

Int_t * TFOOTSortEvent::Get1(Int_t i)
{
  return static_cast < Int_t * > (FOOTRawCh+ 640*i);
}

ClassImp(TFOOTSortEvent)
