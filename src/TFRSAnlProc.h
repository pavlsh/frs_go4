#ifndef TFRSANLPROC_H
#define TFRSANLPROC_H

#include "TFRSBasicProc.h"
#include "TFRSParameter.h"
#include <TRandom3.h>

class TFRSAnlEvent;
class TFRSSortEvent;
class TFRSCalibrEvent;

class TFRSUnpackEvent;

class TFRSAnlProc : public TFRSBasicProc {
public:
  TFRSAnlProc();
  TFRSAnlProc(const char* name);
  //void FRSEventAnalysis(TFRSAnlEvent* target);
  Bool_t BuildEvent(TGo4EventElement* output);

  virtual ~TFRSAnlProc() ;

  Bool_t bDrawHist;

private:
  void Create_MUSIC_Hist();
  void Create_SCI_Hist();
  void Create_SCI_VFTX_Hist();
  void Create_MultiHitTDC_Hist();
  void Create_ID_Hist();
  void Create_MRTOF_Hist();
  void Create_Gated_Hist();
  void Create_Range_Hist();
  // void Create_SI_Hist();

  void Process_MUSIC_Analysis(TFRSSortEvent& srt, TFRSCalibrEvent& clb, TFRSAnlEvent& tgt);
  void Process_SCI_Analysis(TFRSSortEvent& srt, TFRSCalibrEvent& clb, TFRSAnlEvent& tgt);
  void Process_SCI_VFTX_Analysis(TFRSSortEvent& srt, TFRSCalibrEvent& clb, TFRSAnlEvent& tgt);
  void Process_ID_Analysis(TFRSSortEvent& srt, TFRSCalibrEvent&  clb, TFRSAnlEvent& tgt);
  void Process_MRTOF_Analysis(TFRSSortEvent& srt, TFRSCalibrEvent& clb, TFRSAnlEvent& tgt);
  void Process_MultiHitTDC_Analysis(TFRSSortEvent& srt, TFRSCalibrEvent& clb, TFRSAnlEvent& tgt);
  void Process_ID_MultiHitTDC_Analysis(TFRSSortEvent& srt, TFRSCalibrEvent& clb, TFRSAnlEvent& tgt);
    void Process_Gated_Analysis(TFRSSortEvent& srt, TFRSCalibrEvent& clb, TFRSAnlEvent& tgt);
  TMUSICParameter* music;
  TSCIParameter* sci;
  TFRSParameter* frs;
  TIDParameter* id;
  TMRTOFMSParameter* mrtof;
  TRangeParameter* range;

  //MRTOF data

  TH1F* h_MRtof_Start;
  TH1F* h_MRtof_Stop;
  TH1F* h_MRtof_tof;
  TH1F* h_MRtof_Spill;
  TH2I* h_MRtof_Start_Time;
  TH2I* h_MRtof_Stop_Time;
  TH2I* h_MRtof_tof_Time;
  TH2I* h_MRtof_Spill_Time;

  // MUSIC data
  TH1I          *hMUSIC1_E[8];
  TH1I          *hMUSIC1_T[8];
  TH1I          *hMUSIC1_dE;
  TH2I          *hMUSIC1_dE_x;
  TH1I          *hMUSIC1_dECOR;
  TH2I          *hMUSIC1_dECOR_x;

  TH1I          *hMUSIC2_E[8];
  TH1I          *hMUSIC2_T[8];
  TH1I          *hMUSIC2_dE;
  TH2I          *hMUSIC2_dE_x;
  TH1I          *hMUSIC2_dECOR;
  TH2I          *hMUSIC2_dECOR_x;

  TH1I          *hMUSIC3_E[8];
  TH1I          *hMUSIC3_T[8];
  TH1I          *hMUSIC3_dE;
  TH2I          *hMUSIC3_dE_x;
  TH1I          *hMUSIC3_dECOR;
  TH2I          *hMUSIC3_dECOR_x;
  //  TH1I          *hMUSIC3_E_s4gate_Z_AoQ[8][5]; //for range scan

