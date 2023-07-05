#include <iostream>
#include <string>

#include "TROOT.h"
#include "TSystem.h"
#include "TDirectoryFile.h"
#include "TFile.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TKey.h"
#include "TDirectory.h"


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
    //gSystem->cd("/home/goyda-1/work/allpix/allpix-squared/Versions/Allpix_new/Alllpix_PLUS/src/output");
    gSystem->cd("/home/dnn1488/Desktop/Allpix_new/src/output");
/*
    //int nHisto = 100;
    TH1F* h1;
    TFile* f;

    f = TFile::Open("modules.root");
    auto flag = f->IsOpen();
    if (flag){
            h1 = static_cast<TH1F*>(f->Get("GenericPropagation/mydetector/drift_time_histo"));
            h1->GetXaxis()->SetTitle("X axis title");
            h1->GetYaxis()->SetTitle("Y axis title");
            h1->Draw();
            for(){
                Double_t maximum_Y = h1->GetBinContent(h1->GetMaximumBin());
                TAxis *xaxis = h->GetXaxis();
                Double_t binCenter = xaxis->GetBinCenter(bin);
                std::cout<<"Maximum of charge carriers"<<maximum_Y<<std::endl;
                //h->GetXaxis()->SetRangeUser(10, 30);
                //std::cout<<"Drifr time"<<maximum_X<<std::endl;
            }*/
            /*
            Double_t max = h1->GetBinContent(h1->GetMaximumBin());
            //Double_t max = h1->GetMaximum();
            //Double_t x_maximum = h1->GetMaximumX();
            Double_t x_maximum = h1->GetXaxis()->GetBinCenter(max);
            std::cout<<"Drifr time"<<x_maximum<<std::endl;
            std::cout<<"Drift time"<<max<<std::endl;

            //Double_t fwhm_left = h1->GetX(max/2,x_lower_range, x_maximum);
            //Double_t fwhm_right = h1->GetX(max/2, x_maximum, upper_range_x);
            ////Double_t fwhm_left = h1->GetX(max/2,x_lower_range, x_maximum);
            ////Double_t fwhm_right = h1->GetX(max/2, x_maximum, upper_range_x);
            ////cout<<"FWHM = " << fwhm_right - fwhm_left<<endl;

    }
    else { std::cout<<"No such file found"<<std::endl; } 
*/

//std::string inFileName = argv[1];
//TFile *f = new TFile(inFileName.c_str());

//-----------------------------------------------------
TString inFileName = "modules.root";
TFile *f = new TFile;
f = TFile::Open(inFileName);
TList *list = f->GetListOfKeys();
TFile *outFile = new TFile;
outFile = TFile::Open("myfile.root", "recreate");
TDirectory* currdir = new TDirectory;
currdir->cd();
TKey *key;
TIter iter(list); //or TIter iter(list->MakeIterator());
static TString classname_h("TH1D");
static TString classname_g("TGraph");

while((key = (TKey*)iter())) {
  if (key->IsFolder()) {
      TString dirname = key->GetTitle();
      std::cout << dirname << std::endl;
      currdir = (TDirectory*)f->Get(dirname);
      if (currdir->cd()) {
        std::cout << "directory changed" << std::endl;
      
        TList* listdir = currdir->GetListOfKeys();
        TIter iter_dir(listdir);
        TKey* keydir;

        while((keydir = (TKey*)iter_dir())){
          if (keydir->GetClassName() == classname_h) {
          TH1D *hist = (TH1D*)keydir->ReadObj();
            if (hist) {
             // std::cout << keydir->GetTitle();
                TDirectory::AddDirectory(kFALSE);
                outFile->WriteTObject(hist);
             // std::cout << " is read" << std::endl;
                delete hist;
            }
          }
          else if (keydir->GetClassName() == classname_g) {
          TGraph *graph = (TGraph*)keydir->ReadObj();
            if (graph) {
            //  std::cout << keydir->GetTitle();
                TDirectory::AddDirectory(kFALSE);
                outFile->WriteTObject(graph);
             // std::cout << " is read" << std::endl;
                delete graph;
            }
          }
          else if (keydir->IsFolder()) {
            dirname = keydir->GetTitle();
            std::cout << dirname << std::endl;
            currdir = (TDirectory*)currdir->Get(dirname);
            if (currdir->cd()) {
              std::cout << "directory changed" << std::endl;
            
              TList* listdir1 = currdir->GetListOfKeys();
              TIter iter_dir1(listdir1);
              TKey* keydir1;
              TString hname = "";

              while((keydir1 = (TKey*)iter_dir1())){
                if (keydir1->GetClassName() == classname_h) {
                hname = keydir1->GetName();
                TH1D *hist = (TH1D*)keydir1->ReadObj();
                  if (hist) {
                    //std::cout << keydir1->GetTitle();
                      TDirectory::AddDirectory(kFALSE);
                      hist->SetName(hname);
                      outFile->WriteTObject(hist);
                    //std::cout << " is read" << std::endl;
                      delete hist;
                  }
                }
                else if (keydir1->GetClassName() == classname_g) {
                hname = keydir1->GetName();
                TGraph *graph = (TGraph*)keydir1->ReadObj();
                  if (graph) {
                    //std::cout << keydir1->GetTitle();
                      TDirectory::AddDirectory(kFALSE);
                      graph->SetName(hname);
                      outFile->WriteTObject(graph);
                    //std::cout << " is read" << std::endl;
                      delete graph;
                }
                }
              }
            }
              currdir->cd("..");
          }
          currdir->cd("..");
        }
      }
    }

}
outFile->cd();
outFile->Write();
outFile->Close();
//-----------------------------------------------------

}