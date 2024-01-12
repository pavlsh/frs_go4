#include "TFRSUnpackProc.h"

#include "Go4StatusBase/TGo4Picture.h"
#include "Go4EventServer/TGo4MbsEvent.h"

#include "TFRSAnalysis.h"
#include "TFRSUnpackEvent.h"

#include "TH1.h"
#include "TH2.h"
#include "foot/dptc/dptc_unpack.c"
#include <iostream>
#include <bitset>
#include "debug_fcns.cpp"
//#include  <stdio.h>

#if 0
#define DEBUG
#endif

TFRSUnpackProc::TFRSUnpackProc() : TFRSBasicProc("FRSUnpackProc")
{

  frs = dynamic_cast<TFRSParameter*>(GetParameter("FRSPar"));
  
}


TFRSUnpackProc::TFRSUnpackProc(const char* name) :  TFRSBasicProc(name)
{
  // load VFTX calibration parameters
  m_VFTX_Bin2Ps();

  hTrigger = MakeH1I("Unpack", "new readout Trigger", 16, 0.5, 16.5);
  hpTrigger = MakeH1I("Unpack", "physics trigger bit, tpat", 16, 0.5, 16.5);
  hTpat = MakeH1I("Unpack", "Tpat", 16, 0.5, 16.5);
  hNbTrig = MakeH1I("Unpack/tpat_combinations", "Trigger Number", 16, 0.5, 16.5);
  hCombiTrig2  = MakeH2I("Unpack/tpat_combinations","Two Trigger Combinations",16,0.5,16.5,16,0.5,16.5,"first trig","second trig",1);// two trigger/event combinations

  h1_wr_diff_FRS_TM = MakeH1I("Unpack/wr_diff_FRS_TM", "WR difference FRS-TM", 2400, -1200, 1200);

  frs = dynamic_cast<TFRSParameter*>(GetParameter("FRSPar"));
  //ModSetup = dynamic_cast<TModParameter*>(GetParameter("ModPar"));
  if(frs == nullptr)
    {
      std::cout<<"E> FRS parameters not set properly, it is nullptr !"<<std::endl;
      std::exit(-1);
    }
  previousTimeStamp[0] = 0;
  previousTimeStamp[1] = 0;
  previousTimeStamp[2] = 0;
  currentTimeStamp = 0;

  // reset stat counter, this is probably not needed, just now for versus event number plots
  for(int mod=0; mod<VFTX_N; mod++)
    for(int ch=0; ch<VFTX_MAX_CHN; ch++)
      Vftx_Stat[mod][ch]=0;
  
  bool remove_histos = (frs!=nullptr) && (!frs->fill_raw_histos);

  for(int n=0;n<32;n++){
      hVME_MAIN_11[n]         = MakeH1ISeries("Unpack/VME_MAIN", 11, 94, n, remove_histos);
      hVME_MAIN_14[n]         = MakeH1ISeries("Unpack/VME_MAIN/V792", 14, 94, n, remove_histos);
      
      hVME_MAIN_TDC_V1290[n]  = MakeH1ISeriesV1290raw("Unpack/VME_MAIN/TDC_V1290_MUSIC41_42_and_Sci", 0, 94, n, remove_histos);
      hVME_MAIN_TDC_V1290_Multip[n]  = MakeH1ISeriesV1290multip("Unpack/VME_MAIN/TDC_V1290_MUSIC41_42_and_Sci", 0, 94, n, remove_histos);
      hVME_MAIN_TDC_V1290_hit[n] = MakeH1I("Unpack/VME_MAIN/TDC_V1290_MUSIC41_42_and_Sci/00", Form("newHit_v1290_%d",n), 10, 0, 10, "hits", 2, 3, "");   // multi-hit of v1290
      //hVME_MAIN_TDC_V1290[n]  = MakeH1ISeriesLabel("Unpack/VME_MAIN/TDC_V1290_MUSIC41_42_and_Sci", "TDC_V1290", 0, n, remove_histos);
      //hVME_MAIN_TDC_V1290_Multip[n]  = MakeH1ISeriesLabel("Unpack/VME_MAIN/TDC_V1290_MUSIC41_42_and_Sci", "TDC_V1290_multip", 0, n, remove_histos);
      
      hVME_MRTOF_TDC_V1190[n]  = MakeH1ISeriesV1190raw("Unpack/VME_MRTOF/TDC_V1190", 0, 1, n, remove_histos); // crate number must be different from others. otherwise, this will produce same histoname as v1290-main, and cause random crash !!! 
      /*
      hVME_USER_8[n]  = MakeH1ISeries("Unpack/VME_USER",  8, 1, n, remove_histos);
      hVME_USER_9[n]  = MakeH1ISeries("Unpack/VME_USER",  9, 1, n, remove_histos);
      hVME_USER_11[n]  = MakeH1ISeries("Unpack/VME_USER",  3, 1, n, remove_histos); 
      hVME_USER_12[n] = MakeH1ISeries("Unpack/VME_USER", 12, 1, n, remove_histos);
      */
      hVME_USER_8[n]  = MakeH1ISeriesLabel("Unpack/VME_USER",  8, "TDC_MWPC", 36, n, remove_histos);//"LaszloSuraj"
      hVME_USER_9[n]  = MakeH1ISeriesLabel("Unpack/VME_USER",  9, "TDC_MWPC", 36, n, remove_histos);
      hVME_USER_10[n]  = MakeH1ISeriesLabel("Unpack/VME_USER",  10, "ADC_MUSIC41_and_42", 36, n, remove_histos);
      //hVME_USER_11[n]  = MakeH1ISeriesLabel("Unpack/VME_USER",  11, "ADC_MUSIC41_and_42", 36, n, remove_histos);
      hVME_USER_12[n] = MakeH1ISeriesLabel("Unpack/VME_USER", 12, "ADC_TOF_TAC", 36, n, remove_histos);
      
      hVME_USER_2[n]  = MakeH1ISeries("Unpack/VME_USER",  2, 36, n, remove_histos);// should be different number from others. change from 3 to 2 in the second input
      hVME_TOF_11[n] = MakeH1ISeries("Unpack/VME_TOF", 11, 12, n, remove_histos); //S2 crate
      hVME_TOF_16[n] = MakeH1ISeries("Unpack/VME_TOF", 16, 12, n, remove_histos);
      hVME_TPCS2_13[n]   = MakeH1ISeries("Unpack/VME_TPCS2", 13,  5, n, remove_histos);
      hVME_TPCS2_12[n]  = MakeH1ISeries("Unpack/VME_TPCS2", 12, 5, n, remove_histos);
      //hVME_TPCS4_0[n]   = MakeH1ISeries("Unpack/VME_TPCS4", 0,  3, n, remove_histos);
      //hVME_TPCS4_1[n]   = MakeH1ISeries("Unpack/VME_TPCS4", 1,  3, n, remove_histos);
      hVME_ACTSTOP_10[n] = MakeH1ISeries("Unpack/VME_ACTSTOP_10", 10, 4, n, remove_histos);
      hVME_ACTSTOP_12[n] = MakeH1ISeries("Unpack/VME_ACTSTOP_12", 12, 4, n, remove_histos);
      hVME_ACTSTOP_14[n] = MakeH1ISeries("Unpack/VME_ACTSTOP_14", 14, 4, n, remove_histos);
      hVME_ACTSTOP_16[n] = MakeH1ISeries("Unpack/VME_ACTSTOP_16", 16, 4, n, remove_histos);
      hVME_ACTSTOP_18[n] = MakeH1ISeries("Unpack/VME_ACTSTOP_18", 18, 4, n, remove_histos);
      hVME_ACTSTOP_20[n] = MakeH1ISeries("Unpack/VME_ACTSTOP_20", 20, 4, n, remove_histos);
  }

  for(int n=0; n<16; n++){
    hVME_TRMU_ADC[n] = MakeH1I("Unpack/VME_TRMU/ADC", Form("newVME_TRMU_ADC_%d",n), 4096, 0,  0x10000, "channels", 2, 3, "");
    hVME_TRMU_TDC[n] = MakeH1I("Unpack/VME_TRMU/TDC", Form("newVME_TRMU_TDC_%d",n), 4096, 0, 0x10000, "channels", 2, 3, "");
  }
  for(int n=0; n<2; n++){
    hVME_TRMU_Trigger[n] = MakeH1I("Unpack/VME_TRMU/Trigger", Form("newVME_TRMU_Trigger_%d",n), 4096, 0,  0x2000, "channels", 2, 3, "");
  }

  //for s530 fission crate by J.Zhao 20210226 
  for(int n=0;n<32;n++){
    hVME_Scaler_s530[n]  = MakeH1ISeries("Unpack/VME_Fission/Scaler_s530", 0, 6, n, remove_histos);        // scaler SIS-3820 in s530 crate
    hVME_hit_v1290_s530[n] = MakeH1I("Unpack/VME_Fission/hit_v1290_s530", Form("newHit_v1290_s530_%d",n), 20, 0, 10, "channels", 2, 3, "");   // multi-hit of v1290

    hVME_Lt_v1290_s530[n] = MakeH1I("Unpack/VME_Fission/Lt_v1290_s530", Form("newLt_v1290_s530_%d",n), 4096, 0,  0x100000, "channels", 2, 3, "");  // timing from v1290 in s530 crate
    hVME_Rt_v1290_s530[n] = MakeH1I("Unpack/VME_Fission/Rt_v1290_s530", Form("newRt_v1290_s530_%d",n), 4096, 0,  0x100000, "channels", 2, 3, "");  // timing from v1290 in s530 crate
  }
  
  for(int n=0; n<16; n++){
    hVME_Lqdc_s530[n] = MakeH1I("Unpack/VME_Fission/MDPP_Lqdc_s530", Form("newMDPP_Lqdc_s530_%d",n), 4096, 0,  4096, "channels", 2, 3, "");       // Long gate qdc, MDPP in s530 crate
    hVME_Sqdc_s530[n] = MakeH1I("Unpack/VME_Fission/MDPP_Sqdc_s530", Form("newMDPP_Sqdc_s530_%d",n), 4096, 0,  4096, "channels", 2, 3, "");       // Short gate qdc, MDPP in s530 crate
    hVME_TDC_mdpp_s530[n] = MakeH1I("Unpack/VME_Fission/MDPP_TDC_s530", Form("newMDPP_TDC_s530_%d",n), 4096, 0,  0x10000, "channels", 2, 3, "");  // timing from MDPP in s530 crate
    hVME_hit_mdpp_s530[n] = MakeH1I("Unpack/VME_Fission/MDPP_hit_s530", Form("newMDPP_hit_s530_%d",n), 20, 0,  10, "channels", 2, 3, "");         // multi-hit of MDPP
  }
  
  for(int n=0; n<2; n++){
    hVME_MDPP_Trig_s530[n] = MakeH1I("Unpack/VME_Fission/MDPP_Trigger", Form("newMDPP_Trigger_%d",n), 4096, 0,  0x2000, "channels", 2, 3, "");
  }
  //end of s530 fission crate

  
  hVME_MAIN_11All = MakeH2I("Unpack/VME_MAIN","VME_MAIN_11_AllCh",32,0,32,512,0,4096,"#Ch","",1);
  hVME_MAIN_14All = MakeH2I("Unpack/VME_MAIN","VME_MAIN_14_AllCh",32,0,32,512,0,4096,"#Ch","",1);
  hVME_USER_8All  = MakeH2I("Unpack/VME_USER","VME_USER_08_AllCh",32,0,32,512,0,4096,"#Ch","",1);
  hVME_USER_9All  = MakeH2I("Unpack/VME_USER","VME_USER_09_AllCh",32,0,32,512,0,4096,"#Ch","",1);
  hVME_USER_10All  = MakeH2I("Unpack/VME_USER","VME_USER_10_AllCh",32,0,32,512,0,4096,"#Ch","",1);
  //hVME_USER_11All  = MakeH2I("Unpack/VME_USER","VME_USER_11_AllCh",32,0,32,512,0,4096,"#Ch","",1);
  hVME_USER_12All = MakeH2I("Unpack/VME_USER","VME_USER_12_AllCh",32,0,32,512,0,4096,"#Ch","",1);
  hVME_TOF_11All = MakeH2I("Unpack/VME_TOF","VME_FRS_11_AllCh",32,0,32,512,0,4096,"#Ch","",1);
  hVME_TOF_16All = MakeH2I("Unpack/VME_TOF","VME_FRS_16_AllCh",32,0,32,512,0,4096,"#Ch","",1);
  hVME_TPCS2_13All  = MakeH2I("Unpack/VME_TPCS2","VME_TPCS2_13_AllCh",32,0,32,512,0,4096,"#Ch","",1);
  hVME_TPCS2_12All = MakeH2I("Unpack/VME_TPCS2","VME_TPCS2_12_AllCh",32,0,32,512,0,4096,"#Ch","",1);
  hVME_TPCS2_V1190All_firsthit = MakeH2I("Unpack/VME_TPCS2","VME_TPCS2_V1190_AllCh_firsthit",128,0,128,400,0,60000,"#Ch","TDC val",1);
  hVME_TPCS2_V1190All = MakeH2I("Unpack/VME_TPCS2","VME_TPCS2_V1190_AllCh",128,0,128,400,0,60000,"#Ch","TDC val",1);
  hVME_TPCS2_V1190All_bad = MakeH2I("Unpack/VME_TPCS2","VME_TPCS2_V1190_AllCh_BAD",128,0,128,400,0,60000,"#Ch","TDC val",1);
  hVME_TPCS2_V1190_bad_multip = MakeH2I("Unpack/VME_TPCS2","VME_TPCS2_V1190_BAD_MULTIPLICTY",128,0,128,128,0,128,"#Ch","Multp val",1);
  hVME_TPCS2_V1190_multip = MakeH2I("Unpack/VME_TPCS2","VME_TPCS2_V1190_GOOD_MULTIPLICTY",128,0,128,128,0,128,"#Ch","Multp val",1);
//  hVME_TPCS4_0All  = MakeH2I("Unpack/VME_TPCS4","VME_TPCS4_00_AllCh",32,0,32,512,0,4096,"#Ch","",1);
//  hVME_TPCS4_1All  = MakeH2I("Unpack/VME_TPCS4","VME_TPCS4_01_AllCh",32,0,32,512,0,4096,"#Ch","",1);
  hVME_TRMU_ADCAll = MakeH2I("Unpack/VME_TRMU", "VME_TRMU_ADC_Allch",16,0,16,512,0,0x2000,"#Ch","",1);
  hVME_TRMU_TDCAll = MakeH2I("Unpack/VME_TRMU", "VME_TRMU_TDC_Allch",16,0,16,512,0,0x10000,"#Ch","",1);
  hVME_ACTSTOP_10All= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_10Allch",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_12All= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_12Allch",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_14All= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_14Allch",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_16All= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_16Allch",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_18All= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_18Allch",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_20All= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_20Allch",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_10All_Tpat4_8= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_10Allch_Tpat4_8",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_12All_Tpat4_8= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_12Allch_Tpat4_8",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_14All_Tpat4_8= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_14Allch_Tpat4_8",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_16All_Tpat4_8= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_16Allch_Tpat4_8",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_18All_Tpat4_8= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_18Allch_Tpat4_8",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_20All_Tpat4_8= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_20Allch_Tpat4_8",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_10All_Tpat4= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_10Allch_Tpat4",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_12All_Tpat4= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_12Allch_Tpat4",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_14All_Tpat4= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_14Allch_Tpat4",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_16All_Tpat4= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_16Allch_Tpat4",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_18All_Tpat4= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_18Allch_Tpat4",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_20All_Tpat4= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_20Allch_Tpat4",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_10All_Tpat8= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_10Allch_Tpat8",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_12All_Tpat8= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_12Allch_Tpat8",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_14All_Tpat8= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_14Allch_Tpat8",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_16All_Tpat8= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_16Allch_Tpat8",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_18All_Tpat8= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_18Allch_Tpat8",32,0,32,512,0,4096,"#C","",1);
  hVME_ACTSTOP_20All_Tpat8= MakeH2I("Unpack/VME_ACTSTOP","VME_ACTSTOP_20Allch_Tpat8",32,0,32,512,0,4096,"#C","",1);

  h_UnpackStatus = MakeH2I("Unpack","Status",2*21*32,0.,2*21*32,10,0.,10.,"#Ch","Status",1);
  h_TSFlagStatus = MakeH2I("Unpack","TS_flagStatus",1000,0,1000,10,0,10,"Diff_TS","Status",1);
  FOOT_RAW_HIST_IMPL(h2_foot_raw)

  // --- vftx ---
  for (int module=0; module<VFTX_N; module++) {
    for(int channel=0; channel<VFTX_MAX_CHN; channel++){
      h1_vftx_lead_mult[module][channel] = (TH1*) MakeTH1('I', Form("Unpack/VFTX_%i/Mult/VFTX%i_Lead_Mult_ch%02d",module,module,channel),Form("VFTX_%i_Lead_Mult_ch%02d",module,channel),50,0.,50.,Form("VFTX %i multiplicity (leading)",module));
      h1_vftx_trail_mult[module][channel] = (TH1*) MakeTH1('I', Form("Unpack/VFTX_%i/Mult/VFTX%i_Trail_Mult_ch%02d",module,module,channel),Form("VFTX_%i_Trail_Mult_ch%02d",module,channel),50,0.,50.,Form("VFTX %i multiplicity (trailing)",module));
      h1_vftx_leading_ft[module][channel] = (TH1*) MakeTH1('I', Form("Unpack/VFTX_%i/FineTime/Leading/VFTX%i_FineTime_leading_ch%02d",module,module,channel),Form("VFTX%i_FineTime_leading_ch%02d",module,channel),1000,0.,1000.,"channels");
      h1_vftx_leading_cc[module][channel] = (TH1*) MakeTH1('I', Form("Unpack/VFTX_%i/Clock/Leading/VFTX%i_Clock_leading_ch%02d",module,module,channel),Form("VFTX%i_Clock_leading_ch%02d",module,channel),9000,0.,9000.,"channels");
      h1_vftx_leading_time[module][channel] = (TH1*) MakeTH1('I', Form("Unpack/VFTX_%i/Time_ps/Leading/VFTX%i_Time_leading_ch%02d",module,module,channel),Form("VFTX%i_Time_leading_ch%02d",module,channel),1000,0.,1000.,"channels");
      h1_vftx_leading_timediff2ch0[module][channel] = (TH1*) MakeTH1('I', Form("Unpack/VFTX_%i/TimeDiff/VFTX%i_TimeDiff2Ch0_leading_ch%02d",module,module,channel),Form("VFTX%i_TimeDiff2Ch0_leading_ch%02d",module,channel),20000,-10000.,10000.,"channels");
      h2_vftx_leading_timediff2ch0_event[module][channel] = (TH2*) MakeTH2('I', Form("Unpack/VFTX_%i/TimeDiff_Event/VFTX%i_TimeDiff2Ch0_event_ch%02d",module,module,channel),Form("VFTX%i_TimeDiff2Ch0_event_ch%02d",module,channel),400,0,4000000,2000,-10000.,10000.,"event","channels");
      h2_vftx_leading_timediff2ch8_event[module][channel] = (TH2*) MakeTH2('I', Form("Unpack/VFTX_%i/TimeDiff_Event/VFTX%i_TimeDiff2Ch8_event_ch%02d",module,module,channel),Form("VFTX%i_TimeDiff2Ch8_event_ch%02d",module,channel),400,0,4000000,2000,-10000.,10000.,"event","channels");
      h1_vftx_trailing_ft[module][channel] = (TH1*) MakeTH1('I', Form("Unpack/VFTX_%i/FineTime/Trailing/VFTX%i_FineTime_trailing_ch%02d",module,module,channel),Form("VFTX%i_FineTime_trailing_ch%02d",module,channel),1000,0.,1000.,"channels");
      h1_vftx_trailing_cc[module][channel] = (TH1*) MakeTH1('I', Form("Unpack/VFTX_%i/Clock/Trailing/VFTX%i_Clock_trailing_ch%02d",module,module,channel),Form("VFTX%i_Clock_trailing_ch%02d",module,channel),9000,0.,9000.,"channels");
    }
    h2_vftx_leading_time04[module] = (TH2*) MakeTH2('I', Form("Unpack/VFTX_%i/Time_ps/Leading/VFTX%i_Time_test04",module,module),Form("VFTX%i_Time_leading",module),1000,0.,100000.,1000,0.,100000.,"channels","channels");

  }

	// S2: MTDC-32
	h2_S2_MTDC32_trg0 = MakeH2I("Unpack/S2_MTDC", "h2_S2_MTDC_trg0", 32,0,32, 0xffff>>5,0,0xffff, "Chnls", "TDC chnls", 1);
	h2_S2_MTDC32_trg1 = MakeH2I("Unpack/S2_MTDC", "h2_S2_MTDC_trg1", 32,0,32, 0xffff>>5,0,0xffff, "Chnls", "TDC chnls", 1);

	// S2: MQDC-32
	h2_S2_MQDC32 = MakeH2I("Unpack/S2_MQDC", "h2_S2_MQDC", 32,0,32, 0xfff>>4,0,0xfff, "Chnls", "QDC chnls", 1);
}

