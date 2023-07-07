#include <iostream>
#include <fstream>


#include "TROOT.h"
#include "TSystem.h"
#include "TDirectoryFile.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TFile.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TString.h"

void GetHistogramList(TFile* f, TString path, TString ListFileName, TString reg_exp){

    std::ofstream txt(ListFileName, std::ofstream::out);
    if (txt.is_open()){
        std::cout << "File for writing is open .txt file" << std::endl;
    }
    else{
        std::cout << "Problem with oppening .txt file" << std::endl;
    }
    f->cd(path);
    TIter next(gDirectory ->GetListOfKeys());
    TKey *key;
    TString histname;
    gROOT->SetBatch(kTRUE);

    while ((key=(TKey*)next())) {
        histname = key ->GetName();
        if(histname(0, 12) == "amp_pulse_ev"){
            histname = path + histname;
            txt << histname << "\n";
        }
        
    }
    txt.close();

}

int gethist(){
    gSystem->cd("/home/dnn1488/Desktop/Allpix_new/src/output");
    TString inFileName = "modules.root";
    TFile *f = new TFile;
    f = TFile::Open(inFileName);

    GetHistogramList(f,"/CSADigitizer/mydetector/","histos.txt","");
    return 0;
}