#include "TFRSCalibrEvent.h"

#include "Riostream.h"

#include "TFRSCalibrProc.h"
#include "Go4EventServer/TGo4FileSource.h"

TFRSCalibrEvent::TFRSCalibrEvent() : TGo4EventElement("FRSCalibrEvent")//, fxProc(0), fxFileSrc(0) {
{ }

TFRSCalibrEvent::TFRSCalibrEvent(const char* name) : TGo4EventElement(name)//, fxProc(0), fxFileSrc(0) {
{ }

TFRSCalibrEvent::~TFRSCalibrEvent()
{ }

Int_t TFRSCalibrEvent::Init()
{
  Int_t rev=0;
  Clear();
  // if(CheckEventSource("TFRSCalibrProc")){
  //   fxProc = (TFRSCalibrProc*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by " << fxProc->GetName() << " ****" << std::endl;
  // } else
  // if(CheckEventSource("TGo4FileSource")) {
  //   fxFileSrc = (TGo4FileSource*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by File Source ****"<< std::endl;
  // } else rev=1;
  return rev;
}

// Int_t TFRSCalibrEvent::Fill()
// {
//    Clear();
//    if(fxProc) fxProc->FRSCalibr(this); else  // user event processing method
//    if(fxFileSrc)fxFileSrc->BuildEvent(this); // method from framework to restore event from file
//    return 0;
// }