  TH1I          *hMUSIC4_E[8];
  TH1I          *hMUSIC4_T[8];
  TH1I          *hMUSIC4_dE;
  TH2I          *hMUSIC4_dE_x;
  TH1I          *hMUSIC4_dECOR;
  TH2I          *hMUSIC4_dECOR_x;

  TH1I          *hMUSIC4_drift_y1;
  TH1I          *hMUSIC4_drift_y2;
  TH1I          *hMUSIC4_drift_y3;
  TH1I          *hMUSIC4_drift_y4;
  
  // correlation between MUSICs
  TH2I          *hMUSIC_dE1dE2;
  TH2I          *hMUSIC_dE1dE3;
  TH2I          *hMUSIC_dE1dE4;
  TH2I          *hMUSIC_dE2dE3;
  TH2I          *hMUSIC_dE2dE4;
  TH2I          *hMUSIC_dE3dE4;

  TGo4WinCond   *cMusic1_E[8];
  TGo4WinCond   *cMusic1_T[8];
  TGo4WinCond   *cMusic2_E[8];
  TGo4WinCond   *cMusic2_T[8];
  TGo4WinCond   *cMusic3_T[8];
  TGo4WinCond   *cMusic3_E[8];
  TGo4WinCond   *cMusic4_T[8];
  TGo4WinCond   *cMusic4_E[8];


  // Multi-hit TDC SCI timing
  TH1I *hMultiHitTDC_21l_21r;
  TH1I *hMultiHitTDC_22l_22r;
  TH1I *hMultiHitTDC_41l_41r;
  TH1I *hMultiHitTDC_42l_42r;
  TH1I *hMultiHitTDC_43l_43r;
  TH1I *hMultiHitTDC_31l_31r;
  TH1I *hMultiHitTDC_81l_81r;
  TH1I *hMultiHitTDC_M01l_M01r;

  TH2I *hMultiHitTDC_21l_21r_TPCX;
  TH2I *hMultiHitTDC_22l_22r_TPCX;
  TH2I *hMultiHitTDC_41l_41r_TPCX;
  TH2I *hMultiHitTDC_42l_42r_TPCX;
  TH2I *hMultiHitTDC_43l_43r_TPCX;
  TH2I *hMultiHitTDC_31l_31r_TPCX;
  //  TH2I *hMultiHitTDC_81l_81r_TPCX;
  //  TH2I *hMultiHitTDC_M01l_M01r_TPCX;

  TH1I *hMultiHitTDC_SC21X;
  TH1I *hMultiHitTDC_SC22X;
  TH1I *hMultiHitTDC_SC41X;
  TH1I *hMultiHitTDC_SC42X;
  TH1I *hMultiHitTDC_SC43X;
  TH1I *hMultiHitTDC_SC31X;
  TH1I *hMultiHitTDC_SC81X;
  TH1I *hMultiHitTDC_SCM01X;

  TH2I *hMultiHitTDC_SC21X_TPCX;
  TH2I *hMultiHitTDC_SC22X_TPCX;
  TH2I *hMultiHitTDC_SC41X_TPCX;
  TH2I *hMultiHitTDC_SC42X_TPCX;
  TH2I *hMultiHitTDC_SC43X_TPCX;
  TH2I *hMultiHitTDC_SC31X_TPCX;
  //  TH2I *hMultiHitTDC_81l_81r_TPCX;
  //  TH2I *hMultiHitTDC_M01l_M01r_TPCX;

  TH1I *hMultiHitTDC_TOF_41_21;
  TH1I *hMultiHitTDC_TOF_42_21;
  TH1I *hMultiHitTDC_TOF_43_21;
  TH1I *hMultiHitTDC_TOF_31_21;
  TH1I *hMultiHitTDC_TOF_81_21;
  TH1I *hMultiHitTDC_TOF_41_22;
  TH1I *hMultiHitTDC_TOF_M01_21;
  TH1I *hMultiHitTDC_TOF_M01_22;

