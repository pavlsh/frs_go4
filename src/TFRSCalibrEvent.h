#ifndef TFRSCALIBREVENT_H
#define TFRSCALIBREVENT_H

#include "TGo4EventElement.h"
#include "define.hh"

class TFRSCalibrProc;
class TGo4FileSource;

class TFRSCalibrEvent : public TGo4EventElement {
public:
  TFRSCalibrEvent();
  TFRSCalibrEvent(const char* name);
  virtual ~TFRSCalibrEvent();
  /**
   * Method called by the event owner (analysis step) to fill the
   * event element from the set event source. Event source can
   * be the source of the analysis step (if this is a raw event)
   * or the event processor (if this is a reduced event).
   */
  //virtual Int_t Fill();

  /**
   * Method called by the event owner (analysis step) to clear the
   * event element.
   */
  virtual void Clear(Option_t *t="");

  virtual Int_t Init();

  Int_t EventFlag;
  //Scalers
  UInt_t freeTrig;
  UInt_t acptTrig;
  Long64_t mon_inc[64];

  // Trigger and TPAT
  Int_t crtrig ;  //readout trigger
  Int_t cptrig ; //pysics trigger deduced form TPAT
  Int_t ctpat ; // TPAT or trigger pattern
  Int_t cnbtrig ; //number of triggers per event (form TPAT)
  Int_t cmaxtrig ; //second trigger in TPAT

  // Timestamp
  Long64_t	timestamp; //absolute time stamp value

  // MON part
  Int_t         seconds;
  Int_t         tenthsecs;
  Int_t         hundrethsecs;	//mik
  Int_t         extraction_cycle;
  Int_t         coin[16];
  Float_t       seetram;
  Int_t         extraction_time_ms;

  // MW part
  Float_t       mw_xsum[13];     /*                          */
  Float_t       mw_ysum[13];     /*                          */

  Float_t       mw_x[13];        /*                          */
  Float_t       mw_y[13];        /*                          */
  Float_t       mw_wire[13];     /* special for Helmut       */

  Float_t       z_x_s2;
  Float_t       z_y_s2;
  Float_t       z_x_s4;
  Float_t       z_y_s4;
  Float_t       z_x_s8;
  Float_t       z_y_s8;

  Bool_t        b_mw_xsum[13];   /*  wc on sum               */
  Bool_t        b_mw_ysum[13];   /*                          */

  Float_t   mw_sc21_x;
  Float_t   mw_sc21_y;
  Float_t   mw_sc22_x;
  Float_t   mw_sc22_y;

  Float_t       focx_s2;         /*  FRS foci                */
  Float_t       focy_s2;         /*                          */
  Float_t       focx_s4;         /*                          */
  Float_t       focy_s4;         /*                          */
  Float_t       focx_s8;         /*  FRS foci                */
  Float_t       focy_s8;         /*                          */
  Float_t       angle_x_s2;      /*                          */
  Float_t       angle_y_s2;      /*                          */
  Float_t       angle_x_s4;      /*                          */
  Float_t       angle_y_s4;      /*                          */
  Float_t       angle_x_s8;      /*                          */
  Float_t       angle_y_s8;      /*                          */


  // TPC part
  Int_t tpc_dt_s[7][4];//selected frrom multihit
  Int_t tpc_lt_s[7][2];
  Int_t tpc_rt_s[7][2];
  Int_t tpc_xraw[7][2];
  Int_t tpc_yraw[7][4];
  Int_t tpc_csum[7][4];
  Float_t tpc_x[7];
  Float_t tpc_y[7];
  Bool_t b_tpc_csum[7][4];
  Bool_t b_tpc_xy[7];
  Float_t tpc_de[7];
  Float_t tpc_dx12[7];
  Bool_t b_tpc_de[7];
  //timeref
  Bool_t b_tpc_timeref[8];
  Int_t tpc_timeref_s[8];

  //TPCs 21 & 22 @ S2 focus
  Float_t tpc_x_s2_foc_21_22;
  Float_t tpc_y_s2_foc_21_22;
  Float_t tpc_angle_x_s2_foc_21_22;
  Float_t tpc_angle_y_s2_foc_21_22;

  //TPCs 23 & 24 @ S2 focus
  Float_t tpc_x_s2_foc_23_24;
  Float_t tpc_y_s2_foc_23_24;
  Float_t tpc_angle_x_s2_foc_23_24;
  Float_t tpc_angle_y_s2_foc_23_24;

  //TPCs 22 & 24 @ S2 focus
  Float_t tpc_x_s2_foc_22_24;
  Float_t tpc_y_s2_foc_22_24;
  Float_t tpc_angle_x_s2_foc_22_24;
  Float_t tpc_angle_y_s2_foc_22_24;

