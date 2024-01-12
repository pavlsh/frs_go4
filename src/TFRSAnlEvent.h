#ifndef TFRSANLEVENT_H
#define TFRSANLEVENT_H

#include "TGo4EventElement.h"
#include "define.hh"

class TFRSAnlProc;
class TGo4FileSource;

class TFRSAnlEvent: public TGo4EventElement {
public:
  TFRSAnlEvent();
  TFRSAnlEvent(const char* name);
  ~TFRSAnlEvent();
  virtual Int_t Init();
  //virtual Int_t Fill();
  virtual void Clear(Option_t *t="");

  Int_t EventFlag;

  //Trigger and TPAT
  Int_t aptrig ; //physics trigger from TPAT
  Int_t atpat ; // TPAT or trigger pattern
  Int_t anbtrig ; //number of triggers in TPAT
  Int_t amaxtrig ; //second trigger from TPAT

  // MUSIC part
  Int_t         music1_anodes_cnt;
  Int_t         music2_anodes_cnt;
  Int_t         music3_anodes_cnt;
  Int_t         music4_anodes_cnt;
  Bool_t        music_b_e1[8];
  Bool_t        music_b_t1[8];
  Bool_t        music_b_e2[8];
  Bool_t        music_b_t2[8];
  Bool_t        music_b_e3[8];
  Bool_t        music_b_t3[8];
  Bool_t        music_b_e4[8];
  Bool_t        music_b_t4[8];
  Bool_t        b_de1;
  Bool_t        b_de2;
  Bool_t        b_de3;
  Bool_t        b_de4;
  Float_t       de[4];
  Float_t       de_cor[4];
  Bool_t        b_dt3;
  Bool_t        b_dt4;
  Float_t       music1_x_mean;
  Float_t       music2_x_mean;
  Float_t       music3_x_mean;
  Float_t       music4_x_mean;
  Bool_t        b_decor;

  //SCI part
  Float_t       sci_l[15];
  Float_t       sci_r[15];
  Float_t       sci_e[15];
  Float_t       sci_tx[15];
  Float_t       sci_x[15];
  Float_t       itag_42_e;
  Float_t       itag_43_e;
  Float_t       sci_u5, sci_d5;
  Float_t       sci_ty5;
  Float_t       sci_y5;
  Float_t       sci_tofll2;
  Float_t       sci_tofrr2;
  Float_t       sci_tof2;
  Float_t       sci_tof2_calib;
  Float_t       sci_tofll3;
  Float_t       sci_tofrr3;
  Float_t       sci_tof3;
  Float_t       sci_tof3_calib;
  Float_t       sci_tofll4;
  Float_t       sci_tofrr4;
  Float_t       sci_tof4;
  Float_t       sci_tof4_calib;
  Float_t       sci_tofll5;
  Float_t       sci_tofrr5;
  Float_t       sci_tof5;
  Float_t       sci_tof5_calib;
  Float_t       sci_tofll6;
  Float_t       sci_tofrr6;
  Float_t       sci_tof6;
  Float_t       sci_tof6_calib;

  Float_t       sci_veto_l[3];
  Float_t       sci_veto_r[3];
  Float_t       sci_veto_e[3];

  Bool_t        sci_b_l[15];
  Bool_t        sci_b_r[15];
  Bool_t        sci_b_e[15];
  Bool_t        sci_b_tx[15];
  Bool_t        sci_b_x[15];
  Bool_t        sci_b_u5, sci_b_d5;
  Bool_t        sci_b_ty5;
  Bool_t        sci_b_y5;
  Bool_t        sci_b_tofll2;
  Bool_t        sci_b_tofrr2;
  Bool_t        sci_b_tofll3;
  Bool_t        sci_b_tofrr3;
  Bool_t        sci_b_tofll4;
  Bool_t        sci_b_tofrr4;
  Bool_t        sci_b_tofll5;
  Bool_t        sci_b_tofrr5;
  Bool_t        sci_b_tofll6;
  Bool_t        sci_b_tofrr6;
  Bool_t        sci_b_detof;
  Bool_t        sci_b_veto_l[3];
  Bool_t        sci_b_veto_r[4];
  Bool_t        sci_b_veto_e[3];

