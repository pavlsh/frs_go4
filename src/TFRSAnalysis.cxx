#include "TFRSAnalysis.h"

#include "Riostream.h"

#include "TH1.h"
#include "TH2.h"

#include "Go4EventServer/TGo4MbsEvent.h"
#include "Go4ConditionsBase/TGo4WinCond.h"
#include "Go4ConditionsBase/TGo4PolyCond.h"

#include "Go4EventServer/TGo4StepFactory.h"

#include "TFRSParameter.h"

#include "TFRSCalibrProc.h"
#include "TFRSAnlProc.h"

#include <sstream>
#include <fstream>

//***********************************************************
TFRSAnalysis::TFRSAnalysis()
  : fMbsEvent(0), fSize(0), fEvents(0), fLastEvent(0)
{
  std::cout << "Wrong constructor TFRSAnalysis()!" << std::endl;
}

//TCanvas* gPrintCanvas = 0;

//***********************************************************
// this constructor is used
TFRSAnalysis::TFRSAnalysis(const char* lmd,
			   const char* asf,
                           const char* out1,
                           const char* out2,
                           const char* out3,
                           const char* out4,
                           const char* out5)
  : fMbsEvent(0), fSize(0), fEvents(0), fLastEvent(0) {


  //   gPrintCanvas = new TCanvas("can1","can1 title",500,500);



  // lmd: input file name (*.lmd)
  // out1: output file name of first analysis step  (*.root)
  // out2: output file name of second analysis step (*.root)
  std::cout << "**** TFRSAnalysis: Create ****" << std::endl;

  // Enable autosave. Autosave is done when analysis loop is stopped
  // or intervall os over.
  SetAutoSave(kFALSE);   // no autosave
  SetAutoSaveFile(asf);   // optional
  SetAutoSaveInterval(100000); // events , 0 = termination of event loop

  // the step definitions can be changed in the GUI
  // first step definitions:
  // the name of the step can be used later to get event objects
  //TFRSUnpackFact*         factory1 = new TFRSUnpackFact("Unpack-factory");

  TGo4StepFactory* factory1 = new TGo4StepFactory("Unpack-factory");
  factory1->DefEventProcessor("FRSUnpackProc","TFRSUnpackProc");
  factory1->DefOutputEvent("FRSUnpackEvent", "TFRSUnpackEvent");

  //  TGo4MbsFileParameter*   source1  = new TGo4MbsFileParameter(lmd); // lmd file
  // TGo4MbsFileParameter*   source1  = new TGo4MbsFileParameter("/d/frs02/exp/er_2019/general/re168cent_slit_1121.lmd"); //only for development
  TGo4RevServParameter *source1 = new TGo4RevServParameter("lxg1268"); //Remote Event Server
  //  TGo4MbsStreamParameter*   source1  = new TGo4MbsStreamParameter("lxg1266:8000");
  //StepMbsStreamSource("Analysis", "lxg1266:8000", 1);
  TGo4FileStoreParameter* store1   = new TGo4FileStoreParameter(out1);
  TGo4AnalysisStep*       step1    = new TGo4AnalysisStep("Unpack",factory1,source1,store1,0);
  store1->SetOverwriteMode(kTRUE);
  step1->SetSourceEnabled(kTRUE);
  step1->SetStoreEnabled(kFALSE);  // dissable output
  step1->SetProcessEnabled(kTRUE);
  step1->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step1);


  // second step definitions:
  // the name of the step can be used later to get event objects
  TGo4StepFactory* factory2  = new TGo4StepFactory("Sort-factory");
  factory2->DefInputEvent("FRSUnpackEvent", "TFRSUnpackEvent");
  factory2->DefEventProcessor("FRSSortProc","TFRSSortProc");
  factory2->DefOutputEvent("FRSSortEvent", "TFRSSortEvent");

  TGo4FileSourceParameter* source2  = new TGo4FileSourceParameter(out1);
  TGo4FileStoreParameter*  store2   = new TGo4FileStoreParameter(out2);
  TGo4AnalysisStep*       step2    = new TGo4AnalysisStep("Sort",factory2,source2,store2,0);
  store2->SetOverwriteMode(kTRUE);
  step2->SetSourceEnabled(kFALSE); // disable file input
  step2->SetStoreEnabled(kFALSE);  // dissable output
  step2->SetProcessEnabled(kTRUE);
  step2->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step2);


  // third step definitions:
  // the name of the step can be used later to get event objects
  TGo4StepFactory* factory3  = new TGo4StepFactory("Calibr-factory");
  factory3->DefInputEvent("FRSSortEvent", "TFRSSortEvent");
  factory3->DefEventProcessor("FRSCalibrProc","TFRSCalibrProc");
  factory3->DefOutputEvent("FRSCalibrEvent", "TFRSCalibrEvent");
  TGo4FileSourceParameter* source3  = new TGo4FileSourceParameter(out2);
  TGo4FileStoreParameter*  store3   = new TGo4FileStoreParameter(out3);
  TGo4AnalysisStep*       step3    = new TGo4AnalysisStep("Calibr",factory3,source3,store3,0);
  store3->SetOverwriteMode(kTRUE);
  step3->SetSourceEnabled(kFALSE); // disable file input
  step3->SetStoreEnabled(kFALSE);  // dissable output
  step3->SetProcessEnabled(kTRUE);
  step3->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step3);


  // forth step definition
  // step definitions: FRS - Analysis
  TGo4StepFactory* factory4 = new TGo4StepFactory("Anlalysis-factory");
  factory4->DefInputEvent("FRSCalibrEvent", "TFRSCalibrEvent");
  factory4->DefEventProcessor("FRSAnlProc","TFRSAnlProc");
  factory4->DefOutputEvent("FRSAnlEvent", "TFRSAnlEvent");
  TGo4FileSourceParameter* source4  = new TGo4FileSourceParameter(out3);
  TGo4FileStoreParameter*  store4   = new TGo4FileStoreParameter(out4);
  TGo4AnalysisStep*        step4    = new   TGo4AnalysisStep("Analysis",factory4,source4,store4,0);
  store4->SetOverwriteMode(kTRUE);
  step4->SetSourceEnabled(kFALSE); // disable file input
  step4->SetStoreEnabled(kFALSE);  // disable output
  step4->SetProcessEnabled(kTRUE);
  step4->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step4);


  // fifth step definition
  // step definitions: FRS - Analysis
  TGo4StepFactory* factory5 = new TGo4StepFactory("User-factory");
  factory5->DefInputEvent("FRSAnlEvent", "TFRSAnlEvent");
  factory5->DefEventProcessor("FRSUserProc","TFRSUserProc");
  factory5->DefOutputEvent("FRSUserEvent", "TFRSUserEvent");
  TGo4FileSourceParameter* source5  = new TGo4FileSourceParameter(out4);
  TGo4FileStoreParameter*  store5   = new TGo4FileStoreParameter(out5);
  TGo4AnalysisStep*        step5    = new TGo4AnalysisStep("User",factory5,source5,store5,0);
  store5->SetOverwriteMode(kTRUE);
  step5->SetSourceEnabled(kFALSE); // disable file input
  step5->SetStoreEnabled(kFALSE);  // disable output
  step5->SetProcessEnabled(kFALSE);
  step5->SetErrorStopEnabled(kFALSE);
  AddAnalysisStep(step5);


  //////////////// Parameters //////////////////////////
  // At this point, autosave file has not yet been read!
  // Therefore parameter values set here will be overwritten
  // if an autosave file is there.
  fFRSPar = new TFRSParameter("FRSPar");
  AddParameter(fFRSPar);

  fMWPar = new TMWParameter("MWPar");
  AddParameter(fMWPar);

  fMUSICPar = new TMUSICParameter("MUSICPar");
  AddParameter(fMUSICPar);

  fTPCPar = new TTPCParameter("TPCPar");
  AddParameter(fTPCPar);

  fSCIPar = new TSCIParameter("SCIPar");
  AddParameter(fSCIPar);

  fIDPar = new TIDParameter("IDPar");
  AddParameter(fIDPar);

  fLABRPar = new TLABRParameter("LABRPar");
  AddParameter(fLABRPar);

  fSIPar = new TSIParameter("SIPar");
  AddParameter(fSIPar);

  fRangePar = new TRangeParameter("RangePar");
  AddParameter(fRangePar);

