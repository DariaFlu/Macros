#include <iostream>

#include "TSystem.h"
#include "TDirectoryFile.h"
#include "TFile.h"
#include "TH1F.h"


void histo_anal(){

/*------------------------------------------
  KEY: TDirectoryFile	GeometryBuilderGeant4;1	GeometryBuilderGeant4
  KEY: TDirectoryFile	DepositionGeant4;1	DepositionGeant4
  KEY: TDirectoryFile	ElectricFieldReader;1	ElectricFieldReader
  KEY: TDirectoryFile	DopingProfileReader;1	DopingProfileReader
  KEY: TDirectoryFile	GenericPropagation;1	GenericPropagation
  KEY: TDirectoryFile	PulseTransfer;1	PulseTransfer
  KEY: TDirectoryFile	CSADigitizer;1	CSADigitizer
  KEY: TDirectoryFile	DetectorHistogrammer;1	DetectorHistogrammer
  KEY: TDirectoryFile	ROOTObjectWriter;1	ROOTObjectWriter
------------------------------------------*/
    //GenericPropagation: drift_time_histo//
    gSystem->cd("/home/goyda-1/work/allpix/allpix-squared/Versions/Allpix_new/Alllpix_PLUS/src/output");
   
    int nHisto = 100;
    TH1F* h1[nHisto];
    TFile* f[nHisto];

    f[1] = TFile::Open("modules.root");
    auto flag = f[1]->IsOpen();
    if (flag){
            h1[1] = static_cast<TH1F*>(f[1]->Get("GenericPropagation/mydetector/drift_time_histo"));
            h1[1]->Draw();
    }
    else { std::cout<<"No such file found"<<std::endl; }
    


}