#include "TFRSAnlEvent.h"

#include "Riostream.h"
#include "Go4EventServer/TGo4FileSource.h"
#include "TFRSAnlProc.h"

TFRSAnlEvent::TFRSAnlEvent() :TGo4EventElement("FRSAnlEvent")//, fxProc(0), fxFileSrc(0) {
{ }

TFRSAnlEvent::TFRSAnlEvent(const char* name) :TGo4EventElement(name)//, fxProc(0), fxFileSrc(0) {
{ }

TFRSAnlEvent::~TFRSAnlEvent()
{ }

Int_t TFRSAnlEvent::Init()
{
  //--- check for different source types
  Int_t rev = 0;
  // if(CheckEventSource("TFRSAnlProc")) {
  //   fxProc = (TFRSAnlProc*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by " << fxProc->GetName() << " ****" << std::endl;
  // } else
  // if(CheckEventSource("TGo4FileSource")) {
  //   fxFileSrc = (TGo4FileSource*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by File Source ****"<< std::endl;
  // }
  // else rev=1;
  return rev;
}


// Int_t TFRSAnlEvent::Fill()
// {
//    Clear();
//    if(fxProc) fxProc->FRSEventAnalysis(this); else
//    if(fxFileSrc)fxFileSrc->BuildEvent(this); // method from framework to restore event from file

//    return 0;
// }