TFRSUnpackProc::~TFRSUnpackProc()
{ }

/* Use this fcn to get a slice [lo,hi], 0 <= lo <= hi < 32
 * where `lo` is low-bit, and `hi` is high-bit of the slice
 * of a 32-bit word passed as first parameter. To get one specific bit,
 * call it with lo==hi */
Int_t get_bits(int value, uint32_t lo, uint32_t hi) {
	uint32_t mask = (uint32_t)(1ull << (hi-lo+1)) - 1;
	return (value >> lo) & mask;
}

Int_t getbits(Int_t value, int nword, int start, int length)
{
  UInt_t buf = (UInt_t) value;
  buf = buf >> ((nword-1)*16 + start - 1);
  buf = buf & ((1 << length) - 1);
  return buf;
}

Int_t get2bits(Int_t value, int nword, int start, int length)
{
  UInt_t buf = (UInt_t) value;
  buf = buf >> ((nword-1)*32 + start - 1);
  buf = buf & ((1 << length) - 1);
  return buf;
}

int myevent;
Bool_t TFRSUnpackProc::BuildEvent(TGo4EventElement* output)
{

  TFRSUnpackEvent* tgt = dynamic_cast<TFRSUnpackEvent*> (output);
  if (tgt==nullptr)
    return kFALSE;
  tgt->SetValid(kFALSE);  // by default output event rejected

  TGo4MbsEvent *fInput = dynamic_cast<TGo4MbsEvent*> (GetInputEvent());
  if (fInput==nullptr)
    return kFALSE;

  /*  Put the event header stuff into output event  */
  tgt->qlength   = fInput->GetDlen()   ;
  tgt->qtype     = fInput->GetType()   ;
  tgt->qsubtype  = fInput->GetSubtype();
  tgt->qdummy    = fInput->GetDummy()  ;
  tgt->qtrigger  = fInput->GetTrigger();
  tgt->qevent_nr = fInput->GetCount()  ;
  //std::cout << "Event Nr = " << tgt->qevent_nr << std::endl;

  // KW these variables were never used
  myevent = tgt->qevent_nr ;
  //int strange ; strange =0 ; 


  // If trigger 12 or 13 or 14 or 15, event fully can be skipped
  if( (fInput->GetTrigger()==12) || (fInput->GetTrigger()==13) || (fInput->GetTrigger()==14) || (fInput->GetTrigger()==15))
    {
      //std::cout << "Trigger = " << fInput->GetTrigger() << ", event skipped" << std::endl;
      return kTRUE;
    }
	 // AIDA sync trigger, we don't care about it.
	 if(fInput->GetTrigger() == 2) {
	 	return kTRUE;
	 }


  // Special event
  if(fInput->GetCount()!=0)
    {
      // std::cout << "qtype : " << fInput->GetType() << std::endl;
      // std::cout << "qlength(=GetDLen) : " << tgt->qlength  << std::endl;
      // std::cout << "qsubtype : " <<  tgt->qsubtype << std::endl;
      /*  Check event length: if it's <= 4 it is empty or screwed up! */
      if(fInput->GetDlen()<=4)
	{
	  std::cout << "Wrong event length: " << fInput->GetDlen() << " =< 4" << std::endl;
	  return kTRUE;
	}
	  BuildFootEvent(tgt, fInput);
      /*  Event OK, now assign pointer to local subevent header! */
      //std::cout << "trigger = " << fInput->GetTrigger() << std::endl;
      fInput->ResetIterator();
      TGo4MbsSubEvent *psubevt = nullptr;

      while ((psubevt=fInput->NextSubEvent())!= nullptr)
	{
	  // start subevents loop
#ifdef DEBUG
	  psubevt->PrintEvent();

#endif
	  //std::cout << "Event Nr = " << tgt->qevent_nr << std::endl;
	  // print data in a similar style as type ev -v
	  if(0){
	    int temp_procid = psubevt->GetProcid();
	    int temp_type   = psubevt->GetType() ;
	    int temp_subtype = (psubevt->GetSubtype());
		int temp_control = psubevt->GetControl();
	    int temp_len     = (psubevt->GetDlen()-2)/2;
	    int *temp_pdata = psubevt->GetDataField();
	    printf("\nprocid=%d, type=%d, subtype=%d, controlid=%d\n",temp_procid,temp_type,temp_subtype,temp_control);
	    for(int ii=0; ii<100; ii++){
			for(int jj=0; jj<5; jj++){
				if(jj+5*ii < temp_len){
					printf("%08x ",*(temp_pdata+jj+5*ii));
					if(4==jj){ printf("\n"); fflush(stdout);
					}
				}
			}
	    }
		printf("\n");
	  }
	  SKIP_FOOT_SUBEV (psubevt)

	  //------check Type and SubType-------//
	  // this part is still hard-coded.    //
	  // 3700 and 3800 are rejected. (spill end?) //
	  if( !( (psubevt->GetType() == 10) && (psubevt->GetSubtype() == 1) && (psubevt->GetControl() == 20))  &&  //(t,s)=(12,1)->(10,1) modified on 2019/11/13
		  !( (psubevt->GetType() == 10) && (psubevt->GetSubtype() == 1) && (psubevt->GetControl() == 21))  &&  //(t,s)=(12,1)->(10,1) modified on 2019/11/13
	      !( (psubevt->GetType() == 12) && (psubevt->GetSubtype() == 1) && (psubevt->GetControl() == 20))  &&  //(t,s)=(12,1)->(10,1) modified on 2019/11/13
		  !( (psubevt->GetType() == 12) && (psubevt->GetSubtype() == 1) && (psubevt->GetControl() == 21))  &&  //(t,s)=(12,1)->(10,1) modified on 2019/11/13
		  !( (psubevt->GetType() == 10) && (psubevt->GetSubtype() == 1) && (psubevt->GetControl() == 30))  &&  //modified on 2023/08/03
	      !( (psubevt->GetType() == 36) && (psubevt->GetSubtype() == 3600) && (psubevt->GetControl() == 20)) &&
		  !( (psubevt->GetType() == 36) && (psubevt->GetSubtype() == 3600) && (psubevt->GetControl() == 21))  &&  
		  !( (psubevt->GetType() == 88) && (psubevt->GetSubtype() == 8800) && (psubevt->GetControl() == 20))  &&
	      !( (psubevt->GetType() == 88) && (psubevt->GetSubtype() == 8800) && (psubevt->GetControl() == 21)) )
	    {  // for all data
	      std::cout << "getprocid  " << psubevt->GetProcid()  << std::endl;
	      std::cout << "getsubtype " << psubevt->GetSubtype() << std::endl;
	      std::cout << "gettype " << psubevt->GetType() << std::endl;
	      std::cout << "Wrong subevent type " << psubevt->GetType() << std::endl;
	      if( 3700==psubevt->GetSubtype() || 3800== psubevt->GetSubtype() ){//blacklist
		std::cout << "bad event, skip this event for moment" << std::endl;
		return kFALSE;
 	      }
	      continue; // skip subevent SL
	    }

	  if( (psubevt->GetType() == 36) && (psubevt->GetSubtype() == 3600) && (psubevt->GetProcid()) == 10) //tpat
	    {
		   Int_t lenMax = (psubevt->GetDlen()-2)/2;
	      Int_t *pdata = psubevt->GetDataField();
	    // int tpat_len = 0xFF & (*pdata) ;
	      for(int ii=0; (ii<lenMax && ii<32) ; ii++){
		     tgt->tpat_main[ii] = *pdata;
		   //   printf("write tpat data %d = 0x%08x\n",ii,tgt->tpat_main[ii]);
		     pdata++;
	      }
			if (lenMax >= 4) { tgt->utpat = tgt->tpat_main[4] & 0b1111111111111111 ; }
			// uphystrig takes priority lower to highest so I will loop desecnding
			for (int it=16;it>0;--it)
			{
			  if ((tgt->utpat & ((Int_t) pow(2,it-1)))>0){
				tgt->uphystrig = it;
				tgt->unbtrig += 1;
			  }
			}
			if(tgt->unbtrig == 2){
      			  for (int it=0;it<16;it++)
      			  {
	 		    if ((tgt->utpat & ((Int_t) pow(2,it)))>0) {
	   			tgt->umaxtrig = it+1;
	 		    }
      			  }
			}
			continue;
	    }


	  /************************************************************************/
	  /* Here we go for the different triggers.....                           */
	  /************************************************************************/
    if (((psubevt->GetType() == 12) && (psubevt->GetSubtype() == 1) && (psubevt->GetControl() == 20))||((psubevt->GetType() == 12) && (psubevt->GetSubtype() == 1) && (psubevt->GetControl() == 21))) //vme event !! WARNING, make sure we can unpack non VME systems
    {
	    switch(fInput->GetTrigger())
	    { // trigger value curently always one, tpat says who triggered
	      case 1:
	      case 2:
	      case 3:
	      case 4:
	      case 5:
	      case 6:
	      case 7:
	      case 8:
	      case 9:
//	      case 10:
//	      case 11:
	        Event_Extract(tgt,psubevt);
	        break;
	      // here do not need to put skip fir trig=12-15, because these are skipped already in the beginning of Unpack.
         case 10:
         case 11:
		   case 12:
		   case 13:
     //      TimeStampExtract(tgt,psubevt);
			  break ;
	      default:
	        {
		        std::cout << "Invalid trigger " << fInput->GetTrigger() << std::endl;
		        return kFALSE;
	        }

	     }  // switch on trigger value
     }//end test of vme event

    if (((psubevt->GetType() == 10) && (psubevt->GetSubtype() == 1) && (psubevt->GetControl() == 20))||((psubevt->GetType() == 10) && (psubevt->GetSubtype() == 1) && (psubevt->GetControl() == 21))) //nurdlib stimstamp subevent shall be type 10
    {
	 	 if(psubevt->GetProcid() != 60)
	    	TimeStampExtract(tgt,psubevt); 
//	    switch(fInput->GetTrigger())
//	    { // trigger value curently always one, tpat says who triggered
//	      case 1:
//	      case 2:
//	      case 3:
//	      case 4:
//	      case 5:
//	      case 6:
//	      case 7:
//	      case 8:
//	      case 9:
////	      case 10:
////	      case 11:
////	        Event_Extract(tgt,psubevt);
////	        break;
//	      // here do not need to put skip fir trig=12-15, because these are skipped already in the beginning of Unpack.
//         case 10:
//         case 11:
//		   case 12:
//		   case 13:
//           TimeStampExtract(tgt,psubevt);
//			  break ;
//	      default:
//	        {
//		        std::cout << "Invalid trigger " << fInput->GetTrigger() << std::endl;
//		        return kFALSE;
//	        }
//
//	     }  // switch on trigger value
     }//end timestamp subevent

    if ((psubevt->GetType() == 10) && (psubevt->GetSubtype() == 1) && (psubevt->GetControl() == 30)) //vme mvlc event !! WARNING, make sure we can unpack non VME systems
    {
	    switch(fInput->GetTrigger())
	    { // trigger value curently always one, tpat says who triggered
	      case 1:
	      case 2:
	      case 3:
	      case 4:
	      case 5:
	      case 6:
	      case 7:
	      case 8:
	      case 9:
//	      case 10:
//	      case 11:
	        Event_Extract_MVLC(tgt,psubevt);
	        break;
	      // here do not need to put skip fir trig=12-15, because these are skipped already in the beginning of Unpack.
         case 10:
         case 11:
		   case 12:
		   case 13:
     //      TimeStampExtract(tgt,psubevt);
			  break ;
	      default:
	        {
		        std::cout << "Invalid trigger " << fInput->GetTrigger() << std::endl;
		        return kFALSE;
	        }

	     }  // switch on trigger value
     }//end test of vme mvlc event
	} // end subevents loop


    } // end special event

  FillHistograms(tgt); // fill histograms from output event
 
  tgt->SetValid(kTRUE); // accept event
  return kTRUE;
} // close unpack proc

