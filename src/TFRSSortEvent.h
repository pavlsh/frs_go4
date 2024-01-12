#ifndef TFRSSORTEVENT_H
#define TFRSSORTEVENT_H

#include "TGo4EventElement.h"

#include "define.hh"

class TFRSSortProc;
class TGo4FileSource;

class TFRSSortEvent : public TGo4EventElement {
public:
  TFRSSortEvent();
  TFRSSortEvent(const char* name);
  virtual ~TFRSSortEvent();
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


  // MON part
  Int_t EventFlag;
  // time stamp data
  Int_t         ts_id;
  Int_t         ts_word[4]; //for the titris time stammping
  Int_t         tsys_word[3]; //for the system time
  Long64_t      timestamp;  // absolute time stamp value
  Double_t      timespill;  // relative time from start of the spill
  Double_t      timespill2;  // relative time from start of the spill does not reset at end extraction
  Double_t      systemtime_ms;
  Double_t      systemtime_s;

  Int_t         pattern;  //! this is the TPAT
  Int_t         trigger; //! this is the physical trigger
  Int_t         sptrigger; //!this is the physics triggt
  Int_t         snbtrig; //!number of triggers per event (in tpat)
  Int_t         smaxtrig; //!second trigger (in tpat)

  // scaler readings
  UInt_t sc_long[66]; //changed from 32 to 64 (10.07.2018)// add 2 ratios (19.03.21)
  UInt_t sc_long2[32];


  // SEETRAM calib IC energy
  //      Int_t         ic_de;           /*                          */


  // part of MW parameter
  Int_t         mw_an[13];       /*  anode time              */
  Int_t         mw_xl[13];       /*  Rohdaten                */
  Int_t         mw_xr[13];       /*                          */
  Int_t         mw_yu[13];       /*                          */
  Int_t         mw_yd[13];       /*                          */

  // TPC part //(HAPOL-25/03/06) 6 TPCs each one with 2 delay lines each!!
  //7 TPCs (4 at S2, 2 at S4 and one at S3) 03.07.2018 SB
  //ADCs
  Int_t tpc_l[7][2]; //[i_tpc][i_delayline_L]
  Int_t tpc_r[7][2]; //[i_tpc][i_delayline_R]
  Int_t tpc_a[7][4]; //[i_tpc][i_anode]

  //Multihit TDCs
  Int_t tpc_lt[7][2][64];//multihit up to 16
  Int_t tpc_rt[7][2][64];//multihit up to 16
  Int_t tpc_dt[7][4][64];//multihit up to 16
  Int_t tpc_nhit_lt[7][2];//multihit number
  Int_t tpc_nhit_rt[7][2];//multihit number
  Int_t tpc_nhit_dt[7][4];//multihit number

  Int_t tpc_calibgrid[7][64];//multihit up to 16
  Int_t tpc_timeref[8][64];//multihit up to 16
  Int_t tpc_nhit_calibgrid[7];//multihit number
  Int_t tpc_nhit_timeref[8];//multihit number


  // SCI part
  Int_t         de_21l;          /* dE SCI21 left            */
  Int_t         de_21r;          /* de SCI21 right           */
  Int_t         de_41l;          /* dE SCI41 left            */
  Int_t         de_41r;          /* dE SCI41 right           */
  Int_t         de_41u;          /* dE SCI41 up              */
  Int_t         de_41d;          /* dE SCI41 down            */
  Int_t         de_42l;          /* de SCI42 left            */
  Int_t         de_42r;          /* de SCI42 right           */
  Int_t         de_43l;          /* de SCI43 left            */
  Int_t         de_43r;          /* de SCI43 right           */
  Int_t         de_81l;          /* de SCI81 left            */
  Int_t         de_81r;          /* de SCI81 right           */
  Int_t         de_31l;          /* de SCI31 left            */
  Int_t         de_31r;          /* de SCI31 right           */
  Int_t         de_22l;          /* dE SCI22 left            */
  Int_t         de_22r;          /* de SCI22 right           */
  Int_t         de_M01l;          /* dE SCIM01 left            */
  Int_t         de_M01r;          /* dE SCIM01 right           */

  Int_t         dt_21l_21r;      /*                          */
  Int_t         dt_41l_41r;      /*                          */
  Int_t         dt_21l_41l;      /*                          */
  Int_t         dt_21r_41r;      /*                          */
  Int_t         dt_42l_42r;      /*                          */
  Int_t         dt_43l_43r;      /*                          */
  Int_t         dt_42l_21l;      /*                          */
  Int_t         dt_42r_21r;      /*                          */
  Int_t         dt_41u_41d;      /*                          */
  Int_t         dt_81l_81r;
  Int_t         dt_21l_81l;
  Int_t         dt_21r_81r;
  Int_t         dt_22l_22r;
  Int_t         dt_22l_41l;
  Int_t         dt_22r_41r;
  Int_t         dt_22l_81l;
  Int_t         dt_22r_81r;	

  // VFTX for new ToF
  Int_t TRaw_vftx_S1U_L[50];
  Int_t TRaw_vftx_S1U_R[50];
  Int_t TRaw_vftx_S1D_L[50];
  Int_t TRaw_vftx_S1D_R[50];

  Int_t TRaw_vftx_S21_L[50];
  Int_t TRaw_vftx_S21_R[50];
  Int_t TRaw_vftx_S22_L[50];
  Int_t TRaw_vftx_S22_R[50];