  //VFTX
  Float_t       vftx_tof_S21_S41[50];
  Float_t       vftx_tof_S21_S41_calib[50];
  Float_t       vftx_tof_S22_S41[50];
  Float_t       vftx_tof_S22_S41_calib[50];
  Float_t       vftx_tof_S21_S42[50];
  Float_t       vftx_tof_S21_S42_calib[50];
  Float_t       vftx_tof_S22_S42[50];
  Float_t       vftx_tof_S22_S42_calib[50];
  Float_t       vftx_tof_S21_S8[50];
  Float_t       vftx_tof_S21_S8_calib[50];
  Float_t       vftx_tof_S22_S8[50];
  Float_t       vftx_tof_S22_S8_calib[50];

  // SCI dE from Mesytec QDC
  Float_t       sci_mdqd_e_21;
  Float_t       sci_mdqd_e_22;
  Float_t       sci_mdqd_e_41;
  Float_t       sci_mdqd_e_42;
  Float_t       sci_mdqd_e_81;
  
  // MultiHitTDC
  Float_t       mhtdc_tof8121;
  Float_t       mhtdc_tof4121;
  Float_t       mhtdc_tof4221;
  Float_t       mhtdc_tof4321;
  Float_t       mhtdc_tof3121;
  Float_t       mhtdc_tof4122;
  Float_t       mhtdc_tof8122;
  Float_t       mhtdc_tofM0121;
  Float_t       mhtdc_tofM0122;

  Float_t       mhtdc_sc21lr_dt;
  Float_t       mhtdc_sc31lr_dt;
  Float_t       mhtdc_sc41lr_dt;
  Float_t       mhtdc_sc42lr_dt;
  Float_t       mhtdc_sc43lr_dt;
  Float_t       mhtdc_sc81lr_dt;
  Float_t       mhtdc_sc22lr_dt;
  Float_t       mhtdc_scM01lr_dt;

  Float_t       mhtdc_sc21lr_x;
  Float_t       mhtdc_sc31lr_x;
  Float_t       mhtdc_sc41lr_x;
  Float_t       mhtdc_sc42lr_x;
  Float_t       mhtdc_sc43lr_x;
  Float_t       mhtdc_sc81lr_x;
  Float_t       mhtdc_sc22lr_x;
  Float_t       mhtdc_scM01lr_x;

  // ID part

  Float_t       id_x2;
  Float_t       id_y2;
  Float_t       id_a2;
  Float_t       id_b2;
  Float_t       id_x4;
  Float_t       id_y4;
  Float_t       id_a4;
  Float_t       id_b4;
  Float_t       id_x8;
  Float_t       id_y8;
  Float_t       id_a8;
  Float_t       id_b8;

  Bool_t        id_b_x2;
  Bool_t        id_b_x4;
  Bool_t        id_b_x8;
  Bool_t        id_b_detof2;

  Float_t       id_brho[3];      /* position-corr. BRho      */
  Float_t       id_rho[3];       /* Position-corrected Rho   */

  Float_t       id_beta;        /* Beta from TOF            */
  Float_t       id_beta_s2s8;        /* Beta from TOF            */
  Float_t       id_gamma_s2s8;        /* Gamma from TOF            */

  Float_t       id_beta3;        /* Beta from TOF            */
  Float_t       id_gamma;       /* Gamma from TOF           */
  Float_t       id_AoQ;
  Float_t       id_AoQ_corr;
  Float_t       id_AoQ_s2s8;
  Float_t       id_v_cor;       /* Velocity correction  */
  Float_t       id_v_cor2;      /* Velocity correction  */
  Float_t       id_v_cor3;      /* Velocity correction  */
  Float_t       id_v_cor4;      /* Velocity correction  */

  Float_t       id_z;
  Float_t       id_z2;
  Float_t       id_z3;
  Float_t       id_z4;

  Float_t       id_de_s2tpc;
  Bool_t        id_b_de_s2tpc;
  Float_t       id_z_sc81;
  Float_t       id_v_cor_sc81;
  Float_t       id_b_z_sc81;
  Float_t       id_z_s2tpc;
  Float_t       id_v_cor_s2tpc;
  Float_t       id_b_z_s2tpc;

  Float_t       id_gamma_ta_s2;//highZ analysis 
  Float_t       id_dEdegoQ;
  Float_t       id_dEdeg;
  