  //TPCs 41 & 42 @ s4 focus
  Float_t tpc_x_s4;
  Float_t tpc_y_s4;
  Float_t tpc_angle_x_s4;
  Float_t tpc_angle_y_s4;

  Float_t       tpc21_22_sc21_x;      /* SC21 x                    */
  Float_t       tpc21_22_sc21_y;    /* SC21 y                    */
  Float_t       tpc23_24_sc21_x;      /* SC21 x                    */
  Float_t       tpc23_24_sc21_y;    /* SC21 y                    */
  Float_t       tpc22_24_sc21_x;      /* SC21 x                    */
  Float_t       tpc22_24_sc21_y;    /* SC21 y                    */

  Float_t       tpc21_22_sc22_x;      /* SC22 x                    */
  Float_t       tpc21_22_sc22_y;    /* SC22 y                    */
  Float_t       tpc23_24_sc22_x;      /* SC22 x                    */
  Float_t       tpc23_24_sc22_y;    /* SC22 y                    */
  Float_t       tpc22_24_sc22_x;      /* SC22 x                    */
  Float_t       tpc22_24_sc22_y;    /* SC22 y                    */

  Float_t       tpc21_22_s2target_x;      /* S2TARGET x                    */
  Float_t       tpc21_22_s2target_y;    /* S2TARGET y                    */
  Float_t       tpc23_24_s2target_x;      /* S2TARGET x                    */
  Float_t       tpc23_24_s2target_y;    /* S2TARGET y                    */
  Float_t       tpc22_24_s2target_x;      /* S2TARGET x                    */
  Float_t       tpc22_24_s2target_y;    /* S2TARGET y                    */

  Float_t       tpc_sc41_x;      /* SC41 x                    */
  Float_t       tpc_sc41_y;      /* SC41 y                    */
  Float_t       tpc_sc42_x;      /* SC42 x                    */
  Float_t       tpc_sc42_y;      /* SC42 y                    */
  Float_t       tpc_sc43_x;      /* SC43 x                    */
  Float_t       tpc_sc43_y;      /* SC43 y                    */
  Float_t       tpc_music41_x;      /* MUSIC41 x                    */
  Float_t       tpc_music41_y;      /* MUSIC41 y                    */
  Float_t       tpc_music42_x;      /* MUSIC42 x                    */
  Float_t       tpc_music42_y;      /* MUSIC42 y                    */
  Float_t       tpc_music43_x;      /* MUSIC43 x                    */
  Float_t       tpc_music43_y;      /* MUSIC43 y                    */
  Float_t       tpc_music44_x;      /* MUSIC44 x                    */
  Float_t       tpc_music44_y;      /* MUSIC44 y                    */
  Float_t       tpc_s4target_x;      /* S4 target x                    */
  Float_t       tpc_s4target_y;      /* S4 target y                    */

 //LaBr3
 Float_t labr_e_calib1;
 Float_t labr_e_calib2;
 Float_t labr_e_calib3;
 Float_t labr_e_calib4;
 Float_t labr_e_calib5;
 Float_t labr_e_calib6;
 Float_t labr_e_calib7;
 Float_t labr_e_calib8;

 Float_t Labr_t_raw[8];

  //Si detectors
  Float_t si_e1;
  Float_t si_e2;
  Float_t si_e3;
  Float_t si_e4;
  Float_t si_e5;

  //Float_t dssd_e[32]; //[3][2][16]
  Float_t dssd_e_det1[32];
  Float_t dssd_e_det2[32];
  Float_t dssd_e_det3[32];
  Float_t dssd_e_det4[32];
  Float_t dssd_e_det5[32];
  Float_t dssd_e_det6[32];

  Float_t dssd_e_decay_det1[32];
  Float_t dssd_e_decay_det2[32];
  Float_t dssd_e_decay_det3[32];
  Float_t dssd_e_decay_det4[32];
  Float_t dssd_e_decay_det5[32];
  Float_t dssd_e_decay_det6[32];

  Float_t dssd_e_impl_det1[32];
  Float_t dssd_e_impl_det2[32];
  Float_t dssd_e_impl_det3[32];
  Float_t dssd_e_impl_det4[32];
  Float_t dssd_e_impl_det5[32];
  Float_t dssd_e_impl_det6[32];

  Float_t dssd_maxenergy; //[3]
  Int_t dssd_maxindex;  //[3]

  Float_t dssd_xmaxenergy;
  Int_t dssd_xmaxindex;
  Float_t dssd_ymaxenergy;
  Int_t dssd_ymaxindex;

private:
  // TFRSCalibrProc   *fxProc;  //! Don't put this to file
  // TGo4FileSource *fxFileSrc;  //! This is file source

  ClassDef(TFRSCalibrEvent,1)
};

#endif //TFRSCALIBREVENT_H