  Int_t TRaw_vftx_S3_L[50];
  Int_t TRaw_vftx_S3_R[50];
  
  Int_t TRaw_vftx_S41_L[50];
  Int_t TRaw_vftx_S41_R[50];
  Int_t TRaw_vftx_S42_L[50];
  Int_t TRaw_vftx_S42_R[50];
  
  Int_t TRaw_vftx_S8_L[50];
  Int_t TRaw_vftx_S8_R[50];
  
  Int_t vftx_mult_S1U_L;
  Int_t vftx_mult_S1U_R;
  Int_t vftx_mult_S1D_L;
  Int_t vftx_mult_S1D_R;
  
  Int_t vftx_mult_S3_L;
  Int_t vftx_mult_S3_R;
  
  Int_t vftx_mult_S21_L;
  Int_t vftx_mult_S21_R;
  Int_t vftx_mult_S22_L;
  Int_t vftx_mult_S22_R;
  
  Int_t vftx_mult_S41_L;
  Int_t vftx_mult_S41_R;
  Int_t vftx_mult_S42_L;
  Int_t vftx_mult_S42_R;

  Int_t vftx_mult_S8_L;
  Int_t vftx_mult_S8_R;


  // i dont know
  Int_t TRaw_vftx_tri;

  // SCI dE from Mesytec QDC
  Int_t de_mqdc_21l;
  Int_t de_mqdc_21r;
  Int_t de_mqdc_22l;
  Int_t de_mqdc_22r;
  Int_t de_mqdc_41l;
  Int_t de_mqdc_41r;
  Int_t de_mqdc_42l;
  Int_t de_mqdc_42r;
  Int_t de_mqdc_81l;
  Int_t de_mqdc_81r;

  // User multihit TDC
  Int_t tdc_nhit_sc41l;
  Int_t tdc_nhit_sc41r;
  Int_t tdc_nhit_sc21l;
  Int_t tdc_nhit_sc21r;
  Int_t tdc_nhit_sc42l;
  Int_t tdc_nhit_sc42r;
  Int_t tdc_nhit_sc43l;
  Int_t tdc_nhit_sc43r;
  Int_t tdc_nhit_sc81l;
  Int_t tdc_nhit_sc81r;
  Int_t tdc_nhit_sc31l;
  Int_t tdc_nhit_sc31r;
  Int_t tdc_nhit_sc11;
  Int_t tdc_nhit_sc22l;
  Int_t tdc_nhit_sc22r;
  Int_t tdc_nhit_scM01l;
  Int_t tdc_nhit_scM01r;
  
  Int_t tdc_sc41l[10];
  Int_t tdc_sc41r[10];
  Int_t tdc_sc21l[10];
  Int_t tdc_sc21r[10];
  Int_t tdc_sc42l[10];
  Int_t tdc_sc42r[10];
  Int_t tdc_sc43l[10];
  Int_t tdc_sc43r[10];
  Int_t tdc_sc81l[10];
  Int_t tdc_sc81r[10];
  Int_t tdc_sc31l[10];
  Int_t tdc_sc31r[10];
  Int_t tdc_sc11[10];
  Int_t tdc_sc22l[10];
  Int_t tdc_sc22r[10];
  Int_t tdc_scM01l[10];
  Int_t tdc_scM01r[10];

  // MUSIC1 part
  Int_t         music_e1[8];     /* Raw energy signals       */
  Int_t         music_t1[8];     /* Raw drift time           */
  Int_t         music_pres[3];   /* Music Druck              */
  Int_t         music_temp[3];   /* Music Temperatur         */

  // MUSIC2 part
  Int_t         music_e2[8];     /* Raw energy signals       */
  Int_t         music_t2[8];     /* Raw drift time           */

  // MUSIC3 part
  Int_t         music_e3[8];     /* Raw energy signals       */
  Int_t         music_t3[8];     /* Raw drift times          */

  // MUSIC4 part
  Int_t         music_e4[8];     /* Raw energy signals       */
  Int_t         music_t4[8];     /* Raw drift times          */

  // Single Anode
  Int_t SingleAnode_adc;

  //LaBr detectors
  Int_t labr_e_raw[8];   /* Raw energy signals       */
  Int_t	labr_t_raw[8];	 /* Raw time		     */

  //Si detectors
  Int_t si_adc1;
  Int_t si_adc2;
  Int_t si_adc3;
  Int_t si_adc4;
  Int_t si_adc5;

  //DSSD detectors
  Int_t dssd_adc_det1[32];
  Int_t dssd_adc_det2[32];
  Int_t dssd_adc_det3[32];
  Int_t dssd_adc_det4[32];
  Int_t dssd_adc_det5[32];
  Int_t dssd_adc_det6[32];

  //Electron current measurement
  Int_t	ec_signal;

  // MR-TOF
  Int_t mrtof_start;
  // KW inconistent array length for this multihit
  // Int_t mrtof_stop[100];
  Int_t mrtof_stop[10];
  Int_t mrtof_spill;
  Int_t mrtof_ts;

  // Hall probe
  Int_t hall_probe_adc[6];

private:
  // TFRSSortProc   *fxProc;     //! This is processor
  // TGo4FileSource *fxFileSrc;  //! This is file source

  ClassDef(TFRSSortEvent,1)
};

#endif //TFRSSORTEVENT_H
