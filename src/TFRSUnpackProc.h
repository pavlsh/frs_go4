#ifndef TFRSUNPACKPROCESSOR_H
#define TFRSUNPACKPROCESSOR_H

#include "TFRSBasicProc.h"
#include "TFRSUnpackEvent.h"
#include "TFRSParameter.h"

#include "TFRSVftxSetting.h"
#include "TRandom3.h"
#include "foot/foot_unpack_proc.hh"
#include "define.hh"

class TFRSUnpackEvent;
class TGo4MbsSubEvent;

class TFRSUnpackProc : public TFRSBasicProc {
public:
  TFRSUnpackProc() ;
  TFRSUnpackProc(const char* name);
  virtual ~TFRSUnpackProc();

  //void FRSUnpack(TFRSUnpackEvent* tgt);
  // event processing function, default name
  Bool_t BuildEvent(TGo4EventElement* output);
  Bool_t FillHistograms(TFRSUnpackEvent* event);

  void VFTX_Readout(TFRSUnpackEvent * unp, Int_t **pdata, int module);
  
private:
  // KW this function returns nothing, and should be void
  //Bool_t TimeStampExtract(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* rawsub, int type_event=1);
  void TimeStampExtract(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* rawsub, int type_event=1);
  void TimeStampExtract_MVLC(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* rawsub, int type_event=1);
  void TimeStampExtract_TravMus(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* rawsub, int type_event=1);
  Bool_t Event_Extract(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* rawsub, int type_event=1);
  Bool_t Event_Extract_MVLC(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* rawsub, int type_event=1);
  void ExtractV1190(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* rawsub, int type_event=1);
  
#ifdef USELESS
  void UnpackUserSubevent(TGo4MbsSubEvent* psubevt, TFRSUnpackEvent* tgt);
#endif
  TRandom3 rand;
  void m_VFTX_Bin2Ps();
  float  VFTX_Bin2Ps[VFTX_N][VFTX_MAX_CHN][1000];
  double VFTX_GetTraw_ps(int, int, int, int, float);

  TFRSParameter* frs ;
//  TModParameter* ModSetup;

  TH1I* hTrigger;         //!
  TH1I* hpTrigger; 		 //! //physics trigger
  TH1I* hTpat; //! tpat bit
  TH1I* hNbTrig; //! nb of triggers per event in utpat
  TH2I* hCombiTrig2; // two triggers per event combinations

  TH1I* hVME_MAIN_11[32];      //!
  TH2I* hVME_MAIN_11All;  //!
  TH1I* hVME_MAIN_14[32];      //!
  TH2I* hVME_MAIN_14All;  //!
  TH1I* hVME_MAIN_TDC_V1290[32];      //!
  TH1I* hVME_MAIN_TDC_V1290_Multip[32];      //!
  TH1I* hVME_MAIN_TDC_V1290_hit[32];
  TH1I* hVME_MRTOF_TDC_V1190[32];      //

  TH1I* hVME_USER_8[32];      //! FRS crate
  TH1I* hVME_USER_9[32];      //!
  TH1I* hVME_USER_2[32];     //!
  TH1I* hVME_USER_10[32];     //!
 // TH1I* hVME_USER_11[32];     //!
  TH1I* hVME_USER_12[32];     //!
  TH2I* hVME_USER_8All ;  //!
  TH2I* hVME_USER_9All ;  //!
  TH2I* hVME_USER_10All;  //!
  //TH2I* hVME_USER_11All;  //!
  TH2I* hVME_USER_12All;  //!

  TH1I* hVME_TOF_11[32];     //! TOF crate
  TH1I* hVME_TOF_16[32]; //!
  TH2I* hVME_TOF_11All;  //!
  TH2I* hVME_TOF_16All;  //!
        	
  TH1I* hVME_TPCS2_13[32];     //! TPCS2 crate
  TH1I* hVME_TPCS2_12[32];    //!
  TH1I* hVME_TPCS4_0[32];     //! TPCS4 crate
  TH1I* hVME_TPCS4_1[32];     //!
  TH2I* hVME_TPCS2_13All;  //!
  TH2I* hVME_TPCS2_12All;  //!
  TH2I* hVME_TPCS4_0All;  //!
  TH2I* hVME_TPCS4_1All;  //!
  TH2I* hVME_TPCS2_V1190All_firsthit;
  TH2I* hVME_TPCS2_V1190All;
  TH2I* hVME_TPCS2_V1190All_bad;
  TH2I* hVME_TPCS2_V1190_bad_multip;
  TH2I* hVME_TPCS2_V1190_multip;

