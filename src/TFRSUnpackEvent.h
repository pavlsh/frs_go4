#ifndef TFRSUNPACKEVENT_H
#define TFRSUNPACKEVENT_H


#include "TGo4EventElement.h"

#include "define.hh"
#include "foot/foot_unpack_event.hh"
#include "TFRSVftxSetting.h"

class TFRSUnpackProc;
class TGo4FileSource;

class TFRSUnpackEvent : public TGo4EventElement {
public:
  TFRSUnpackEvent();
  TFRSUnpackEvent(const char* name);
  virtual ~TFRSUnpackEvent();
  // /**
  //  * Method called by the event owner (analysis step) to fill the
  //  * event element from the set event source. Event source can
  //  * be the source of the analysis step (if this is a raw event)
  //  * or the event processor (if this is a reduced event).
  //  */
  // virtual Int_t Fill();

  /**
   * Method called by the event owner (analysis step) to clear the
   * event element.
   */
  virtual void Clear(Option_t *t="");

  /**
   * Method called to clear the vftx.
   */
  virtual void ClearVftx();

  // Clear MTDC-32
  virtual void Clear_MTDC_32();

  // Clear MQDC-32
  virtual void Clear_MQDC_32();

  virtual Int_t Init();

//  Int_t EventFlag;

  //------------
  UInt_t vme_main[21][32];          // Main crate
  UInt_t timestamp_main[32];        // Time stamp in main crate
  UInt_t scaler_main[32];           // VME scaler in main crate
  UInt_t scaler_mrtof[32];           // VME scaler in mrtof crate
  UInt_t nhit_v1290_main[32][2];    // multi-hit TDC in main crate [ch][leading or trailing]
  UInt_t nhit_v1190_mrtof[32][2];    // multi-hit TDC in mrtof crate [ch][leading or trailing]
  UInt_t leading_v1290_main[32][10];     // leading timing V1290 in main crate
  UInt_t trailing_v1290_main[32][10];    // trailing timing V1290 in main crate
  UInt_t leading_v1190_mrtof[32][10];     // leading timing V1190 in mrtof crate
  UInt_t trailing_v1190_mrtof[32][10];    // trailing timing V1190 in mrtof crate
  UInt_t mrtof_ts; //Time stamp from Vulom in mrtof crate
  UInt_t tpat_main[32];

  UInt_t vme_frs[21][32];           // FRS crate
  UInt_t scaler_frs[32];            // VME scaler in frs crate

  UInt_t vme_tof[21][32];           // TOF crate

  //TPC crate for 2021 beam time (Yoshiki Tanaka)
  //at start of 2021, we use only 1 VME crate for whole TPC(S2/S3/S4)
  //I use vme_tpcs2[][] and vme_v1190_tpcs2[][].
  //I still keep vme_tpcs4[][] in case we put 2 crates in future
  UInt_t vme_tpcs2[21][32];         // TPC crate (S2 TPC)
  UInt_t leading_v1190_tpcs2[128][64];         // TPC crate (S2 TPC)
  UInt_t nhit_v1190_tpcs2[128];         // TPC crate (S2 TPC)
  UInt_t vme_tpcs4[21][32];         // TPC crate (S4/S3 TPC)

  UInt_t vme_trmu_adc[16];              // Travel Music crate
  UInt_t vme_trmu_tdc[16];              // Travel Music crate
  UInt_t vme_trmu_trigger[2];           // Travel Music crate
  UInt_t vme_actstop[21][32];       //active stopper
	bool v1190_is_bad;
	uint32_t v1190_bad_multip[128];
  //for s530 fission crate by J.Zhao 20210317
  UInt_t scaler_s530[32];           // VME scaler SIS-3820 in s530 crate

  UInt_t leading_v1290_s530[32][10];    // leading timing, CAEN v1290 TDC in s530 crate
  UInt_t trailing_v1290_s530[32][10];   // trailing timing, CAEN v1290 TDC in s530 crate
  UInt_t nhit_v1290_s530[32][2];        // multi-hit of leading and trailing timing 

  UInt_t lqdc_mdpp_s530[16][10];       // Long gate qdc, MDPP-16-QDC in s530 crate
  UInt_t sqdc_mdpp_s530[16][10];       // Short gate qdc, MDPP-16-QDC in s530 crate
  UInt_t tdc_mdpp_s530[16][10];        // TDC timing, MDPP-16-QDC in s530 crate
  UInt_t nhit_mdpp_s530[16][3];        // multi-hit of MDPP-16 TDC, LQDC, SQDC in s530 crate
  UInt_t mdpp_trig_s530[2];            // T0, T1 trigger of MDPP-16-QDC 
  //end of s530 fission crate
  uint64_t frs_wr;
  uint64_t travmus_wr;

//------------
  Int_t         qlength;         /* From event header     */
  Int_t         qtype;           /*                       */
  Int_t         qsubtype;        /*                       */
  Int_t         qdummy;          /*                       */
  Int_t         qtrigger;        /*qtrigger is a readout trigger   */
  Int_t         qevent_nr;       /*                       */

//for time stamping buisiness
  Int_t         wrid ;           

// special nurdlib
  Int_t 			 utpat;  //! //This is to pass down the line
  Int_t 			 uphystrig; //! // to have something else than TPC
  Int_t 			 unbtrig; //! // number of triggers in utpat
  Int_t 			 umaxtrig; //! // second trigger in utpat

//Phystrig is build based on tpat, but trigger priority first tpat
//trigger box, sc41..... etc.


//
  // --- ------------- --- //
  // --- TDC/GSI/VFTX2 --- //
  // --- ------------- --- //
  Int_t vftx_leading_cc[VFTX_N][VFTX_MAX_CHN][VFTX_MAX_HITS];
  Int_t vftx_leading_ft[VFTX_N][VFTX_MAX_CHN][VFTX_MAX_HITS];
  Double_t vftx_leading_time[VFTX_N][VFTX_MAX_CHN][VFTX_MAX_HITS];
  Int_t vftx_trailing_cc[VFTX_N][VFTX_MAX_CHN][VFTX_MAX_HITS];
  Int_t vftx_trailing_ft[VFTX_N][VFTX_MAX_CHN][VFTX_MAX_HITS];
  Double_t vftx_trailing_time[VFTX_N][VFTX_MAX_CHN][VFTX_MAX_HITS];
  Int_t vftx_mult[VFTX_N][VFTX_MAX_CHN];
  Int_t vftx_lead_mult[VFTX_N][VFTX_MAX_CHN];
  Int_t vftx_trail_mult[VFTX_N][VFTX_MAX_CHN];

  //-----------------------------------------------------
  // MTDC
  //-----------------------------------------------------

  Int_t mtdc32_dt_trg0_raw[32];
  Int_t mtdc32_dt_trg1_raw[32];


  //-----------------------------------------------------
  // MQDC
  //-----------------------------------------------------

  Int_t mqdc32_raw[32];
  FOOT_UNPACK_STRUCTURES_DECL

private:
  //      TFRSUnpackProc *fxProc;     //! This is processor
  //      TGo4FileSource *fxFileSrc;  //! This is file source

  ClassDef(TFRSUnpackEvent,1)
};
#endif //TFRSUNPACKEVENT_H