void TFRSCalibrEvent::Clear(Option_t *t)
{
  EventFlag = 0;

  timestamp = 0;

  cptrig =0;
  ctpat =0;
  crtrig =0;
  cnbtrig =0;
  cmaxtrig =0;

  seconds = 0;
  tenthsecs = 0;
  hundrethsecs = 0;	//mik
  extraction_cycle = 1;

  freeTrig=0;
  acptTrig=0;

  for (int i=0;i<13;i++)
    {
      mw_xsum[i] = 0;
      mw_ysum[i] = 0;
      mw_x[i] = 0.;
      mw_y[i] = 0.;
      mw_wire[i] = 0.;
      b_mw_xsum[i] = kFALSE;
      b_mw_ysum[i] = kFALSE;
  }

  z_x_s2 = 0.;
  z_y_s2 = 0.;
  z_x_s4 = 0.;
  z_y_s4 = 0.;
  z_x_s8 = 0.;
  z_y_s8 = 0.;
  mw_sc21_x = 0.;
  mw_sc21_y = 0.;
  mw_sc22_x = 0.;
  mw_sc22_y = 0.;

  //----- focal plane variables -----//
  angle_x_s2 = -999. ;
  angle_y_s2 = -999. ;
  angle_x_s4 = -999. ;
  angle_y_s4 = -999. ;
  angle_x_s8 = -999. ;
  angle_y_s8 = -999. ;
  focx_s2 = -999. ;
  focy_s2 = -999. ;
  focx_s4 = -999. ;
  focy_s4 = -999. ;
  focx_s8 = -999. ;
  focy_s8 = -999. ;

  tpc_x_s2_foc_21_22=-999.;
  tpc_y_s2_foc_21_22=-999.;
  tpc_x_s2_foc_23_24=-999.;
  tpc_y_s2_foc_23_24=-999.;
  tpc_x_s2_foc_22_24=-999.;
  tpc_y_s2_foc_22_24=-999.;
  tpc_x_s4=-999.;
  tpc_y_s4=-999.;
  tpc_angle_x_s2_foc_21_22=-999.;
  tpc_angle_y_s2_foc_21_22=-999.;
  tpc_angle_x_s2_foc_23_24=-999.;
  tpc_angle_y_s2_foc_23_24=-999.;
  tpc_angle_x_s2_foc_22_24=-999.;
  tpc_angle_y_s2_foc_22_24=-999.;
  tpc_angle_x_s4=-999.;
  tpc_angle_y_s4=-999.;

  tpc21_22_sc21_x = -999.;      /* SC21                     */
  tpc23_24_sc21_x = -999.;      /* SC21                     */
  tpc22_24_sc21_x = -999.;      /* SC21                     */
  tpc21_22_sc21_y = -999.;      /* SC21                     */
  tpc23_24_sc21_y = -999.;      /* SC21                     */
  tpc22_24_sc21_y = -999.;      /* SC21                     */

  tpc21_22_sc22_x = -999.;      /* SC22                     */
  tpc23_24_sc22_x = -999.;      /* SC22                     */
  tpc22_24_sc22_x = -999.;      /* SC22                     */
  tpc21_22_sc22_y = -999.;      /* SC22                     */
  tpc23_24_sc22_y = -999.;      /* SC22                     */
  tpc22_24_sc22_y = -999.;      /* SC22                     */

  tpc21_22_s2target_x = -999.;      /* S2TARGET                     */
  tpc23_24_s2target_x = -999.;      /* S2TARGET                     */
  tpc22_24_s2target_x = -999.;      /* S2TARGET                     */
  tpc21_22_s2target_y = -999.;      /* S2TARGET                     */
  tpc23_24_s2target_y = -999.;      /* S2TARGET                     */
  tpc22_24_s2target_y = -999.;      /* S2TARGET                     */

  tpc_sc41_x = -999.;      /* SC41                     */
  tpc_sc41_y = -999.;      /* SC41 Y                   */
  tpc_sc42_x = -999.;      /* SC42                     */
  tpc_sc42_y = -999.;      /* tracked SC42 Y pos       */
  tpc_sc43_x = -999.;      /* SC43                     */
  tpc_sc43_y = -999.;      /* SC43 Y                   */

  tpc_music41_x = -999.;      /* MUSIC41 x                     */
  tpc_music41_y = -999.;      /* tracked MUSIC41 Y pos       */
  tpc_music42_x = -999.;      /* MUSIC42 x                     */
  tpc_music42_y = -999.;      /* tracked MUSIC42 Y pos       */
  tpc_music43_x = -999.;      /* MUSIC43 x                     */
  tpc_music43_y = -999.;      /* tracked MUSIC43 Y pos       */
  tpc_music44_x = -999.;      /* MUSIC44 x                     */
  tpc_music44_y = -999.;      /* tracked MUSIC44 Y pos       */
  tpc_s4target_x = -999.;     /* S4 target */
  tpc_s4target_y = -999.;     /* S4 target */
  
  //TPC Part
    for(int i=0; i<8; i++){
      tpc_timeref_s[i] = -999;
      b_tpc_timeref[i] = kFALSE;
    }

    for (int i = 0; i<7;i++){
        tpc_x[i] = 9999.9;
        tpc_y[i] = 9999.9;
        tpc_de[i] = 0.0;
        tpc_dx12[i]=-9999.;
        b_tpc_xy[i] = kFALSE;
        for (int j=0;j<4;j++){
      	  tpc_csum[i][j] =0;
      	  b_tpc_csum[i][j] = kFALSE;
      	  b_tpc_de[i]= kFALSE;
          tpc_dt_s[i][j] = -999;
          tpc_yraw[i][j] = -9999999;
        }
        for (int j=0;j<2;j++){
          tpc_lt_s[i][j] = -999;
          tpc_rt_s[i][j] = -999;
          tpc_xraw[i][j] = -9999999;
        }
    }

//LaBr3
labr_e_calib1=0;
labr_e_calib2=0;
labr_e_calib3=0;
labr_e_calib4=0;
labr_e_calib5=0;
labr_e_calib6=0;
labr_e_calib7=0;
labr_e_calib8=0;

for(int i = 0; i<8; i++){
	Labr_t_raw[i] = 0;
}

// Si
  si_e1=0;
  si_e2=0;
  si_e3=0;
  si_e4=0;
  si_e5=0;

  for(int i=0;i<32;i++)
  	{
	dssd_e_det1[i]=0;
	dssd_e_det2[i]=0;
	dssd_e_det3[i]=0;
	dssd_e_det4[i]=0;
	dssd_e_det5[i]=0;
	dssd_e_det6[i]=0;

	dssd_e_decay_det1[i]=0;
	dssd_e_decay_det2[i]=0;
	dssd_e_decay_det3[i]=0;
	dssd_e_decay_det4[i]=0;
	dssd_e_decay_det5[i]=0;
	dssd_e_decay_det6[i]=0;

	dssd_e_impl_det1[i]=0;
	dssd_e_impl_det2[i]=0;
	dssd_e_impl_det3[i]=0;
	dssd_e_impl_det4[i]=0;
	dssd_e_impl_det5[i]=0;
	dssd_e_impl_det6[i]=0;

	}
   dssd_maxenergy=0;
   dssd_maxindex=-1;

  dssd_xmaxenergy=0;
  dssd_xmaxindex=-1;
  dssd_ymaxenergy=0;
  dssd_ymaxindex=-1;
}

ClassImp(TFRSCalibrEvent)