// KW this function returns nothing, and should be void
//Bool_t TFRSUnpackProc::TimeStampExtract(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* psubevt, int)
void TFRSUnpackProc::TimeStampExtract(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* psubevt, int)
{
  Int_t *pdata = psubevt->GetDataField();
  Int_t len = 0;
 // Int_t vme_chn; //usually redefined for each crate
  Int_t lenMax = (psubevt->GetDlen()-2)/2;
  //decide to print the WR identifier
  int hexwrid ; 
  hexwrid = getbits(*pdata,1,9,4) ;

  event_out->wrid = hexwrid ;  
  pdata++ ; len ++ ; //skip WR identifier for today

  switch(psubevt->GetProcid())
  {
   case 20:
   case 30:
   case 35:
    //std::cout <<"This is S2 crate in TimeStampExtract TFRSUnpackProc, wrid ID:" << event_out->wrid << std::endl ;//JZ 2022-05-04
   case 40:
   case 45:
   case 50:
   case 120:
     break ; 
   case 10:	// Main crate     
	  if((psubevt->GetType() == 10) && (psubevt->GetSubtype() == 1) && lenMax < 15) // in this case, time stamp data.
	   {
	     int ii =0;
	     while(len < lenMax){
	       event_out->timestamp_main[ii] = *pdata;
	       pdata++; len++; ii++;
	     }
     //std::cout <<"TimeStampExtract TFRSUnpackProc, proc ID:" << psubevt->GetProcid()<< std::endl ;//JZ 2022-05-04
	   }
     //std::cout <<"This is Main crate in TimeStampExtract TFRSUnpackProc, wrid ID:" << event_out->wrid << std::endl ;//JZ 2022-05-04	
	break ;

    default:
     std::cout <<"proc ID unknow in TimeStampExtract TFRSUnpackProc " << psubevt->GetProcid()<< std::endl ; 
     break ; 
  }  
}

void TFRSUnpackProc::TimeStampExtract_TravMus(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* psubevt, int) {
  Int_t *pdata = psubevt->GetDataField();
  Int_t len = 0;
  Int_t lenMax = (psubevt->GetDlen()-2)/2;
  uint32_t wr_id = get_bits(*pdata++,0,11); len++;
if(! (wr_id == 0x200)) { //
	printf("travmus WR ID not 0x200 while trying to match it ...! It is %x\n", wr_id);
	return;
}
if(! ((*pdata & 0xffff0000) == 0x03e10000)) {
	printf("travmus not matching LoLo of WR ...! It is %x\n", *pdata);
	return;
}
	uint64_t travmus_wr = 
		((uint64_t)(*pdata++ & 0xffff)) |
		((uint64_t)(*pdata++ & 0xffff) << 16) |
		((uint64_t)(*pdata++ & 0xffff) << 32) |
		((uint64_t)(*pdata++ & 0xffff) << 48);
	len += 4;

  event_out->travmus_wr = travmus_wr; 
}

void TFRSUnpackProc::TimeStampExtract_MVLC(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* psubevt, int) {
  Int_t *pdata = psubevt->GetDataField();
  Int_t len = 0;
 // Int_t vme_chn; //usually redefined for each crate
  //Int_t lenMax = (psubevt->GetDlen()-2)/2;
  //decide to print the WR identifier
  uint32_t wr_id = get_bits(*pdata++,0,11); len++;
if(! (wr_id == 0x100)) {
	printf("FRS WR ID not 0x100 while trying to match it ...! It is %x\n", wr_id);
	return;
}
if(! ((*pdata & 0xffff0000) == 0x03e10000)) {
	printf("FRS not matching LoLo of WR ...! It is %x\n", *pdata);
	return;
}
	uint64_t frs_wr = 
		((uint64_t)(*pdata++ & 0xffff)) |
		((uint64_t)(*pdata++ & 0xffff) << 16) |
		((uint64_t)(*pdata++ & 0xffff) << 32) |
		((uint64_t)(*pdata++ & 0xffff) << 48);
	len += 4;
	event_out->frs_wr = frs_wr;
  }