  //MHTDC S2-S8
  TH1I *hID_MHTDCS2S8_DELTA;
  TH1I *hID_MHTDCS2S8_BETA;
  TH1I *hID_MHTDCS2S8_AOQ; 
  TH2I *hID_MHTDCS2S8_DELTA_BETA;
  TH2I *hID_MHTDCS2S8_AOQ_BETA;
  TH1I *hID_MHTDCS2S8_dES2TPC  ;  
  TH2I *hID_MHTDCS2S8_dES2TPC_BETA;
  TH2I *hID_MHTDCS2S8_dES2TPC_AOQ ;
  TH1I *hID_MHTDCS2S8_ZS2TPC    ; 
  TH2I *hID_MHTDCS2S8_ZS2TPC_AOQ ;
  TH2I *hID_MHTDCS2S8_dESC81_BETA;
  TH2I *hID_MHTDCS2S8_dESC81_AOQ;
  TH1I *hID_MHTDCS2S8_ZSC81     ;
  TH2I *hID_MHTDCS2S8_ZSC81_AOQ ;

  //MHTDC S2-S4
  TH1I *hID_MHTDCS2S4_DELTA; 
  TH1I *hID_MHTDCS2S4_BETA; 
  TH1I *hID_MHTDCS2S4_AOQ;
  TH2I *hID_MHTDCS2S4_DELTA_BETA;
  TH2I *hID_MHTDCS2S4_AOQ_BETA;
  TH2I *hID_MHTDCS2S4_dE41_BETA;
  TH2I *hID_MHTDCS2S4_dE41_AOQ;
  TH1I *hID_MHTDCS2S4_Z41;
  TH2I *hID_MHTDCS2S4_Z41_AOQ ;
  TH2I *hID_MHTDCS2S4_Z41_AOQ_corr ;
  TH2I *hID_MHTDCS2S4_dE42_BETA;
  TH2I *hID_MHTDCS2S4_dE42_AOQ ;
  TH1I *hID_MHTDCS2S4_Z42;
  TH2I *hID_MHTDCS2S4_Z42_AOQ;
  TH2I *hID_MHTDCS2S4_Z42_AOQ_corr;

  //MHTDC S2-HTM
  TH1I *hID_MHTDCS2HTM_DELTA; 
  TH1I *hID_MHTDCS2HTM_BETA; 
  TH1I *hID_MHTDCS2HTM_AOQ;
  TH2I *hID_MHTDCS2HTM_DELTA_BETA;
  TH2I *hID_MHTDCS2HTM_AOQ_BETA;
  TH2I *hID_MHTDCS2HTM_dEHTM_BETA;
  TH2I *hID_MHTDCS2HTM_dEHTM_AOQ;
  TH1I *hID_MHTDCS2HTM_ZHTM;
  TH2I *hID_MHTDCS2HTM_ZHTM_AOQ ;
  TH2I *hID_MHTDCS2HTM_ZHTM_AOQ_corr ;
  
  // SCI data
  TH1I          *hSCI_L[15];
  TH1I          *hSCI_R[15];
  TH1I          *hSCI_E[15];
  TH1I          *hSCI_Tx[15];
  TH1I          *hSCI_X[15];
  TH2I          *hSCI_Tx_XTPC[15];
  TH2I          *hSCI_X_XTPC[15];

  TH1I          *hSCI_TofLL2;
  TH1I          *hSCI_TofRR2;
  TH1I          *hSCI_Tof2;
  TH1I          *hSCI_Tof2calib;

  TH1I          *hSCI_TofLL3;
  TH1I          *hSCI_TofRR3;
  TH1I          *hSCI_Tof3;
  TH1I          *hSCI_Tof3calib;

  TH1I          *hSCI_TofLL4;
  TH1I          *hSCI_TofRR4;
  TH1I          *hSCI_Tof4;
  TH1I          *hSCI_Tof4calib;

  TH1I          *hSCI_TofLL5;
  TH1I          *hSCI_TofRR5;
  TH1I          *hSCI_Tof5;
  TH1I          *hSCI_Tof5calib;

  TH1I          *hSCI_TofLL6;
  TH1I          *hSCI_TofRR6;
  TH1I          *hSCI_Tof6;
  TH1I          *hSCI_Tof6calib;

