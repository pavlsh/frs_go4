#include "TFRSSortProc.h"
#include "TFRSSortEvent.h"
#include "TFRSUnpackEvent.h"

#include "TFRSParameter.h"

#include "TFRSVftxSetting.h"

//#include  <iostream.h>
#include  <stdio.h>

TFRSSortProc::TFRSSortProc() : TFRSBasicProc("FRSSortProc")
{
  StartOfSpilTime = -1;
  PreviousTS = -1;
  v1190_channel_init();
}

TFRSSortProc::TFRSSortProc(const char* name) : TFRSBasicProc(name)
{
  StartOfSpilTime = -1;
  PreviousTS = -1;
  counter = 0;

  v1190_channel_init();
}

TFRSSortProc::~TFRSSortProc() {
}

Bool_t TFRSSortProc::BuildEvent(TGo4EventElement* output)
{

  TFRSSortEvent* tgt = dynamic_cast<TFRSSortEvent*> (output);
  if (tgt==nullptr)
    return kFALSE;

  tgt->SetValid(kTRUE);  // all events after unpack always accepted

  TFRSUnpackEvent *src = dynamic_cast<TFRSUnpackEvent*> (GetInputEvent());
  if (src==nullptr)
    return kFALSE;

  /* now we can assign the parameters according to cabling:  */

  /* ### timestamp: */
  tgt->ts_id = src->wrid ; 
  tgt->ts_word[0] = src->timestamp_main[0];
  tgt->ts_word[1] = src->timestamp_main[1];
  tgt->ts_word[2] = src->timestamp_main[2];
  tgt->ts_word[3] = src->timestamp_main[3];


  tgt->timestamp = Long64_t(1)*tgt->ts_word[0] + Long64_t(0x10000)*tgt->ts_word[1] + Long64_t(0x100000000)*tgt->ts_word[2] + Long64_t(0x1000000000000)*tgt->ts_word[3];
  // printf("qtrigger=%d timestamp=%ld \n",src->qtrigger,tgt->timestamp);
  tgt->tsys_word[0] = Long64_t(1)*tgt->ts_word[0] + Long64_t(0x10000)*tgt->ts_word[1] ; //s time low word
  tgt->tsys_word[1] = Long64_t(0x100000000)*tgt->ts_word[2] + Long64_t(0x1000000000000)*tgt->ts_word[3] ; //s time high worid... we do not use it
  if(PreviousTS < 0)
    tgt->tsys_word[2] = 0;
  else
    tgt->tsys_word[2] = (tgt->timestamp - PreviousTS)*1.e-5 ; //ms time since the previous s time (ftime routine)
  tgt->systemtime_s = tgt->tsys_word[2]*1e-3; //tgt->tsys_word[0] ;
  tgt->systemtime_ms= tgt->tsys_word[2] ;


  if (src->qtrigger==12)
    {
      StartOfSpilTime = tgt->timestamp;
      //StartOfSpilTime = 0;
      StartOfSpilTime2 = tgt->timestamp;
      //StartOfSpilTime2 = 0;
      // printf("12 spill start at %ld ",StartOfSpilTime);
    }
  else if (src->qtrigger==13)
    {
      StartOfSpilTime = -1;
    }
  //else                         //changed 170309
  // rest are interesting only if trigger == 1
  //if (src->qtrigger!=1 ) return;

  // calculate time from spill start in sec
  if (StartOfSpilTime>=0)
    {
      tgt->timespill = (tgt->timestamp - StartOfSpilTime) * 1e-2;// microsec // 50000000.;
      //tgt->timespill = 1;
      //printf("timespill= %f \n",tgt->timespill);
    }

  tgt->timespill2 = (tgt->timestamp - StartOfSpilTime2) * 1e-2; //microsec  // 50000000.;
  //tgt->timespill2 = 1;

  /* Trigger and TPAT*/
  tgt->trigger = src->qtrigger;
  tgt->pattern = src->utpat ;
  tgt->sptrigger = src->uphystrig;
  tgt->snbtrig = src->unbtrig;
  tgt->smaxtrig = src->umaxtrig;

  /* ### scalers:  */
  /* these are treated as 32-bit integers directly  */
  for(int i=0;i<32;i++)
    {
      tgt->sc_long[i]    = src->scaler_frs[i]; //frs crate
      tgt->sc_long[32+i] = src->scaler_main[i]; //main crate
    }
  tgt->sc_long[64] = 1;// calc Accepted/Free Trigger
  tgt->sc_long[65] = 1;// 100 kHz X veto dead-time/100kHz clock
   
  //std::cout<<"1Hz sort,"<<src->vme0[6][3]<<std::endl;

  /* ### TA Ionization Chamber dE:  */
  //  tgt->ic_de = src->vme0[13][16] & 0xfff;
   /* ### MW anodes:  MWPC 11, 31, 21, 22, 51, 71, 81, 82 */
 // only 8 MW are used, but keep index of the array 13 as it was
  for(int i=0;i<13;i++){
    tgt->mw_an[i] = src->vme_frs[8][i] & 0xfff;
  }
  /* ### MW cathodes:  */
  // from MW11 then MW21 then MW22 then MW31
  for(int i=0;i<4;++i)
    {
      tgt->mw_xl[i] = src->vme_frs[8][16+i*4] & 0xfff;
      tgt->mw_xr[i] = src->vme_frs[8][17+i*4] & 0xfff;
      tgt->mw_yu[i] = src->vme_frs[8][18+i*4] & 0xfff;
      tgt->mw_yd[i] = src->vme_frs[8][19+i*4] & 0xfff;
    }
  // from MW51 then MW71 then MW81 then MW82
  for(int i=0;i<4;++i)
    {
      tgt->mw_xl[i+4] = src->vme_frs[9][0+i*4] & 0xfff;
      tgt->mw_xr[i+4] = src->vme_frs[9][1+i*4] & 0xfff;
      tgt->mw_yu[i+4] = src->vme_frs[9][2+i*4] & 0xfff;
      tgt->mw_yd[i+4] = src->vme_frs[9][3+i*4] & 0xfff;
    }

 /*
  // ### MW anodes:
  for(int i=0;i<13;i++)
    tgt->mw_an[i] = src->vme_frs[8][i] & 0xfff;

  // ### MW cathodes:
  // from MW11 -> MW31
  for(int i=0;i<4;++i)
    {
      tgt->mw_xr[i] = src->vme_frs[8][16+i*4] & 0xfff;
      tgt->mw_xl[i] = src->vme_frs[8][17+i*4] & 0xfff;
      tgt->mw_yu[i] = src->vme_frs[8][18+i*4] & 0xfff;
      tgt->mw_yd[i] = src->vme_frs[8][19+i*4] & 0xfff;
    }
  // from MW41 -> MW51
  for(int i=0;i<3;++i)
    {
      tgt->mw_xr[i+4] = src->vme_frs[9][0+i*4] & 0xfff;
      tgt->mw_xl[i+4] = src->vme_frs[9][1+i*4] & 0xfff;
      tgt->mw_yu[i+4] = src->vme_frs[9][2+i*4] & 0xfff;
      tgt->mw_yd[i+4] = src->vme_frs[9][3+i*4] & 0xfff;
    }
  // skip MW61
  //putting MW61 all to zero (09.07.2018)
      tgt->mw_xr[7] = 0;
      tgt->mw_xl[7] = 0;
      tgt->mw_yu[7] = 0;
      tgt->mw_yd[7] = 0;

  // from MW71 -> MWB2
  for(int i=0;i<5;++i)
    {
      tgt->mw_xr[i+8] = src->vme_frs[9][12+i*4] & 0xfff;
      tgt->mw_xl[i+8] = src->vme_frs[9][13+i*4] & 0xfff;
      tgt->mw_yu[i+8] = src->vme_frs[9][14+i*4] & 0xfff;
      tgt->mw_yd[i+8] = src->vme_frs[9][15+i*4] & 0xfff;
    }
 */


  //////////////////////////////////////
  // TPC part                         //
  //                                  //
  //////////////////////////////////////

  //ADC

  //TPC 1 at S2 (TPC 21) in vaccuum
  tgt->tpc_a[0][0]=src->vme_tpcs2[12][0] & 0xfff;
  tgt->tpc_a[0][1]=src->vme_tpcs2[12][1] & 0xfff;
  tgt->tpc_a[0][2]=src->vme_tpcs2[12][2] & 0xfff;
  tgt->tpc_a[0][3]=src->vme_tpcs2[12][3] & 0xfff;
  tgt->tpc_l[0][0]=src->vme_tpcs2[12][4] & 0xfff;
  tgt->tpc_r[0][0]=src->vme_tpcs2[12][5] & 0xfff;
  tgt->tpc_l[0][1]=src->vme_tpcs2[12][6] & 0xfff;
  tgt->tpc_r[0][1]=src->vme_tpcs2[12][7] & 0xfff;

  //TPC 2 at S2 (TPC 22) in vaccuum
  tgt->tpc_a[1][0]=src->vme_tpcs2[12][8] & 0xfff;
  tgt->tpc_a[1][1]=src->vme_tpcs2[12][9] & 0xfff;
  tgt->tpc_a[1][2]=src->vme_tpcs2[12][10] & 0xfff;
  tgt->tpc_a[1][3]=src->vme_tpcs2[12][11] & 0xfff;
  tgt->tpc_l[1][0]=src->vme_tpcs2[12][12] & 0xfff;
  tgt->tpc_r[1][0]=src->vme_tpcs2[12][13] & 0xfff;
  tgt->tpc_l[1][1]=src->vme_tpcs2[12][14] & 0xfff;
  tgt->tpc_r[1][1]=src->vme_tpcs2[12][15] & 0xfff;

  //TPC 3 at S2 (TPC 23) in air
  tgt->tpc_a[2][0]=src->vme_tpcs2[12][16] & 0xfff;
  tgt->tpc_a[2][1]=src->vme_tpcs2[12][17] & 0xfff;
  tgt->tpc_a[2][2]=src->vme_tpcs2[12][18] & 0xfff;
  tgt->tpc_a[2][3]=src->vme_tpcs2[12][19] & 0xfff;
  tgt->tpc_l[2][0]=src->vme_tpcs2[12][20] & 0xfff;
  tgt->tpc_r[2][0]=src->vme_tpcs2[12][21] & 0xfff;
  tgt->tpc_l[2][1]=src->vme_tpcs2[12][22] & 0xfff;
  tgt->tpc_r[2][1]=src->vme_tpcs2[12][23] & 0xfff;

  //TPC 4 at S2 (TPC 24) in air
  tgt->tpc_a[3][0]=src->vme_tpcs2[12][24] & 0xfff;
  tgt->tpc_a[3][1]=src->vme_tpcs2[12][25] & 0xfff;
  tgt->tpc_a[3][2]=src->vme_tpcs2[12][26] & 0xfff;
  tgt->tpc_a[3][3]=src->vme_tpcs2[12][27] & 0xfff;
  tgt->tpc_l[3][0]=src->vme_tpcs2[12][28] & 0xfff;
  tgt->tpc_r[3][0]=src->vme_tpcs2[12][29] & 0xfff;
  tgt->tpc_l[3][1]=src->vme_tpcs2[12][30] & 0xfff;
  tgt->tpc_r[3][1]=src->vme_tpcs2[12][31] & 0xfff;

  //TPC 5  at S4 (TPC 41) in air
  tgt->tpc_a[4][0]=src->vme_tpcs2[13][0] & 0xfff;
  tgt->tpc_a[4][1]=src->vme_tpcs2[13][1] & 0xfff;
  tgt->tpc_a[4][2]=src->vme_tpcs2[13][2] & 0xfff;
  tgt->tpc_a[4][3]=src->vme_tpcs2[13][3] & 0xfff;
  tgt->tpc_l[4][0]=src->vme_tpcs2[13][4] & 0xfff;
  tgt->tpc_r[4][0]=src->vme_tpcs2[13][5] & 0xfff;
  tgt->tpc_l[4][1]=src->vme_tpcs2[13][6] & 0xfff;
  tgt->tpc_r[4][1]=src->vme_tpcs2[13][7] & 0xfff;

  //TPC 6 at S4 (TPC 42) in air
  tgt->tpc_a[5][0]=src->vme_tpcs2[13][8] & 0xfff;
  tgt->tpc_a[5][1]=src->vme_tpcs2[13][9] & 0xfff;
  tgt->tpc_a[5][2]=src->vme_tpcs2[13][10] & 0xfff;
  tgt->tpc_a[5][3]=src->vme_tpcs2[13][11] & 0xfff;
  tgt->tpc_l[5][0]=src->vme_tpcs2[13][12] & 0xfff;
  tgt->tpc_r[5][0]=src->vme_tpcs2[13][13] & 0xfff;
  tgt->tpc_l[5][1]=src->vme_tpcs2[13][14] & 0xfff;
  tgt->tpc_r[5][1]=src->vme_tpcs2[13][15] & 0xfff;

  //TPC at S3 (TPC 31) (checkSB)
  tgt->tpc_a[6][0]=src->vme_tpcs2[13][16] & 0xfff;
  tgt->tpc_a[6][1]=src->vme_tpcs2[13][17] & 0xfff;
  tgt->tpc_a[6][2]=src->vme_tpcs2[13][18] & 0xfff;
  tgt->tpc_a[6][3]=src->vme_tpcs2[13][19] & 0xfff;
  tgt->tpc_l[6][0]=src->vme_tpcs2[13][20] & 0xfff;
  tgt->tpc_r[6][0]=src->vme_tpcs2[13][21] & 0xfff;
  tgt->tpc_l[6][1]=src->vme_tpcs2[13][22] & 0xfff;
  tgt->tpc_r[6][1]=src->vme_tpcs2[13][23] & 0xfff;


  //TDC
  for(int itpc=0; itpc<7; itpc++){
    //anode
    for(int j=0; j<4; j++){
      tgt->tpc_nhit_dt[itpc][j] = src->nhit_v1190_tpcs2[ (v1190_channel_dt[itpc][j]) ];
      for(int ihit=0; ihit<(tgt->tpc_nhit_dt[itpc][j]); ihit++){
        if(ihit<64){
          tgt->tpc_dt[itpc][j][ihit] = src->leading_v1190_tpcs2[ (v1190_channel_dt[itpc][j]) ][ihit];
        }
      }
    }

    //delay line
    for(int j=0; j<2; j++){
      //Left side
      tgt->tpc_nhit_lt[itpc][j] = src->nhit_v1190_tpcs2[ (v1190_channel_lt[itpc][j]) ];
      for(int ihit=0; ihit<(tgt->tpc_nhit_lt[itpc][j]); ihit++){
        if(ihit<64){
          tgt->tpc_lt[itpc][j][ihit] = src->leading_v1190_tpcs2[ (v1190_channel_lt[itpc][j]) ][ihit];
        }
      }
      //Right side
      tgt->tpc_nhit_rt[itpc][j] = src->nhit_v1190_tpcs2[ (v1190_channel_rt[itpc][j]) ];
      for(int ihit=0; ihit<(tgt->tpc_nhit_rt[itpc][j]); ihit++){
        if(ihit<64){
          tgt->tpc_rt[itpc][j][ihit] = src->leading_v1190_tpcs2[ (v1190_channel_rt[itpc][j]) ][ihit];
        }
      }
    }

    //calib grid
    tgt->tpc_nhit_calibgrid[itpc] = src->nhit_v1190_tpcs2[ (v1190_channel_calibgrid[itpc]) ];
    for(int ihit=0; ihit<(tgt->tpc_nhit_calibgrid[itpc]); ihit++){
      if(ihit<64){
        tgt->tpc_calibgrid[itpc][ihit] = src->leading_v1190_tpcs2[ (v1190_channel_calibgrid[itpc]) ][ihit];
      }
    }
  }//  end of for(itpc)

  // TPC time ref TDCs
  for(int i=0; i<8; i++){
      tgt->tpc_nhit_timeref[i] = src->nhit_v1190_tpcs2[ (v1190_channel_timeref[i]) ];
      for(int ihit=0; ihit<(tgt->tpc_nhit_timeref[i]); ihit++){
        if(ihit<64){
          tgt->tpc_timeref[i][ihit] = src->leading_v1190_tpcs2[ (v1190_channel_timeref[i]) ][ihit];
        }
      }
  }


  /* ### SCI dE:  */
  tgt->de_41l = src->vme_main[14][13] & 0xfff; //src->vme_main[11][0] & 0xfff;
  tgt->de_41r = src->vme_main[14][1] & 0xfff;
  tgt->de_21l = src->vme_main[14][2] & 0xfff;
  tgt->de_21r = src->vme_main[14][3] & 0xfff;
  tgt->de_42l = src->vme_main[14][4] & 0xfff;
  tgt->de_42r = src->vme_main[14][5] & 0xfff;

  tgt->de_31l = src->vme_main[14][9] & 0xfff;
  tgt->de_31r = src->vme_main[14][10] & 0xfff;
  tgt->de_43l = src->vme_main[14][11] & 0xfff;
  tgt->de_43r = src->vme_main[14][12] & 0xfff;
  //  tgt->de_81l = src->vme_main[11][13] & 0xfff; // changed on 21/Feb/2020 23:00
  //  tgt->de_81l = src->vme_main[4][15] & 0xfff; // changed on 21/Feb/2020 23:00
  tgt->de_81l = src->vme_main[14][6] & 0xfff; // changed on 16.03.21 19:30
  tgt->de_81r = src->vme_main[14][14] & 0xfff;
  
  //  tgt->de_22l = src->vme_main[4][0] & 0xfff; // changed on 17/Feb/2021
  tgt->de_22l = src->vme_main[14][15] & 0xfff; // changed on 17 Feb 2021, ch0 QDC seems to have a problem
  tgt->de_22r = src->vme_main[14][8] & 0xfff;
  tgt->de_M01l = src->vme_main[14][17] & 0xfff;
  tgt->de_M01r = src->vme_main[14][18] & 0xfff;


  /* ### SCI times:  */
  tgt->dt_21l_21r = src->vme_frs[12][0] & 0xfff;
  tgt->dt_41l_41r = src->vme_frs[12][1] & 0xfff;
  tgt->dt_42l_42r = src->vme_frs[12][2] & 0xfff;
  tgt->dt_43l_43r = src->vme_frs[12][3] & 0xfff;
  tgt->dt_81l_81r = src->vme_frs[12][4] & 0xfff;
  tgt->dt_21l_41l = src->vme_frs[12][5] & 0xfff;
  tgt->dt_21r_41r = src->vme_frs[12][6] & 0xfff;
  tgt->dt_42r_21r = src->vme_frs[12][7] & 0xfff;//swapped recently 28/02/2020
  tgt->dt_42l_21l = src->vme_frs[12][8] & 0xfff;//swapped recently 28/02/2020
  tgt->dt_21l_81l = src->vme_frs[12][9] & 0xfff;
  tgt->dt_21r_81r = src->vme_frs[12][10] & 0xfff;
  tgt->dt_22l_22r = src->vme_frs[12][11] & 0xfff;
  tgt->dt_22l_41l = src->vme_frs[12][12] & 0xfff;
  tgt->dt_22r_41r = src->vme_frs[12][13] & 0xfff;
  tgt->dt_22l_81l = src->vme_frs[12][14] & 0xfff;
  tgt->dt_22r_81r = src->vme_frs[12][15] & 0xfff;


  
  
  // Time raw in ps from VFTX module
  //S1
  if(src->vftx_mult[SCI_MOD][CH_S1U_L]<51){
    tgt->vftx_mult_S1U_L = src->vftx_mult[SCI_MOD][CH_S1U_L];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S1U_L]+1;i++){
      tgt->TRaw_vftx_S1U_L[i] = src->vftx_leading_time[SCI_MOD][CH_S1U_L][i];
    }
  }
  if(src->vftx_mult[SCI_MOD][CH_S1U_R]<51){
    tgt->vftx_mult_S1U_R = src->vftx_mult[SCI_MOD][CH_S1U_R];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S1U_R]+1;i++){
      tgt->TRaw_vftx_S1U_R[i] = src->vftx_leading_time[SCI_MOD][CH_S1U_R][i];
    }
  }

  if(src->vftx_mult[SCI_MOD][CH_S1D_L]<51){
    tgt->vftx_mult_S1D_L = src->vftx_mult[SCI_MOD][CH_S1D_L];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S1D_L]+1;i++){
      tgt->TRaw_vftx_S1D_L[i] = src->vftx_leading_time[SCI_MOD][CH_S1D_L][i];
    }
  }
  if(src->vftx_mult[SCI_MOD][CH_S1D_R]<51){
    tgt->vftx_mult_S1D_R = src->vftx_mult[SCI_MOD][CH_S1D_R];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S1D_R]+1;i++){
      tgt->TRaw_vftx_S1D_R[i] = src->vftx_leading_time[SCI_MOD][CH_S1D_R][i];
    }
  }


  //S2
  if(src->vftx_mult[SCI_MOD][CH_S21_L]<51){
    tgt->vftx_mult_S21_L = src->vftx_mult[SCI_MOD][CH_S21_L];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S21_L]+1;i++){
      tgt->TRaw_vftx_S21_L[i] = src->vftx_leading_time[SCI_MOD][CH_S21_L][i];
    }
  }
  if(src->vftx_mult[SCI_MOD][CH_S21_R]<51){
    tgt->vftx_mult_S21_R = src->vftx_mult[SCI_MOD][CH_S21_R];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S21_R]+1;i++){
      tgt->TRaw_vftx_S21_R[i] = src->vftx_leading_time[SCI_MOD][CH_S21_R][i];
    }
  }

  if(src->vftx_mult[SCI_MOD][CH_S22_L]<51){
    tgt->vftx_mult_S22_L = src->vftx_mult[SCI_MOD][CH_S22_L];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S22_L]+1;i++){
      tgt->TRaw_vftx_S22_L[i] = src->vftx_leading_time[SCI_MOD][CH_S22_L][i];
    }
  }
  if(src->vftx_mult[SCI_MOD][CH_S22_R]<51){
    tgt->vftx_mult_S22_R = src->vftx_mult[SCI_MOD][CH_S22_R];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S22_R]+1;i++){
      tgt->TRaw_vftx_S22_R[i] = src->vftx_leading_time[SCI_MOD][CH_S22_R][i];
    }
  }

  //S3
  if(src->vftx_mult[SCI_MOD][CH_S3_L]<51){
    tgt->vftx_mult_S3_L = src->vftx_mult[SCI_MOD][CH_S3_L];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S3_L]+1;i++){
      tgt->TRaw_vftx_S3_L[i] = src->vftx_leading_time[SCI_MOD][CH_S3_L][i];
    }
  }
  if(src->vftx_mult[SCI_MOD][CH_S3_R]<51){
    tgt->vftx_mult_S3_R = src->vftx_mult[SCI_MOD][CH_S3_R];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S3_R]+1;i++){
      tgt->TRaw_vftx_S3_R[i] = src->vftx_leading_time[SCI_MOD][CH_S3_R][i];
    }
  }

  //S4
  if(src->vftx_mult[SCI_MOD][CH_S41_L]<51){
    tgt->vftx_mult_S41_L = src->vftx_mult[SCI_MOD][CH_S41_L];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S41_L]+1;i++){
      tgt->TRaw_vftx_S41_L[i] = src->vftx_leading_time[SCI_MOD][CH_S41_L][i];
    }
  }
  if(src->vftx_mult[SCI_MOD][CH_S41_R]<51){
    tgt->vftx_mult_S41_R = src->vftx_mult[SCI_MOD][CH_S41_R];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S41_R]+1;i++){
      tgt->TRaw_vftx_S41_R[i] = src->vftx_leading_time[SCI_MOD][CH_S41_R][i];
    }
  }

  if(src->vftx_mult[SCI_MOD][CH_S42_L]<51){
    tgt->vftx_mult_S42_L = src->vftx_mult[SCI_MOD][CH_S42_L];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S42_L]+1;i++){
      tgt->TRaw_vftx_S42_L[i] = src->vftx_leading_time[SCI_MOD][CH_S42_L][i];
    }
  }
  if(src->vftx_mult[SCI_MOD][CH_S42_R]<51){
    tgt->vftx_mult_S42_R = src->vftx_mult[SCI_MOD][CH_S42_R];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S42_R]+1;i++){
      tgt->TRaw_vftx_S42_R[i] = src->vftx_leading_time[SCI_MOD][CH_S42_R][i];
    }
  }

  //S8
  if(src->vftx_mult[SCI_MOD][CH_S8_L]<51){
    tgt->vftx_mult_S8_L = src->vftx_mult[SCI_MOD][CH_S8_L];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S8_L]+1;i++){
      tgt->TRaw_vftx_S8_L[i] = src->vftx_leading_time[SCI_MOD][CH_S8_L][i];
    }
  }
  if(src->vftx_mult[SCI_MOD][CH_S8_R]<51){
    tgt->vftx_mult_S8_R = src->vftx_mult[SCI_MOD][CH_S8_R];
    for(int i=0; i<src->vftx_mult[SCI_MOD][CH_S8_R]+1;i++){
      tgt->TRaw_vftx_S8_R[i] = src->vftx_leading_time[SCI_MOD][CH_S8_R][i];
    }
  }
  
  // SCI dE from Mesytec QDC: 
  tgt->de_mqdc_21l = src->vme_tof[11][0] & 0xfff;
  tgt->de_mqdc_21r = src->vme_tof[11][1] & 0xfff;
  tgt->de_mqdc_22l = src->vme_tof[11][8] & 0xfff;
  tgt->de_mqdc_22r = src->vme_tof[11][9] & 0xfff;
  tgt->de_mqdc_41l = src->vme_tof[11][2] & 0xfff;
  tgt->de_mqdc_41r = src->vme_tof[11][3] & 0xfff;
  tgt->de_mqdc_42l = src->vme_tof[11][4] & 0xfff;
  tgt->de_mqdc_42r = src->vme_tof[11][5] & 0xfff;
  tgt->de_mqdc_81l = src->vme_tof[11][6] & 0xfff;
  tgt->de_mqdc_81r = src->vme_tof[11][7] & 0xfff;
  
  // Magnetic fields
  tgt->hall_probe_adc[0] = src->vme_frs[12][16] & 0xfff;
  tgt->hall_probe_adc[1] = src->vme_frs[12][17] & 0xfff;
  tgt->hall_probe_adc[2] = src->vme_frs[12][18] & 0xfff;
  tgt->hall_probe_adc[3] = src->vme_frs[12][19] & 0xfff;
  tgt->hall_probe_adc[4] = src->vme_frs[12][20] & 0xfff;
  tgt->hall_probe_adc[5] = src->vme_frs[12][21] & 0xfff;

  /* ### SCI Multihit TDC time:  */
  // multiplicity added 22.11.22 CH
  tgt->tdc_nhit_sc41l = src->nhit_v1290_main[0][0];
  tgt->tdc_nhit_sc41r = src->nhit_v1290_main[1][0];
  tgt->tdc_nhit_sc21l = src->nhit_v1290_main[2][0];
  tgt->tdc_nhit_sc21r = src->nhit_v1290_main[3][0];
  tgt->tdc_nhit_sc42l = src->nhit_v1290_main[4][0];
  tgt->tdc_nhit_sc42r = src->nhit_v1290_main[15][0];
  tgt->tdc_nhit_sc43l = src->nhit_v1290_main[6][0];
  tgt->tdc_nhit_sc43r = src->nhit_v1290_main[7][0];
  tgt->tdc_nhit_sc81l = src->nhit_v1290_main[8][0];
  tgt->tdc_nhit_sc81r = src->nhit_v1290_main[9][0];
  tgt->tdc_nhit_sc31l = src->nhit_v1290_main[10][0];
  tgt->tdc_nhit_sc31r = src->nhit_v1290_main[11][0];
  tgt->tdc_nhit_sc11 = src->nhit_v1290_main[12][0];
  tgt->tdc_nhit_sc22l = src->nhit_v1290_main[13][0];
  tgt->tdc_nhit_sc22r = src->nhit_v1290_main[14][0];
  tgt->tdc_nhit_scM01l = src->nhit_v1290_main[16][0];
  tgt->tdc_nhit_scM01r = src->nhit_v1290_main[17][0];
  
  for(int i=0;i<10;i++){
    tgt->tdc_sc41l[i] = src->leading_v1290_main[0][i];
    tgt->tdc_sc41r[i] = src->leading_v1290_main[1][i];
    tgt->tdc_sc21l[i] = src->leading_v1290_main[2][i];
    tgt->tdc_sc21r[i] = src->leading_v1290_main[3][i];
    tgt->tdc_sc42l[i] = src->leading_v1290_main[4][i]; //changed mapping 05.05.22
    tgt->tdc_sc42r[i] = src->leading_v1290_main[15][i]; //changed mapping 03.03.21
    tgt->tdc_sc43l[i] = src->leading_v1290_main[6][i];
    tgt->tdc_sc43r[i] = src->leading_v1290_main[7][i];
    tgt->tdc_sc81l[i] = src->leading_v1290_main[8][i];
    tgt->tdc_sc81r[i] = src->leading_v1290_main[9][i];
    tgt->tdc_sc31l[i] = src->leading_v1290_main[10][i]; //changed mapping 05.05.22
    tgt->tdc_sc31r[i] = src->leading_v1290_main[11][i];
    tgt->tdc_sc11[i]  = src->leading_v1290_main[12][i];
    tgt->tdc_sc22l[i] = src->leading_v1290_main[13][i];
    tgt->tdc_sc22r[i]  = src->leading_v1290_main[14][i];
    tgt->tdc_scM01l[i] = src->leading_v1290_main[16][i]; //changed mapping 07.05.21
    tgt->tdc_scM01r[i] = src->leading_v1290_main[17][i]; //changed mapping 07.05.21
    //tgt->tdc_sc_cave_c[i] = src->leading_v1290_main[5][i]; //newly defined, changed mapping 05.05.22
  }


   //---MUSIC configuration. 2x TUM-MUSIC from FRS crate and 1 TRavel-MUsic from TRMU crate (2020/Jan/23, YT)
   for(int i=0;i<8;i++)
     {
      tgt->music_e1[i] = (src->vme_frs[10][i]) & 0xfff;   //
      tgt->music_e2[i] = (src->vme_frs[10][8+i]) & 0xfff; //
      tgt->music_e3[i] = (src->vme_trmu_adc[i])        ; // Travel-MUSIC (from special VME crate)
      //tgt->music_e4[i] = (src->vme_frs[10][i]) & 0xfff;   // fill with MUSIC1 as test 

      tgt->music_t1[i] = src->leading_v1290_main[16+i][0]; //TUM-MUSIC, modified by JZ 17-04-2021
      tgt->music_t2[i] = src->leading_v1290_main[24+i][0]; //TUM-MUSIC
      //tgt->music_t1[i] = src->leading_v1290_main[16+i][0] & 0xfff; //TUM-MUSIC
      //tgt->music_t2[i] = src->leading_v1290_main[24+i][0] & 0xfff; //TUM-MUSIC
      tgt->music_t3[i] = (src->vme_trmu_tdc[i]);                   //Travel-MUSIC (frAnalysis/Histograms/MUSIC/MUSIC(2)/E/MUSIC2_E(5)om special VME crate)
      tgt->music_t4[i] = 0; // no signal connected 

    }
   for(int i=0;i<4;i++)
     {
       tgt->music_e4[i] = (src->vme_frs[12][16+i]) & 0xfff;   // Prototype MUSIC with only 4 anodes
     }
   for(int i=0;i<4;i++)
     {
       tgt->music_e4[4+i] = 0;   
     }

    //---LaBr3 configuration
    for(int i = 0; i < 8; i++)
    {
      tgt->labr_e_raw[i] = (src->vme_trmu_adc[i+8]);
      tgt->labr_t_raw[i] = (src->vme_trmu_tdc[i+8]);
    }