//  ModPar = new TModParameter("ModPar");
//  AddParameter(ModPar);

  MRtofPar = new TMRTOFMSParameter("MRTOFMSPar");
  AddParameter(MRtofPar);
}

Bool_t TFRSAnalysis::InitEventClasses()
{
  Bool_t res = TGo4Analysis::InitEventClasses();
  std::string nameExperiment;
  std::ifstream ifs ( "config.txt" );
  if(ifs.is_open())
    {
      const std::string CommentSymbol("#");
      const std::string ExpNameSymbol("NameExperiment");

      std::string temp_line;
      while(std::getline(ifs,temp_line))
	{
	  std::stringstream stream(temp_line);
	  std::string testComment(stream.str());
	  std::size_t it_comment = testComment.find(CommentSymbol);
	  if(it_comment!=std::string::npos)
	    {
	      //std::cout<<"!> Skip comment"<<temp_line<<std::endl;
	      continue;
	    }
	  std::string key, value;
	  stream >> key >> value ;
	  if(key==ExpNameSymbol)
	    nameExperiment=value;
	}
    }
  std::string nameSetupFile ("setup/setup_");
  nameSetupFile+=nameExperiment;
  nameSetupFile+=".C";

  std::cout << "TFRSAnalysis::Call "<< nameSetupFile<<" script" << std::endl;

  std::ifstream testingFileExist(nameSetupFile.c_str());
  if(testingFileExist.good()==false)
    {
      std::cout<<" E> !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<std::endl;
      std::cout<<" E> Setup file "<<nameSetupFile<<" not found ! Please look to directory ./setup/ if it exits or set proper name of experiment in config.txt"<<std::endl;
      std::cout<<" E> !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<std::endl;

      std::exit(-1);
    }

  std::string processL(".x ");
  processL += nameSetupFile;
  gROOT->ProcessLine(processL.c_str());

  //std::string processL(".L ");
  // processL += nameSetupFile;
  //gROOT->ProcessLine(processL.c_str());
  //gROOT->ProcessLine("setup()");

  //  ModPar->setMap();
  SetupPolyCond("cID_ZAoQ(0)", 5, fIDPar->ID_Z_AoverQ[0]);
  SetupPolyCond("cID_ZAoQ(1)", 5, fIDPar->ID_Z_AoverQ[1]);
  SetupPolyCond("cID_ZAoQ(2)", 5, fIDPar->ID_Z_AoverQ[2]);
  SetupPolyCond("cID_ZAoQ(3)", 5, fIDPar->ID_Z_AoverQ[3]);
  SetupPolyCond("cID_ZAoQ(4)", 5, fIDPar->ID_Z_AoverQ[4]);

  SetupPolyCond("cID_x2AoQ(0)", 5, fIDPar->ID_x2AoverQ[0]);
  SetupPolyCond("cID_x2AoQ(1)", 5, fIDPar->ID_x2AoverQ[1]);
  SetupPolyCond("cID_x2AoQ(2)", 5, fIDPar->ID_x2AoverQ[2]);
  SetupPolyCond("cID_x2AoQ(3)", 5, fIDPar->ID_x2AoverQ[3]);
  SetupPolyCond("cID_x2AoQ(4)", 5, fIDPar->ID_x2AoverQ[4]);

  SetupPolyCond("cID_x4AoQ(0)", 5, fIDPar->ID_x4AoverQ[0]);
  SetupPolyCond("cID_x4AoQ(1)", 5, fIDPar->ID_x4AoverQ[1]);
  SetupPolyCond("cID_x4AoQ(2)", 5, fIDPar->ID_x4AoverQ[2]);
  SetupPolyCond("cID_x4AoQ(3)", 5, fIDPar->ID_x4AoverQ[3]);
  SetupPolyCond("cID_x4AoQ(4)", 5, fIDPar->ID_x4AoverQ[4]);

  //-----Reset Histogram ranges based on Parameters from setup.C----
  SetupH1("ID_AoQ_S2S4",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q S2-S4");
  SetupH1("ID_AoQ_corr_S2S4",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q S2-S4");
  SetupH1("ID_Z",1200,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z from MUSIC41");
  SetupH1("ID_Z2",1200,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z from MUSIC42");
  SetupH1("ID_Z3",1200,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z from MUSIC43");
  SetupH1("ID_Z4",1200,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z from MUSIC44");
  SetupH2("ID_DBrho_AoQ",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,500,0.,2.5,"A/Q s2-s4", "Brho(ta-s2) - Brho(s2-s4) [Tm]");
  SetupH2("ID_x2AoQ", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 200,-100.,100.,"A/Q s2-s4", "X at S2 [mm]");
  SetupH2("ID_Z_AoQ", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q s2-s4", "Z from MUSIC41");
  SetupH2("ID_Z_AoQcorr", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q s2-s4 with correction", "Z from MUSIC41");
  SetupH2("ID_Z_AoQ_zsame (Z1==Z2)", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q s2-s4", "Z from MUSIC41");
  SetupH2("ID_Z2_AoQ", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q s2-s4", "Z from MUSIC42");
  SetupH2("ID_Z2_AoQcorr", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q s2-s4 with correction", "Z from MUSIC42");
  SetupH2("ID_Z_Z2", 600,fIDPar->min_z_plot,fIDPar->max_z_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z", "Z2");
  SetupH2("ID_Z_Z3", 600,fIDPar->min_z_plot,fIDPar->max_z_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z", "Z3 'MUSIC'");
  SetupH2("ID_Z_Z4", 600,fIDPar->min_z_plot,fIDPar->max_z_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z", "Z4 'MUSIC'");
  SetupH2("ID_Z2_Z3", 600,fIDPar->min_z_plot,fIDPar->max_z_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z2", "Z3 'MUSIC'");
  SetupH2("ID_Z2_Z4", 600,fIDPar->min_z_plot,fIDPar->max_z_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z2", "Z4 'MUSIC'");
  SetupH2("ID_Z3_Z4", 600,fIDPar->min_z_plot,fIDPar->max_z_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z3", "Z4 'MUSIC'");
  SetupH2("ID_Z_Sc21E", 600, fIDPar->min_z_plot,fIDPar->max_z_plot ,400,0,4000.,"Z from MUSIC41", "sqrt(Sc21_L*sC21_R)");
  SetupH2("ID_x4AoQ", 100,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 100,-100.,100.,"A/Q s2-s4", "X at S4 [mm]");
  SetupH2("ID_x4z", 600,fIDPar->min_z_plot,fIDPar->max_z_plot, 200,-100.,100., "Z from MUSIC41", "X at S4 [mm]");
  SetupH2("ID_x2z", 600,fIDPar->min_z_plot,fIDPar->max_z_plot, 200,-100.,100., "Z from MUSIC41", "X at S2 [mm]");
  SetupH1("ID_Z_sc81" ,1200,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z from Sc81");
  SetupH1("ID_Z_s2tpc",1200,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z from S2TPC");
  SetupH1("ID_AoQ_S2S8",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q S2-S8");
  SetupH2("ID_Z(sc81)_AoQ(s2s8)", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q s2-s8", "Z from sc81");
  SetupH2("ID_Z(s2tpc)_AoQ(s2s8)", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q s2-s8", "Z from s2tpc");
  SetupH2("ID_dE(sc81)_AoQ(s2s8)", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 400,1,2000,"A/Q s2-s8", "Z from sc81");
  SetupH2("ID_dE(s2tpc)_AoQ(s2s8)", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 400,1,4000,"A/Q s2-s8", "Z from s2tpc");

  for(int i=0;i<8;i++){
    SetupH1(Form("MUSIC1_E(%d)",i),4096,0,fMUSICPar->max_adc_music1,Form("dE MUSIC1(%d)",i));
    SetupH1(Form("MUSIC1_T(%d)",i),4096,0,fMUSICPar->max_tdc_music1,Form("T MUSIC1(%d)",i));
    SetupH1(Form("MUSIC2_E(%d)",i),4096,0,fMUSICPar->max_adc_music2,Form("dE MUSIC2(%d)",i));
    SetupH1(Form("MUSIC2_T(%d)",i),4096,0,fMUSICPar->max_tdc_music2,Form("T MUSIC2(%d)",i));
    SetupH1(Form("MUSIC3_E(%d)",i),4096,0,fMUSICPar->max_adc_music3,Form("dE MUSIC3(%d)",i));
    SetupH1(Form("MUSIC3_T(%d)",i),4096,0,fMUSICPar->max_tdc_music3,Form("T MUSIC3(%d)",i));
    SetupH1(Form("MUSIC4_E(%d)",i),4096,0,fMUSICPar->max_adc_music4,Form("dE MUSIC4(%d)",i));
    SetupH1(Form("MUSIC4_T(%d)",i),4096,0,fMUSICPar->max_tdc_music4,Form("T MUSIC4(%d)",i));
    if( fMUSICPar->exclude_de1_adc_channel[i]){//true means exclude
      SetupWinCond(Form("cMusic1_E(%d)",i), 10, 10);
    }else{
      SetupWinCond(Form("cMusic1_E(%d)",i), 10, fMUSICPar->max_adc_music1-10);
    }
    SetupWinCond(Form("cMusic1_T(%d)",i), 10, fMUSICPar->max_tdc_music1-10);
    if( fMUSICPar->exclude_de2_adc_channel[i]){//true means exclude
      SetupWinCond(Form("cMusic2_E(%d)",i), 10, 10);
    }else{
      SetupWinCond(Form("cMusic2_E(%d)",i), 10, fMUSICPar->max_adc_music2-10);
    }
    SetupWinCond(Form("cMusic2_T(%d)",i), 10, fMUSICPar->max_tdc_music2-10);
    if( fMUSICPar->exclude_de3_adc_channel[i]){//true means exclude
      SetupWinCond(Form("cMusic3_E(%d)",i), 10, 10);
    }else{
      SetupWinCond(Form("cMusic3_E(%d)",i), 10, fMUSICPar->max_adc_music3-10);
    }
    SetupWinCond(Form("cMusic4_T(%d)",i), 10, fMUSICPar->max_tdc_music4-10);
    if( fMUSICPar->exclude_de4_adc_channel[i]){//true means exclude
      SetupWinCond(Form("cMusic4_E(%d)",i), 10, 10);
    }else{
      SetupWinCond(Form("cMusic4_E(%d)",i), 10, fMUSICPar->max_adc_music4-10);
    }
    SetupWinCond(Form("cMusic4_T(%d)",i), 10, fMUSICPar->max_tdc_music4-10);
  }
  SetupH1("MUSIC1_dE",4096,0.0,fMUSICPar->max_adc_music1,"Average dE MUSIC41 (root)");
  SetupH1("MUSIC2_dE",4096,0.0,fMUSICPar->max_adc_music2,"Average dE MUSIC42 (root)");
  SetupH1("MUSIC3_dE",4096,0.0,fMUSICPar->max_adc_music3,"Average dE MUSIC43 (root)");
  SetupH1("MUSIC4_dE",4096,0.0,fMUSICPar->max_adc_music4,"Average dE MUSIC44 (root)");
  SetupH2("dE1_dE2",1024,0,fMUSICPar->max_adc_music1,1024,0,fMUSICPar->max_adc_music2,"dE MUSIC41","dE MUSIC42");
  SetupH2("dE1_dE3",1024,0,fMUSICPar->max_adc_music1,1024,0,fMUSICPar->max_adc_music3,"dE MUSIC41","dE MUSIC43");
  SetupH2("dE1_dE4",1024,0,fMUSICPar->max_adc_music1,1024,0,fMUSICPar->max_adc_music4,"dE MUSIC41","dE MUSIC44");
  SetupH2("dE2_dE3",1024,0,fMUSICPar->max_adc_music2,1024,0,fMUSICPar->max_adc_music3,"dE MUSIC42","dE MUSIC43");
  SetupH2("dE2_dE4",1024,0,fMUSICPar->max_adc_music2,1024,0,fMUSICPar->max_adc_music4,"dE MUSIC42","dE MUSIC44");
  SetupH2("dE3_dE4",1024,0,fMUSICPar->max_adc_music3,1024,0,fMUSICPar->max_adc_music4,"dE MUSIC43","dE MUSIC44");
  SetupH2("MUSIC1_dE_x",100,-100,100,200,0,fMUSICPar->max_adc_music1,"Average x position in MUSIC41","dE MUSIC41 [channels]");
  SetupH2("MUSIC2_dE_x",100,-100,100,200,0,fMUSICPar->max_adc_music2,"Average x position in MUSIC42","dE MUSIC42 [channels]");
  SetupH2("MUSIC3_dE_x",100,-100,100,200,0,fMUSICPar->max_adc_music3,"Average x position in MUSIC43","dE MUSIC43 [channels]");
  SetupH2("MUSIC4_dE_x",100,-100,100,200,0,fMUSICPar->max_adc_music4,"Average x position in MUSIC44","dE MUSIC44 [channels]");
  SetupH1("MUSIC1_dECOR",4096,0,fMUSICPar->max_adc_music1,"dE MUSIC41 corrected for position (ch)");
  SetupH1("MUSIC2_dECOR",4096,0,fMUSICPar->max_adc_music2,"dE MUSIC42 corrected for position (ch)");
  SetupH1("MUSIC3_dECOR",4096,0,fMUSICPar->max_adc_music3,"dE MUSIC43 corrected for position (ch)");
  SetupH1("MUSIC4_dECOR",4096,0,fMUSICPar->max_adc_music4,"dE MUSIC44 corrected for position (ch)");
  SetupH2("MUSIC1_dECOR_x",100,-100,+100,200,0,fMUSICPar->max_adc_music1,"Average x position in MUSIC41", "dE MUSIC41 corrected for position  (ch)");
  SetupH2("MUSIC2_dECOR_x",100,-100,+100,200,0,fMUSICPar->max_adc_music2,"Average x position in MUSIC42", "dE MUSIC42 corrected for position  (ch)");
  SetupH2("MUSIC3_dECOR_x",100,-100,+100,200,0,fMUSICPar->max_adc_music3,"Average x position in MUSIC43", "dE MUSIC43 corrected for position  (ch)");
  SetupH2("MUSIC4_dECOR_x",100,-100,+100,200,0,fMUSICPar->max_adc_music4,"Average x position in MUSIC44", "dE MUSIC44 corrected for position  (ch)");
  for(int igate=0; igate<15;igate++){
    char gate_name[256];
    if(0 <=igate && igate<5){  sprintf(gate_name,"cID_ZAoQ(%d)",igate); }
    if(5 <=igate && igate<10){ sprintf(gate_name,"cID_x2AoQ(%d)",igate-5); }
    if(10<=igate && igate<15){ sprintf(gate_name,"cID_x4AoQ(%d)",igate-10); }
    for(int ch=0; ch<8; ch++){
      SetupH1(Form("MUSIC1dE_%d_%s",ch,gate_name),4096,0,fMUSICPar->max_adc_music1,"MUSIC dE [channel]");
      SetupH1(Form("MUSIC2dE_%d_%s",ch,gate_name),4096,0,fMUSICPar->max_adc_music2,"MUSIC dE [channel]");
      SetupH1(Form("MUSIC3dE_%d_%s",ch,gate_name),4096,0,fMUSICPar->max_adc_music3,"MUSIC dE [channel]");
      SetupH1(Form("MUSIC4dE_%d_%s",ch,gate_name),4096,0,fMUSICPar->max_adc_music4,"MUSIC dE [channel]");
    }
    SetupH2(Form("total_range_vs_z_%s",gate_name), 1000,0,12000, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Range in Al [mg/cm^2]", "Z");
    SetupH2(Form("range_post_degrader_vs_z_%s",gate_name), 1000,0,12000, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Range in Al [mg/cm^2]", "Z");
    SetupH2(Form("total_range_corr_vs_z_%s",gate_name), 1000,0,12000, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Range in Al [mg/cm^2]", "Z");
    SetupH2(Form("range_post_degrader_corr_vs_z_%s",gate_name), 1000,0,12000, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Range in Al [mg/cm^2]", "Z");
  }
  SetupH2("ID_Z_AoQ_Range", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q s2-s4", "Z from MUSIC41");
  SetupH2("ID_Z_AoQcorr_Range", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q s2-s4 with correction", "Z from MUSIC41");
  SetupH2("ID_Z_AoQ_Range_deg", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q s2-s4", "Z from MUSIC41");
  SetupH2("ID_Z_AoQcorr_Range_deg", 600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot, 600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q s2-s4 with correction", "Z from MUSIC41");
  
  //MHTDC S2-S8
  SetupH1("AoQ_S2-S8",1000,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"AoQ S2-S8");
  SetupH2("aoq_vs_beta_S2-S8",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,0.6,1.2,"aoq S2-S8","beta S2-S8");
  SetupH2("dE(s2tpc)_vs_aoq(S2-S8)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,500,0,3000,"AoQ S2-S8","dE(s2tpc)");
  SetupH1("Z(s2tpc)",600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z(s2tpc)");
  SetupH2("Z(s2tpc)_vs_aoq(S2-S8)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"AoQ S2-S8","Z(s2tpc)");
  SetupH2("dE(sc81)_vs_aoq(S2-S8)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,500,0,3000,"AoQ S2-S8","dE(sc81)");
  SetupH1("Z(sc81)",600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z(sc81)");
  SetupH2("Z(sc81)_vs_aoq(S2-S8)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"AoQ S2-S8","Z(sc81)");

  //MHTDC S2-S4
  SetupH1("AoQ_S2-S4",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"AoQ S2-S4");
  SetupH2("aoq_vs_beta_S2-S4",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,0.6,1.2,"aoq S2-S4","beta S2-S4");
  SetupH2("dE(music41)_vs_aoq(S2-S4)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,500,0,3000,"AoQ S2-S4","dE(music41)");
  SetupH1("Z(music41)",1200,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z(music41)");
  SetupH2("Z(music41)_vs_aoq(S2-S4)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"AoQ S2-S4","Z(music41)");
  SetupH2("Z(music41)_vs_aoq_corr(S2-S4)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"AoQ_corr S2-S4","Z(music41)");
  SetupH2("dE(music42)_vs_aoq(S2-S4)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,500,0,3000,"AoQ S2-S4","dE(music42)");
  SetupH1("Z(music42)",1200,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z(music42)");
  SetupH2("Z(music42)_vs_aoq(S2-S4)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"AoQ S2-S4","Z(music42)");
  SetupH2("Z(music42)_vs_aoq_corr(S2-S4)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"AoQ_corr S2-S4","Z(music42)");

  //MHTDC S2-S8
  SetupH1("AoQ_S2-HTM",1000,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"AoQ S2-HTM");
  SetupH2("aoq_vs_beta_S2-HTM",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,0.6,1.2,"aoq S2-HTM","beta S2-HTM");
  SetupH2("dE(HTM)_vs_aoq(S2-HTM)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,500,0,3000,"AoQ S2-HTM","dE(HTM)");
  SetupH1("Z(HTM)",600,fIDPar->min_z_plot,fIDPar->max_z_plot,"Z(HTM)");
  SetupH2("Z(HTM)_vs_aoq(S2-HTM)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"AoQ S2-HTM","Z(HTM)");
  SetupH2("Z(HTM)_vs_aoq_corr(S2-HTM)",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"AoQ_corr S2-HTM","Z(HTM)");

  //VFTX
  SetupH1("ID_AoQ_S21_S41",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q SCI21-SCI41");
  SetupH1("ID_AoQ_S21_S41_mhit",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q SCI21-SCI41");
  SetupH1("ID_AoQcorr_S21_S41",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q corr SCI21-SCI41");
  SetupH1("ID_AoQcorr_S21_S41_mhit",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q corr SCI21-SCI41");
  SetupH2("ID_x2_AoQ_S21_S41",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,200,-100,100,"A/Q corr SCI21-SCI41","X at S2 [mm]");
  SetupH2("ID_x2_AoQ_S21_S41_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,200,-100,100,"A/Q corr SCI21-SCI41","X at S2 [mm]");
  SetupH2("ID_Z1_AoQ_S21_S41",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI21-SCI41","Z(music41)");
  SetupH2("ID_Z1_AoQ_S21_S41_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI21-SCI41","Z(music41)");
  SetupH2("ID_Z1_AoQcorr_S21_S41",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI21-SCI41","Z(music41)");
  SetupH2("ID_Z1_AoQcorr_S21_S41_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI21-SCI41","Z(music41)");
  SetupH2("ID_Z2_AoQ_S21_S41",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI21-SCI41","Z(music42)");
  SetupH2("ID_Z2_AoQ_S21_S41_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI21-SCI41","Z(music42)");
  SetupH2("ID_Z2_AoQcorr_S21_S41",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI21-SCI41","Z(music42)");
  SetupH2("ID_Z2_AoQcorr_S21_S41_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI21-SCI41","Z(music42)");

  SetupH1("ID_AoQ_S22_S41",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q SCI22-SCI41");
  SetupH1("ID_AoQ_S22_S41_mhit",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q SCI22-SCI41");
  SetupH1("ID_AoQcorr_S22_S41",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q corr SCI22-SCI41");
  SetupH1("ID_AoQcorr_S22_S41_mhit",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q corr SCI22-SCI41");
  SetupH2("ID_x2_AoQ_S22_S41",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,200,-100,100,"A/Q corr SCI22-SCI41","X at S2 [mm]");
  SetupH2("ID_x2_AoQ_S22_S41_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,200,-100,100,"A/Q corr SCI22-SCI41","X at S2 [mm]");
  SetupH2("ID_Z1_AoQ_S22_S41",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI22-SCI41","Z(music41)");
  SetupH2("ID_Z1_AoQ_S22_S41_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI22-SCI41","Z(music41)");
  SetupH2("ID_Z1_AoQcorr_S22_S41",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI22-SCI41","Z(music41)");
  SetupH2("ID_Z1_AoQcorr_S22_S41_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI22-SCI41","Z(music41)");
  SetupH2("ID_Z2_AoQ_S22_S41",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI22-SCI41","Z(music42)");
  SetupH2("ID_Z2_AoQ_S22_S41_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI22-SCI41","Z(music42)");
  SetupH2("ID_Z2_AoQcorr_S22_S41",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI22-SCI41","Z(music42)");
  SetupH2("ID_Z2_AoQcorr_S22_S41_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI22-SCI41","Z(music42)");

  SetupH1("ID_AoQ_S21_S42",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q SCI21-SCI42");
  SetupH1("ID_AoQ_S21_S42_mhit",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q SCI21-SCI42");
  SetupH1("ID_AoQcorr_S21_S42",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q corr SCI21-SCI42");
  SetupH1("ID_AoQcorr_S21_S42_mhit",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q corr SCI21-SCI42");
  SetupH2("ID_x2_AoQ_S21_S42",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,200,-100,100,"A/Q corr SCI21-SCI42","X at S2 [mm]");
  SetupH2("ID_x2_AoQ_S21_S42_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,200,-100,100,"A/Q corr SCI21-SCI42","X at S2 [mm]");
  SetupH2("ID_Z1_AoQ_S21_S42",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI21-SCI42","Z(music42)");
  SetupH2("ID_Z1_AoQ_S21_S42_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI21-SCI42","Z(music42)");
  SetupH2("ID_Z1_AoQcorr_S21_S42",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI21-SCI42","Z(music42)");
  SetupH2("ID_Z1_AoQcorr_S21_S42_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI21-SCI42","Z(music42)");
  SetupH2("ID_Z2_AoQ_S21_S42",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI21-SCI42","Z(music42)");
  SetupH2("ID_Z2_AoQ_S21_S42_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI21-SCI42","Z(music42)");
  SetupH2("ID_Z2_AoQcorr_S21_S42",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI21-SCI42","Z(music42)");
  SetupH2("ID_Z2_AoQcorr_S21_S42_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI21-SCI42","Z(music42)");

  SetupH1("ID_AoQ_S22_S42",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q SCI22-SCI42");
  SetupH1("ID_AoQ_S22_S42_mhit",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q SCI22-SCI42");
  SetupH1("ID_AoQcorr_S22_S42",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q corr SCI22-SCI42");
  SetupH1("ID_AoQcorr_S22_S42_mhit",1200,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,"A/Q corr SCI22-SCI42");
  SetupH2("ID_x2_AoQ_S22_S42",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,200,-100,100,"A/Q corr SCI22-SCI42","X at S2 [mm]");
  SetupH2("ID_x2_AoQ_S22_S42_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,200,-100,100,"A/Q corr SCI22-SCI42","X at S2 [mm]");
  SetupH2("ID_Z1_AoQ_S22_S42",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI22-SCI42","Z(music42)");
  SetupH2("ID_Z1_AoQ_S22_S42_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI22-SCI42","Z(music42)");
  SetupH2("ID_Z1_AoQcorr_S22_S42",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI22-SCI42","Z(music42)");
  SetupH2("ID_Z1_AoQcorr_S22_S42_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI22-SCI42","Z(music42)");
  SetupH2("ID_Z2_AoQ_S22_S42",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI22-SCI42","Z(music42)");
  SetupH2("ID_Z2_AoQ_S22_S42_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q SCI22-SCI42","Z(music42)");
  SetupH2("ID_Z2_AoQcorr_S22_S42",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI22-SCI42","Z(music42)");
  SetupH2("ID_Z2_AoQcorr_S22_S42_mhit",600,fIDPar->min_aoq_plot,fIDPar->max_aoq_plot,600,fIDPar->min_z_plot,fIDPar->max_z_plot,"A/Q corr SCI22-SCI42","Z(music42)");

  //TPC csum
  char tpcname[7][256]={"TPC21_","TPC22_","TPC23_","TPC24_","TPC41_","TPC42_", "TPC31_"};
  for(int i=0;i<7;i++)
    {
      // It seems that cname(2nd) should be different from histoname(last) YT-2020/Jan/24. (if they are same, no error, but no picture shows up in go4)
      SetupWinCond(Form("CSUM1_%s",tpcname[i]), fTPCPar->lim_csum1[i][0], fTPCPar->lim_csum1[i][1]);
      SetupWinCond(Form("CSUM2_%s",tpcname[i]), fTPCPar->lim_csum2[i][0], fTPCPar->lim_csum2[i][1]);
      SetupWinCond(Form("CSUM3_%s",tpcname[i]), fTPCPar->lim_csum3[i][0], fTPCPar->lim_csum3[i][1]);
      SetupWinCond(Form("CSUM4_%s",tpcname[i]), fTPCPar->lim_csum4[i][0], fTPCPar->lim_csum4[i][1]);

      for(int j=0; j<2; j++){
        SetupWinCond(Form("%s_LT%d_TDCWindow",tpcname[i],j), fTPCPar->lim_lt[i][j][0], fTPCPar->lim_lt[i][j][1]);
        SetupWinCond(Form("%s_RT%d_TDCWindow",tpcname[i],j), fTPCPar->lim_rt[i][j][0], fTPCPar->lim_rt[i][j][1]);
      }
      char tpc_anode_name[4][32]={"A11","A12","A21","A22"};
      for(int j=0; j<4; j++){
          SetupWinCond(Form("%s_%s_TDCWindow",tpcname[i],tpc_anode_name[j]), fTPCPar->lim_dt[i][j][0], fTPCPar->lim_dt[i][j][1]);
      }

    }

  for(int i=0;i<8;i++){
    SetupWinCond(Form("Timeref_%d_TDCWindow",i), fTPCPar->lim_timeref[i][0], fTPCPar->lim_timeref[i][1]);
  }

  printf("Analysis Name: %s\n",GetName());
  return res;
}


TFRSAnalysis::~TFRSAnalysis()
{
  std::cout << "**** TFRSAnalysis: Delete ****" << std::endl;
}


Int_t TFRSAnalysis::UserPreLoop()
{
  std::cout << "**** TFRSAnalysis: PreLoop ****" << std::endl;
  // we update the pointers to the current event structures here:
  fMbsEvent    = dynamic_cast<TGo4MbsEvent*>   (GetInputEvent("Unpack"));   // of step "Unpack"

  fEvents=0;
  fLastEvent=0;

  // create histogram for UserEventFunc
  // At this point, the histogram has been restored from autosave file if any.
  fSize=(TH1I*)GetHistogram("Eventsize");
  if(fSize==0)
    { // no autosave read, create new and register
      fSize = new TH1I ("Eventsize", "Event size[b]",500,0,500);
      fSize->SetLineColor(2);
      fSize->SetFillColor(2);
      AddHistogram(fSize);
    }

  // ======= initialization of some event processors

  if (IsBatchMode()) printf("go4 found in batch mode\n");
  if (IsClientMode()) printf("go4 found in client mode\n");
  if (IsServerMode()) printf("go4 found in server mode\n");

  TGo4AnalysisStep* step3 = GetAnalysisStep("Calibr");
  if (step3!=0) {
    TFRSCalibrProc* proc3 = dynamic_cast<TFRSCalibrProc*> (step3->GetEventProcessor());
    if (proc3) proc3->InitProcessor();
    if (IsBatchMode() && !IsAutoSaveOn()) {
      printf("Disabled Calibr histograms drawing!\n");
      proc3->bDrawHist=kFALSE;
    }

  }
  TGo4AnalysisStep* step4 = GetAnalysisStep("Analysis");
  if (step4!=0) {
    TFRSAnlProc* proc4 = dynamic_cast<TFRSAnlProc*> (step4->GetEventProcessor());
    if (IsBatchMode() && !IsAutoSaveOn()) {
      printf("Disabled Analysis histograms drawing!\n");
      proc4->bDrawHist=kFALSE;
    }

  }

  // ============= run initialization script =========

  return 0;
}

Int_t TFRSAnalysis::UserPostLoop()
{
  std::cout << "**** TFRSAnalysis: PostLoop ****" << std::endl;
  std::cout << "Last event: " << fLastEvent << " Total events: " << fEvents << std::endl;
  fMbsEvent = 0; // reset to avoid invalid pointer if analysis is changed in between
  fEvents=0;

  TGo4AnalysisStep* step3 = GetAnalysisStep("Calibr");
  if (step3!=nullptr)
    {
      TFRSCalibrProc* proc3 = dynamic_cast<TFRSCalibrProc*> (step3->GetEventProcessor());
      if(proc3!=nullptr)
	{
	  //proc3->xxxxx (here you can access to TFRSCalibrProc::xxxx after the event loop)
	}
    }
  return 0;
}

Int_t TFRSAnalysis::UserEventFunc()
{
  //// This function is called once for each event.
  Int_t value=0;
  Int_t count=0;
  if(fMbsEvent) value = fMbsEvent->GetDlen()/2+2; // total longwords
  fSize->Fill(value); // fill histogram
  fEvents++;
  if(fMbsEvent) count=fMbsEvent->GetCount();
  if(fEvents == 1) std::cout << "First event: " << count  << std::endl;
  fLastEvent=count;

  if (IsBatchMode() && fEvents % 2000 == 0) {
    printf("Event %d\r",fEvents);
    fflush(stdout);
  }


  return 0;
}

TH1* TFRSAnalysis::SetupH1(const char* histoname,
                           Int_t nbinsx, Float_t xmin, Float_t xmax,
                           const char* xtitle) {
  TH1* histo = dynamic_cast<TH1*> (GetHistogram(histoname));
  if (histo==0) {
    std::cout << "Histogram " << histoname << " did not found" << std::endl;
    return 0;
  }

  histo->SetBins(nbinsx, xmin, xmax);
  if (xtitle) histo->SetXTitle(xtitle);
  histo->Reset();

  return histo;

}

TH2* TFRSAnalysis::SetupH2(const char* histoname,
                           Int_t nbinsx, Float_t xmin, Float_t xmax,
                           Int_t nbinsy, Float_t ymin, Float_t ymax,
                           const char* xtitle, const char* ytitle) {
  TH2* histo = dynamic_cast<TH2*> (GetHistogram(histoname));
  if (histo==0) {
    std::cout << "Histogram " << histoname << " did not found" << std::endl;
    return 0;
  }

  histo->SetBins(nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  if (xtitle) histo->SetXTitle(xtitle);
  if (ytitle) histo->SetYTitle(ytitle);
  histo->Reset();

  return histo;
}

TGo4WinCond* TFRSAnalysis::SetupWinCond(const char* condname,
                                        float left, float right,
                                        Bool_t enable,
                                        Bool_t alwaysreturn) {
  TGo4WinCond* cond = dynamic_cast<TGo4WinCond*> (GetAnalysisCondition(condname));
  if (cond==0) {
    std::cout << "Window condition " << condname << " did not found" << std::endl;
    return 0;
  }

  cond->SetValues(left, right);
  cond->ResetCounts();
  if (enable)
    cond->Enable();
  else
    cond->Disable(alwaysreturn);

  return cond;
}

void TFRSAnalysis::AssignCond(const char* condname, const char* HistoName)
{
  TGo4Condition* cond = GetAnalysisCondition(condname);
  if (cond==0)
    std::cout << "Condition " << condname << " did not found" << std::endl;
  else
    cond->SetHistogram(HistoName);
}

TGo4PolyCond* TFRSAnalysis::SetupPolyCond(const char* condname,
                                          Int_t size,
                                          void *_points, //  Float_t (*points)[2],
                                          Bool_t enable,
                                          Bool_t alwaysreturn) {
  TGo4PolyCond* cond = dynamic_cast<TGo4PolyCond*> (GetAnalysisCondition(condname));
  if (cond==0) {
    std::cout << "Polygon condition " << condname << " did not found" << std::endl;
    return 0;
  }

  if ((size<3) || (_points==0)) {
    std::cout << "SetupPolyCond error: invalid conditions points for " << condname << std::endl;
    return 0;
  }

  Float_t (*points)[2] = (Float_t (*)[2]) _points;

  Float_t fullx[size+1], fully[size+1];
  int numpoints = size;
  for (int i=0;i<numpoints;i++) {
    fullx[i] = points[i][0];
    fully[i] = points[i][1];
  }

  // connect first and last points
  if ((fullx[0]!=fullx[numpoints-1]) || (fully[0]!=fully[numpoints-1])) {
    fullx[numpoints] = fullx[0];
    fully[numpoints] = fully[0];
    numpoints++;
  }

  TCutG mycat("initialcut", numpoints, fullx, fully);
  cond->SetValues(&mycat);
  cond->ResetCounts();
  if (enable)
    cond->Enable();
  else
    cond->Disable(alwaysreturn);
  return cond;
}

ClassImp(TFRSAnalysis)