  Float_t       id_energy_geL;
  Float_t       id_tac_41_geL;
  Float_t       id_stopper_x;
  Int_t         id_energy_geL_raw;
  Int_t         id_tac_41_geL_raw;
  Int_t         id_trigger;
  Int_t         id_scal_seconds;
  Int_t         id_scal_geL;
  Int_t         id_scal_sc21;
  Int_t         id_scal_sc41;
  Int_t         id_scal_sc42;
  Int_t         id_scal_sc43;
  Int_t         id_scal_sc81;

  Bool_t        id_b_AoQ;
  Bool_t        id_b_AoQ_s2s8;
  Bool_t        id_b_z;
  Bool_t        id_b_z2;
  Bool_t        id_b_z3;
  Bool_t        id_b_z4;
  Bool_t        id_b_x2AoQ[5];
  Bool_t        id_b_x4AoQ[5];
  Bool_t        id_b_z_AoQ[5];
  Bool_t        id_b_music_z[5];

  Float_t       id_vftx_beta_S21_S41[50];
  Float_t       id_vftx_gamma_S21_S41[50];
  Float_t       id_vftx_delta_S21_S41[50];
  Float_t       id_vftx_aoq_S21_S41[50];
  Float_t       id_vftx_aoq_corr_S21_S41[50];
  Float_t       id_vftx_Z1_S21_S41[50];
  Float_t       id_vftx_Z2_S21_S41[50];
  Float_t       id_vftx_v_cor_S21_S41[50];
  
  Float_t       id_vftx_beta_S22_S41[50];
  Float_t       id_vftx_gamma_S22_S41[50];
  Float_t       id_vftx_delta_S22_S41[50];
  Float_t       id_vftx_aoq_S22_S41[50];
  Float_t       id_vftx_aoq_corr_S22_S41[50];
  Float_t       id_vftx_Z1_S22_S41[50];
  Float_t       id_vftx_Z2_S22_S41[50];
  Float_t       id_vftx_v_cor_S22_S41[50];
  
  Float_t       id_vftx_beta_S21_S42[50];
  Float_t       id_vftx_gamma_S21_S42[50];
  Float_t       id_vftx_delta_S21_S42[50];
  Float_t       id_vftx_aoq_S21_S42[50];
  Float_t       id_vftx_aoq_corr_S21_S42[50];
  Float_t       id_vftx_Z1_S21_S42[50];
  Float_t       id_vftx_Z2_S21_S42[50];
  Float_t       id_vftx_v_cor_S21_S42[50];
  
  Float_t       id_vftx_beta_S22_S42[50];
  Float_t       id_vftx_gamma_S22_S42[50];
  Float_t       id_vftx_delta_S22_S42[50];
  Float_t       id_vftx_aoq_S22_S42[50];
  Float_t       id_vftx_aoq_corr_S22_S42[50];
  Float_t       id_vftx_Z1_S22_S42[50];
  Float_t       id_vftx_Z2_S22_S42[50];
  Float_t       id_vftx_v_cor_S22_S42[50];
  
  Float_t       id_vftx_beta_S21_S8[50];
  Float_t       id_vftx_gamma_S21_S8[50];
  Float_t       id_vftx_delta_S21_S8[50];
  Float_t       id_vftx_aoq_S21_S8[50];
  Float_t       id_vftx_aoq_corr_S21_S8[50];
  Float_t       id_vftx_Z_S21_S8[50];
  Float_t       id_vftx_v_cor_S21_S8[50];
  
  Float_t       id_vftx_beta_S22_S8[50];
  Float_t       id_vftx_gamma_S22_S8[50];
  Float_t       id_vftx_delta_S22_S8[50];
  Float_t       id_vftx_aoq_S22_S8[50];
  Float_t       id_vftx_aoq_corr_S22_S8[50];
  Float_t       id_vftx_Z_S22_S8[50];
  Float_t       id_vftx_v_cor_S22_S8[50];
  
  Float_t       id_mhtdc_beta_s2s8;
  Float_t       id_mhtdc_gamma_s2s8;
  Float_t       id_mhtdc_delta_s2s8;
  Float_t       id_mhtdc_aoq_s2s8;
  Float_t       id_mhtdc_z_s2tpc;
  Float_t       id_mhtdc_zcor_s2tpc;
  Float_t       id_mhtdc_v_cor_s2tpc;
  Float_t       id_mhtdc_z_sc81;
  Float_t       id_mhtdc_zcor_sc81;
  Float_t       id_mhtdc_v_cor_sc81;

