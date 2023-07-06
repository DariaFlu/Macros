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

#include "TApplication.h"


void output_writter(const TString& inFileName, const TString& outFileName){
//TString inFileName
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
//-----------------------------------------------------
//TString inFileName = "modules.root";
//-----------------------------------------------------

//gSystem->cd("/home/goyda-1/work/allpix/allpix-squared/Versions/Allpix_new/Alllpix_PLUS/output");
gSystem->cd("/home/goyda-1/work/allpix/allpix-squared/Versions/Allpix_new/Alllpix_PLUS/src/output");

if (!gApplication) {
  new TApplication("MyApplication", nullptr, nullptr);
}

TFile *f = new TFile;
f = TFile::Open(inFileName);
TList *list = f->GetListOfKeys();
TFile *outFile = new TFile;
outFile = TFile::Open(outFileName, "recreate");
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

if(gApplication){
  gApplication->Terminate();
}
//-----------------------------------------------------

}

int main1(){
    
  TString inputFile = "modules.root";
  TString outputFile = "calc_out.root";
  output_writter(inputFile, outputFile);

return 0;
}
