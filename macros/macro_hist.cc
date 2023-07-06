#include <iostream>

#include "TROOT.h"
#include "TSystem.h"
#include "TDirectoryFile.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TFile.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TString.h"
//#include "GetHistogramList.cc"
//#include "TApplication.h"

void FWHM(){

}

int macro_hist(){
//     // GetHistogramList(const TFile* f, 
//     // TString path, 
//     // TString ListFileName, 
//     // TString reg_exp)
//      //gSystem->cd("/home/goyda-1/work/allpix/allpix-squared/Versions/Allpix_new/Alllpix_PLUS/src/output");
//     gSystem->cd("/home/goyda-1/work/allpix/allpix-squared/Versions/Allpix_new/Alllpix_PLUS/src/output");

//     if (!gApplication) { new TApplication("MyApplication", nullptr, nullptr);}

//     TString inputFile = "modules.root";
//     TFile *f = new TFile;
//     f = TFile::Open(inputFile);
//    /* GetHistogramList(f, 
//                     " ",
//                     ),

// */
//     if(gApplication){  gApplication->Terminate(); }
gSystem->cd("/home/goyda-1/work/allpix/allpix-squared/Versions/Allpix_new/Alllpix_PLUS/src/output");
// CSADigitizer->mydetector->response_function
// ........................->amp_pulse_ev1_px48-56
// ........................->amp_pulse_noise_ev1_px48-56
// GenericPropagation->mydetector->drift_time_histo
//DetectorHistogrammer->mydetector->hit_map
TFile* inputFile = new TFile;
inputFile = TFile::Open("modules.root");
auto drift_histo = inputFile->FindObjectAny("drift_time_histo");
drift_histo->Draw();
/*TKey* key_histo;
key_histo=drift_histo->GetKey();*/
TDirectory* currdir = new TDirectory;
currdir->cd();

return 0;
}