//----Active Stopper
for(int i = 0; i<32; i++)
 {
	 tgt->dssd_adc_det1[i] = src->vme_actstop[10][i] & 0xfff;
	 tgt->dssd_adc_det2[i] = src->vme_actstop[12][i] & 0xfff;
	 tgt->dssd_adc_det3[i] = src->vme_actstop[14][i] & 0xfff;
	 tgt->dssd_adc_det4[i] = src->vme_actstop[16][i] & 0xfff;
	 tgt->dssd_adc_det5[i] = src->vme_actstop[18][i] & 0xfff;
	 tgt->dssd_adc_det6[i] = src->vme_actstop[20][i] & 0xfff;
 }

  /* ### MUSIC temp & pressure:  */
  // tgt->music_pres[0] = 0;
  // tgt->music_temp[0] = 0;
  // tgt->music_pres[1] = 0;
  // tgt->music_temp[1] = 0;
  // tgt->music_pres[2] = 0;
  // tgt->music_temp[2] = 0;

  // //Channeltron detectors (vme must be adjusted)
  // tgt->ct_time = src->vme3s_MT[2][0] & 0xfff;
  // tgt->ct_signal = src->vme2scaler[20] & 0xfff;
  // tgt->ct_trigger_DU = src->vme2scaler[5]& 0xfff;
  // tgt->ct_trigger_SY = src->vme2scaler[6]& 0xfff;

  // //Electron current measurement (vme must be adjused)
  // tgt->ec_signal = src->vme0[10][1]& 0xfff;

  // // mrtof
  tgt->mrtof_start = src->leading_v1190_mrtof[8][0];
  //tgt->multihit = scr->nhit_v1190_mrtof[9][0];

  // KW there are only 10 hits stored in leading_v1190_mrtof!! 
  // for(int i = 0; i<100; i++)
  for(int i = 0; i<10; i++)
    {
    tgt->mrtof_stop[i] = src->leading_v1190_mrtof[9][i];
    }
  tgt->mrtof_spill = src->leading_v1190_mrtof[10][0];
  tgt->mrtof_ts = (src->mrtof_ts-21600-31557600*51)%(24*3600);
  //printf("unix time %02d mrtof_ts %02d\n", src->mrtof_ts, tgt->mrtof_ts);

  return kTRUE;
} // end of ProcSort