  TH2I          *hSCI_dETof2;
  TH2I          *hSCI_dETof3;
  TH2I          *hSCI_dETof4;
  TH2I          *hSCI_dETof5;
  TH2I          *hSCI_dETof6;

  TGo4WinCond   *cSCI_L[12];
  TGo4WinCond   *cSCI_R[12];
  TGo4WinCond   *cSCI_E[12];
  TGo4WinCond   *cSCI_Tx[12];
  TGo4WinCond   *cSCI_X[12];

  TGo4WinCond   *cSCI_TofLL2;
  TGo4WinCond   *cSCI_TofRR2;
  TGo4WinCond   *cSCI_TofLL3;
  TGo4WinCond   *cSCI_TofRR3;
  TGo4WinCond   *cSCI_TofLL4;
  TGo4WinCond   *cSCI_TofRR4;
  TGo4WinCond   *cSCI_TofLL5;
  TGo4WinCond   *cSCI_TofRR5;
  TGo4WinCond   *cSCI_TofLL6;
  TGo4WinCond   *cSCI_TofRR6;

  // VFTX
  TH1D          *h1_deltaT_S1U;
  TH1D          *h1_deltaT_S1U_mhit;
  TH1D          *h1_deltaT_S1D;
  TH1D          *h1_deltaT_S1D_mhit;

  TH1D          *h1_deltaT_S21;
  TH1D          *h1_deltaT_S21_mhit;
  TH2I          *h1_deltaT_S21_TPC;
  TH1D          *h1_deltaT_S22;
  TH1D          *h1_deltaT_S22_mhit;
  TH2I          *h1_deltaT_S22_TPC;

  TH1D          *h1_deltaT_S3;
  TH1D          *h1_deltaT_S3_mhit;

  TH1D          *h1_deltaT_S41;
  TH1D          *h1_deltaT_S41_mhit;
  TH2I          *h1_deltaT_S41_TPC;
  TH1D          *h1_deltaT_S42;
  TH1D          *h1_deltaT_S42_mhit;
  TH2I          *h1_deltaT_S42_TPC;

  TH1D          *h1_deltaT_S8;
  TH1D          *h1_deltaT_S8_mhit;

  TH1D	        *h1_TOF_S21_S41_LL;
  TH1D	        *h1_TOF_S21_S41_RR;
  TH1D	        *h1_TOF_S21_S42_LL;
  TH1D	        *h1_TOF_S21_S42_RR;
  TH1D	        *h1_TOF_S22_S41_LL;
  TH1D	        *h1_TOF_S22_S41_RR;
  TH1D	        *h1_TOF_S22_S42_LL;
  TH1D	        *h1_TOF_S22_S42_RR;
     	        
  TH1D	        *h1_TOF_S21_S41_LL_mhit;
  TH1D	        *h1_TOF_S21_S41_RR_mhit;
  TH1D	        *h1_TOF_S21_S42_LL_mhit;
  TH1D	        *h1_TOF_S21_S42_RR_mhit;
  TH1D	        *h1_TOF_S22_S41_LL_mhit;
  TH1D	        *h1_TOF_S22_S41_RR_mhit;
  TH1D	        *h1_TOF_S22_S42_LL_mhit;
  TH1D	        *h1_TOF_S22_S42_RR_mhit;

  TH1D          *h1_TOF_S21_S8_LL;
  TH1D          *h1_TOF_S21_S8_RR;
  TH1D          *h1_TOF_S22_S8_LL;
  TH1D          *h1_TOF_S22_S8_RR;

  TH1D          *h1_TOF_S21_S8_LL_mhit;
  TH1D          *h1_TOF_S21_S8_RR_mhit;
  TH1D          *h1_TOF_S22_S8_LL_mhit;
  TH1D          *h1_TOF_S22_S8_RR_mhit;

  
  TH1D          *h1_VFTX_TOF_S21_S41;
  TH1D          *h1_VFTX_TOF_S21_S41_mhit;
  TH1D          *h1_VFTX_TOF_S21_S41_calib;
  TH1D          *h1_VFTX_TOF_S22_S41;
  TH1D          *h1_VFTX_TOF_S22_S41_mhit;
  TH1D          *h1_VFTX_TOF_S22_S41_calib;