Bool_t TFRSUnpackProc::Event_Extract(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* psubevt, int){
  Int_t *pdata = psubevt->GetDataField();
  Int_t len = 0;
  // Int_t vme_chn; //usually redefined for each crate
  Int_t lenMax = (psubevt->GetDlen()-2)/2;
  // KW event_flag never used
  //Int_t event_flag = *pdata++;  len++; event_out->mrtof_ts = *pdata;  *pdata++;  len++;// skip the first two words of an event, read time from header for mrtof analysis
  pdata++;  len++;
  event_out->mrtof_ts = *pdata;
  pdata++;  len++;// skip the first two words of an event, read time from header for mrtof analysis
  // end KW

  if(*pdata != ((Int_t) 0xbabababa)){ std::cout<<"E> ProcID :"<< psubevt->GetProcid() << "First Barrier missed in Event_Extrackt!" << *pdata<< std::endl; 
    std::cout.setf (std::ios::hex, std::ios::basefield ) ; 
    std::cout.setf ( std::ios::showbase) ;
    std::cout <<*pdata <<std::endl ; 
    std::cout.setf (std::ios::dec, std::ios::basefield) ;
    
    std::cout <<" type : " <<  psubevt->GetType() <<" subtype "<<psubevt->GetSubtype() <<std::endl ;      
    return kTRUE ; 
  }
  pdata++; len++;

  switch(psubevt->GetProcid())
  {
    //===========
      case 30:  // now 30 is FRS CRATE (06.07.2018)
   {

     
//     for(int ii=0; ii<100; ii++){
//       printf("%03d : 0x%08x \n",ii,*(pdata+ii));
//       fflush(stdout);
//     }
      Int_t isADCgeo2 ; isADCgeo2= 0 ; 
     // check if scaler is in
                //Int_t scalertest; scalertest = getbits(*pdata,2,5,12) ; 
 		//std::cout <<" should be 72 "<<scalertest <<endl ; 
		//if (scalertest == 72) // 72 is 0x48 for the header and number of words
                //if (scalertest == 68) // 68 is 0x44 for the header and number of words
		{
		 // start of v830 (do not remove this bracket)
	    Int_t vme_geo = getbits(*pdata,2,12,5);
	    Int_t vme_type = getbits(*pdata,2,9,3);
	    Int_t vme_nlw =  getbits(*pdata,2,3,6);
	    //printf("Proc ID 30, geo %d, type %d, length %d\n", vme_geo, vme_type,vme_nlw);
	    
	    pdata++; len++;
	    //printf("Proc ID 30, geo %d, type %d\n", vme_geo, vme_type);
	    if(vme_type!=4){   std::cout<<"E> Scaler type missed match ! Proc ID 30 GEO "<<vme_geo<<" "<<" type 4 =/="<<vme_type<<std::endl; break ;  }
	    for(int i_ch=0; i_ch<vme_nlw; i_ch++){
	      event_out->scaler_frs[i_ch] = getbits(*pdata,1,1,26);

	      //event_out->scaler_frs[i_ch] = *pdata;
	      //printf("scaler_frs[ch=%d] = %d\n",i_ch,getbits(*pdata,1,1,26));
	      pdata++; len++;
	    }
	   }
                //else{   std::cout<<"No scaler in Proc ID 30 GEO"<<std::endl;}
		//end of V830 

	//----- v775 TDC/V785 ADC -----//
	while (len < lenMax){
	  /* read the header longword and extract slot, type & length  */
	  Int_t vme_chn = 0;
	  Int_t vme_geo = getbits(*pdata,2,12,5);
	  Int_t vme_type = getbits(*pdata,2,9,3);
	  Int_t vme_nlw =  getbits(*pdata,1,9,6);
	  //	  std::cout << "vme_geo, vme_type, vme_nlw ="<< vme_geo << ", " <<  vme_type << ", " << vme_nlw <<std::endl ;
	  pdata++; len++;
     if (2 == vme_geo){ isADCgeo2=1 ; }

	  /* read the data */
	  if ((vme_nlw > 0 && 2 == vme_type )) {
	    for(int i=0;i<vme_nlw;i++) {
	      vme_geo = getbits(*pdata,2,12,5);
	      vme_type = getbits(*pdata,2,9,3);
	      vme_chn = getbits(*pdata,2,1,5);
	      event_out->vme_frs[vme_geo][vme_chn] = getbits(*pdata,1,1,12);
	      printf("vme_frs[geo=%d][ch=%d] = %d\n",vme_geo,vme_chn,getbits(*pdata,1,1,12));
	      pdata++; len++;
	    }
	  }

	  /* sanity check at the end of a v7x5 unpacking */
	  vme_type = getbits(*pdata,2,9,3);
     if (vme_type != 4 ) {std::cout <<"issue in unpacking Proc Id 30, existing" <<std::endl ; break ; }
      pdata++; len++;


	}  /* end of the while... loop  */
	 if (1 == isADCgeo2 )
    {
	   for (int i =0 ; i < 32 ; i++)
	   {
	    event_out->vme_frs[3][i] = event_out->vme_frs[2][i] ; 
	   }
	 }	
  }

	break;

     //================
      case 10:	// Main crate

	if((psubevt->GetType() == 10) && (psubevt->GetSubtype() == 1) && lenMax < 15) // in this case, time stamp data.
	  {
	    int ii =0;
	    while(len < lenMax){
	      event_out->timestamp_main[ii] = *pdata;
	      pdata++; len++; ii++;
	    }
	    break;
	  }

      // next should be v1290 TDC
      if (len < lenMax){
	Int_t vme_geo = getbits(*pdata,1,1,5);
	Int_t vme_type = getbits(*pdata,2,12,5);
	pdata++; len++;
	Int_t multihit = 0;//, counter = 0;

#ifdef DEBUG
	std::cout<<"mTDC geo = "<<vme_geo<<", type = "<<vme_type<<std::endl;
#endif

	if (vme_type == 8)
	  {
	    while (len < lenMax)
	      {
#ifdef DEBUG
		std::cout<<"word : "<<std::bitset<32>(*pdata)<<" ";
#endif
		vme_type = getbits(*pdata,2,12,5);
		if(vme_type==1) // headerTDC
		  {
		    pdata++; len++;
		  }
		//multihit = 0;
#ifdef DEBUG
		std::cout<<"reading :"<<vme_type<<std::endl;
#endif
		vme_type = getbits(*pdata,2,12,5);
#ifdef DEBUG
		std::cout<<"word : "<<std::bitset<32>(*pdata)<<" type:"<<vme_type<<" ";
#endif
		if(vme_type == 0)
		  {
		    // this indicates a TDC measurement
		    Int_t vme_chn = getbits(*pdata,2,6,5);
		    Int_t LeadingOrTrailing = getbits(*pdata,2,11,1);
		    Int_t value = getbits(*pdata,1,1,21);

		    multihit = event_out->nhit_v1290_main[vme_chn][LeadingOrTrailing];
#ifdef DEBUG
		    std::cout << "     tdc vme_chn = " << vme_chn;
		    std::cout << " multihit: " << multihit << " "<<endl;
#endif		    
		    hVME_MAIN_TDC_V1290_Multip[vme_chn]->Fill(multihit); //
		    if (multihit >= 10)
		      {
			pdata++; len++;
			continue;
		      }
		    if(LeadingOrTrailing == 0)
		      {
#ifdef DEBUG
			std::cout << " +-> tdc L value = " << value << std::endl;
#endif
			if (value > 0)
			  {
			    event_out->leading_v1290_main[vme_chn][multihit] = value;
			    hVME_MAIN_TDC_V1290[vme_chn]->Fill(value); //
			  }
		      }
		    else
		      {
#ifdef DEBUG
			std::cout << " +-> tdc T value = " << value << std::endl;
#endif
			if (value > 0)
			  event_out->trailing_v1290_main[vme_chn][multihit] = value;
		      }
		    event_out->nhit_v1290_main[vme_chn][LeadingOrTrailing]++;

		    pdata++; len++;
		  }
		else
		  {
		    // TDC trailer vme_type == 3
#ifdef DEBUG
		    std::cout<<"\n";
#endif
		    if(vme_type != 3 && vme_type !=16)
		      std::cout<<"E> MTDC strange type :"<<vme_type<<std::endl;
		    if(vme_type==16)
		      {
			Int_t vme_geoEnd = getbits(*pdata,1,1,5);
			if(vme_geo!=vme_geoEnd)
			  std::cout<<"E> MTDC strange end buffer header :"<<vme_type<<" "<<vme_geo<<" != "<<vme_geoEnd<<std::endl;
			pdata++; len++;
			break;
		      }
		    pdata++; len++;
		  }
	      } //while len loop
	  }//if type==8
      }//end if len more due to strange logic // end of V1290


      //---- 3rd barrier ---
      if(*pdata != ((Int_t) 0xbabababa)){ std::cout<<"E> ProcID 10 : Barrier-3 missed !" << *pdata  << std::endl; }
      pdata++; len++;

      //----- Next is V830 Scaler-----
      	{ // start of v830 (do not remove this bracket)
				  Int_t vme_geo = getbits(*pdata,2,12,5);
				  Int_t vme_type = getbits(*pdata,2,9,3);
				  Int_t vme_nlw =  getbits(*pdata,2,3,6);
				  //printf("Proc ID 10, geo %d, type %d\n", vme_geo, vme_type);
				  pdata++; len++;
				  if(vme_type!=4){   std::cout<<"E> Scaler type missed match ! ProcID 10 GEO "<<vme_geo<<" "<<" type 4 =/="<<vme_type<<std::endl;  break ;}
				  for(int i_ch=0; i_ch<vme_nlw; i_ch++){
				    //event_out->scaler_main[i_ch] = *pdata;
				    event_out->scaler_main[i_ch] = getbits(*pdata,1,1,26);
				    //printf("pdata = 0x%08x, i_ch=%d \n", *pdata, i_ch);
       		  int chan ; int bit ; 
				  chan  = get2bits(*pdata,1,28,5) ; 
			     bit = get2bits(*pdata,1,27,1) ; 
			     if ((chan != i_ch) || (0 != bit)) 
			     {
			       cout<< "error in unpacking v830 from FRS MAIN" ; 
			       if (0 != bit ) {cout <<"26th bit in channel word is not 0 "<<endl ;}
			       else { cout<<"channel error asignement, unpacker says : "<<i_ch<< "while module say "<<chan<<endl ; }
			     }
			  

				    //printf("scaler_main[ch=%d] = %d\n",i_ch,getbits(*pdata,1,1,26));
							 pdata++; len++;
				  }
		} //end of V830
      

		//--------the rest of the unpacking...V792-------- (while loop removed while includig second V792 10.02.21)
		//REMOVED 26022021
		//while (len < (psubevt->GetDlen()-2)/2)
//		{
//				  Int_t vme_chn = 0;
//				  Int_t vme_geo = getbits(*pdata,2,12,5);
//				  Int_t vme_type = getbits(*pdata,2,9,3);
//				  Int_t vme_nlw =  getbits(*pdata,1,9,6);
//				  pdata++; len++;
//				  //     printf("%08x %d\n",pdata[-1],vme_nlw) ; 
//#ifdef DEBUG
//				  std::cout<<"data "<<vme_geo<<" "<<vme_type<<" "<<vme_nlw<<" idmod:"<<IdMod<<std::endl;
//#endif
//				  // read the data
//				  if(vme_type == 6){
//							 // not valid data !
//				  }
//				  if ((vme_type == 2) && (vme_nlw > 0)){
//							 for(int i=0;i<vme_nlw;i++){
//										vme_geo = getbits(*pdata,2,12,5);
//										vme_type = getbits(*pdata,2,9,3);
//										vme_chn = getbits(*pdata,2,1,5);
//										// VERY UGLY PLEAS EUPDATE TO NUMBER 3
//										if (vme_geo != 3 ) {cout << "problem first QDC vme Main"<< endl ; }
//										vme_geo = 11 ; 
//										event_out->vme_main[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
//										pdata++; len++;
//							 }
//							 // read and ignore the expected "end-of-block" longword
//							 pdata++; len++;
//				  }
//		}  // end of the while... loop
		//--------unpacking...2nd V792--------
	{
	  Int_t vme_chn = 0;
	  Int_t vme_geo = getbits(*pdata,2,12,5);
	  Int_t vme_type = getbits(*pdata,2,9,3);
	  Int_t vme_nlw =  getbits(*pdata,1,9,6);
	  pdata++; len++;

#ifdef DEBUG
	  std::cout<<"data "<<vme_geo<<" "<<vme_type<<" "<<vme_nlw<<" idmod:"<<IdMod<<std::endl;
#endif
	  // read the data
	  if(vme_type == 6){
	      // not valid data !
	  }
	  if ((vme_type == 2) && (vme_nlw > 0)){
	    for(int i=0;i<vme_nlw;i++){
	      vme_geo = getbits(*pdata,2,12,5);
	      vme_type = getbits(*pdata,2,9,3);
	      vme_chn = getbits(*pdata,2,1,5);
	      vme_geo = 4; // vme_geo_read=2 but vme_geo_bus=4, caused by this the 4 is hard coded for the time beeing (27.02.21)
	      event_out->vme_main[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
	      pdata++; len++;
	    }
	    // read and ignore the expected "end-of-block" longword
	    pdata++; len++;
	  }
	  }
      
      break; // proc ID 10


      //=========
      //=========
      //case 25:
      case 20:

      //TPC crate

      // Next, here somewhat VME scaler V830 was placed recently.
      // not used ?? (I dont know exact purpose of this scaler)
      // it has a fixed lenth of 17 (32bit-word)
      // 2022May05. We do not have V830 this year.
      // for(int ii=0;ii<17;ii++){
      //  	 pdata++; len++;
      // } 

      // check if end of this procid (with/without barrier)
      if(lenMax == len){ break; }
      //if(*pdata != 0xbabababa){ std::cout<<"E> ProcID 20 : Barrier missed !" << *pdata  << std::endl;} pdata++; len++;
      if(*pdata != (Int_t)0xbabababa){} pdata++; len++;
      if(lenMax == len){ break; }

      // from here V1190
      while(len < lenMax && (*pdata)!=(Int_t)0xbabababa){
        int type_tpc_v1190 = 0x1F & ((*pdata)>>27);
        if(0x8 == type_tpc_v1190){
          //global header, ignore
        }else if(0x1 == type_tpc_v1190){
          //TDC header, ignore
        }else if(0x3 == type_tpc_v1190){
          //TDC trailer, ignore
        }else if(0x4 == type_tpc_v1190){
          //error status
          printf("TPC crate, V1190, error data are found: 0x%08x \n",*pdata); fflush(stdout);
        }else if(0x10 == type_tpc_v1190){
          //global trailer, ignore
        }else if(0x0 == type_tpc_v1190){
          // this is real data
          int ch_tpc_v1190 = 0x7F & ((*pdata)>>19);
          int check_leading_tpc_v1190 = 0x1 & ((*pdata)>>26); //0 leading, 1 trailing
          int data_tpc_v1190 = 0x7FFFF & (*pdata);//19bit
          if(0 == check_leading_tpc_v1190){
            int tmp_nhit_prev = event_out->nhit_v1190_tpcs2[ch_tpc_v1190];
            event_out->nhit_v1190_tpcs2[ch_tpc_v1190] ++;
	    // KW only 16 hits are used, this is completely inconsistent throughout the code, the TFRSUnpackEvent has leading_v1190_tpcs2[128][64];
            if(tmp_nhit_prev<16){ //16 is max prepared. ignore if >=16. nhit will be counted.
              event_out->leading_v1190_tpcs2[ch_tpc_v1190][tmp_nhit_prev]=data_tpc_v1190;
            }
            hVME_TPCS2_V1190All->Fill(ch_tpc_v1190,data_tpc_v1190);
          }
        }else{
          //unknown data type ?
          printf("TPC crate, V1190, strange data are found: 0x%08x \n",*pdata); fflush(stdout);
        }
        pdata++; len++;
      }// end of TDC_V1190

      // check if end of this procid (with/without barrier)
      if(lenMax == len){ break; }
      if(*pdata == (Int_t)0xbabababa){  pdata++; len++; }
      if(lenMax == len){ break; }

      // from here, 2 ADCs (opotional)
      while (len < lenMax){
        Int_t vme_type = 0x7 & ((*pdata)>>24);
        Int_t vme_geo  = 0x1F & ((*pdata)>>27);
        if(2 == vme_type){
          //header, ignore
        }else if(4==vme_type){
          //end-of-block, ignore
        }else if(0==vme_type){
          //data
           Int_t vme_chn    = 0x3F & ((*pdata)>>16);
           Int_t vme_data  =  0x3FFF & (*pdata);
  	       event_out->vme_tpcs2[vme_geo][vme_chn] = vme_data;
           //           printf("TPC crate, geo %d ch %d data %d\n",vme_geo, vme_chn, vme_data); fflush(stdout);
        }else if(*pdata == (Int_t)0xbabababa){
          //barrier between 2 ADCs, for example, ignore
        }else{
          // unknown word?
          printf("TPC crate, ADC data, strange data are found: 0x%08x \n",*pdata); fflush(stdout);
        }
        pdata++; len++;
      }

      break;

     //=========	  
 
  case 35: // --- vftx at S2 --- (2020/Mar/29)
    //no header
    if(*pdata == ((Int_t) 0xbabababa)){
      cout << "pdata = 0x" << (hex) << *pdata << (dec) << endl;
    }
    else{
      event_out->ClearVftx();
      for(int module=0; module<VFTX_N; module++)
	VFTX_Readout(event_out, &pdata, module);
    }
    
    if(*pdata != (Int_t)0xbabababa){ std::cout<<"E> ProcID 35 : 2rd barrier missed !" << *pdata  << std::endl;} pdata++; len++;
    if(lenMax == len){ break; }
 
    {//----- Mesytec MQDC-32 -----
      Int_t vme_geo = 11;
      Int_t vme_type = getbits(*pdata,2,15,2);
      // KW never used
      // Int_t module_id = getbits(*pdata,2,1,8); // depending on board coder settings

      int n_mqdc_bank = 2; //YT,2022May03
      for(int i_bank=0; i_bank<n_mqdc_bank; i_bank++){    //YT,2022May03   
	if(vme_type == 1){ // header
		int vme_nlw = getbits(*pdata,1,1,12); //indicates amount n of following 32-bit words: n-1 events +1 end of event marker)
		for(int i=0;i<vme_nlw;i++) {
		  pdata++;len++;
		  //printf("2: 0x%x\n",*pdata);
		  Int_t  vme_type = getbits(*pdata,2,15,2);
		  if ((vme_type == 0)  && (vme_nlw > 0)){// data
		    Int_t vme_chn = getbits(*pdata,2,1,5); //number of ADC channel
		    Int_t value = getbits(*pdata,1,1,12);
		    // printf("ch=%d, data=%d\n",vme_chn, value);
		    // KW never used
		    // Int_t overfolw = getbits(*pdata,1,16,1); // V = 1 indicates ADC over- or underflow
		    //printf("2: 0x%x\n",*pdata);
		    if (value > 0)
		      {
			event_out->vme_tof[vme_geo][vme_chn] = value;
		      }
		  }
		  else if(vme_type==3) {
		    //printf("This is end of module MQCG\n");// event mark
		  }
		  else printf("ProcID 35: Mesytec MQDC-32: Unknown data\n");
		}
	      }
	  pdata++;len++;
      } //YT,2022May03    
    } //----- end of Mesytec MQDC-32 -----
    
    break; //For 2022FRS DAQ, we do not have more. YT,2022May03 

    if(*pdata != (Int_t)0xbabababa){ std::cout<<"E> ProcID 35 : 3rd barrier missed !" << *pdata  << std::endl;} pdata++; len++;
    if(lenMax == len){ break; }
      
    {//----- Mesytec MTDC-32 -----
      Int_t vme_geo = 16;
      pdata++;len++;
      // KW never used
      //int module_id = getbits(*pdata,2,1,8); // depending on board coder settings
      int vme_type = getbits(*pdata,2,15,2);
      // KW never used
      //int hit = 0;
	   
      if(vme_type == 1){
	int vme_nlw = getbits(*pdata,1,1,12); //indicates amount n of following 32-bit words: n-1 events + 1 end of event marker)
	for(int ii=0; ii<vme_nlw; ii++){
	  pdata++;
	  int value = getbits(*pdata,1,1,16);
	  //printf("real data 0x%08x\n",value);
	       
	  if(vme_type == 0){//check data type
	    int vme_chn = getbits(*pdata,2,1,5);
	    // KW never used
	    //int vme_trig = getbits(*pdata,2,6,1) ; //Trigger channel, {T,chan#} = 32 for trig0, or = 33 for trig1

	    event_out->vme_tof[vme_geo][vme_chn] = value;

	    //  if( (event_out->mtdc01_s530[ch][event_out->hitmtdc01_s530[ch]]) <= 0 ){ //first-come-first-served, for detailed multi-hit analysis, investigation needed.
	    //	 event_out->mtdc01_s530[ch][event_out->hitmtdc01_s530[ch]] = tdc_data;
	    //	 event_out->hitmtdc01_s530[ch] = event_out->hitmtdc01_s530[ch] + 1;
	    //	 printf("event_out->mtdc01_s530[%d][%d] = %d; \n",ch,event_out->hitmtdc01_s530[ch],tdc_data);
	    //       }
	       
	    //printf("TDC data !!! vme_ch = %2d, TDC = %d \n",vme_ch, tdc_data);
	  }else if(3 == getbits(*pdata,2,15,2));
	  // printf("This is end of MTDC.\n");
	  else printf("ProcID 35: Mesyrec MQDC-32: Unknown data\n");
	}
      }
      // check if end of this procid (with/without barrier)
      if(lenMax == len){ break; }
      //if(*pdata != 0xbabababa){ std::cout<<"E> ProcID 35 : 4th barrier missed !" << *pdata  << std::endl;} pdata++; len++;
      if(lenMax == len){ break; }} //----- end of Mesytec MTDC-32 -----

    // Scaler missing SIS_3820
	
    break; 
    //=========
     case 40: // Travel music crate (2020/Jan/23) + 6 ADC for active stopper

    {//-----MDPP module----- (do not remove this bracket)
	 // header
	 Int_t header = *pdata;
	 Int_t nword_mdpp = (0x3FF & header);
	 // KW never used and also missing the getbits!!
	 //Int_t adc_res = (*pdata,1,11,3);
	 //Int_t tdc_res = (*pdata,1,14,3);
	 pdata++; len++ ;
	 
	 // main data (data or time information or end counter)
	 for(int ii=0; ii<nword_mdpp; ii++){
	   int tmp_data = *pdata;
	   if( 1 == (0xF & (tmp_data>>28))   ){
	   int tmp_data = *pdata;
	     //printf("real data 0x%08x\n",tmp_data);
	     int ch = 0x1F & (tmp_data >> 16);
	     int trigger = 0x1 & (tmp_data >> 21);
	     // printf("ch = %2d, trigger = %2d\n",ch,trigger);
	     if(trigger == 0){
	       if(0<=ch && ch<=15){
		 int adc_data = 0xFFFF & tmp_data;
		 //printf("ADC data !!! ch = %2d, ADC = %d \n",ch, adc_data);
		 if( (event_out->vme_trmu_adc[ch]) <= 0 ){ //first-come-first-served, for detailed multi-hit analysis, investigation needed.
		   event_out->vme_trmu_adc[ch] = adc_data;
		 //  printf("event_out->vme_trmu_adc[%d] = 0x%08x; \n",ch,adc_data);
		 }
	       }else if(16<=ch && ch<=31){
		 int tdc_data = 0xFFFF & tmp_data;
		 //printf("TDC data !!! ch = %2d, TDC = %d \n",ch-16, tdc_data);
		 if( (event_out->vme_trmu_tdc[ch-16]) <= 0 ){//first-come-first-served, for detailed multi-hit analysis, investigation needed.
		 event_out->vme_trmu_tdc[ch-16] = tdc_data;
		 //////        	 printf("event_out->vme_trmu_tdc[%d] = 0x%08x; \n",ch-16,tdc_data);
		 }
	       }
	     }else if (trigger == 1){
	       int trigger_data = 0xFFFF & tmp_data;
		 // printf("Trigger time stamp !!! ch = %2d, Tigger time stamp = %d \n",ch, trigger_data);
		 event_out->vme_trmu_trigger[ch] = trigger_data;
		 //  printf("event_out->vme_trmu_adc[%d] = 0x%08x; \n",ch,adc_data);
		 
	     }
	   }else if( 2 == (0xF & (tmp_data>>28))){
	     // printf("ext time stamp 0x%08x\n",tmp_data);
	   }else if(0x0 == tmp_data ){
	     // printf("dummy 0x%08x\n",tmp_data);
	   }else if( 3 == (0x3 & (tmp_data>>30))){
	     // printf("end counter 0x%08x\n",tmp_data);
	   }else{
	     /// printf("unknown data0x%08x\n",tmp_data);
	   }
	   pdata++; len++ ;
	 }
    }//---end of MDPP module ---
       /*    
   //----  barrier ---
      if(*pdata != ((Int_t) 0xbabababa)){ std::cout<<"E> ProcID 40 : Barrier missed !" << std::endl; }
      pdata++; len++;


		// v775 TDC/V785 ADC for the active stopper 
        while (len < lenMax){

	// read the header longword and extract slot, type & length  
	Int_t vme_chn = 0;
	Int_t vme_geo = getbits(*pdata,2,12,5);
	Int_t vme_type = getbits(*pdata,2,9,3);
	Int_t vme_nlw =  getbits(*pdata,1,9,6);
	// printf("pdata=0x%08x\n",*pdata); fflush(stdout);
	//std::cout<< vme_geo << std::endl;
	pdata++; len++;

	// read the data 
	if ((vme_type == 2) && (vme_nlw > 0)) {
	  for(int i=0;i<vme_nlw;i++) {
	    vme_geo = getbits(*pdata,2,12,5);
	    vme_type = getbits(*pdata,2,9,3);
	    vme_chn = getbits(*pdata,2,1,5);
	    //printf("pdata=0x%08x\n",*pdata); fflush(stdout);
		 event_out->vme_actstop[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
	    pdata++; len++;
	  }
	}

	// read and ignore the expected "end-of-block" longword 
	pdata++; len++;
  }  // end of the while... loop  
*/
       break;

     //=========  
     case 45: // S530 crate for 1 CAEN v1290, 1 mesytec MDPP-16-QDC, 1 SIS-3820 Scaler
       //v1290 TDC in s511
       if (len < lenMax){
	Int_t vme_geo = getbits(*pdata,1,1,5);
	Int_t vme_type = getbits(*pdata,2,12,5); 
	pdata++; len++;
	Int_t multihit = 0;//, counter = 0;

#ifdef DEBUG
	std::cout<<"mTDC geo = "<<vme_geo<<", type = "<<vme_type<<std::endl;
#endif

	if (vme_type == 8){
	  //printf("Global header, %d, 0x%08x\n",len, (*pdata));//just for check
	  while (len < lenMax){

#ifdef DEBUG
	  std::cout<<"word : "<<std::bitset<32>(*pdata)<<" ";
#endif
          vme_type = getbits(*pdata,2,12,5);
	  if(vme_type==1) // headerTDC
	    {
	      pdata++; len++;
	    }
		//multihit = 0;
#ifdef DEBUG
	  std::cout<<"reading :"<<vme_type<<std::endl;
#endif
	  vme_type = getbits(*pdata,2,12,5);
#ifdef DEBUG
	  std::cout<<"word : "<<std::bitset<32>(*pdata)<<" type:"<<vme_type<<" ";
#endif
	  if(vme_type == 0)// this indicates a TDC measurement
	    {
	      Int_t vme_chn = getbits(*pdata,2,6,5);
	      Int_t LeadingOrTrailing = getbits(*pdata,2,11,1);
	      Int_t value = getbits(*pdata,1,1,21);
	      //printf("ch %d, leading time 0x%08x\n",vme_chn, value);//just for check
	      multihit = event_out->nhit_v1290_s530[vme_chn][LeadingOrTrailing];

#ifdef DEBUG
	  std::cout << " tdc vme_chn = " << vme_chn;
	  std::cout << " multihit: " << multihit << " ";
#endif
	      if (multihit >= 10){
		pdata++; len++;
		continue;
	      }
	      if(LeadingOrTrailing == 0)
		{
#ifdef DEBUG
		  std::cout << "mhit = "<< multihit <<"  +-> tdc L value = " << value << std::endl;
#endif
		if (value > 0)
		  {
		    event_out->leading_v1290_s530[vme_chn][multihit] = value;
		    //printf("hit %d, leading time 0x%08x\n",vme_chn, value);//just for check
		    hVME_Lt_v1290_s530[vme_chn]->Fill(value);
		  }
		}
	      else{
#ifdef DEBUG
		std::cout << " +-> tdc T value = " << value << std::endl;
#endif
		if (value > 0)
		  {
		    event_out->trailing_v1290_s530[vme_chn][multihit] = value;
		    hVME_Rt_v1290_s530[vme_chn]->Fill(value);
		  }
	      }
	      event_out->nhit_v1290_s530[vme_chn][LeadingOrTrailing]++;
	      hVME_hit_v1290_s530[vme_chn]->Fill(event_out->nhit_v1290_s530[vme_chn][0]);//only fill the hit for leading timing
	       pdata++; len++;
	    }
	  else{
		    // TDC trailer vme_type == 3
#ifdef DEBUG
		    std::cout<<"\n";
#endif
		    if(vme_type != 3 && vme_type !=16)
		      std::cout<<"E> ProcID 45: v1290 TDC strange type :"<<vme_type<<std::endl;
		    if(vme_type==16)
		      {
			//printf("Global trailer, %d, 0x%08x\n",len, (*pdata));//just for check
			Int_t vme_geoEnd = getbits(*pdata,1,1,5);
			if(vme_geo!=vme_geoEnd)
			  std::cout<<"E> ProcID 45: v1290 TDC strange end buffer header :"<<vme_type<<" "<<vme_geo<<" != "<<vme_geoEnd<<std::endl;
			pdata++; len++;
			break;
		      }
		    pdata++; len++;
		  }
	      } //while len loop
	  }//if type==8
      }	
       
       if(*pdata != ((Int_t) 0xbabababa)){ std::cout<<"E> ProcID :"<< psubevt->GetProcid() << ", Second Barrier missed!" << *pdata<< std::endl; }
       pdata++; len++;
       //v1290 in s511 end
       
         //for mesytec MDPP-16-QDC	 
	 {//-----MDPP module----- (do not remove this bracket)
	   // KW never used
	   //Int_t header = *pdata;
	 int tmp_nw = getbits(*pdata,1,1,10);
	 //printf("No. of words is: %d, %d\n",nword_mdpp, tmp_nw);
	 pdata++; len++ ;
	 //printf("This is header for module 2:  %d\n",getbits(*pdata,2,1,8) );
	 //int tmp_nw = getbits(*pdata,1,1,10);
	 //printf("No. of words is: %d\n",tmp_nw);
	 Int_t mdpphit = 0;
	     
	   for(int ii=0; ii<tmp_nw; ii++){
	     int tmp_data = *pdata;
	     //printf("real data 0x%08x\n",tmp_data);
	     if(1 == getbits(*pdata,2,13,4)){
	       int ch = 0x3F & (tmp_data >> 16);
	       int tdc_data = 0xFFFF & tmp_data;
	       int adc_data = 0xFFF & tmp_data;
	       if(ch >= 16 && ch <= 31){
		 mdpphit = event_out->nhit_mdpp_s530[ch-16][0];
		 event_out->tdc_mdpp_s530[ch-16][mdpphit] = tdc_data;
		 event_out->nhit_mdpp_s530[ch-16][0]++;
		 if(event_out->nhit_mdpp_s530[ch-16][0]>=10){pdata++; len++; continue;}
		 //printf("TDC data !!! ch = %2d -16, TDC = %d \n",ch, tdc_data);
		 //printf("TDC data hit !!! tmp = %d, array = %d \n",tmp_nhit_mdpp, event_out->nhit_mdpp_s530[ch-16]);
	       }
	       if(ch >= 0 && ch <= 15){
		 mdpphit = event_out->nhit_mdpp_s530[ch][1];
		 //printf("hit !!! mdpphit = %2d\n",mdpphit);
		 event_out->lqdc_mdpp_s530[ch][mdpphit] = adc_data;
		 event_out->nhit_mdpp_s530[ch][1]++;
		 if(event_out->nhit_mdpp_s530[ch][1]>=10){pdata++; len++; continue;}
		 //printf("L_QDC data !!! ch = %2d, QDC = %d \n",ch, adc_data);
	       }
	       if((ch == 32 || ch == 33) && event_out->mdpp_trig_s530[ch-32]<=0){
                event_out->mdpp_trig_s530[ch-32] = tdc_data;
                //printf("Trigger TDC data !!! ch = %2d -32, TDC = %d \n",ch, tdc_data);
	       }
	       if(ch >= 48 && ch <= 63 && event_out->sqdc_mdpp_s530[ch-48][0]<=0){
		 mdpphit = event_out->nhit_mdpp_s530[ch-48][2];
		 //printf("hit !!! mdpphit = %2d\n",mdpphit);
		 event_out->sqdc_mdpp_s530[ch-48][mdpphit] = adc_data;
		 event_out->nhit_mdpp_s530[ch-48][2]++;
		 if(event_out->nhit_mdpp_s530[ch-48][2]>=10){pdata++; len++; continue;}
                //printf("S_QDC data !!! ch = %2d -48, QDC = %d \n",ch, adc_data);
	       }
	     }else if(2 == getbits(*pdata,2,13,4)){
	       // printf("This is the extended time stamp of mdpp module.\n");
	     }else if(0x0 == tmp_data){
	       // printf("This is the fill dummy of mdpp module.\n");
	     }else if(3 == getbits(*pdata,2,15,2)){
	       // printf("This is ender of mdpp module.\n");
	     }else{
	       printf("Unknow data\n");
	       printf("real data 0x%08x\n",tmp_data);
	     }
	     pdata++;
	    }//end of for-loop
	   }	   
       //end of the fission crate

       break;
    
     //=========
     case 120: // MR-TOF-MS crate (2021/Feb/02)
       // scip vulom
       //std::cout<<"ProcID 120: word : "<<std::bitset<32>(*pdata)<<" "<< std::endl;
       pdata++; len++;
       pdata++; len++;
       /*
       if((psubevt->GetType() == 10) && (psubevt->GetSubtype() == 1) && lenMax < 15) // in this case, time stamp data.
	  {
	    int ii =0;
	    while(len < lenMax){
	      event_out->timestamp_main[ii] = *pdata;
	      pdata++; len++; ii++;
	    }
	    break;
	  }
       */

       //---- 2st  barrier ---
          if(*pdata != ((Int_t) 0xbabababa)){ std::cout<<"E> ProcID 120 : 2st barrier missed !" << std::endl; }
          pdata++; len++;
       //----  CAEN_V820 ---
       /*      { // start of v830 (do not remove this bracket)
	       Int_t vme_geo = getbits(*pdata,2,12,5);
	       Int_t vme_type = getbits(*pdata,2,9,3);
	       Int_t vme_nlw =  getbits(*pdata,2,3,6);
	       pdata++; len++;
	       if(vme_type!=4){   std::cout<<"E> Scaler type missed match ! GEO"<<vme_geo<<" "<<" type 4 =/="<<vme_type<<std::endl; }
	       for(int i_ch=0; i_ch<vme_nlw; i_ch++){
		 event_out->scaler_mrtof[i_ch] = *pdata;
		 //printf("scaler_mrtof[ch=%d] = %d\n",i_ch,*pdata);
		 pdata++; len++;
	       }
	     } //end of V830
       */ // CAEN_V820 currently does not show data

       //----  SIS_3820 ---
//for(int ii=0; ii<32;ii++){
//       if(*pdata != ((Int_t) 0x00000000)){ std::cout<<"E> ProcID 120 : No data missed !" << std::endl; }
       //skip header
       pdata++; len++; pdata++; len++;
       // skip 15 channels
       for(int ii=0; ii<15;ii++){
       pdata++; len++;
       }
       
       //---- 3nd  barrier ---
       if(*pdata != ((Int_t) 0xbabababa)){ std::cout<<"E> ProcID 120 : 3nd barrier missed !" << std::endl; }
       pdata++; len++;

       //----  CAEN_V1190  --
      if (len < lenMax){
	Int_t vme_geo = getbits(*pdata,1,1,5); // checked for the V1190
	Int_t vme_type = getbits(*pdata,2,12,5); // checked for the V1190
	pdata++; len++;
	Int_t multihit = 0;//, counter = 0;

#ifdef DEBUG
	std::cout<<"mTDC geo = "<<vme_geo<<", type = "<<vme_type<<std::endl;
#endif

	if (vme_type == 8)
	  {
	    while (len < lenMax)
	      {
#ifdef DEBUG
		std::cout<<"word : "<<std::bitset<32>(*pdata)<<" ";
#endif
		vme_type = getbits(*pdata,2,12,5);
		if(vme_type==1) // headerTDC
		  {
		    pdata++; len++;
		  }
		//multihit = 0;
#ifdef DEBUG
		std::cout<<"reading :"<<vme_type<<std::endl;
#endif
		vme_type = getbits(*pdata,2,12,5);
#ifdef DEBUG
		std::cout<<"word : "<<std::bitset<32>(*pdata)<<" type:"<<vme_type<<" ";
#endif
		if(vme_type == 0)
		  {
		    // this indicates a TDC measurement
		    Int_t vme_chn = getbits(*pdata,2,4,7); // checked for the V1190
		    Int_t LeadingOrTrailing = getbits(*pdata,2,11,1); // checked for the V1190
		    Int_t value = getbits(*pdata,1,1,19); // checked for the V1190

		    multihit = event_out->nhit_v1190_mrtof[vme_chn][LeadingOrTrailing];
#ifdef DEBUG
		    std::cout << "     tdc vme_chn = " << vme_chn;
		    std::cout << " multihit: " << multihit << " ";
#endif
		    // KW here 100 hits are taken, yet the TFRSUnpackEvent only allows for 10!! leading_v1190_mrtof[32][10];
		    //if (multihit >= 100)
		    if (multihit >= 10)
		      {
			pdata++; len++;
			continue;
		      }
		    if(LeadingOrTrailing == 0) //Leading measurement
		      {
#ifdef DEBUG
			std::cout << " +-> tdc L value = " << value << std::endl;
#endif
			if (value > 0)
			  {
			    event_out->leading_v1190_mrtof[vme_chn][multihit] = value;
			    hVME_MRTOF_TDC_V1190[vme_chn]->Fill(value); //
			  }
		      }
		    else //Trailing measurement
		      {
#ifdef DEBUG
			std::cout << " +-> tdc T value = " << value << std::endl;
#endif
			if (value > 0)
			  event_out->trailing_v1190_mrtof[vme_chn][multihit] = value;

		      }
		    event_out->nhit_v1190_mrtof[vme_chn][LeadingOrTrailing]++;

		    pdata++; len++;
		  }
		else
		  {
		    // TDC trailer vme_type == 3
#ifdef DEBUG
		    std::cout<<"\n";
#endif
		    if(vme_type != 3 && vme_type !=16)
		      std::cout<<"E> ProcID 120: MTDC strange type :"<<vme_type<<std::endl;
		    if(vme_type==16) // Trailer
		      {
			Int_t vme_geoEnd = getbits(*pdata,1,1,5);
			if(vme_geo!=vme_geoEnd)
			  std::cout<<"E> ProcID 120: MTDC strange end buffer header :"<<vme_type<<" "<<vme_geo<<" != "<<vme_geoEnd<<std::endl;
			pdata++; len++;
			break;
		      }
		    pdata++; len++;
		  }
	      } //while len loop
	  }//if type==8
      }//end if len more due to strange logic // end of V1190

       break;

     //=========
     default :
       break;

  } // end switch prodID

  return kTRUE;
}

Bool_t TFRSUnpackProc::Event_Extract_MVLC(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* psubevt, int){
	Int_t *pdata = psubevt->GetDataField();
	Int_t len = 0;
	  Int_t lenMax = (psubevt->GetDlen()-2)/2;
	
	switch(psubevt->GetProcid())
	{
		//===========
		case 30:  // FRS User Crate
		{
			//----  CAEN V820 ---
			{ 
				if(getbits(*pdata,2,1,16) != 62752){ std::cout<<"E> Event Nr: "<< myevent <<", ProcID 30 : Barrier missed <V820>! " << *pdata  << std::endl; }
				else{//Int_t words = getbits(*pdata,1,1,16);
					//std::cout<< "Number of words of this modul: "<< words << std::endl;
					pdata++; len++;
					Int_t vme_geo = getbits(*pdata,2,12,5);
					Int_t vme_type = getbits(*pdata,2,9,3);
					Int_t vme_nlw =  getbits(*pdata,2,3,6);
					//printf("Proc ID 10, geo %d, type %d, length %d\n", vme_geo, vme_type,vme_nlw);
					if(vme_type!=4){   std::cout<<"E> Scaler type missed match ! GEO"<<vme_geo<<" "<<" type 4 =/="<<vme_type<<std::endl; }
					pdata++; len++;
					for(int i_ch=0; i_ch<vme_nlw; i_ch++){
						event_out->scaler_frs[i_ch] = *pdata;
						//printf("scaler_frs[ch=%d] = %d\n",i_ch,*pdata);
						pdata++; len++;	
					}
					pdata++; len++;	//skipp trailer
				}
			} //end of V830	
			
			for (int ii=0; ii < 4;ii++){ //process 2 V775 and 2 V785
			//while (len < lenMax){
				//----  CAEN V775 and V785---
				{ 
					if(get_bits(*pdata,16,31) != 0xf520){ std::cout<<"E> Event Nr: "<< myevent <<", ProcID 30 : Barrier missed <V775/85>! " << std::hex << *pdata <<std::dec << std::endl; }
					else {
						Int_t words = get_bits(*pdata,0,15);
						//std::cout<< "Number of words of this modul: "<< words << std::endl;
						pdata++; len++;
						int i_word = 0;
						// read the header longword and extract slot, type & length 
						Int_t vme_chn = 0;
						Int_t vme_geo = get_bits(*pdata,27,31);
						Int_t vme_type = get_bits(*pdata,24,26);
						Int_t vme_nlw =  get_bits(*pdata,8,13);
						//std::cout << "vme_geo, vme_type, vme_nlw ="<< vme_geo << ", " <<  vme_type << ", " << vme_nlw <<std::endl ;
						pdata++; len++; i_word++;

						// read the data

						if ((vme_nlw > 0 && 2 == vme_type )) {
							for(int i=0;i<vme_nlw;i++) {
								vme_geo = get_bits(*pdata,27,31);
								vme_type = get_bits(*pdata,24,26);
								vme_chn = get_bits(*pdata,16,20);
								event_out->vme_frs[vme_geo][vme_chn] = get_bits(*pdata,0,11);
								pdata++; len++; i_word++;
							}
						}

						// sanity check at the end of a v7x5 unpacking 
						vme_type = getbits(*pdata,2,9,3);
						//std::cout << "vme_type= "<< vme_type <<std::endl ;
						if (vme_type != 4 ) {std::cout <<"issue in unpacking ProcID 30, existing" <<std::endl ; break ; }
						pdata++; len++; i_word++;
						// skip the last words of V7x5 (these words do not apear in the RIO data)
						//std::cout << "word= "<< i_word << ", words= "<< words<<std::endl ;
						for(int i=0; i<(words-i_word);i++) {
							pdata++; len++; 
						}
					}
				}//end of V7x5
			}			
		}
		break;

		//================
		case 10:	// Main crate
		{
			// TS Extractor
			TimeStampExtract_MVLC(event_out, psubevt);
			pdata += 5;
			len += 5;

			//----  CAEN V820 ---
			{ 
				if(getbits(*pdata,2,1,16) != 62752){ std::cout<<"E> Event Nr: "<< myevent <<", ProcID 10 : Barrier missed! " << std::hex << *pdata <<std::dec << std::endl; }
				else{//Int_t words = getbits(*pdata,1,1,16);
					//std::cout<< "Number of words of this modul: "<< words << std::endl;
					pdata++; len++;
					Int_t vme_geo = getbits(*pdata,2,12,5);
					Int_t vme_type = getbits(*pdata,2,9,3);
					Int_t vme_nlw =  getbits(*pdata,2,3,6);
					//printf("Proc ID 10, geo %d, type %d, length %d\n", vme_geo, vme_type,vme_nlw);
					if(vme_type!=4){   std::cout<<"E> Scaler type missed match ! GEO"<<vme_geo<<" "<<" type 4 =/="<<vme_type<<std::endl; }
					pdata++; len++;
					for(int i_ch=0; i_ch<vme_nlw; i_ch++){
						event_out->scaler_main[i_ch] = *pdata;
						//printf("scaler_main[ch=%d] = %d\n",i_ch,*pdata);
						pdata++; len++;	
					}
					pdata++; len++;	//skipp trailer
				}
			} //end of V830	
			
			//----  CAEN V792 ---
			{
				if(getbits(*pdata,2,1,16) != 62752){ std::cout<<"E> Event Nr: "<< myevent <<", ProcID 10 : Barrier missed! " << std::hex << *pdata <<std::dec << std::endl; }
				else{//Int_t words = getbits(*pdata,1,1,16);
					//std::cout<< "Number of words of this modul: "<< words << std::endl;
					pdata++; len++;
					Int_t vme_chn = 0;
					Int_t vme_geo = getbits(*pdata,2,12,5);
					Int_t vme_type = getbits(*pdata,2,9,3);
					Int_t vme_nlw =  getbits(*pdata,1,9,6);
					//printf("Proc ID 10, geo %d, type %d, length %d\n", vme_geo, vme_type,vme_nlw);
					pdata++; len++;
					if(vme_type == 6){// not valid data !
				  	}
				  	if ((vme_type == 2) && (vme_nlw > 0)){
						for(int i=0;i<vme_nlw;i++){
							vme_chn = getbits(*pdata,2,1,5);
							event_out->vme_main[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
							//printf("vme_main[%d][%d] = %d\n",vme_geo,vme_chn,*pdata);
							pdata++; len++;
						}
						pdata++; len++;pdata++; len++;pdata++; len++;
					}
				}
			}  //end of V792
			
			//----  CAEN V1290 ---
			{
				if(getbits(*pdata,2,1,16) != 62752){ std::cout<<"E> Event Nr: "<< myevent <<", ProcID 10 : Barrier missed! " << std::hex << *pdata <<std::dec << std::endl; }
				else{Int_t words = getbits(*pdata,1,1,16);
					//std::cout<< "Number of words of this modul: "<< words << std::endl;
					pdata++; len++;
					Int_t vme_geo = getbits(*pdata,1,1,5);
					Int_t vme_type = getbits(*pdata,2,12,5);
					//printf("ProcID 10, geo %d, type %d, words %d\n", vme_geo, vme_type,words);
					pdata++; len++;
					Int_t multihit = 0;					
					if(vme_type == 8){ // Global header
						bool in_event = 0;
						Int_t vme_chn = 0;
						for(int i_word=2; i_word<= words;i_word++){
							vme_type = getbits(*pdata,2,12,5);
							//printf("ProcID 10, geo %d, type %d, word %d\n", vme_geo, vme_type,i_word);
							if(vme_type==1){ // TDC header
								in_event = 1;
							}
							if(vme_type == 0 && in_event == 1){// this indicates a TDC measurement

								vme_chn = getbits(*pdata,2,6,5);
								Int_t LeadingOrTrailing = getbits(*pdata,2,11,1);
								Int_t value = getbits(*pdata,1,1,21);

								multihit = event_out->nhit_v1290_main[vme_chn][LeadingOrTrailing];    
								if(LeadingOrTrailing == 0){
									if (value > 0){
										event_out->leading_v1290_main[vme_chn][multihit] = value;
										hVME_MAIN_TDC_V1290_Multip[vme_chn]->Fill(value);
										if(multihit == 0) hVME_MAIN_TDC_V1290[vme_chn]->Fill(value); 
									}
									//printf("leading_v1290_main[%d][%d] = %d\n",vme_chn,multihit,value);
								}
								else{
									if (value > 0){
									event_out->trailing_v1290_main[vme_chn][multihit] = value;
									}
									//printf("trailing_v1290_main[%d][%d] = %d\n",vme_chn,multihit,value);
								}
								event_out->nhit_v1290_main[vme_chn][LeadingOrTrailing]++;
							}
							if(vme_type == 0 && in_event != 1){	
								std::cout<<"E> ProcID 10 MTDC type 0 without header (word " << i_word << " of "<< words <<"):"<< std::hex << *pdata << std::dec<<std::endl;
							}
							if(vme_type == 3 && in_event != 1){	
								std::cout<<"E> ProcID 10 MTDC type 3 without header (word " << i_word << " of "<< words <<"):"<< std::hex << *pdata << std::dec<<std::endl;
							}							
							if(vme_type==3 && in_event == 1){ // TDC trailer
							  hVME_MAIN_TDC_V1290_hit[vme_chn]->Fill(multihit);
							  in_event = 0;
							}
							if(vme_type==4){ // Error status
								std::cout<<"E> ProcID 10 MTDC error data found (word " << i_word << " of "<< words <<"): "<< std::hex << *pdata << std::dec<<std::endl;
							}
							if(vme_type==17){ // Extended trigger time tag
							}								
							if(vme_type==16){
								Int_t vme_geoEnd = getbits(*pdata,1,1,5);
								if(vme_geo!=vme_geoEnd){
									std::cout<<"E> ProcID 10 MTDC strange end buffer header :"<<vme_type<<" "<<vme_geo<<" != "<<vme_geoEnd<<std::endl;
									pdata++; len++;
									break;
								}
							}
							if(vme_type==24) {}
							if(vme_type != 1 && vme_type != 0 && vme_type != 3 && vme_type != 4 && vme_type !=17 && vme_type !=16 && vme_type!=24) std::cout<<"E> ProcID 10 MTDC strange type :"<<vme_type<< " (word " << i_word << " of "<< words <<"): "<< std::hex << *pdata << std::dec<<std::endl;
							pdata++; len++;
						}
					}
				}
			} //end of V1290
		}
		break;

		//================
		case 20:	// TPC crate
		{
			if(*pdata == 0xbad00bad) break; 
			for (int ii=0; ii < 2;ii++){ // read out 2 of them
				//----  CAEN V775 and V785---
				{ 
					if(getbits(*pdata,2,1,16) != 62752){ std::cout<<"E> Event Nr: "<< myevent <<", ProcID 20 : Barrier missed! " << std::hex << *pdata <<std::dec << std::endl; }
					else{Int_t words = getbits(*pdata,1,1,16);
						//std::cout<< "Number of words of this modul: "<< words << std::endl;
						pdata++; len++;
						int i_word = 0;
						// read the header longword and extract slot, type & length 
						Int_t vme_chn = 0;
						Int_t vme_geo = getbits(*pdata,2,12,5);
						Int_t vme_type = getbits(*pdata,2,9,3);
						Int_t vme_nlw =  getbits(*pdata,1,9,6);
						//std::cout << "vme_geo, vme_type, vme_nlw ="<< vme_geo << ", " <<  vme_type << ", " << vme_nlw <<std::endl ;
						pdata++; len++; i_word++;
						// read the data 
						if ((vme_nlw > 0 && 2 == vme_type )) {
							for(int i=0;i<vme_nlw;i++) {
								vme_geo = getbits(*pdata,2,12,5);
								vme_type = getbits(*pdata,2,9,3);
								vme_chn = getbits(*pdata,2,1,5);
								event_out->vme_tpcs2[vme_geo][vme_chn] = getbits(*pdata,1,1,12);
								//printf("vme_tpcs2[geo=%d][ch=%d] = %d\n",vme_geo,vme_chn,getbits(*pdata,1,1,12));
								pdata++; len++; i_word++;
							}
						}
						// sanity check at the end of a v7x5 unpacking 
						vme_type = getbits(*pdata,2,9,3);
						//std::cout << "vme_type= "<< vme_type <<std::endl ;
						if (vme_type != 4 ) {std::cout <<"issue in unpacking ProcID 20, existing" <<std::endl ; break ; }
						pdata++; len++; i_word++;
						// skip the last words of V7x5 (these words do not apear in the RIO data)
						//std::cout << "word= "<< i_word << ", words= "<< words<<std::endl ;
						for(int i=0; i<(words-i_word);i++) {
							pdata++; len++; 
						}
					}
				}//end of V7x5
			}
			
			//----  CAEN V1190 ---
			{
				int mvlc_flag_v1190 = getbits(*pdata,2,1,16);
				if(mvlc_flag_v1190 == 0xf580) { // 0xf5800 `overflown V1190` mvlc flag
						  std::cout<<"E> Event Nr: "<< myevent <<",  ProcID 20 V1190: Strange event! " << std::hex << *pdata <<std::dec << std::endl;
						  std::cout<< "Will still unpack it into histogram labelled `bad`" << endl;
					event_out->v1190_is_bad = true;
				}
			  
				if(mvlc_flag_v1190 == 0xf520 || mvlc_flag_v1190 == 0xf580) { // usual, good header 0xf520, or 0xf580 'strange' header
					Int_t words = getbits(*pdata,1,1,16);
					//std::cout<< "Number of words of this modul: "<< words << std::endl;
					pdata++; len++;
//					uint32_t some_mask = 0x0000ffff;
					if(*pdata == 0xffffffff || *(pdata+1) == 0xffffffff) {
						//printf("Found 0xffffff, next word: %8x\n", *(pdata+1));
						pdata += 2;
						words -= 2;
					}
					Int_t vme_geo = getbits(*pdata,1,1,5);
					Int_t vme_type = getbits(*pdata,2,12,5);
					//printf("ProcID 20, geo %d, type %d, length %d\n", vme_geo, vme_type,words);
					pdata++; len++;
					Int_t multihit = 0;	
					if(vme_type == 8) {
						bool in_event = 0;
						for(int i_word=2; i_word<= words;i_word++) {
							vme_type = getbits(*pdata,2,12,5);
							//printf("ProcID 20, geo %d, type %d, word %d\n", vme_geo, vme_type,i_word);
							if(vme_type==1){ // TDC header
								in_event = 1;
							}
							if(vme_type == 0 && in_event == 1){// this indicates a TDC measurement

								//Int_t vme_chn = getbits(*pdata,2,6,5);
								Int_t vme_chn = get_bits(*pdata,19,25);
								Int_t LeadingOrTrailing = getbits(*pdata,2,11,1);
//								Int_t value = getbits(*pdata,1,1,21);
								Int_t value = get_bits(*pdata,0,18);

								//multihit = event_out->nhit_v1190_tpcs2[vme_chn][LeadingOrTrailing];  
																
								if(LeadingOrTrailing == 0){
									if (value > 0){
										multihit = event_out->nhit_v1190_tpcs2[vme_chn]; 
										event_out->leading_v1190_tpcs2[vme_chn][multihit] = value;
										event_out->nhit_v1190_tpcs2[vme_chn]++;
									}
									//printf("leading_v1190_tpcs2[%d][%d] = %d\n",vme_chn,multihit,value);
									if(multihit==0 && !event_out->v1190_is_bad) {
										hVME_TPCS2_V1190All_firsthit->Fill(vme_chn,value);
									}

									if(event_out->v1190_is_bad) {
										hVME_TPCS2_V1190All_bad->Fill(vme_chn,value);
									}
									else {
										hVME_TPCS2_V1190All->Fill(vme_chn,value);
									}
								}
								else {
									if (value > 0){
									//event_out->trailing_v1190_tpcs2[vme_chn][multihit] = value;
									}
									//printf("trailing_v1190_tpcs2[%d][%d] = %d\n",vme_chn,multihit,value);
								}
								//event_out->nhit_v1190_tpcs2[vme_chn][LeadingOrTrailing]++;
								
							}
							if(vme_type == 0 && in_event != 1){	
								std::cout<<"E> ProcID 20 MTDC type 0 without header (word " << i_word << " of "<< words <<"): "<< std::hex << *pdata << std::dec<<std::endl;
							}
							if(vme_type == 3 && in_event != 1){	
								std::cout<<"E> ProcID 20 MTDC type 3 without header (word " << i_word << " of "<< words <<"): "<< std::hex << *pdata << std::dec<<std::endl;
							}							
							if(vme_type==3 && in_event == 1){ // TDC trailer						
								in_event = 0;
							}
							if(vme_type==4){ // Error status
								std::cout<<"E> ProcID 20 MTDC error data found (word " << i_word << " of "<< words <<"): "<< std::hex << *pdata << std::dec<<std::endl;
							}
							if(vme_type==17){ // Extended trigger time tag
							}	
							if(vme_type==16){
								Int_t vme_geoEnd = getbits(*pdata,1,1,5);
								if(vme_geo!=vme_geoEnd){
									std::cout<<"E> Proc ID 20 MTDC strange end buffer header :"<<vme_type<<" "<<vme_geo<<" != "<<vme_geoEnd<<std::endl;
									pdata++; len++;
									break;
								}
							}
							if(vme_type==24) {}
							if(vme_type != 1 && vme_type != 0 && vme_type != 3 && vme_type != 4 && vme_type != 17 && vme_type !=16 && vme_type!=24) std::cout<<"E> ProcID 20 MTDC strange type :"<<vme_type<< " (word " << i_word << " of "<< words <<"): "<< std::hex << *pdata << std::dec<<std::endl;
							pdata++; len++;
						}
						// Out of the loop, fill the multip hist for good/bad V1190 events:
						for(int i=0; i<128; ++i) {
								  if(event_out->nhit_v1190_tpcs2[i] == 0) continue;
								  if(event_out->v1190_is_bad) {
											 hVME_TPCS2_V1190_bad_multip->Fill(i, event_out->nhit_v1190_tpcs2[i]);
								  }
								  else {
											 hVME_TPCS2_V1190_multip->Fill(i, event_out->nhit_v1190_tpcs2[i]);
								  }
						}
					}
					else {
						std::cout<<"E> ProcID 20 MTDC global header not found :" <<vme_type<< " ; data field: " << std::hex << std::setprecision(8) << *(pdata-1) << " " << *pdata << std::dec<<std::endl;
						print_curr_module(pdata, len, lenMax);
						printf("\n\n");
					}
				}
				else { 
						  std::cout<<"E> Event Nr: "<< myevent <<",  ProcID 20 : Barrier V1190 missed! " << std::hex << *pdata <<std::dec << std::endl;
						  std::cout << "Not unpacking this event ..." << endl;
				}
			}// end of V1190
		}
		break;

		//================
		case 40:	// --- vftx at S2 ---
		{
		// skip triva and vetar information
			// EDIT: VETAR s2 removed for now: 17.11.2023 --Martin 
			//for(int ii=0; ii<8;ii++){
			//	pdata++; len++;
			//}
			// VFTX
				  {
							 // first 4 bits are reserved for ID: in S2 case, it's 0 //
							 if(get_bits(*pdata,4,31) != 0xab00000){ std::cout<<"E> Event Nr: "<< myevent <<", ProcID 40 : Barrier missed! " << std::hex << *pdata <<std::dec << std::endl; }
							 else if(get_bits(*pdata,0,4) != 0) { std::cout<<"E> Event Nr: "<< myevent <<", ProcID 40 : VFTX ID should be 0, is: " << std::hex << get_bits(*pdata,0,4) <<std::dec << std::endl; }
							 else{ // is good
										// skip next two words: which are always trigger window register and status register
										pdata+=3;
										len+=3;
										// now match MVLC stack header:
										if(get_bits(*pdata,16,31) != 0xf500) { std::cout<<"E> ProcID 40 : MVLC stack header missed! " << std::hex << *pdata <<std::dec << std::endl; }
										else {
												  Int_t words = get_bits(*pdata,0,15);
												  //std::cout<< "Number of words of this modul: "<< words << std::endl;
												  pdata++; len++;
												  event_out->ClearVftx();
												  pdata++; len++; // skipp header
												  int channel;
												  int word;
												  int module=0;
												  for(word=0; word<words-1;word++) {
															 channel = getbits(*pdata,2,10,5);
															 bool trailing = channel%2;
															 channel /= 2;

															 float r = (double)rand.Rndm() - 0.5 ;
															 int cc = (uint16_t)((*pdata & 0x00fff800)>>11);
															 int ft = (uint16_t)(*pdata & 0x07ff);
															 Double_t ti = VFTX_GetTraw_ps(module,channel,cc,ft,r);
															 pdata++; len++;

															 if(event_out->vftx_lead_mult[module][channel]<VFTX_MAX_HITS && !trailing){
																		event_out->vftx_leading_cc[module][channel][event_out->vftx_mult[module][channel]] = cc;
																		event_out->vftx_leading_ft[module][channel][event_out->vftx_mult[module][channel]] = ft;
																		event_out->vftx_leading_time[module][channel][event_out->vftx_mult[module][channel]] = ti;

																		event_out->vftx_lead_mult[module][channel]++;
															 }
															 if(event_out->vftx_trail_mult[module][channel]<VFTX_MAX_HITS && trailing){
																		event_out->vftx_trailing_cc[module][channel][event_out->vftx_mult[module][channel]] = cc;
																		event_out->vftx_trailing_ft[module][channel][event_out->vftx_mult[module][channel]] = ft;
																		event_out->vftx_trailing_time[module][channel][event_out->vftx_mult[module][channel]] = ti;
															 }
												  }
										}
							 }
				  }

			
			// Divyang: MTDC
			// Note: current data structure is VFTX --> MTDC --> MQDC
			//       so if the structure is changed, please change this part as well
			{
				int num_data_words_from_MVLC = 0;

				// MVLC header
				if(((*pdata & 0xffff0000) >> 16) == 0xf520)
				{
					num_data_words_from_MVLC = *pdata & 0xfff;

					// std::cout << "VLC header for MTDC-32: " << std::hex << *pdata <<std::dec << std::endl;
					pdata++; len++; // Barrier detected. Good to go ahead
				}
				else {std::cout<<"E> Event Nr: "<< myevent <<", ProcID 40: Barrier missed! " << std::hex << *pdata <<std::dec << std::endl;}

				// No. of words from MTDC-32 header. NOT from MVLC header.
				// For me, MTDC-32 manual is easy and quick to follow.
				Int_t no_of_words;


				// MTDC-32 Header
				if (((*pdata & 0xff000000)>>24) == 0b01000000) // check if the first MTDC-32 word is header
				{
					// std::cout << "MTDC-32 header: " << std::hex << *pdata <<std::dec << std::endl;

					no_of_words = *pdata & 0x00000fff; // number of words from MTDC-32 header
					pdata++; len++; // go ahead
				}
				else
				{
					// if there is no data, don't show this message
					if (num_data_words_from_MVLC != 0) {std::cout<<"E> ProcID 40: MTDC-32 header missed! " << std::hex << *pdata <<std::dec << std::endl;}
				}


				// data starts
				if (((*pdata & 0xffc00000) >> 22) == 0b0000010000) // first 10 bits of data word are 0b0000010000
				{
					event_out->Clear_MTDC_32(); // initialize MTDC-32 variabls

					for (int i_wrd=0; i_wrd<no_of_words-1; i_wrd++)
					{
						// std::cout << "MTDC-32 data: " << std::hex << *pdata <<std::dec << std::endl;
						
						int MTDC_trig_flg = (*pdata >> 21) & 0x1;
						int MTDC_chnl_num = (*pdata >> 16) & 0x1f;
						int MTDC_time_dif = *pdata & 0xffff;
						// cout << MTDC_trig_flg << " | " << MTDC_chnl_num << " | " << MTDC_time_dif << endl;
					
						if      (MTDC_trig_flg == 0) { event_out->mtdc32_dt_trg0_raw[MTDC_chnl_num] = MTDC_time_dif; }
						else if (MTDC_trig_flg == 1) { event_out->mtdc32_dt_trg1_raw[MTDC_chnl_num] = MTDC_time_dif; }

						pdata++; len++;
					}
				}


				// ender
				if (((*pdata & 0xc0000000) >> 30) == 0b11) 
				{
					// std::cout << "MTDC-32 ender: " << std::hex << *pdata <<std::dec << std::endl; 
					pdata++; len++; // go ahead
				}
				else
				{
					// if there is no data, don't show this message
					if (num_data_words_from_MVLC != 0) {std::cout<<"E> ProcID 40 : MTDC-32 ender missed! " << std::hex << *pdata <<std::dec << std::endl;}
				}

			} // end of MTDC-32

			
			// Divyang: MQDC
			// Note: current data structure is VFTX --> MTDC --> MQDC
			//       so if the structure is changed, please change this part as well
			{
			  int num_data_words_from_MVLC = 0;

				// MVLC header
				if(((*pdata & 0xffff0000) >> 16) == 0xf520)
				{
					num_data_words_from_MVLC = *pdata & 0xfff;

					// std::cout << "VLC header for MQDC-32: " << std::hex << *pdata <<std::dec << std::endl;
					pdata++; len++; // Barrier detected. Good to go ahead
				}
				else {std::cout<<"E> Event Nr: "<< myevent <<", ProcID 40: Barrier missed! " << std::hex << *pdata <<std::dec << std::endl;}

#ifndef QDC_IS_BAD

				// No. of words from MQDC-32 header. NOT from MVLC header.
				// For me, MQDC-32 manual is easy and quick to follow.
				Int_t no_of_words;


				// MQDC-32 Header
				if (((*pdata & 0xff000000) >> 24) == 0b01000000)
				{
					// std::cout << "MQDC-32 header: " << std::hex << *pdata <<std::dec << std::endl;

					no_of_words = *pdata & 0x00000fff;
					pdata++; len++; // go ahead
				}
				else
				{
					// if there is no data, don't show this message
					if (num_data_words_from_MVLC != 0) {std::cout<<"E> ProcID 40 : MQDC-32 header missed! " << std::hex << *pdata <<std::dec << std::endl;}
				}


				// data starts
				if (((*pdata & 0xffe00000) >> 21) == 0b00000100000) // first 11 bits of data word are 0b00000100000
				{
					event_out->Clear_MQDC_32();

					for (int i_wrd=0; i_wrd<no_of_words-1; i_wrd++)
					{
						// std::cout << "MQDC-32 data: " << std::hex << *pdata <<std::dec << std::endl;

						int MQDC_chnl_num = (*pdata >> 16) & 0x1f;
						int MQDC_ampltd   = *pdata & 0xfff;
						// cout << MQDC_chnl_num << " | " << MQDC_ampltd << endl;
					
						event_out->mqdc32_raw[MQDC_chnl_num] = MQDC_ampltd;

						pdata++; len++;
					}
				}


				// ender
				if (((*pdata & 0xc0000000) >> 30) == 0b11) 
				{
					// std::cout << "MQDC-32 ender: " << std::hex << *pdata <<std::dec << std::endl; 
					pdata++; len++; // go ahead
				}
				else
				{
					// if there is no data, don't show this message
					if (num_data_words_from_MVLC != 0) {std::cout<<"E> ProcID 40: MQDC-32 ender missed! " << std::hex << *pdata <<std::dec << std::endl;}
				}
#endif
			} // end of MQDC-32
			
		}
		break;
		//================
		case 35:	// --- travelling MUSIC crate ---
		{
			// First 5 words are whiterabbit!
			TimeStampExtract_TravMus(event_out, psubevt);
			pdata+=5;
			len+=5;
		  if(getbits(*pdata,2,1,16) != 62752){ std::cout<<"E> Event Nr: "<< myevent <<", ProcID 35 : Barrier missed !" << *pdata  << std::endl; }
		  else{ //-----MDPP module----- (do not remove this bracket)
		    pdata++; len++;
		    // header
		    Int_t header = *pdata;
		    Int_t nword_mdpp = (0x3FF & header);
		    pdata++; len++ ;

		    // main data (data or time information or end counter)
		    for(int ii=0; ii<nword_mdpp; ii++){
		      int tmp_data = *pdata;
		      if( 1 == (0xF & (tmp_data>>28))   ){
			int tmp_data = *pdata;
			//printf("real data 0x%08x\n",tmp_data);
			int ch = 0x1F & (tmp_data >> 16);
			int trigger = 0x1 & (tmp_data >> 21);
			// printf("ch = %2d, trigger = %2d\n",ch,trigger);
			if(trigger == 0){
			  if(0<=ch && ch<=15){
			    int adc_data = 0xFFFF & tmp_data;
			    //printf("ADC data !!! ch = %2d, ADC = %d \n",ch, adc_data);
			    if( (event_out->vme_trmu_adc[ch]) <= 0 ){ //first-come-first-served, for detailed multi-hit analysis, investigation needed.
			      event_out->vme_trmu_adc[ch] = adc_data;
			      //  printf("event_out->vme_trmu_adc[%d] = 0x%08x; \n",ch,adc_data);
			    }
			  }else if(16<=ch && ch<=31){
			    int tdc_data = 0xFFFF & tmp_data;
			    //printf("TDC data !!! ch = %2d, TDC = %d \n",ch-16, tdc_data);
			    if( (event_out->vme_trmu_tdc[ch-16]) <= 0 ){//first-come-first-served, for detailed multi-hit analysis, investigation needed.
			      event_out->vme_trmu_tdc[ch-16] = tdc_data;
			      //////        	 printf("event_out->vme_trmu_tdc[%d] = 0x%08x; \n",ch-16,tdc_data);
			    }
			  }
			}else if (trigger == 1){
			  int trigger_data = 0xFFFF & tmp_data;
			  // printf("Trigger time stamp !!! ch = %2d, Tigger time stamp = %d \n",ch, trigger_data);
			  event_out->vme_trmu_trigger[ch] = trigger_data;
			  //  printf("event_out->vme_trmu_adc[%d] = 0x%08x; \n",ch,adc_data);
			}
		      }else if( 2 == (0xF & (tmp_data>>28))){
			// printf("ext time stamp 0x%08x\n",tmp_data);
		      }else if(0x0 == tmp_data ){
			// printf("dummy 0x%08x\n",tmp_data);
		      }else if( 3 == (0x3 & (tmp_data>>30))){
			// printf("end counter 0x%08x\n",tmp_data);
		      }else{
			/// printf("unknown data0x%08x\n",tmp_data);
		      }
		      pdata++; len++ ;
		    }
		  }//---end of MDPP module ---
		}
		break;
	} // end switch prodID
	return kTRUE;
}

void TFRSUnpackProc::VFTX_Readout(TFRSUnpackEvent* unp, Int_t **pdata, int module)
{
  uint32_t p32_tmp, marker;
  uint16_t cnt, channel;
  int      word;
 
  // first 32-bit word is a marker
  p32_tmp = (uint32_t)*(*pdata)++;
  marker  = (uint32_t)(p32_tmp & 0xff000000);
  cnt     = (uint16_t)((p32_tmp & 0x0003fe00)>>9);

  printf("1: 0x%x    %d \n",p32_tmp, cnt);
  if(cnt<1){(*pdata)++;
    return;}
  else{
    if(marker==0xab000000) {
      // second 32-bit word is the trigger window
      p32_tmp = (uint32_t)*(*pdata)++;
      // printf("2: 0x%x\n",p32_tmp);
      // third 32-bit word is a header we don't care of
      // p32_tmp = (uint32_t)*(*pdata)++;
      // printf("3: 0x%x\n",p32_tmp);
      // DATA BLOCK
      for(word=0; word<cnt-1;word++) {
	p32_tmp = (uint32_t)*(*pdata)++;

	channel = getbits(p32_tmp,2,10,5);
	//cout << " channel = " << channel << "\tchannel%2 = "  << channel%2 << "\tchannel/2 = "  << channel/2 << endl;
	bool trailing = channel%2;
	channel /= 2;

	//printf("4: 0x%x  %d  \n",p32_tmp,channel);
	//printf("in UNPACK TDC/VFTX_%02d,  %02d: %02d \n",module,channel, unp->vftx_mult[module][channel]);
	float r = (double)rand.Rndm() - 0.5 ;
	int cc = (uint16_t)((p32_tmp & 0x00fff800)>>11);
	int ft = (uint16_t)(p32_tmp & 0x07ff);
	Double_t ti = VFTX_GetTraw_ps(module,channel,cc,ft,r);
	
	if(unp->vftx_mult[module][channel]<VFTX_MAX_HITS){
	  if(!trailing){
	    unp->vftx_leading_cc[module][channel][unp->vftx_mult[module][channel]] = cc;
	    unp->vftx_leading_ft[module][channel][unp->vftx_mult[module][channel]] = ft;
	    unp->vftx_leading_time[module][channel][unp->vftx_mult[module][channel]] = ti;
	    
	    //printf("vftx leading: ch=%d  ct=%d  ft=%d ti=%.4f\n",channel,unp->vftx_leading_cc[module][channel][unp->vftx_mult[module][channel]],unp->vftx_leading_ft[module][channel][unp->vftx_mult[module][channel]],unp->vftx_leading_time[module][channel][unp->vftx_mult[module][channel]]);
	  }
	  else{
	    unp->vftx_trailing_cc[module][channel][unp->vftx_mult[module][channel]] = cc;
	    unp->vftx_trailing_ft[module][channel][unp->vftx_mult[module][channel]] = ft;
	    unp->vftx_trailing_time[module][channel][unp->vftx_mult[module][channel]] = ti;
	    //printf("vftx trailing: ch=%d  ct=%d  ft=%d \n",channel,unp->vftx_trailing_cc[module][channel][unp->vftx_mult[module][channel]],unp->vftx_trailing_ft[module][channel][unp->vftx_mult[module][channel]]);
	    unp->vftx_mult[module][channel]++;
	  }
	  //unp->vftx_ft[module][channel][unp->vftx_mult[module][channel]] = getbits(p32_tmp,1,1,11);
	}//hits
      }//words left
    }//check marker
    else {
      printf("error in UNPACK TDC/VFTX_%02d marker: 0x%x\n",module,p32_tmp);
    }

    return;
  }
}

Bool_t TFRSUnpackProc::FillHistograms(TFRSUnpackEvent* event)
{

  hTrigger->Fill(event->qtrigger);
  hpTrigger->Fill(event->uphystrig);
  hNbTrig->Fill (event->unbtrig);

  if (hTpat) {
    for (int i=0;i<16;i++)
    {
	   if ( (event->utpat & ((Int_t) pow(2,i)))> 0 )  hTpat->Fill(i+1) ;

    }
  }

    if (event->unbtrig== 2) {
      hCombiTrig2->Fill(event->uphystrig, event->umaxtrig);
    }

//hTpat->Fill(event->utpat);//This shall be done differently

    if(frs->fill_raw_histos)
      {
	// --- vftx --- //
	int module, channel;
	for(module=0; module<VFTX_N; module++){
	  for(channel=0; channel<VFTX_MAX_CHN; channel++) {
	    h1_vftx_lead_mult[module][channel]->Fill(event->vftx_lead_mult[module][channel]);
	    h1_vftx_trail_mult[module][channel]->Fill(event->vftx_trail_mult[module][channel]);
	    //std::cout<<channel<< " "<<event->vftx_lead_mult[module][channel]<< "  "<<event->vftx_trail_mult[module][channel]<< ""<< event->vftx_leading_ft[module][channel][0]<< " "<< event->vftx_trailing_ft[module][channel][0]<<std::endl;
	    if(event->vftx_lead_mult[module][channel]>=1 || event->vftx_trail_mult[module][channel]>=1){
	      h1_vftx_leading_cc[module][channel]->Fill(event->vftx_leading_cc[module][channel][0]);
	      h1_vftx_leading_ft[module][channel]->Fill(event->vftx_leading_ft[module][channel][0]);
	      h1_vftx_trailing_cc[module][channel]->Fill(event->vftx_trailing_cc[module][channel][0]);
	      h1_vftx_trailing_ft[module][channel]->Fill(event->vftx_trailing_ft[module][channel][0]);

	      h1_vftx_leading_time[module][channel]->Fill(event->vftx_leading_time[module][channel][0]);
	      h1_vftx_leading_timediff2ch0[module][channel]->Fill(event->vftx_leading_time[module][channel][0] - event->vftx_leading_time[module][0][0]);

	      //versus time/event number to check for drifts
	      h2_vftx_leading_timediff2ch0_event[module][channel]->Fill(Vftx_Stat[module][channel], event->vftx_leading_time[module][channel][0] - event->vftx_leading_time[module][0][0]);
	      h2_vftx_leading_timediff2ch8_event[module][channel]->Fill(Vftx_Stat[module][channel], event->vftx_leading_time[module][channel][0] - event->vftx_leading_time[module][8][0]);

	      Vftx_Stat[module][channel]++;
	
	    }
	  }//channels
	  h2_vftx_leading_time04[module]->Fill(event->vftx_leading_time[module][0][0],event->vftx_leading_time[module][4][0]);
	}//vftx modules


	// S2: MTDC-32
	for (int i_chnl=0; i_chnl<32; i_chnl++)
	{
		h2_S2_MTDC32_trg0 ->Fill(i_chnl, event->mtdc32_dt_trg0_raw[i_chnl]);
		h2_S2_MTDC32_trg1->Fill(i_chnl, event->mtdc32_dt_trg1_raw[i_chnl]);
	}

	// S2: MQDC-32
	for (int i_chnl=0; i_chnl<32; i_chnl++)
	{
		h2_S2_MQDC32->Fill(i_chnl, event->mqdc32_raw[i_chnl]);
	}


  for(int i=0;i<32;i++)
	{
	  if(hVME_MAIN_14[i])  hVME_MAIN_14[i]->Fill(event->vme_main[14][i] & 0xfff);
	  if(hVME_MAIN_11[i])  hVME_MAIN_11[i]->Fill(event->vme_main[11][i] & 0xfff);
	  if(hVME_MAIN_14All)  hVME_MAIN_14All->Fill(i,event->vme_main[14][i] & 0xfff);
	  if(hVME_MAIN_11All)  hVME_MAIN_11All->Fill(i,event->vme_main[11][i] & 0xfff);
	  // TDC time is filled in the eventbuild, to put all multi-hits in the histograms
	}
      for(int i=0;i<32;i++)
	{
	  if(hVME_USER_8[i])  hVME_USER_8[i]->Fill(event->vme_frs[8][i] & 0xfff);
	  if(hVME_USER_9[i])  hVME_USER_9[i]->Fill(event->vme_frs[9][i] & 0xfff);
	  if(hVME_USER_10[i])  hVME_USER_10[i]->Fill(event->vme_frs[10][i] & 0xfff);
	  //if(hVME_USER_11[i])  hVME_USER_11[i]->Fill(event->vme_frs[11][i] & 0xfff);
	  if(hVME_USER_12[i]) hVME_USER_12[i]->Fill(event->vme_frs[12][i] & 0xfff);
	  if(hVME_USER_8All)  hVME_USER_8All->Fill(i,event->vme_frs[8][i] & 0xfff);
	  if(hVME_USER_9All)  hVME_USER_9All->Fill(i,event->vme_frs[9][i] & 0xfff);
	  if(hVME_USER_10All)  hVME_USER_10All->Fill(i,event->vme_frs[10][i] & 0xfff);
	  //if(hVME_USER_11All)  hVME_USER_11All->Fill(i,event->vme_frs[11][i] & 0xfff);
	  if(hVME_USER_12All) hVME_USER_12All->Fill(i,event->vme_frs[12][i] & 0xfff);

	  //	  if(5==i)  printf("UnpackProc tof2ll = %d\n",event->vme_frs[12][i]);fflush(stdout);//AAAAA
	  //      if(6==i)  printf("UnpackProc tof2rr = %d\n",event->vme_frs[12][i]);fflush(stdout);//AAAAA
	  //      if(7==i)  printf("UnpackProc tof3ll = %d\n",event->vme_frs[12][i]);fflush(stdout);//AAAAA
	  //      if(8==i)  printf("UnpackProc tof3rr = %d\n",event->vme_frs[12][i]);fflush(stdout);//AAAAA
	  
	}
      
      for(int i=0;i<32;i++)
	{
	  if(hVME_TOF_11[i]) hVME_TOF_11[i]->Fill(event->vme_tof[11][i] & 0xfff);
	  if(hVME_TOF_16[i]) hVME_TOF_16[i]->Fill(event->vme_tof[16][i] & 0xfff);
	  if(hVME_TOF_11All) hVME_TOF_11All->Fill(i,event->vme_tof[11][i] & 0xfff);
	  if(hVME_TOF_16All) hVME_TOF_16All->Fill(i,event->vme_tof[16][i] & 0xfff);
	}
     
      for(int i=0;i<32;i++)
	{
	  if (hVME_TPCS2_13[i] ) hVME_TPCS2_13[i] ->Fill(event->vme_tpcs2[13][i] & 0xfff);
	  if (hVME_TPCS2_12[i]) hVME_TPCS2_12[i]->Fill(event->vme_tpcs2[12][i] & 0xfff);
	  //if (hVME_TPCS4_0[i] ) hVME_TPCS4_0[i] ->Fill(event->vme_tpcs4[0][i] & 0xfff);
	  //if (hVME_TPCS4_1[i] ) hVME_TPCS4_1[i] ->Fill(event->vme_tpcs4[1][i] & 0xfff);
	  if (hVME_TPCS2_13All ) hVME_TPCS2_13All ->Fill(i,event->vme_tpcs2[13][i] & 0xfff);
	  if (hVME_TPCS2_12All) hVME_TPCS2_12All->Fill(i,event->vme_tpcs2[12][i] & 0xfff);
	  //if (hVME_TPCS4_0All ) hVME_TPCS4_0All ->Fill(i,event->vme_tpcs4[0][i] & 0xfff);
	  //if (hVME_TPCS4_1All ) hVME_TPCS4_1All ->Fill(i,event->vme_tpcs4[1][i] & 0xfff);
	  //printf("UnpackProc vme_tpcs2 = %d\n",event->vme_tpcs2[13][i]);fflush(stdout);//AAAAA
	}

    for (int i=0;i<32;i++)
   {
	  if (hVME_ACTSTOP_10[i]) hVME_ACTSTOP_10[i]->Fill(event->vme_actstop[10][i] & 0xfff);
	  if (hVME_ACTSTOP_12[i]) hVME_ACTSTOP_12[i]->Fill(event->vme_actstop[12][i] & 0xfff);
 	  if (hVME_ACTSTOP_14[i]) hVME_ACTSTOP_14[i]->Fill(event->vme_actstop[14][i] & 0xfff);
  	  if (hVME_ACTSTOP_16[i]) hVME_ACTSTOP_16[i]->Fill(event->vme_actstop[16][i] & 0xfff);
 	  if (hVME_ACTSTOP_18[i]) hVME_ACTSTOP_18[i]->Fill(event->vme_actstop[18][i] & 0xfff);
 	  if (hVME_ACTSTOP_20[i]) hVME_ACTSTOP_20[i]->Fill(event->vme_actstop[20][i] & 0xfff);
     	  if (hVME_ACTSTOP_10All) hVME_ACTSTOP_10All->Fill(i,event->vme_actstop[10][i] & 0xfff);
	  if (hVME_ACTSTOP_12All) hVME_ACTSTOP_12All->Fill(i,event->vme_actstop[12][i] & 0xfff);
 	  if (hVME_ACTSTOP_14All) hVME_ACTSTOP_14All->Fill(i,event->vme_actstop[14][i] & 0xfff);
  	  if (hVME_ACTSTOP_16All) hVME_ACTSTOP_16All->Fill(i,event->vme_actstop[16][i] & 0xfff);
 	  if (hVME_ACTSTOP_18All) hVME_ACTSTOP_18All->Fill(i,event->vme_actstop[18][i] & 0xfff);
 	  if (hVME_ACTSTOP_20All) hVME_ACTSTOP_20All->Fill(i,event->vme_actstop[20][i] & 0xfff);
     	  if (hVME_ACTSTOP_10All_Tpat4_8&&event->unbtrig==2&&event->uphystrig==4&&event->umaxtrig==8) hVME_ACTSTOP_10All_Tpat4_8->Fill(i,event->vme_actstop[10][i] & 0xfff);
	  if (hVME_ACTSTOP_12All_Tpat4_8&&event->unbtrig==2&&event->uphystrig==4&&event->umaxtrig==8) hVME_ACTSTOP_12All_Tpat4_8->Fill(i,event->vme_actstop[12][i] & 0xfff);
 	  if (hVME_ACTSTOP_14All_Tpat4_8&&event->unbtrig==2&&event->uphystrig==4&&event->umaxtrig==8) hVME_ACTSTOP_14All_Tpat4_8->Fill(i,event->vme_actstop[14][i] & 0xfff);
  	  if (hVME_ACTSTOP_16All_Tpat4_8&&event->unbtrig==2&&event->uphystrig==4&&event->umaxtrig==8) hVME_ACTSTOP_16All_Tpat4_8->Fill(i,event->vme_actstop[16][i] & 0xfff);
 	  if (hVME_ACTSTOP_18All_Tpat4_8&&event->unbtrig==2&&event->uphystrig==4&&event->umaxtrig==8) hVME_ACTSTOP_18All_Tpat4_8->Fill(i,event->vme_actstop[18][i] & 0xfff);
 	  if (hVME_ACTSTOP_20All_Tpat4_8&&event->unbtrig==2&&event->uphystrig==4&&event->umaxtrig==8) hVME_ACTSTOP_20All_Tpat4_8->Fill(i,event->vme_actstop[20][i] & 0xfff);
     	  if (hVME_ACTSTOP_10All_Tpat4&&(event->uphystrig==4||event->umaxtrig==4)) hVME_ACTSTOP_10All_Tpat4->Fill(i,event->vme_actstop[10][i] & 0xfff);
	  if (hVME_ACTSTOP_12All_Tpat4&&(event->uphystrig==4||event->umaxtrig==4)) hVME_ACTSTOP_12All_Tpat4->Fill(i,event->vme_actstop[12][i] & 0xfff);
 	  if (hVME_ACTSTOP_14All_Tpat4&&(event->uphystrig==4||event->umaxtrig==4)) hVME_ACTSTOP_14All_Tpat4->Fill(i,event->vme_actstop[14][i] & 0xfff);
  	  if (hVME_ACTSTOP_16All_Tpat4&&(event->uphystrig==4||event->umaxtrig==4)) hVME_ACTSTOP_16All_Tpat4->Fill(i,event->vme_actstop[16][i] & 0xfff);
 	  if (hVME_ACTSTOP_18All_Tpat4&&(event->uphystrig==4||event->umaxtrig==4)) hVME_ACTSTOP_18All_Tpat4->Fill(i,event->vme_actstop[18][i] & 0xfff);
 	  if (hVME_ACTSTOP_20All_Tpat4&&(event->uphystrig==4||event->umaxtrig==4)) hVME_ACTSTOP_20All_Tpat4->Fill(i,event->vme_actstop[20][i] & 0xfff);
     	  if (hVME_ACTSTOP_10All_Tpat8&&(event->uphystrig==8||event->umaxtrig==8)) hVME_ACTSTOP_10All_Tpat8->Fill(i,event->vme_actstop[10][i] & 0xfff);
	  if (hVME_ACTSTOP_12All_Tpat8&&(event->uphystrig==8||event->umaxtrig==8)) hVME_ACTSTOP_12All_Tpat8->Fill(i,event->vme_actstop[12][i] & 0xfff);
 	  if (hVME_ACTSTOP_14All_Tpat8&&(event->uphystrig==8||event->umaxtrig==8)) hVME_ACTSTOP_14All_Tpat8->Fill(i,event->vme_actstop[14][i] & 0xfff);
  	  if (hVME_ACTSTOP_16All_Tpat8&&(event->uphystrig==8||event->umaxtrig==8)) hVME_ACTSTOP_16All_Tpat8->Fill(i,event->vme_actstop[16][i] & 0xfff);
 	  if (hVME_ACTSTOP_18All_Tpat8&&(event->uphystrig==8||event->umaxtrig==8)) hVME_ACTSTOP_18All_Tpat8->Fill(i,event->vme_actstop[18][i] & 0xfff);
 	  if (hVME_ACTSTOP_20All_Tpat8&&(event->uphystrig==8||event->umaxtrig==8)) hVME_ACTSTOP_20All_Tpat8->Fill(i,event->vme_actstop[20][i] & 0xfff);
	 }


        for(int i=0;i<16;i++)
	{
	  if(hVME_TRMU_ADC[i]) hVME_TRMU_ADC[i]->Fill(event->vme_trmu_adc[i]);
	  if(hVME_TRMU_ADCAll) hVME_TRMU_ADCAll->Fill(i, event->vme_trmu_adc[i]);
	  if(hVME_TRMU_TDC[i]) hVME_TRMU_TDC[i]->Fill(event->vme_trmu_tdc[i]);
	  if(hVME_TRMU_TDCAll) hVME_TRMU_TDCAll->Fill(i, event->vme_trmu_tdc[i]);
	}
	for(int i=0;i<2;i++)
	{
	  if(hVME_TRMU_Trigger[i]) hVME_TRMU_Trigger[i]->Fill(event->vme_trmu_trigger[i]);
	}

	//start of s530 fission crate
	for(int i=0;i<16;i++){
	  for(int j=0;j<10;j++){
	  if(hVME_Lqdc_s530[i]) hVME_Lqdc_s530[i]->Fill(event->lqdc_mdpp_s530[i][j]);
	  if(hVME_Sqdc_s530[i]) hVME_Sqdc_s530[i]->Fill(event->sqdc_mdpp_s530[i][j]);
	  if(hVME_TDC_mdpp_s530[i]) hVME_TDC_mdpp_s530[i]->Fill(event->tdc_mdpp_s530[i][j]);
	  }
	}
	for(int i=0;i<2;i++){
	  if(hVME_MDPP_Trig_s530[i]) hVME_MDPP_Trig_s530[i]->Fill(event->mdpp_trig_s530[i]);
	}
	//end of s530 fission crate
	
    }

if(event->frs_wr > 0 && event->travmus_wr > 0) h1_wr_diff_FRS_TM->Fill(event->frs_wr - event->travmus_wr);
if(event->frs_wr > 0 && event->travmus_wr > 0) h1_wr_diff_FRS_TM->Fill(event->frs_wr - event->travmus_wr);
  return kTRUE;
}
// --- ----------------------------------- --- //
// --- RECONSTRUCT Traw FROM THE VFTX DATA --- //
// --- ----------------------------------- --- //

// reads calibration parameters
void TFRSUnpackProc::m_VFTX_Bin2Ps(){
  int l_VFTX_SN[VFTX_N]  = VFTX_SN;

  for(int i=0; i<VFTX_N; i++)
    for(int j=0; j<VFTX_MAX_CHN; j++)
      for(int k=0; k<1000; k++)
	VFTX_Bin2Ps[i][j][k]=0.;

  int b; double ft_ps;
  for(int mod=0; mod<VFTX_N; mod++){
    for(int ch=0; ch<VFTX_MAX_CHN; ch++){
      std::ifstream in;
            
      in.open(Form("Bin2Ps/VFTX_%05d_Bin2Ps_ch%02d.dat",l_VFTX_SN[mod],ch));
      if(!in.is_open()){
	for(int bin=0; bin<1000; bin++)
	  VFTX_Bin2Ps[mod][ch][bin] = 0.; // no data in ps if we don't have the files
	printf("WARNING : VFTX %05d ch %02d file not found, you will not have precise data\n",l_VFTX_SN[mod],ch);
      }
      else {
	while(!in.eof()) {
	  in >>b >>ft_ps;
	  VFTX_Bin2Ps[mod][ch][b] = ft_ps;
	  if(b>1000) printf(" !!! WARNING !!!! file Bin2PS/VFTX%02d_Bin2Ps_ch%2d.dat, overflow b=%i \n",mod,ch,b);
	}// end of for (ch over VFTX_CHN)
	in.close();
      }
    }//end of for (ch)
  }// end of for (mod over VFTX_N
  return;
}


Double_t TFRSUnpackProc::VFTX_GetTraw_ps(int module, int channel, int cc, int ft, float rand) {
  Double_t gain;
  Double_t calib = (Double_t)VFTX_Bin2Ps[module][channel][ft];
  if (calib==0)
    calib = ft;
  
  if(rand<0) {
    Double_t calib_prev = (Double_t)VFTX_Bin2Ps[module][channel][ft-1];
    gain = calib - calib_prev;
  }
  else {
    Double_t calib_next = (Double_t)VFTX_Bin2Ps[module][channel][ft+1];
    gain = calib_next - calib;
  }
  float ft_ps = calib + gain*rand ;
  Double_t result = 5000.*cc - ft_ps;
  return (result);
}
UNPACK_FOOT_PROC_IMPL (TFRSUnpackEvent, TFRSUnpackProc)

ClassImp(TFRSUnpackProc)