void TFRSSortProc::v1190_channel_init(){
  // ======= upper NIM crate (TPC21+22)=========
  v1190_channel_dt[0][0] = 0; //TPC21-A11
  v1190_channel_dt[0][1] = 1; //TPC21-A12
  v1190_channel_dt[0][2] = 2; //TPC21-A21
  v1190_channel_dt[0][3] = 3; //TPC21-A22
  v1190_channel_lt[0][0] = 4; //TPC21-DL1
  v1190_channel_rt[0][0] = 5; //TPC21-DR1
  // 6, 7 empty
  v1190_channel_lt[0][1] = 8; //TPC21-DL2
  v1190_channel_rt[0][1] = 9; //TPC21-DR2
  v1190_channel_dt[1][0] = 10; //TPC22-A11
  v1190_channel_dt[1][1] = 11; //TPC22-A12
  v1190_channel_dt[1][2] = 12; //TPC22-A21
  v1190_channel_dt[1][3] = 13; //TPC22-A22
  // 14, 15 empty
  //17(top channel of 3rd module is dead)
  //18(skip)
  v1190_channel_lt[1][0] = 18; //TPC22-DL1
  v1190_channel_rt[1][0] = 19; //TPC22-DR1
  v1190_channel_lt[1][1] = 20; //TPC22-DL2
  v1190_channel_rt[1][1] = 21; //TPC22-DR2
  // 22,23 empty

  // ======= middle NIM crate (TPC23+24)=========
  v1190_channel_dt[2][0] = 24; //TPC23-A11
  v1190_channel_dt[2][1] = 25; //TPC23-A12
  v1190_channel_dt[2][2] = 26; //TPC23-A21
  v1190_channel_dt[2][3] = 27; //TPC23-A22
  v1190_channel_lt[2][0] = 28; //TPC23-DL1
  v1190_channel_rt[2][0] = 29; //TPC23-DR1
  //30,31 empty
  v1190_channel_lt[2][1] = 32; //TPC23-DL2
  v1190_channel_rt[2][1] = 33; //TPC23-DR2
  v1190_channel_dt[3][0] = 34; //TPC24-A11
  v1190_channel_dt[3][1] = 35; //TPC24-A12
  v1190_channel_dt[3][2] = 36; //TPC24-A21
  v1190_channel_dt[3][3] = 37; //TPC24-A22
  //38,39 empty
  v1190_channel_lt[3][0] = 40; //TPC24-DL1
  v1190_channel_rt[3][0] = 41; //TPC24-DR1
  v1190_channel_lt[3][1] = 42; //TPC24-DL2
  v1190_channel_rt[3][1] = 43; //TPC24-DR2
  //46,47 empty


  // ======= bottom NIM crate (TPC41+42+31)=========
  v1190_channel_dt[4][0] = 64; //TPC41-A11
  v1190_channel_dt[4][1] = 65; //TPC41-A12
  v1190_channel_dt[4][2] = 66; //TPC41-A21
  v1190_channel_dt[4][3] = 67; //TPC41-A22
  v1190_channel_lt[4][0] = 68; //TPC41-DL1
  v1190_channel_rt[4][0] = 69; //TPC41-DR1
  //70,71 empty
  v1190_channel_lt[4][1] = 72; //TPC41-DL2
  v1190_channel_rt[4][1] = 73; //TPC41-DR2
  v1190_channel_dt[5][0] = 74; //TPC42-A11
  v1190_channel_dt[5][1] = 75; //TPC42-A12
  v1190_channel_dt[5][2] = 76; //TPC42-A21
  v1190_channel_dt[5][3] = 77; //TPC42-A22
  //78,79 empty
  v1190_channel_lt[5][0] = 80; //TPC42-DL1
  v1190_channel_rt[5][0] = 81; //TPC42-DR1
  v1190_channel_lt[5][1] = 82; //TPC42-DL2
  v1190_channel_rt[5][1] = 83; //TPC42-DR2
  v1190_channel_dt[6][0] = 84; //TPC31-A11
  v1190_channel_dt[6][1] = 85; //TPC31-A12
  //86,87 empty
  v1190_channel_dt[6][2] = 88; //TPC31-A21
  v1190_channel_dt[6][3] = 89; //TPC31-A22
  v1190_channel_lt[6][0] = 90; //TPC31-DL1
  v1190_channel_rt[6][0] = 91; //TPC31-DR1
  v1190_channel_lt[6][1] = 92; //TPC31-DL2
  v1190_channel_rt[6][1] = 93; //TPC31-DR2
  //94,95 empty

  //time reference signal
  v1190_channel_timeref[0] = 96; //accept trig
  v1190_channel_timeref[1] = 97; //sc21
  v1190_channel_timeref[2] = 98; //sc22
  v1190_channel_timeref[3] = 99; //sc31
  v1190_channel_timeref[4] =100; //sc41
  v1190_channel_timeref[5] =101; //
  v1190_channel_timeref[6] =102; //
  v1190_channel_timeref[7] =103; //

  //
  v1190_channel_calibgrid[0] = 104;//tpc21grid
  v1190_channel_calibgrid[1] = 105;//tpc22grid
  v1190_channel_calibgrid[2] = 106;//tpc23grid
  v1190_channel_calibgrid[3] = 107;//tpc24grid
  v1190_channel_calibgrid[4] = 108;//tpc41grid
  v1190_channel_calibgrid[5] = 109;//tpc42grid
  v1190_channel_calibgrid[6] = 110;//tpc31grid //to be checked maybe 111

}


ClassImp(TFRSSortProc)