  TH1D          *h1_VFTX_TOF_S21_S42;
  TH1D          *h1_VFTX_TOF_S21_S42_mhit;
  TH1D          *h1_VFTX_TOF_S21_S42_calib;
  TH1D          *h1_VFTX_TOF_S22_S42;
  TH1D          *h1_VFTX_TOF_S22_S42_mhit;
  TH1D          *h1_VFTX_TOF_S22_S42_calib;
  
  TH1D          *h1_VFTX_TOF_S21_S8;
  TH1D          *h1_VFTX_TOF_S21_S8_mhit;
  TH1D          *h1_VFTX_TOF_S21_S8_calib;
  TH1D          *h1_VFTX_TOF_S22_S8;
  TH1D          *h1_VFTX_TOF_S22_S8_mhit;
  TH1D          *h1_VFTX_TOF_S22_S8_calib;

  TH1I          *h1_VFTX_beta_S21_S41;
  TH1I          *h1_VFTX_beta_S21_S41_mhit;
  TH1I          *h1_VFTX_AoQ_S21_S41;
  TH1I          *h1_VFTX_AoQ_S21_S41_mhit;
  TH1I          *h1_VFTX_AoQcorr_S21_S41;
  TH1I          *h1_VFTX_AoQcorr_S21_S41_mhit;
  TH2I          *h1_VFTX_x2_AoQ_S21_S41;
  TH2I          *h1_VFTX_x2_AoQ_S21_S41_mhit;
  TH2I          *h1_VFTX_Z1_AoQ_S21_S41;
  TH2I          *h1_VFTX_Z1_AoQ_S21_S41_mhit;
  TH2I          *h1_VFTX_Z1_AoQcorr_S21_S41;
  TH2I          *h1_VFTX_Z1_AoQcorr_S21_S41_mhit;
  TH2I          *h1_VFTX_Z2_AoQ_S21_S41;
  TH2I          *h1_VFTX_Z2_AoQ_S21_S41_mhit;
  TH2I          *h1_VFTX_Z2_AoQcorr_S21_S41;
  TH2I          *h1_VFTX_Z2_AoQcorr_S21_S41_mhit;
					
  TH1I          *h1_VFTX_beta_S22_S41;
  TH1I          *h1_VFTX_beta_S22_S41_mhit;
  TH1I          *h1_VFTX_AoQ_S22_S41;
  TH1I          *h1_VFTX_AoQ_S22_S41_mhit;
  TH1I          *h1_VFTX_AoQcorr_S22_S41;
  TH1I          *h1_VFTX_AoQcorr_S22_S41_mhit;
  TH2I          *h1_VFTX_x2_AoQ_S22_S41;
  TH2I          *h1_VFTX_x2_AoQ_S22_S41_mhit;
  TH2I          *h1_VFTX_Z1_AoQ_S22_S41;
  TH2I          *h1_VFTX_Z1_AoQ_S22_S41_mhit;
  TH2I          *h1_VFTX_Z1_AoQcorr_S22_S41;
  TH2I          *h1_VFTX_Z1_AoQcorr_S22_S41_mhit;
  TH2I          *h1_VFTX_Z2_AoQ_S22_S41;
  TH2I          *h1_VFTX_Z2_AoQ_S22_S41_mhit;
  TH2I          *h1_VFTX_Z2_AoQcorr_S22_S41;
  TH2I          *h1_VFTX_Z2_AoQcorr_S22_S41_mhit;
					
