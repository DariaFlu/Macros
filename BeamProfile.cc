#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <cmath>

#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TF1.h"
#include "TH1.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TString.h"

//#include "BeamMonitoring_22.cc"

struct position{

    string  name;
    Double_t xTimepix; // cm
    Double_t yTimepix; // cm

    Double_t xTimepixError; // cm
    Double_t yTimepixError; // cm
};

int BeamProfile(){

  /*KEY: TH1D   fHisClusterAreaFullStat;1   fHisClusterAreaEvtByEvt
  KEY: TH1D fHisClusterTOTFullStatNoCut;1   fHisClusterTOTEvtByEvt
  KEY: TH1D fHisClusterEnergyFullStatNoCut;1    fHisClusterEnergyEvtByEvt
  KEY: TH1D fHisClusterAreaFullStatAboveCut;1   fHisClusterAreaEvtByEvt
  KEY: TH1D fHisClusterTOTFullStat;1    fHisClusterTOTEvtByEvt
  KEY: TH1D fHisClusterEnergyFullStat;1 fHisClusterEnergyEvtByEvt
  KEY: TH1D fHisSingleClusterAreaFullStatAboveCut;1 fHisClusterAreaEvtByEvt
  KEY: TH1D fHisSingleClusterTOTFullStat;1  fHisClusterTOTEvtByEvt
  KEY: TH1D fHisSingleClusterEnergyFullStat;1   fHisClusterEnergyEvtByEvt
  KEY: TH1D fHisClusterProfileX;1   fHisClusterProfile
  KEY: TH1D fHisClusterProfileY;1   fHisClusterProfileY
  KEY: TH1D fHisNofHits1D;1 fHisNofHits1D
  KEY: TH1D fHisNoisyRows;1 fHisNoisyRows
  KEY: TH1D fAnalSummary;1  fAnalSummary
  KEY: TH1D fEventSummary;1 fEventSummary
  KEY: TH1I fCuts;1 Analysis cuts*/

  /*
  14:23  poistion [198,249]    ... timepix at beam center 2000 frames   Align_0mm_1.dat
  14:30  poistion [195,249]    ...                                      Align_0mm_2.dat
  14:36  poistion [192,249]    ...                                      Align_0mm_3.dat

  14:41  poistion [201,249]    ...                                      Align_0mm_4.dat
  14:47  poistion [204,249]    ...                                      Align_0mm_5.dat

  14:52  poistion [198,249]    ...  timepix at beam center              Align_0mm_6.dat
  14:56  poistion [198,252]    ...                                      Align_0mm_7.dat
  15:00  poistion [198,255]    ...                                      Align_0mm_8.dat

  15:05  poistion [198,246]    ...                                      Align_0mm_9.dat
  15:10  poistion [198,243]    ...                                      Align_0mm_10.dat
  */
    /*
    Position when the center of the chamber is in the beam    [X,Y] = [198,259] mm
    Position when the center of the Timepix is in the beam    [X,Y] = [198,249] mm
    */
  /*                                                     [198,255] (TC(y) + 6) mm
                                                         [198,252] (TC(y) + 3) mm
    [198,243] TC(x) - 6)mm || [198,246] (TC(x) - 3)mm || [198,249] TC(x,y)     mm  || [201,249] (TC(x) + 3)mm || [204,249] (TC(x) + 6)mm
                                                         [195,249] (TC(y) - 3) mm
                                                         [192,249] (TC(y) - 6) mm
  */

   /*vector<Int_t>    timestamp; //timestamp
   vector<Int_t>    x;         //position x [mm]
   vector<Int_t>    y;         //position y [mm]
   vector<Double_t> IC;        //ionization chamber current [A]

   ifstream file;
   file.open("/home/dnn1488/Desktop/Root/DariaCalib2022Nov22/BeamMonitoring/LOG_1637575860.txt", ifstream::in);

   string c0;
   int i            = 0;
   int valTimeStamp = 0;
   int valX         = 0;
   int valY         = 0;
   double valueIC   = 0.0;

   while(getline(file,c0)){
      istringstream ss;
      ss.str(c0);

      ss>>valTimeStamp;
      timestamp.push_back(valTimeStamp);  //timestamp

      ss>>valX;
      x.push_back(valX);  //position x

      ss>>valY;
      y.push_back(valY);  //positon y

      ss>>valueIC;
      IC.push_back(valueIC); //ionization chamber current

      i++;
   }

   file.close();*/

    //chamber = retChamber();
    /*extern dataChamber *chamber;
    vector<Int_t> timestamp1 = chamber->time; //timestamp
    vector<Int_t> xpos = chamber->x;   //position x [mm]
    vector<Int_t> ypos = chamber->y;    //position y [mm]
    vector<Double_t> IC1 = chamber->IC; //ionization chamber current [A]*/

    gSystem->cd("/home/dnn1488/Desktop/Root/DariaCalib2022Nov22/Alignment");
    gROOT->ProcessLine(".!ls>FileList.txt"); //List of files

    ifstream fileName;
    fileName.open("FileList.txt", ifstream::in); //Stream for reading the file

    string  c;
    TString c1;
    string  mask = "root";    //Mask for finding a  'smt.dat.root' file

    position st[100];

    Double_t areaPixel = (sqrt(2)/256)*10000; // cm-> microm
    TFile*   myfile[100];
    TH1D*    h[100];
    TH1D*    h2[100];
    TCanvas* canv[100];
    TCanvas* canv2[100];
    TF1*     fitFun[100];
    TLegend* leg[100];

    TF1* line  = new TF1("line","[0]",0,1);
    TF1* line0 = new TF1("line0","[0]",0,1);

    int i = 0;
    int k = 1;
    int io = 0;
    int ic = 0;
    size_t counter = 0;

    TString aPosition[] = {
                          //Timepix center (X,Y) +- X mm
                          "Timepix center (X,Y)",
                          "Timepix center (X - 3) mm",
                          "Timepix center (X - 6) mm",
                          "Timepix center (X + 3) mm",
                          "Timepix center (X + 6) mm",
                          "Timepix center (X,Y)",
                          //Timepix center (X,Y) +- Y mm
                          "Timepix center (Y + 3) mm",
                          "Timepix center (Y + 6) mm",
                          "Timepix center (Y - 3) mm",
                          "Timepix center (Y - 6) mm"
                      };

    while(getline(fileName,c)){

        string check (c.end()-4, c.end());
        if(check == mask){
            c1 = c;
            myfile[i] = TFile::Open(c1);

            canv[i] = new TCanvas(Form("canv%d",i),Form("Align_0mm_%d.dat.root: fHisClusterProfileX",k),0,0,800,500);
            h[i]    = new TH1D();
            h[i]    = (TH1D*)myfile[i]->Get("fHisClusterProfileX");
            h[i] -> SetTitle("fHisClusterProfileX");
            h[i] -> Draw();
            h[i] -> Fit("gaus","R"," ",5,230);
            fitFun[i] = h[i]->GetFunction("gaus");
            Double_t mean      = fitFun[i]->GetParameter(1);
            Double_t meanErr   = fitFun[i]->GetParError(1);
            cout<<mean<<endl;

            leg[i] = new TLegend(0.15, 0.15, 0.55, 0.35);
            line->SetLineColor(9);
            leg[i]->AddEntry((TObject*) line, aPosition[i]);
            line0->SetLineColor(2);
            leg[i]->AddEntry((TObject*) line0, Form("mean = %.1f #pm %.1f #mum", mean*areaPixel , meanErr*areaPixel),"l");
            leg[i]->SetTextSize(0.045);
            leg[i] -> Draw();


            canv2[i] = new TCanvas(Form("canv2%d",i),Form("Align_0mm_%d.dat.root: fHisClusterProfileY",k),0,0,800,500);
            h2[i]    = new TH1D();
            h2[i]    = (TH1D*)myfile[i]->Get("fHisClusterProfileY");
            h2[i] -> Draw();
            h2[i] -> Fit("gaus");
            fitFun[i+1] = h2[i]->GetFunction("gaus");
            Double_t mean1    = fitFun[i+1]->GetParameter(1);
            Double_t mean1Err = fitFun[i+1]->GetParError(1);

            leg[i+1] = new TLegend(0.15, 0.15, 0.55, 0.35);
            line->SetLineColor(9);
            leg[i+1]->AddEntry((TObject*) line, aPosition[i]);
            line0->SetLineColor(2);
            leg[i+1]->AddEntry((TObject*) line0, Form("mean = %.1f #pm %.1f #mum", mean*areaPixel , meanErr*areaPixel),"l");
            leg[i+1]->SetTextSize(0.045);
            leg[i+1] -> Draw();

            st[i] = {Form("Align_0mm_%d.dat.root",k),mean*areaPixel,mean1*areaPixel,meanErr*areaPixel,mean1Err*areaPixel};

            canv[i] ->SaveAs(Form("XAlign_0mm_%d.dat.png",k));
            canv2[i]->SaveAs(Form("YAlign_0mm_%d.dat.png",k));
            i++;
            k++;

            counter++;

            // struct position{
            //
            //     string  name;
            //     Double_t xTimepix; // cm
            //     Double_t yTimepix; // cm
            //
            //     Double_t xTimepixError; // cm
            //     Double_t yTimepixError; // cm
            // };
        }
    }

    fileName.close();

    for(size_t i = 0; i < counter; i++){ cout<<st[i].name<<" x = ("<<st[i].xTimepix<<" +- "<<st[i].xTimepixError<<") cm y = ("<<st[i].yTimepix<<" +- "<<st[i].yTimepixError<<") cm"<<endl;}

    /*for(size_t i = 0; i <= 10; i++) {
        Print();
        //cout<<st.name[i]<<endl;
    }*/

    //cout<<chamber->time[1]<<endl;

    ///TIME: ALIGNMENT scane
    /*Double_t time[100];
    pair<Int_t, Int_t> AlignScan = TimeToUnix(timestamp, 14, 23, 0, 14, 30, 0);

    time[0] = (Double_t) AlignScan.first;  //14:23  poistion [198,249] Align_0mm_1.dat
    time[1] = (Double_t) AlignScan.second; //14:30  poistion [195,249] Align_0mm_2.dat

    AlignScan = TimeToUnix(timestamp, 14, 36, 0, 14, 41, 0);
    time[2] = (Double_t) AlignScan.first;  //14:36  poistion [192,249] Align_0mm_3.dat
    time[3] = (Double_t) AlignScan.second; //14:41  poistion [201,249] Align_0mm_4.dat

    AlignScan = TimeToUnix(timestamp, 14, 47, 0, 14, 52, 0);
    time[4] = (Double_t) AlignScan.first;  //14:47  poistion [204,249] Align_0mm_5.dat
    time[5] = (Double_t) AlignScan.second; //14:52  poistion [198,249] Align_0mm_6.dat

    AlignScan = TimeToUnix(timestamp, 14, 56, 0, 15, 0, 0);
    time[6] = (Double_t) AlignScan.first;  //14:56  poistion [198,252] Align_0mm_7.dat
    time[7] = (Double_t) AlignScan.second; //15:00  poistion [198,255] Align_0mm_8.dat

    AlignScan = TimeToUnix(timestamp, 15, 5, 0, 15, 10, 0);
    time[8] = (Double_t) AlignScan.first;  //15:05  poistion [198,246] Align_0mm_9.dat
    time[9] = (Double_t) AlignScan.second; //15:10  poistion [198,243] Align_0mm_10.dat */

    /*for(size_t i = 0; i < 10; i++){
        cout<<time[i]<<endl;
    }*/


   /*int npoints = 0;
    TCanvas* canv3 = new TCanvas("canvas"," ",0,0,800,500);
    TGraph* IonizationCurrent = new TGraph();

    for(size_t i = 0; i < timestamp.size(); i++){

        if(time[0] <= timestamp[i] && timestamp[i] <= time[9]){
            npoints = IonizationCurrent->GetN();
            IonizationCurrent->Set(npoints+1);
            IonizationCurrent->SetPoint(npoints, timestamp[i], IC[i]*1.0);
        }
    }

    canv3->cd();
    IonizationCurrent->GetXaxis()-> SetTimeFormat("%H:%M %F1970-01-01 00:00:00");
    IonizationCurrent->GetXaxis()-> SetTimeDisplay(1);
    IonizationCurrent->Draw("AP");

   pair<Double_t, Double_t> calibrationFactor = BeamMonitoring_22();
   TGraph* IonCurrent[100];
   TCanvas* canvas[100];
   size_t z = 1;
    for(size_t k = 0; k < 10; k++){
        IonCurrent[k] = new TGraph();
        for(size_t i = 0; i < timestamp.size(); i++){
        if(time[9]){ time[10] = time[9]+6*60; }
          if(time[k] <= timestamp[i] && timestamp[i] <= time[k+1]){
            npoints = IonCurrent[k]->GetN();
            IonCurrent[k]->Set(npoints+1);
            IonCurrent[k]->SetPoint(npoints, timestamp[i], IC[i]*1.0);
         }
      }
        canvas[k] = new TCanvas(Form("can%d",k), Form("Align_0mm_%d.dat",z),0,0,800,500);
        canvas[k]->cd();
        IonCurrent[k]->GetXaxis()-> SetTimeFormat("%H:%M %F1970-01-01 00:00:00");
        IonCurrent[k]->GetXaxis()-> SetTimeDisplay(1);
        IonCurrent[k]->SetMarkerSize(25.);
        IonCurrent[k]->Draw("AP");
        z++;
    }*/


return 0;
}
