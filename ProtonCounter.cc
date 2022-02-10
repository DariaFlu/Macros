#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <algorithm>

#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TBrowser.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TF1.h"


using namespace std;

vector<string> FileTimepix;
void FileTimepixName (vector<string> &vec);

vector<pair<Double_t, Double_t>> ProtonCounter(){
//int ProtonCounter(){
    
    gSystem->cd("/home/dnn1488/Desktop/Root/DariaCalib2022Nov22/TimePixCalib1");
    //gROOT->ProcessLine(".!ls>FileName.txt"); //List of files
    
   //+++++++++++++++++++++++++++++++++++++
   //OPEN FILE AND READ DATA   
   //+++++++++++++++++++++++++++++++++++++

    vector<pair<Double_t, Double_t>> FluxAndError;
    pair<Double_t,Double_t>          FlAndErr;
    
    ifstream fileDir; //Open the RootFile.txt
    fileDir.open("RootFile.txt", ifstream::in); //Stream for reading the file
    string c; 
    TString c1;
    //vector<string> filenames; // List of files
    Double_t Area, ClusterMax, ClusterArea, ClusterTOT, MeanTOT, MeanArea, Frame, N_TOT, N_Area, lifeTime, N_Mean, sigmaN, Flux, sigmaFlux;
    Area = sqrt(2)/256;
    TFile* myfile[100];
    TH1D* h1[100];
    TH1D* h2[100];
    TH1D* h3[100];
    TH1D* h4[100];
    int i = 0;

    ofstream ofs;
    ofs.open("ProtonFlux.txt");
    while(getline(fileDir,c)){
          c1 = c;
          myfile[i] = TFile::Open(c1);
          //myfile->ls();
          h1[i] = new TH1D();
          h2[i] = new TH1D();
          h3[i] = new TH1D();
          h4[i] = new TH1D();
    
          h1[i] = (TH1D*)myfile[i] -> Get("fAnalSummary");
          h2[i] = (TH1D*)myfile[i] -> Get("fHisSingleClusterAreaFullStatAboveCut");//fHisSingleClusterAreaFullStatAboveCut
          h3[i] = (TH1D*)myfile[i] -> Get("fHisSingleClusterTOTFullStat");         //fHisSingleClusterTOTFullStat
          h4[i] = (TH1D*)myfile[i] -> Get("fEventSummary");                        //fEventSummary
    
          ClusterMax  = h1[i] -> GetBinContent(1);
          ClusterArea = h1[i] -> GetBinContent(2);
          ClusterTOT  = h1[i] -> GetBinContent(3);
          MeanArea    = h2[i] -> GetMean();
          MeanTOT     = h3[i] -> GetMean();
          Frame       = h4[i] -> GetBinContent(1);
    
          N_TOT     = ClusterTOT/MeanTOT;
          N_Area    = ClusterArea/MeanArea;
          lifeTime  = Frame*1; //Frame*1ms
          N_Mean    = (ClusterMax+N_TOT+N_Area)/3;
          sigmaN    = sqrt(((ClusterMax - N_Mean)*(ClusterMax - N_Mean)+(N_Area - N_Mean)*(N_Area - N_Mean)+(N_TOT - N_Mean)*(N_TOT - N_Mean))/(3-1));
          Flux      = N_Mean/(Area*lifeTime);
          sigmaFlux = sigmaN/(Area*lifeTime);
          cout<<"--------------------------------"<<endl;
          cout<<"File Name: "<< c <<endl;
          cout<<"Proton counting max: "<<ClusterMax << endl;
          cout<<"Cluster area: "       <<ClusterArea<< endl;
          cout<<"Cluster TOT: "        <<ClusterTOT << endl;
          cout<<"TOT mean: "           <<MeanTOT    << endl;
          cout<<"Mean area: "          <<MeanArea   << endl;
          cout<<"Frame: "              <<Frame      << endl;
          cout<<"Life time: "          <<lifeTime   << endl;
          cout<<endl;
          cout<<"N_TOT: "              <<N_TOT      << endl;
          cout<<"N_Area: "             <<N_Area     << endl;
          cout<<endl;
          cout<<"N_Mean: "             <<N_Mean     << endl;
          cout<<"Sigma N: "            <<sigmaN     << endl;
          cout<<endl;
          cout<<"Proton flux: "        <<Flux       << endl;
          cout<<"Sigma flux: "         <<sigmaFlux  << endl;
          cout<<"--------------------------------"  << endl;
          
          ofs << Flux      << endl;
          ofs << sigmaFlux << endl;
          
          FlAndErr = make_pair(Flux, sigmaFlux);
          FluxAndError.push_back(FlAndErr);
        
          FileTimepix.push_back(c);
        
          i++;
      }
    ofs.close();
   

//return 0;
return FluxAndError;
}

void FileTimepixName (vector<string> &vec){

    for(size_t i = 0; i < FileTimepix.size(); i++){
      vec.push_back(FileTimepix[i]);
    }

}