  TH1I          *h1_VFTX_beta_S21_S42;
  TH1I          *h1_VFTX_beta_S21_S42_mhit;
  TH1I          *h1_VFTX_AoQ_S21_S42;
  TH1I          *h1_VFTX_AoQ_S21_S42_mhit;
  TH1I          *h1_VFTX_AoQcorr_S21_S42;
  TH1I          *h1_VFTX_AoQcorr_S21_S42_mhit;
  TH2I          *h1_VFTX_x2_AoQ_S21_S42;
  TH2I          *h1_VFTX_x2_AoQ_S21_S42_mhit;
  TH2I          *h1_VFTX_Z1_AoQ_S21_S42;
  TH2I          *h1_VFTX_Z1_AoQ_S21_S42_mhit;
  TH2I          *h1_VFTX_Z1_AoQcorr_S21_S42;
  TH2I          *h1_VFTX_Z1_AoQcorr_S21_S42_mhit;
  TH2I          *h1_VFTX_Z2_AoQ_S21_S42;
  TH2I          *h1_VFTX_Z2_AoQ_S21_S42_mhit;
  TH2I          *h1_VFTX_Z2_AoQcorr_S21_S42;
  TH2I          *h1_VFTX_Z2_AoQcorr_S21_S42_mhit;
					
  TH1I          *h1_VFTX_beta_S22_S42;
  TH1I          *h1_VFTX_beta_S22_S42_mhit;
  TH1I          *h1_VFTX_AoQ_S22_S42;
  TH1I          *h1_VFTX_AoQ_S22_S42_mhit;
  TH1I          *h1_VFTX_AoQcorr_S22_S42;
  TH1I          *h1_VFTX_AoQcorr_S22_S42_mhit;
  TH2I          *h1_VFTX_x2_AoQ_S22_S42;
  TH2I          *h1_VFTX_x2_AoQ_S22_S42_mhit;
  TH2I          *h1_VFTX_Z1_AoQ_S22_S42;
  TH2I          *h1_VFTX_Z1_AoQ_S22_S42_mhit;
  TH2I          *h1_VFTX_Z1_AoQcorr_S22_S42;
  TH2I          *h1_VFTX_Z1_AoQcorr_S22_S42_mhit;
  TH2I          *h1_VFTX_Z2_AoQ_S22_S42;
  TH2I          *h1_VFTX_Z2_AoQ_S22_S42_mhit;
  TH2I          *h1_VFTX_Z2_AoQcorr_S22_S42;
  TH2I          *h1_VFTX_Z2_AoQcorr_S22_S42_mhit;

  TH1I          *h1_VFTX_beta_S21_S8;
  TH1I          *h1_VFTX_beta_S21_S8_mhit;
  TH1I          *h1_VFTX_AoQ_S21_S8;
  TH1I          *h1_VFTX_AoQ_S21_S8_mhit;
  TH1I          *h1_VFTX_AoQcorr_S21_S8;
  TH1I          *h1_VFTX_AoQcorr_S21_S8_mhit;
  TH2I          *h1_VFTX_x2_AoQ_S21_S8;
  TH2I          *h1_VFTX_x2_AoQ_S21_S8_mhit;
  TH2I          *h1_VFTX_Z_AoQ_S21_S8;
  TH2I          *h1_VFTX_Z_AoQ_S21_S8_mhit;
  TH2I          *h1_VFTX_Z_AoQcorr_S21_S8;
  TH2I          *h1_VFTX_Z_AoQcorr_S21_S8_mhit;