  TH1I* hVME_ACTSTOP_10[32]; //!
  TH1I* hVME_ACTSTOP_12[32]; //!
  TH1I* hVME_ACTSTOP_14[32]; //!
  TH1I* hVME_ACTSTOP_16[32]; //!
  TH1I* hVME_ACTSTOP_18[32]; //!
  TH1I* hVME_ACTSTOP_20[32]; //!
  TH2I* hVME_ACTSTOP_10All;  //!
  TH2I* hVME_ACTSTOP_12All;  //!
  TH2I* hVME_ACTSTOP_14All;  //!
  TH2I* hVME_ACTSTOP_16All;  //!
  TH2I* hVME_ACTSTOP_18All;  //!
  TH2I* hVME_ACTSTOP_20All;  //!
  TH2I* hVME_ACTSTOP_10All_Tpat4_8;  //!
  TH2I* hVME_ACTSTOP_12All_Tpat4_8;  //!
  TH2I* hVME_ACTSTOP_14All_Tpat4_8;  //!
  TH2I* hVME_ACTSTOP_16All_Tpat4_8;  //!
  TH2I* hVME_ACTSTOP_18All_Tpat4_8;  //!
  TH2I* hVME_ACTSTOP_20All_Tpat4_8;  //!
  TH2I* hVME_ACTSTOP_10All_Tpat4;  //!
  TH2I* hVME_ACTSTOP_12All_Tpat4;  //!
  TH2I* hVME_ACTSTOP_14All_Tpat4;  //!
  TH2I* hVME_ACTSTOP_16All_Tpat4;  //!
  TH2I* hVME_ACTSTOP_18All_Tpat4;  //!
  TH2I* hVME_ACTSTOP_20All_Tpat4;  //!
  TH2I* hVME_ACTSTOP_10All_Tpat8;  //!
  TH2I* hVME_ACTSTOP_12All_Tpat8;  //!
  TH2I* hVME_ACTSTOP_14All_Tpat8;  //!
  TH2I* hVME_ACTSTOP_16All_Tpat8;  //!
  TH2I* hVME_ACTSTOP_18All_Tpat8;  //!
  TH2I* hVME_ACTSTOP_20All_Tpat8;  //!

  TH1I* hVME_TRMU_ADC[16]; //!
  TH2I* hVME_TRMU_ADCAll;  //!
  TH1I* hVME_TRMU_TDC[16]; //!
  TH2I* hVME_TRMU_TDCAll;  //!
  TH1I* hVME_TRMU_Trigger[2]; //!

  //for s530 fission crate by J.Zhao 20210226
  TH1I* hVME_Scaler_s530[32];       // scaler, SIS-3820 in s530 crate
  TH1I* hVME_Lt_v1290_s530[32];     // leading timing, v1290 TDC in s530 crate
  TH1I* hVME_Rt_v1290_s530[32];     // trailing timing, v1290 TDC in s530 crate
  TH1I* hVME_hit_v1290_s530[32];    // multi-hit of v1290 TDC in s530 crate
  
  TH1I* hVME_Lqdc_s530[16];         // Long gate qdc, MDPP in s530 crate
  TH1I* hVME_Sqdc_s530[16];         // Short gate qdc, MDPP in s530 crate
  TH1I* hVME_TDC_mdpp_s530[16];     // timing from MDPP in s530 crate
  TH1I* hVME_hit_mdpp_s530[16];     // multi-hit of MDPP in s530 crate
  TH1I* hVME_MDPP_Trig_s530[2];   // T0, T1 trigger of MTDC-32

  TH2I* h_UnpackStatus; //!
  TH2I* h_TSFlagStatus; //!
  TH2I* h_Multiplicity; //!

  bool firstTS[3] ;
  Long64_t previousTimeStamp[3];
  Long64_t currentTimeStamp;

  TH1 * h1_vftx_leading_cc[VFTX_N][VFTX_MAX_CHN];
  TH1 * h1_vftx_leading_ft[VFTX_N][VFTX_MAX_CHN];
  TH1 * h1_vftx_leading_time[VFTX_N][VFTX_MAX_CHN];
  TH1 * h1_vftx_leading_timediff2ch0[VFTX_N][VFTX_MAX_CHN];
  TH2 * h2_vftx_leading_timediff2ch0_event[VFTX_N][VFTX_MAX_CHN];
  TH2 * h2_vftx_leading_timediff2ch8_event[VFTX_N][VFTX_MAX_CHN];
  TH2 * h2_vftx_leading_time04[VFTX_N];
  TH1 * h1_vftx_trailing_cc[VFTX_N][VFTX_MAX_CHN];
  TH1 * h1_vftx_trailing_ft[VFTX_N][VFTX_MAX_CHN];
  TH1 * h1_vftx_lead_mult[VFTX_N][VFTX_MAX_CHN];
  TH1 * h1_vftx_trail_mult[VFTX_N][VFTX_MAX_CHN];
  TH1I * h1_wr_diff_FRS_TM;

  // maybe not needed, just event counter
  UInt_t Vftx_Stat[VFTX_N][VFTX_MAX_CHN];


  // S2: MTDC-32
  TH2I *h2_S2_MTDC32_trg0;
  TH2I *h2_S2_MTDC32_trg1;


  // S2: MQDC
  TH2I *h2_S2_MQDC32;
  FOOT_UNPACK_PROC_DECL(TFRSUnpackEvent, h2_foot_raw)

  ClassDef(TFRSUnpackProc,1)
};

#endif //TFRSUNPACKPROCESSOR_H
