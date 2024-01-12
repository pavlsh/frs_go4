#include "TFRSSortEvent.h"

#include "Riostream.h"

#include "TFRSSortProc.h"
#include "Go4EventServer/TGo4FileSource.h"


TFRSSortEvent::TFRSSortEvent() : TGo4EventElement("FRSSortEvent")//, fxProc(0), fxFileSrc(0) {
{ }

TFRSSortEvent::TFRSSortEvent(const char* name) : TGo4EventElement(name)//, fxProc(0), fxFileSrc(0) {
{ }

TFRSSortEvent::~TFRSSortEvent()
{ }

Int_t TFRSSortEvent::Init()
{
  Int_t rev=0;
  Clear();
  // if(CheckEventSource("TFRSSortProc")) {
  //   fxProc = (TFRSSortProc*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by " << fxProc->GetName() << " ****" << std::endl;
  // } else
  // if(CheckEventSource("TGo4FileSource")) {
  //   fxFileSrc = (TGo4FileSource*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by File Source ****"<< std::endl;
  // } else rev=1;
  return rev;
}

// Int_t TFRSSortEvent::Fill()
// {
//   Clear();
//   if(fxProc) fxProc->FRSSort(this); else    // user event processing method
//     if(fxFileSrc)fxFileSrc->BuildEvent(this); // method from framework to restore event from file
//   return 0;
// }