  TH1I          *h1_VFTX_beta_S22_S8;
  TH1I          *h1_VFTX_beta_S22_S8_mhit;
  TH1I          *h1_VFTX_AoQ_S22_S8;
  TH1I          *h1_VFTX_AoQ_S22_S8_mhit;
  TH1I          *h1_VFTX_AoQcorr_S22_S8;
  TH1I          *h1_VFTX_AoQcorr_S22_S8_mhit;
  TH2I          *h1_VFTX_x2_AoQ_S22_S8;
  TH2I          *h1_VFTX_x2_AoQ_S22_S8_mhit;
  TH2I          *h1_VFTX_Z_AoQ_S22_S8;
  TH2I          *h1_VFTX_Z_AoQ_S22_S8_mhit;
  TH2I          *h1_VFTX_Z_AoQcorr_S22_S8;
  TH2I          *h1_VFTX_Z_AoQcorr_S22_S8_mhit;
  /*  
  TH1I          *h_VFTX_beta2141;    // s2-s4
  TH1I          *h_VFTX_beta2141_mhit;
  TH1I          *h_VFTX_AoQ2141;
  TH1I          *h_VFTX_AoQ2141_mhit;
  TH1I          *h_VFTX_AoQ2141corr;
  TH1I          *h_VFTX_AoQ2141corr_mhit;
  TH2I          *h_VFTX_x2AoQ2141;
  TH2I          *h_VFTX_x2AoQ2141_mhit;
  TH2I          *h_VFTX_Z1_AoQ2141;
  TH2I          *h_VFTX_Z1_AoQ2141_mhit;
  TH2I          *h_VFTX_Z1_AoQ2141corr;
  TH2I          *h_VFTX_Z1_AoQ2141corr_mhit;
  TH1I          *h_VFTX_beta2241;
  TH1I          *h_VFTX_beta2241_mhit;
  TH1I          *h_VFTX_AoQ2241;
  TH1I          *h_VFTX_AoQ2241_mhit;
  TH1I          *h_VFTX_AoQ2241corr;
  TH1I          *h_VFTX_AoQ2241corr_mhit;
  TH2I          *h_VFTX_x2AoQ2241;
  TH2I          *h_VFTX_x2AoQ2241_mhit;
  TH2I          *h_VFTX_Z1_AoQ2241;
  TH2I          *h_VFTX_Z1_AoQ2241_mhit;
  TH2I          *h_VFTX_Z1_AoQ2241corr;
  TH2I          *h_VFTX_Z1_AoQ2241corr_mhit;
  */
  
  TH1I          *h_MQDC_SCI21_L;
  TH1I          *h_MQDC_SCI21_R;
  TH1I          *h_MQDC_SCI21_dE;
  TH1I          *h_MQDC_SCI22_L;
  TH1I          *h_MQDC_SCI22_R;
  TH1I          *h_MQDC_SCI22_dE;
  TH1I          *h_MQDC_SCI41_L;
  TH1I          *h_MQDC_SCI41_R;
  TH1I          *h_MQDC_SCI41_dE;
  TH1I          *h_MQDC_SCI42_L;
  TH1I          *h_MQDC_SCI42_R;
  TH1I          *h_MQDC_SCI42_dE;
  TH1I          *h_MQDC_SCI81_L;
  TH1I          *h_MQDC_SCI81_R;
  TH1I          *h_MQDC_SCI81_dE;

  /* TH2I          *hSCI_dE24; */
  /* TH2I          *hSCI_dEx2; */
  /* TH2I          *hSCI_dEx2del; */
  /* TH2I          *hSCI_dEx5; */
  /* TGo4PolyCond  *cSCI_detof;  */

  // ID data

  // ID_S4
  TH1I          *hID_BRho[3]; // 0: TA-S2, 1: S2-S4, 2: S2-S8
  TH1I          *hID_beta;    // S2S4
  TH2I          *hID_dEToF;
  TH1I          *hID_AoQ;
  TH1I          *hID_AoQcorr;
  TH1I          *hID_Z1;
  TH1I          *hID_Z2;
  TH1I          *hID_Z3;
  TH1I          *hID_Z4;
  TH2I          *hID_DeltaBrho_AoQ;
  TH2I          *hID_x2AoQ;
  TH2I          *hID_Z1_AoQ;
  TH2I		*hID_Z1_AoQ_zsame;
  TH2I          *hID_Z1_AoQcorr;
  TH2I          *hID_x2x4;
  TH2I          *hID_Z2_AoQ;
  TH2I          *hID_Z2_AoQcorr;
  TH2I          *hID_Z1_Z2;
  TH2I          *hID_Z1_Z3;
  TH2I          *hID_Z1_Z4;
  TH2I          *hID_Z2_Z3;
  TH2I          *hID_Z2_Z4;
  TH2I          *hID_Z3_Z4;
  TH2I          *hID_x4AoQ;
  TH2I          *hID_x2z;
  TH2I          *hID_x4z;
  TH2I          *hID_Z1_Sc21E;
  TH2I          *hID_Z3_Sc21E;
  TH2I          *hID_Z3_Sc22E;