  Float_t       id_mhtdc_beta_s2s4;
  Float_t       id_mhtdc_gamma_s2s4;
  Float_t       id_mhtdc_delta_s2s4;
  Float_t       id_mhtdc_aoq_s2s4;
  Float_t       id_mhtdc_aoq_s2s4_corr;
  Float_t       id_mhtdc_z_music41;
  Float_t       id_mhtdc_zcor_music41;
  Float_t       id_mhtdc_v_cor_music41;
  Float_t       id_mhtdc_z_music42;
  Float_t       id_mhtdc_zcor_music42;
  Float_t       id_mhtdc_v_cor_music42;

  Float_t       id_mhtdc_beta_s2HTM;
  Float_t       id_mhtdc_gamma_s2HTM;
  Float_t       id_mhtdc_delta_s2HTM;
  Float_t       id_mhtdc_aoq_s2HTM;
  Float_t       id_mhtdc_aoq_s2HTM_corr;
  Float_t       id_mhtdc_v_cor_scM01;
  Float_t       id_mhtdc_v_cor_sc21;
  Float_t       id_mhtdc_v_cor_sc22;
  Float_t       id_mhtdc_dE_HTM;
  Float_t       id_mhtdc_z_HTM;

  Long64_t firsttimestamp;
  Bool_t firstevent;

  Long64_t      timestamp;  // absolute time stamp value
  Double_t      ts;  // relative time from start of the spill
  Double_t      ts2;  // relative time from start of the spill does not reset at end extraction

//Range calculation
  Float_t       id_A;
  Float_t       id_E;
  Float_t       id_highest_z;
  Float_t       id_range;
  Float_t       id_range_lost; // Range lost in degraders
  Float_t       id_x_pos_degrader; //x position at degrader
  Float_t       range_Y1;
  Float_t       range_Y2;
  Float_t       range_Y3;
  Float_t       range_Y4;
  Float_t       range_exponent;
  Bool_t        id_b_range;
  Bool_t        id_b_range_deg;
  Float_t       id_range_1;
  Float_t       id_range_1_Z;
  Float_t       id_range_lost_1;
  Float_t       id_range_2;
  Float_t       id_range_2_Z;
  Float_t       id_range_lost_2;

    //DSSD
  Float_t Dssd_e_det1[32];
  Float_t Dssd_e_det2[32];
  Float_t Dssd_e_det3[32];
  Float_t Dssd_e_det4[32];
  Float_t Dssd_e_det5[32];
  Float_t Dssd_e_det6[32];
  
  Float_t Dssd_e_decay_det1[32];
  Float_t Dssd_e_decay_det2[32];
  Float_t Dssd_e_decay_det3[32];
  Float_t Dssd_e_decay_det4[32];
  Float_t Dssd_e_decay_det5[32];
  Float_t Dssd_e_decay_det6[32];
  
  Float_t Dssd_e_impl_det1[32];
  Float_t Dssd_e_impl_det2[32];
  Float_t Dssd_e_impl_det3[32];
  Float_t Dssd_e_impl_det4[32];
  Float_t Dssd_e_impl_det5[32];
  Float_t Dssd_e_impl_det6[32];

//LaBr
 Float_t Labr_e_calib1;
 Float_t Labr_e_calib2;
 Float_t Labr_e_calib3;
 Float_t Labr_e_calib4;
 Float_t Labr_e_calib5;
 Float_t Labr_e_calib6;
 Float_t Labr_e_calib7;
 Float_t Labr_e_calib8;

 Float_t LaBr_t_raw[8];

  // MRTOF part :
  Float_t mrtof_start;
  // KW inconistent array length for this multihit
  //Float_t mrtof_stop[100];
  //Float_t mrtof_tof[100];
  Float_t mrtof_stop[10];
  Float_t mrtof_tof[10];
  Float_t mrtof_spill;

  Float_t mrtof_si_e1;
  Float_t mrtof_si_e2;
  Float_t mrtof_si_e3;
  Float_t mrtof_si_e4;
  Float_t mrtof_si_e5;

private:

  // TFRSAnlProc *fxProc;        //! This is processor
  // TGo4FileSource *fxFileSrc;  //! This is file source

  ClassDef(TFRSAnlEvent,2)
};

#endif //TFRSANLEVENT_H
