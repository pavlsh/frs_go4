#include "TFRSUnpackEvent.h"

#include "Riostream.h"
#include "TFRSUnpackProc.h"
#include "Go4EventServer/TGo4FileSource.h"


TFRSUnpackEvent::TFRSUnpackEvent() : TGo4EventElement("FRSUnpackEvent")
				     //  fxProc(0),fxFileSrc(0)
{ FOOT_PTR_ASSIGN }

TFRSUnpackEvent::TFRSUnpackEvent(const char* name)
  :TGo4EventElement(name)
   //fxProc(0), fxFileSrc(0)
{ FOOT_PTR_ASSIGN }

TFRSUnpackEvent::~TFRSUnpackEvent()
{  }

Int_t TFRSUnpackEvent::Init()
{
  Int_t rev=0;
  Clear();
  ClearVftx();
  // if(CheckEventSource("TFRSUnpackProc")){
  //   fxProc = (TFRSUnpackProc*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by " << fxProc->GetName() << " ****" << std::endl;
  // } else
  // if(CheckEventSource("TGo4FileSource")) {
  //   fxFileSrc = (TGo4FileSource*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by File Source ****"<< std::endl;
  // } else rev=1;
  return rev;
}

// Int_t TFRSUnpackEvent::Fill()
// {
//   Int_t rev=0;
//   Clear();
//   if(fxProc)fxProc->FRSUnpack(this); else  // user event processing method
//   if(fxFileSrc)fxFileSrc->BuildEvent(this); // method from framework to restore event from file
//   return rev;
// }

void TFRSUnpackEvent::ClearVftx()
{
  // --- ------------ --- //
  // --- TDC/GSI/VFTX --- //
  // --- ------------ --- //
  for(int module=0; module<VFTX_N; module++)
    for(int channel=0; channel<VFTX_MAX_CHN; channel++) {
      for(int hit=0; hit<VFTX_MAX_HITS; hit++) {
	vftx_leading_cc[module][channel][hit] = 0;
	vftx_leading_ft[module][channel][hit] = 0;
	vftx_leading_time[module][channel][hit] = 0;
	vftx_trailing_cc[module][channel][hit] = 0;
	vftx_trailing_ft[module][channel][hit] = 0;
	vftx_trailing_time[module][channel][hit] = 0;
      }
      vftx_mult[module][channel] = 0;
      vftx_lead_mult[module][channel] = 0;
      vftx_trail_mult[module][channel] = 0;
    }

}

void TFRSUnpackEvent::Clear_MTDC_32()
{
  for (int i=0; i<32; i++)
  {
    mtdc32_dt_trg0_raw[i] = -9999;
    mtdc32_dt_trg1_raw[i] = -9999;
  }
}

void TFRSUnpackEvent::Clear_MQDC_32()
{
  for (int i=0; i<32; i++)
  {
    mqdc32_raw[i] = -9999;
  }
}


void TFRSUnpackEvent::Clear(Option_t *t)
{
  CLEAR_FOOT
//  EventFlag = -1;

  for(int i=0;i<21;i++)
    {
      for(int j=0;j<32;j++)
	{
	  vme_frs[i][j] = 0;
	  vme_tof[i][j] = 0;
	  vme_tpcs2[i][j] = 0;
	  vme_tpcs4[i][j] = 0;  
	  vme_main[i][j] = 0;
	  vme_actstop[i][j] = 0;
	}
    }
v1190_is_bad = false;

  for(int i=0; i<128; i++){
      nhit_v1190_tpcs2[i] = 0;
    for(int j=0; j<64; j++){
      leading_v1190_tpcs2[i][j] = -1;
    }
  }
  for(int j=0;j<32;j++)
    {
      scaler_frs[j]=0;
      scaler_main[j]=0;
      scaler_mrtof[j]=0;
      timestamp_main[j]=0;
      tpat_main[j]=0;
      nhit_v1290_main[j][0]=0; nhit_v1290_main[j][1]=0;
      nhit_v1190_mrtof[j][0]=0; nhit_v1190_mrtof[j][1]=0; //200122 - V1190 added for mrtof crate
      for(int kk=0; kk<10; kk++){
        leading_v1290_main[j][kk] = 0;
     	trailing_v1290_main[j][kk] = 0;
	leading_v1190_mrtof[j][kk] = 0;
     	trailing_v1190_mrtof[j][kk] = 0;
      }
    }
  mrtof_ts = 0;

  for(int j=0;j<16;j++)
    {
      vme_trmu_adc[j] = 0;
      vme_trmu_tdc[j] = 0;
    }
   for(int j=0;j<2;j++)
    {
      vme_trmu_trigger[j] = 0;
    }

   //for s530 fission crate by J.Zhao 20210226
   for(int j=0;j<32;j++){
     scaler_s530[j] = 0; //scaler   

     nhit_v1290_s530[j][0] = 0; //hit, leading timing 
     nhit_v1290_s530[j][1] = 0; //hit, trailing timing
     for(int kk=0; kk<10; kk++){
       leading_v1290_s530[j][kk] = 0;     // leading timing, v1290 TDC in s530 crate
       trailing_v1290_s530[j][kk] = 0;    // trailing timing, v1290 TDC in s530 crate
      }
    }

   for(int j=0;j<16;j++){
     nhit_mdpp_s530[j][0] = 0;           // multi-hit in mdpp-16, tqdc in s530 crate
     nhit_mdpp_s530[j][1] = 0;           // multi-hit in mdpp-16, lqdc in s530 crate 
     nhit_mdpp_s530[j][2] = 0;           // multi-hit in mdpp-16, sqdc in s530 crate 	 
     for(int kk=0; kk<10; kk++){
       lqdc_mdpp_s530[j][kk] = 0;        // Long gate qdc, MDPP in s530 crate
       sqdc_mdpp_s530[j][kk] = 0;        // Short gate qdc, MDPP in s530 crate
       tdc_mdpp_s530[j][kk] = 0;         // timing from MDPP in s530 crate
      }
    }  

   for(int j=0;j<2;j++){               
       mdpp_trig_s530[j] = 0;  //T0, T1 trigger of mesytec MDPP-16-QDC
    }
   //end of s530 fission crate 

   travmus_wr = 0;
   frs_wr = 0;

   qlength = 0;
   qtype = 0;
   qsubtype = 0;
   qdummy = 0;
   qtrigger  = 0;
   qevent_nr = 0;

	utpat     = 0;
	uphystrig = 0;
	unbtrig   = 0;
	umaxtrig   = 0;
	wrid = 0;
}


ClassImp(TFRSUnpackEvent)