  TH2I  *hgammain_gammaout;
  TH2I  *hdEdegoQ_Z;
  TH2I  *hdEdeg_Z;
  
  // ID_S8
  TH1I *hID_tof_s2s8;
  TH1I *hID_beta_s2s8;
  TH1I *hID_dE_sc81; // z from sc81
  TH1I *hID_Z_sc81; // z from sc81
  TH1I *hID_dE_s2tpc; // z from tpc21222324
  TH1I *hID_Z_s2tpc; // z from tpc21222324
  TH1I *hID_AoQ_s2s8; //
  TH2I *hID_Z_sc81_AoQ_s2s8;
  TH2I *hID_Z_s2tpc_AoQ_s2s8;
  TH2I *hID_dE_s2tpc_AoQ_s2s8;
  TH2I *hID_dE_sc81_AoQ_s2s8;

  // Range histograms
  TH2I * total_range_vs_z;
  TH2I * range_post_degrader_vs_z;
  TH2I * total_range_corr_vs_z;
  TH2I * range_post_degrader_corr_vs_z;
  TH2I * total_range_vs_z2;
  TH2I * range_post_degrader_vs_z2;
  TH2I * total_range_corr_vs_z2;
  TH2I * range_post_degrader_corr_vs_z2;
  TH2I * total_range_corr_vs_highest_z;
  TH2I * range_post_degrader_corr_vs_highest_z;
  
  // Gated Histograms
  TH1I *hTPC_X_gate[7][15];
  TH1I *hTPC_Y_gate[7][15];
  TH1I *hTPC_S2X_gate[15];
  TH1I *hTPC_S2Y_gate[15];
  TH1I *hTPC_S2A_gate[15];
  TH1I *hTPC_S2B_gate[15];
  TH1I *hTPC_S4X_gate[15];
  TH1I *hTPC_S4Y_gate[15];
  TH1I *hTPC_S4A_gate[15];
  TH1I *hTPC_S4B_gate[15];
  TH1I *hTPC_S2TargetX_gate[15];
  TH1I *hTPC_S2TargetY_gate[15];
  TH1I *hTPC_S4TargetX_gate[15];
  TH1I *hTPC_S4TargetY_gate[15];
  TH1I *hMUSIC1_E_gate[8][15];
  TH1I *hMUSIC2_E_gate[8][15];
  TH1I *hMUSIC3_E_gate[8][15];
  TH1I *hMUSIC4_E_gate[8][15];
  TH2I *total_range_vs_z_gate[15];
  TH2I *range_post_degrader_vs_z_gate[15];
  TH2I *total_range_corr_vs_z_gate[15];
  TH2I *range_post_degrader_corr_vs_z_gate[15];

  TH2I          *hID_Z1_AoQ_cdEdegZ;
  TH2I		*hID_Z1_AoQ_zsame_cdEdegZ;
  TH2I          *hID_Z1_AoQcorr_cdEdegZ;
  TH2I		*hID_Z1_AoQcorr_zsame_cdEdegZ;
  TH2I          *hID_Z_AoQ_Range;
  TH2I          *hID_Z_AoQcorr_Range;
  TH2I          *hID_Z_AoQ_Range_deg;
  TH2I          *hID_Z_AoQcorr_Range_deg;
  TGo4WinCond   *cID_x2;
  TGo4WinCond   *cID_x4;
  TGo4WinCond   *cID_x8;
  TGo4PolyCond  *cID_dEToF;
  TGo4PolyCond  *cID_x2AoQ[5];
  TGo4PolyCond  *cID_Z1_AoQ[5];
  TGo4PolyCond  *cID_x4AoQ[5];
  TGo4PolyCond  *cID_dEdeg_Z1;
  TGo4PolyCond  *cID_Range_Z;
  TGo4PolyCond  *cID_Range_deg_Z;

  Float_t rand3(void);
  TRandom3 random3;

  ClassDef(TFRSAnlProc,1)
};

#endif //--------------- TFRSANLPROC_H