void TFRSSortEvent::Clear(Option_t *t)
{
  EventFlag = 0;

  ts_id = 0;
  for(int i=0;i<4;i++)
    ts_word[i] = 0;
  for(int i=0;i<3;i++)
    tsys_word[i] = 0;

  timestamp = 0;
  timespill = 0.;
  timespill2 = 0.;

  systemtime_ms = 0.;
  systemtime_s = 0.;

  pattern = 0;
  trigger = 0;
  sptrigger = 0;
  snbtrig = 0;
  smaxtrig = 0;
  for(int i=0;i<32;i++)
    {
      //sc_long[i] = 0;
      sc_long2[i] = 0;
    }

   for(int i=0;i<64;i++)
    {
      sc_long[i] = 0;
    }
  //  ic_de = 0;

  for(int i=0;i<13;i++)
    {
      mw_an[i] = 0;
      mw_xl[i] = 0;
      mw_xr[i] = 0;
      mw_yu[i] = 0;
      mw_yd[i] = 0;
    }

  //TPC part
  for(int i=0;i<7;i++){
      // ADCs
      tpc_l[i][0]=0;
      tpc_r[i][0]=0;
      tpc_l[i][1]=0;
      tpc_r[i][1]=0;
    	tpc_a[i][0]=0;
      tpc_a[i][1]=0;
      tpc_a[i][2]=0;
      tpc_a[i][3]=0;

      //TDCs
      tpc_nhit_lt[i][0] = 0;   for(int j=0; j<64; j++){ tpc_lt[i][0][j] = -1; }
      tpc_nhit_lt[i][1] = 0;   for(int j=0; j<64; j++){ tpc_lt[i][1][j] = -1; }
      tpc_nhit_rt[i][0] = 0;   for(int j=0; j<64; j++){ tpc_rt[i][0][j] = -1; }
      tpc_nhit_rt[i][1] = 0;   for(int j=0; j<64; j++){ tpc_rt[i][1][j] = -1; }
      tpc_nhit_dt[i][0] = 0;   for(int j=0; j<64; j++){ tpc_dt[i][0][j] = -1; }
      tpc_nhit_dt[i][1] = 0;   for(int j=0; j<64; j++){ tpc_dt[i][1][j] = -1; }
      tpc_nhit_dt[i][2] = 0;   for(int j=0; j<64; j++){ tpc_dt[i][2][j] = -1; }
      tpc_nhit_dt[i][3] = 0;   for(int j=0; j<64; j++){ tpc_dt[i][3][j] = -1; }

      //calib grid
      tpc_nhit_calibgrid[i] = 0; for(int j=0; j<64; j++){ tpc_calibgrid[i][j]= -1; }
  }

  // TPC time ref
  for(int i=0; i<8; i++){
    tpc_nhit_timeref[i] = 0; for(int j=0; j<64; j++){  tpc_timeref[i][j] = -1; }
  }


  de_21l = 0;
  de_21r = 0;
  de_41l = 0;
  de_41r = 0;
  de_41u = 0;
  de_41d = 0;
  de_42l = 0;
  de_42r = 0;
  de_43l = 0;
  de_43r = 0;
  de_81l = 0;
  de_81r = 0;
  de_31l = 0;
  de_31r = 0;
  de_22l = 0;
  de_22r = 0;
  de_M01l = 0;
  de_M01r = 0;

  dt_21l_21r = 0;
  dt_41l_41r = 0;
  dt_21l_41l = 0;
  dt_21r_41r = 0;
  dt_42l_42r = 0;
  dt_43l_43r = 0;
  dt_42l_21l = 0;
  dt_42r_21r = 0;
  dt_41u_41d = 0;
  dt_81l_81r = 0;
  dt_21l_81l = 0;
  dt_21r_81r = 0;
  dt_22l_22r = 0;
  dt_22l_41l = 0;
  dt_22r_41r = 0;
  dt_22l_81l = 0;
  dt_22r_81r = 0;

  // vftx
  for(int i=0;i<50;i++) {
    TRaw_vftx_S1U_L[i] = 0;
    TRaw_vftx_S1U_R[i] = 0;
    TRaw_vftx_S1D_L[i] = 0;
    TRaw_vftx_S1D_R[i] = 0;
    
    TRaw_vftx_S21_L[i] = 0;
    TRaw_vftx_S21_R[i] = 0;
    TRaw_vftx_S22_L[i] = 0;
    TRaw_vftx_S22_R[i] = 0;

    TRaw_vftx_S3_L[i] = 0;
    TRaw_vftx_S3_R[i] = 0;

    TRaw_vftx_S41_L[i] = 0;
    TRaw_vftx_S41_R[i] = 0;
    TRaw_vftx_S42_L[i] = 0;
    TRaw_vftx_S42_R[i] = 0;

    TRaw_vftx_S8_L[i] = 0;
    TRaw_vftx_S8_R[i] = 0;
  }

  vftx_mult_S1U_L = 0;
  vftx_mult_S1U_R = 0;
  vftx_mult_S1D_L = 0;
  vftx_mult_S1D_R = 0;

  vftx_mult_S21_L = 0;
  vftx_mult_S21_R = 0;
  vftx_mult_S22_L = 0;
  vftx_mult_S22_R = 0;

  vftx_mult_S3_L = 0;
  vftx_mult_S3_R = 0;

  vftx_mult_S41_L = 0;
  vftx_mult_S41_R = 0;
  vftx_mult_S42_L = 0;
  vftx_mult_S42_R = 0;

  vftx_mult_S8_L = 0;
  vftx_mult_S8_R = 0;

  // SCI dE from Mesytec QDC
  de_mqdc_21l = 0;
  de_mqdc_21r = 0;
  de_mqdc_22l = 0;
  de_mqdc_22r = 0;
  de_mqdc_41l = 0;
  de_mqdc_41r = 0;
  de_mqdc_42l = 0;
  de_mqdc_42r = 0;
  de_mqdc_81l = 0;
  de_mqdc_81r = 0;

  // User multihit TDC
  tdc_nhit_sc41l = 0;
  tdc_nhit_sc41r = 0;
  tdc_nhit_sc21l = 0;
  tdc_nhit_sc21r = 0;
  tdc_nhit_sc42l = 0;
  tdc_nhit_sc42r = 0;
  tdc_nhit_sc43l = 0;
  tdc_nhit_sc43r = 0;
  tdc_nhit_sc81l = 0;
  tdc_nhit_sc81r = 0;
  tdc_nhit_sc31l = 0;
  tdc_nhit_sc31r = 0;
  tdc_nhit_sc11 = 0;
  tdc_nhit_sc22l = 0;
  tdc_nhit_sc22r = 0;
  tdc_nhit_scM01l = 0;
  tdc_nhit_scM01r = 0;
  
  for(int i=0;i<10;i++) {
      tdc_sc41l[i] = 0;
      tdc_sc41r[i] = 0;
      tdc_sc21l[i] = 0;
      tdc_sc21r[i] = 0;
      tdc_sc42l[i] = 0;
      tdc_sc42r[i] = 0;
      tdc_sc43l[i] = 0;
      tdc_sc43r[i] = 0;
      tdc_sc81l[i] = 0;
      tdc_sc81r[i] = 0;
      tdc_sc31l[i] = 0;
      tdc_sc31r[i] = 0;
      tdc_sc11[i] = 0;
      tdc_sc22l[i] = 0;
      tdc_sc22r[i] = 0;
      tdc_scM01l[i] = 0;
      tdc_scM01r[i] = 0;
  }

  for(int i=0;i<8;i++)
    {
      music_e1[i] = 0;
      music_t1[i] = 0;
      music_e2[i] = 0;
      music_t2[i] = 0;
      music_e3[i] = 0;
      music_t3[i] = 0;
      music_e4[i] = 0;
      music_t4[i] = 0;
    }

  music_pres[0] = 0; music_pres[1] = 0;  music_pres[2] = 0;
  music_temp[0] = 0; music_temp[1] = 0;  music_temp[2] = 0;

  //LaBr3
  for(int i = 0; i<8; i++)
  {
       labr_e_raw[i] = 0;
       labr_t_raw[i] = 0;
  }

  //Si detectors
  si_adc1=0;
  si_adc2=0;
  si_adc3=0;
  si_adc4=0;
  si_adc5=0;

  for (int i=0;i<32;i++)
   {
     dssd_adc_det1[i]=0;
     dssd_adc_det2[i]=0;
     dssd_adc_det3[i]=0;
     dssd_adc_det4[i]=0;
     dssd_adc_det5[i]=0;
     dssd_adc_det6[i]=0;
   }

  SingleAnode_adc = 0;

  //Elctron current
  ec_signal=0;

  //MR-TOF-MS
  mrtof_start = 0;
  // KW inconistent array length for this multihit
  //for(int i = 0; i<100; i++)
  for(int i = 0; i<10; i++)
    {
    mrtof_stop[i] = 0;
    }
  mrtof_spill = 0;
  mrtof_ts = 0;

  for(int i=0; i<6; i++){
    hall_probe_adc[i] = 0;
  }
}

ClassImp(TFRSSortEvent)