void TFRSAnlEvent::Clear(Option_t *t)
{
  // MUSIC part
  EventFlag = 0;

  aptrig = 0;
  atpat = 0;
  anbtrig = 0;
  amaxtrig = 0;

  for(int i=0;i<8;i++)
    {
      music_b_e1[i] = kFALSE;
      music_b_t1[i] = kFALSE;
      music_b_e2[i] = kFALSE;
      music_b_t2[i] = kFALSE;
      music_b_t3[i] = kFALSE;
      music_b_e3[i] = kFALSE;
      music_b_t4[i] = kFALSE;
      music_b_e4[i] = kFALSE;
    }

  music1_anodes_cnt = 0;
  music2_anodes_cnt = 0;
  music3_anodes_cnt = 0;
  music4_anodes_cnt = 0;
  b_de1 = kFALSE;
  b_de2 = kFALSE;
  b_de3 = kFALSE;
  b_de4 = kFALSE;
  b_dt3 = kFALSE;
  b_dt4 = kFALSE;
  de[0] = 0.;
  de[1] = 0.;
  de[2] = 0.;
  de[3] = 0.;
  de_cor[0] = 0.;
  de_cor[1] = 0.;
  de_cor[2] = 0.;
  de_cor[3] = 0.;

  music1_x_mean = -999.;
  music2_x_mean = -999.;
  music3_x_mean = -999.;
  music4_x_mean = -999.;
  b_decor = kFALSE;

  // SCI part
  for(int i=0;i<15;i++)
    {
      sci_l[i] = 1.;
      sci_r[i] = 0.;
      sci_e[i] = 0.;
      sci_tx[i] = 0.;
      sci_x[i] = 0.;
      sci_b_l[i] = kFALSE;
      sci_b_r[i] = kFALSE;
      sci_b_e[i] = kFALSE;
      sci_b_tx[i] = kFALSE;
      sci_b_x[i] = kFALSE;
    }

  // veto SCI
  for(int i=0;i<3;i++)
    {
      sci_veto_l[i] = 1.;
      sci_veto_r[i] = 1.;
      sci_b_veto_l[i] = kFALSE;
      sci_b_veto_r[i] = kFALSE;
      sci_b_veto_e[i] = kFALSE;
    }

  sci_b_veto_r[3] = kTRUE; // veto3 missing channel

  itag_42_e =0.;
  itag_43_e =0.;

  sci_u5 = 1.; sci_d5 = 1.;
  sci_b_u5 = kFALSE; sci_b_d5 = kFALSE;

  sci_ty5 = 0.;
  sci_b_ty5 = kFALSE;
  sci_y5 = 0.;
  sci_b_y5 = kFALSE;

  sci_tofll2 = 0.; sci_b_tofll2 = kFALSE;
  sci_tofrr2 = 0.; sci_b_tofrr2 = kFALSE;
  sci_tofll3 = 0.; sci_b_tofll3 = kFALSE;
  sci_tofrr3 = 0.; sci_b_tofrr3 = kFALSE;
  sci_tofll4 = 0.; sci_b_tofll4 = kFALSE;
  sci_tofrr4 = 0.; sci_b_tofrr4 = kFALSE;
  sci_tofll5 = 0.; sci_b_tofll5 = kFALSE;
  sci_tofrr5 = 0.; sci_b_tofrr5 = kFALSE;
  sci_tofll6 = 0.; sci_b_tofll6 = kFALSE;
  sci_tofrr6 = 0.; sci_b_tofrr6 = kFALSE;
  sci_tof2 = 0.; sci_tof3 = 0.; sci_tof4 = 0.; sci_tof5 = 0.; sci_tof6 = 0.;
  sci_tof2_calib = 0.; sci_tof3_calib = 0.; sci_tof4_calib = 0.; sci_tof5_calib = 0.; sci_tof6_calib = 0.;

  sci_b_detof = kFALSE;

  //VFTX
  // KW inconistent array length for this multihit should be 50!!
  //for(int i=0;i<51;i++)
  for(int i=0;i<50;i++)
    {
      vftx_tof_S21_S41[i] = -999.9;
      vftx_tof_S21_S41_calib[i] = -999.9;
      vftx_tof_S22_S41[i] = -999.9;
      vftx_tof_S22_S41_calib[i] = -999.9;
    }	  

  // SCI dE from Mesytec QDC
  sci_mdqd_e_21 = -999.9;
  sci_mdqd_e_22 = -999.9;
  sci_mdqd_e_41 = -999.9;
  sci_mdqd_e_42 = -999.9;
  sci_mdqd_e_81 = -999.9;

  // MultiHitTDC
   mhtdc_tof8121 = -999.9;
   mhtdc_tof4121 = -999.9;
   mhtdc_tof4221 = -999.9;
   mhtdc_tof4321 = -999.9;
   mhtdc_tof3121 = -999.9;
   mhtdc_tof4122 = -999.9;
   mhtdc_tof8122 = -999.9;
   mhtdc_tofM0121 = -999.9;
   mhtdc_tofM0122 = -999.9;

   mhtdc_sc21lr_dt = -999.9;
   mhtdc_sc31lr_dt = -999.9;
   mhtdc_sc41lr_dt = -999.9;
   mhtdc_sc42lr_dt = -999.9;
   mhtdc_sc43lr_dt = -999.9;
   mhtdc_sc81lr_dt = -999.9;
   mhtdc_sc22lr_dt = -999.9;
   mhtdc_scM01lr_dt = -999.9;

   mhtdc_sc21lr_x = -999.9;
   mhtdc_sc31lr_x = -999.9;
   mhtdc_sc41lr_x = -999.9;
   mhtdc_sc42lr_x = -999.9;
   mhtdc_sc43lr_x = -999.9;
   mhtdc_sc81lr_x = -999.9;
   mhtdc_sc22lr_x = -999.9;
   mhtdc_scM01lr_x = -999.9;

  // ID part

  /*  some dummy values...           */
  id_x2=999.0;
  id_y2=999.0;
  id_a2=999.0;
  id_b2=999.0;
  id_x4=999.0;
  id_y4=999.0;
  id_a4=999.0;
  id_b4=999.0;
  id_x8=999.0;
  id_y8=999.0;
  id_a8=999.0;
  id_b8=999.0;

  id_b_x2 = kFALSE;
  id_b_x4 = kFALSE;
  id_b_x8 = kFALSE;
  id_b_detof2 = kFALSE;

  for (int i=0;i<3;i++){
      id_brho[i] = 0.;
      id_rho[i] = 0.;
  }
  id_beta     = 0.;
  id_beta_s2s8= 0.;
  id_beta3     = 0.;
  id_gamma    = 1.;
  id_gamma_s2s8    = 1.;
  id_AoQ      = 0.0;
  id_AoQ_corr = 0.0;
  id_AoQ_s2s8      = 0.0;
  
  id_gamma_ta_s2 = 0.;//highZ analysis 
  id_dEdegoQ =0.;
  id_dEdeg   =0.;

  
  id_v_cor = 0.;
  id_v_cor2 = 0.;
  id_v_cor3 = 0.;
  id_v_cor4 = 0.;
  id_z = 0.;
  id_z2 = 0.;
  id_z3 = 0.;
  id_z4 = 0.;
  id_energy_geL = 0.;
  id_tac_41_geL = 0.;
  id_energy_geL_raw = 0;
  id_tac_41_geL_raw = 0;
  id_stopper_x = -666;
  id_trigger = 0;
  id_scal_seconds = 0;
  id_scal_geL = 0;
  id_scal_sc21 = 0;
  id_scal_sc41 = 0;
  id_scal_sc42 = 0;
  id_scal_sc43 = 0;
  id_scal_sc81 = 0;
  id_de_s2tpc =0.0;
  id_b_de_s2tpc = kFALSE;
  id_z_sc81 = 0.0;
  id_v_cor_sc81 = 0.0;
  id_b_z_sc81 = kFALSE;
  id_z_s2tpc =0.0;
  id_v_cor_s2tpc =0.0;
  id_b_z_s2tpc = kFALSE;

  id_b_AoQ    = kFALSE;
  id_b_z      = kFALSE;
  id_b_z2      = kFALSE;
  id_b_z3      = kFALSE;
  id_b_z4      = kFALSE;
  id_b_AoQ_s2s8    = kFALSE;

  // KW inconistent array length for this multihit should be 50!!
  //for (int i=0;i<51;i++){
  for (int i=0;i<50;i++){
    id_vftx_beta_S21_S41[i] = -999;
    id_vftx_gamma_S21_S41[i] = -999;
    id_vftx_delta_S21_S41[i] = -999;
    id_vftx_aoq_S21_S41[i]   = -999;
    id_vftx_aoq_corr_S21_S41[i]   = -999;
    id_vftx_Z1_S21_S41[i]  = -999;
    id_vftx_Z2_S21_S41[i]  = -999;
    id_vftx_v_cor_S21_S41[i] =0;
    id_vftx_beta_S22_S41[i] = -999;
    id_vftx_gamma_S22_S41[i] = -999;
    id_vftx_delta_S22_S41[i] = -999;
    id_vftx_aoq_S22_S41[i]   = -999;
    id_vftx_aoq_corr_S22_S41[i]   = -999;
    id_vftx_Z1_S22_S41[i]  = -999;
    id_vftx_Z2_S22_S41[i]  = -999;
    id_vftx_v_cor_S22_S41[i] =0;

    id_vftx_beta_S21_S42[i] = -999;
    id_vftx_gamma_S21_S42[i] = -999;
    id_vftx_delta_S21_S42[i] = -999;
    id_vftx_aoq_S21_S42[i]   = -999;
    id_vftx_aoq_corr_S21_S42[i]   = -999;
    id_vftx_Z1_S21_S42[i]  = -999;
    id_vftx_Z2_S21_S42[i]  = -999;
    id_vftx_v_cor_S21_S42[i] =0;
    id_vftx_beta_S22_S42[i] = -999;
    id_vftx_gamma_S22_S42[i] = -999;
    id_vftx_delta_S22_S42[i] = -999;
    id_vftx_aoq_S22_S42[i]   = -999;
    id_vftx_aoq_corr_S22_S42[i]   = -999;
    id_vftx_Z1_S22_S42[i]  = -999;
    id_vftx_Z2_S22_S42[i]  = -999;
    id_vftx_v_cor_S22_S42[i] =0;

    id_vftx_beta_S21_S8[i] = -999;
    id_vftx_gamma_S21_S8[i] = -999;
    id_vftx_delta_S21_S8[i] = -999;
    id_vftx_aoq_S21_S8[i]   = -999;
    id_vftx_aoq_corr_S21_S8[i]   = -999;
    id_vftx_Z_S21_S8[i]  = -999;
    id_vftx_v_cor_S21_S8[i] =0;
    id_vftx_beta_S22_S8[i] = -999;
    id_vftx_gamma_S22_S8[i] = -999;
    id_vftx_delta_S22_S8[i] = -999;
    id_vftx_aoq_S22_S8[i]   = -999;
    id_vftx_aoq_corr_S22_S8[i]   = -999;
    id_vftx_Z_S22_S8[i]  = -999;
    id_vftx_v_cor_S22_S8[i] =0;
  }
  
  id_mhtdc_beta_s2s8  = -999;
  id_mhtdc_gamma_s2s8 = -999;
  id_mhtdc_delta_s2s8 = -999;
  id_mhtdc_aoq_s2s8   = -999;
  id_mhtdc_z_s2tpc    = -999;
  id_mhtdc_zcor_s2tpc = -999;
  id_mhtdc_z_sc81     = -999;
  id_mhtdc_zcor_sc81  = -999;
  id_mhtdc_v_cor_s2tpc = 0.;
  id_mhtdc_v_cor_sc81 = 0.;
  
  id_mhtdc_beta_s2s4  = -999;
  id_mhtdc_gamma_s2s4 = -999;
  id_mhtdc_delta_s2s4 = -999;
  id_mhtdc_aoq_s2s4   = -999;
  id_mhtdc_aoq_s2s4_corr   = -999;
  id_mhtdc_z_music41  = -999;
  id_mhtdc_zcor_music41  =-999;
  id_mhtdc_v_cor_music41 =0;
  id_mhtdc_z_music42 =-999;
  id_mhtdc_zcor_music42 =-999;
  id_mhtdc_v_cor_music42 =0;

  id_mhtdc_beta_s2HTM  = -999;
  id_mhtdc_gamma_s2HTM = -999;
  id_mhtdc_delta_s2HTM = -999;
  id_mhtdc_aoq_s2HTM   = -999;
  id_mhtdc_aoq_s2HTM_corr   = -999;
  id_mhtdc_v_cor_scM01 =0;
  id_mhtdc_v_cor_sc21 =0;
  id_mhtdc_v_cor_sc22 =0;
  id_mhtdc_dE_HTM  = -999;
  id_mhtdc_z_HTM  = -999;
  
  for(int i=0;i<5;i++)
    {
      id_b_x2AoQ[i]  = kFALSE;
      id_b_x4AoQ[i] = kFALSE;
      id_b_z_AoQ[i] = kFALSE;
      id_b_music_z[i] = kFALSE;
    }
  timestamp=0;
  ts=0;
  ts2=0;


  mrtof_start = 0;
  // KW inconistent array length for this multihit
  for(int i = 0; i< 10; i++)
    {
      mrtof_stop[i] = 0;
      mrtof_tof[i] = 0.;
    }
  mrtof_spill = 0;

  mrtof_si_e1 = 0;
  mrtof_si_e2 = 0;
  mrtof_si_e3 = 0;
  mrtof_si_e4 = 0;
  mrtof_si_e5 = 0;

//DSSD
for(int i = 0; i<32; i++){
 
 Dssd_e_det1[i]=0;
 Dssd_e_det2[i]=0;
 Dssd_e_det3[i]=0;
 Dssd_e_det4[i]=0;
 Dssd_e_det5[i]=0;
 Dssd_e_det6[i]=0;

 Dssd_e_decay_det1[i]=0;
 Dssd_e_decay_det2[i]=0;
 Dssd_e_decay_det3[i]=0;
 Dssd_e_decay_det4[i]=0;
 Dssd_e_decay_det5[i]=0;
 Dssd_e_decay_det6[i]=0;

 Dssd_e_impl_det1[i]=0;
 Dssd_e_impl_det2[i]=0;
 Dssd_e_impl_det3[i]=0;
 Dssd_e_impl_det4[i]=0;
 Dssd_e_impl_det5[i]=0;
 Dssd_e_impl_det6[i]=0;
}

//LaBr

 Labr_e_calib1= 0;
 Labr_e_calib2= 0;
 Labr_e_calib3= 0;
 Labr_e_calib4= 0;
 Labr_e_calib5= 0;
 Labr_e_calib6= 0;
 Labr_e_calib7= 0;
 Labr_e_calib8= 0;

 //Range calculation
    id_A = 0.;
    id_E = 0.;
    id_highest_z = 999.9;
    id_range = 0;
    id_range_lost = 0;
    id_x_pos_degrader = 0;
    id_b_range = kFALSE;
    id_b_range_deg = kFALSE;
    id_range_1 = 0;
    id_range_1_Z = 999.9;
    id_range_lost_1 = 0;
    id_range_2 = 0;
    id_range_2_Z = 999.9;
    id_range_lost_2 = 0;

 for(int i=0; i<8; i++){
  LaBr_t_raw[i] = 0;
 }

}

ClassImp(TFRSAnlEvent)
