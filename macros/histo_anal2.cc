#include <iostream>

#include "TROOT.h"
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
    TFile* input_file[nHisto];
    TH1F*  histo_drif[nHisto];
    TH1F*  histo_response[nHisto];
    //gROOT->ProcessLine(".!ls>FileName.txt");
    input_file[1] = TFile::Open("modules.root");
    auto flag = input_file[1]->IsOpen();
    //input_file[1]->GetListOfKeys()->Print();
    //input_file[1]->cd("GenericPropagation/mydetector");
    input_file[1]->cd("GenericPropagation/mydetector");
    //input_file[1]->cd("GenericPropagation/mydetector")->GetList()->ls();
    auto dummy = input_file[1]->GetList()->ls();
    std::cout<<dummy<<std::endl;
    //input_file[1]->GetList()->Print();
    //input_file[1]->ls();
    //input_file[1]->GetList()->ls();
    //input_file[1]->cd("GenericPropagation/mydetector");
    //gDirectory->pwd();
    //input_file[1]->GetList()->Print();
    //input_file[1]->cd("CSADigitizer/mydetector/response_function");
    //input_file[1]->cd("GenericPropagation/mydetector");

    //gDirectory->pwd();
    //input_file[1]->ls();
    //gROOT->ProcessLine(".!ls>FileName.txt"); //List of files
    /*
    if (flag){
            histo_drif[1] = static_cast<TH1F*>(input_file[1]->Get("GenericPropagation/mydetector/drift_time_histo"));
            //histo_drif[1]->Draw();
            //histo_response[1] = static_cast<TH1F*>(input_file[1]->Get("CSADigitizer/mydetector/response_function/response_function"));
            //histo_response[1]->Draw();
    }
    else { std::cout<<"No such file found"<<std::endl; }
    */


}