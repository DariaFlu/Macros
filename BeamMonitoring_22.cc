#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "TGraph.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TString.h"
#include "TAxis.h"
#include <cmath>
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TFile.h"
#include "TBrowser.h"
#include "TH1.h"
#include "TF1.h"
#include "TStyle.h"
#include "TMath.h"
#include "TBox.h"
#include "TH2D.h"
#include "TLatex.h"
#include "TGraphErrors.h"

#include "Twinpad.h"

#include "TimepixCounter.cc"
#include "ProtonCounter.cc"
#include "Graph.cc"


using namespace std;

vector<TDatime> TimeToGMT(const vector<Double_t>& time1);
pair<string, string> TimepixTime(string nameFile);
vector<pair<Double_t, Double_t>> TimepixCounter();
vector<pair<Double_t, Double_t>> ProtonCounter();

bool operator<(const pair<string, string>& l, const pair<string, string>& r); 
bool operator==(const pair<string, string>& l, const pair<string, string>& r);

void SetGraph(TGraph* gr, TString titleX, TString titleY, Bool_t btime);
pair<Int_t, Int_t>  TimeToUnix(const vector<Int_t>& timestamp, Int_t hour, Int_t min, Int_t sec, Int_t hour2, Int_t min2, Int_t sec2);


struct info{                  //Information about structure data

    Double_t t0;                     //The start of scan
    Double_t t1;                     //The beginning of the approximation interval (Chamber)
    Double_t t2;                     //The end of the approximation interval (Chamber)
    Double_t t3;                     //The beginning of the approximation interval (Timepix)
    Double_t t4;                     //The end of the approximation interval (Timepix)
    Double_t t5;                     //The end of scan

    string timepixName;              //Name of timepix file
    Double_t FluxProton;             //The amount of proton flux
    Double_t ErFluxProton;           //Sigma for proton flux

    Double_t CenterChamber;          //Ionization current (beam is at the chamber center)
    Double_t CenterTimepix;          //Ionization current (beam is at the timepix center ) 

    Double_t ErrorCenterChamber;     //Error of ionization current (beam is at the chamber center)
    Double_t ErrorCenterTimepix;     //Error of ionization current (beam is at the timepix center ) 

    Double_t ICCenterTimepix;         //Mean value of ionization current (beam is at the timepix center ) 
    Double_t ErrorICCenterTimepix;    //Mean value of ionization current (beam is at the timepix center ) 

    Double_t ratio;                   //The ratio between current when beam is at center of chamber and timepix I_ch/I_timepix
    Double_t ratioErr;                //The error of ratio between current when beam is at center of chamber and timepix I_ch/I_timepix

    Double_t CorrespondCurrent;        //i = ratio*ICCenterTimepix=(CenterChamber/CenterTimepix)*ICCenterTimepix
    Double_t CorrespondCurrentError;   //error = ErrorCenterTimepix*CorrespondCurrent 

};

/*struct dataChamber{

   vector<Int_t> time; //timestamp
   vector<Int_t> x;    //position x [mm]
   vector<Int_t> y;    //position y [mm]

   vector<Double_t> IC; //ionization chamber current [A]

};

extern dataChamber *chamber = new dataChamber;*/

 pair<Double_t, Double_t> BeamMonitoring_22(){
   
   gSystem->cd("/home/dnn1488/Desktop/Root/DariaCalib2022Nov22/BeamMonitoring");

   vector<Int_t> timestamp; //timestamp
   vector<Int_t> x;    //position x [mm]
   vector<Int_t> y;    //position y [mm]

   vector<Double_t> IC; //ionization chamber current [A]


   //-------------------------------------
   //OPEN FILE AND READ DATA   
   //-------------------------------------
   //Reading data using a stream
   ifstream file;
   file.open("LOG_1637575860.txt", ifstream::in);

   string c;
   int i = 0;
   int valTimeStamp = 0;
   int valX = 0;
   int valY = 0;
   double valueIC = 0.0;

   while(getline(file,c)){
      istringstream ss;
      ss.str(c);
      
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

   file.close();
    
    //(*chamber) = {timestamp, x, y, IC};
    
   //cout<<chamber->time[1]<<endl;


   //////////////////////////////////////////////////
   //The file "ProtonFlux.txt" creates a file that contains proton flux and errors. This code may be used for receiving value of proton flux by file. 
   //PROTON FLUX
   //gSystem->cd("/home/dnn1488/Desktop/Root/DariaCalib2022Nov22/TimePixCalib1");

   /*string cs;
   Double_t dummyFlux    = 0;
   Double_t dummyFluxErr = 0;
   vector<Double_t> Flux;
   vector<Double_t> FluxErr;
   i = 0;
    
   ifstream fileFlux;
   fileFlux.open("ProtonFlux.txt", ifstream::in);

   while(getline(fileFlux,cs)){
      istringstream ss;
      ss.str(cs);
      if( i % 2 == 0 ){
        ss>>dummyFlux;
         Flux.push_back(dummyFlux);
      }
      else{
      ss>>dummyFluxErr;
      FluxErr.push_back(dummyFluxErr); 
      
      }

      i++;
      
   }

    fileFlux.close();*/
    
    //Int_t CounterDummy = 0;
    /*for(size_t j = 0; j < Flux.size(); j++){
        cout<<"Proton Flux: "<<Flux[j]<<endl;
        cout<<"Error proton flux: "<<FluxErr[j]<<endl;
        //CounterDummy++;
    }*/
     /*cout<<"-------------------------------"<<endl;
     cout<<"Number of iteration: "<<CounterDummy<<endl;
     cout<<"-------------------------------"<<endl;*/
    
    //////////////////////////////////////////////////
    vector<string> FileNameTimepix; // File names timepix
    
    Int_t CounterDummy = 0;
    vector<pair<Double_t, Double_t>> ProtonFlux_Error = ProtonCounter(); // Here is used ProtonCounter() function for calculation value of proton flux. It is more comfortable to use rather than reading the file
    vector<string> FileTimepixNames;
    FileTimepixName (FileTimepixNames);
    for(size_t j = 0; j < ProtonFlux_Error.size(); j++){ //This loop is used for receiving values of flux and errors
        cout<<"---------------------------------------------------"<<endl;
        cout<<FileTimepixNames[j]<<endl;
        cout<<"Proton Flux: " <<(ProtonFlux_Error[j]).first<<" +- "; //Proton flux
        cout<<"Sigma: "       <<(ProtonFlux_Error[j]).second<<endl;  //Errors of proton flux
        CounterDummy++;
        cout<<"---------------------------------------------------"<<endl;
    }
    cout<<"Number of iteration: "<<CounterDummy<<endl;
    
   //--------------------------------------------------------------------------------
   //PLOTTING GRAPH USING TIME RANGE OF TIMEPIX DETECTOR (TimepixCounter.cc)
   //--------------------------------------------------------------------------------
    Double_t ChamberTime[100]; // The array of time (beginning of scans)
    
    int size = timestamp.size(); //size of timestamp vector
    Int_t npoints;
    Int_t io;
    
    vector<Double_t> startMeas;        //Start of the iteration
    vector<Double_t> stopMeas;         //End of the iteration
    vector<Double_t> startMeasTIMEPIX; //Beam is at center of Timepix
    
    //Time to UNIX format. It is the time from logbook for event when chamber is at the beam center
    
    pair<Int_t, Int_t> ChamberUNIXtime = TimeToUnix(timestamp, 11, 17, 0, 0,0,0); //Mon Nov 22 11:17:00(start) 2021 // 11:18:00 (end)
    ChamberTime[0] = ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 11, 26, 0, 0,0,0); //Mon Nov 22 11:26:00 2021// 11:28:00
    ChamberTime[1]  = (Double_t) ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 11, 37, 0, 0,0,0); //Mon Nov 22 11:37:00 2021 // 
    ChamberTime[2]  = (Double_t) ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 11, 45, 0, 0,0,0); //Mon Nov 22 11:45:00 2021
    ChamberTime[3]  = (Double_t) ChamberUNIXtime.first;
   
    ChamberUNIXtime = TimeToUnix(timestamp, 11, 53, 0, 0,0,0); //Mon Nov 22 11:45:00 2021
    ChamberTime[4]  = (Double_t) ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 11, 59, 0, 0,0,0); //Mon Nov 22 11:59:00 2021
    ChamberTime[5]  = (Double_t) ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 12, 4, 0, 0,0,0); //Mon Nov 22 12:04:00 2021
    ChamberTime[6]  = (Double_t) ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 12, 10, 0, 0,0,0); //Mon Nov 22 12:10:00 2021
    ChamberTime[7]  = (Double_t) ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 12, 15, 0, 0,0,0); //Mon Nov 22 12:15:00 2021
    ChamberTime[8]  = (Double_t) ChamberUNIXtime.first;
    
    //Calibration with plate 0.5 mm
    ChamberUNIXtime = TimeToUnix(timestamp, 12, 44, 0, 0,0,0); //Mon Nov 22 12:44:00 2021 Cal_05mm_1.dat 
    ChamberTime[9]  = (Double_t) ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 13, 00, 0, 0,0,0); // Mon Nov 22 13:00:00 2021 Cal_05mm_2.dat
    ChamberTime[10]  = (Double_t) ChamberUNIXtime.first; 
    
    ChamberUNIXtime = TimeToUnix(timestamp, 13, 07, 0, 0,0,0); // Mon Nov 22 13:07:00 2021 Cal_05mm_3.dat
    ChamberTime[11]  = (Double_t) ChamberUNIXtime.first; 
    
    ChamberUNIXtime = TimeToUnix(timestamp, 13, 11, 0, 0,0,0); // Mon Nov 22 13:11:00 2021 Cal_05mm_4.dat
    ChamberTime[12]  = (Double_t) ChamberUNIXtime.first; 
    
    ChamberUNIXtime = TimeToUnix(timestamp, 13, 15, 0, 0,0,0); // Mon Nov 22 13:15:00 2021 Cal_05mm_5.dat
    ChamberTime[13]  = (Double_t) ChamberUNIXtime.first; 
    
    ChamberUNIXtime = TimeToUnix(timestamp, 13, 19, 0, 0,0,0); // Mon Nov 22 13:19:00 2021 Cal_05mm_6.dat
    ChamberTime[14]  = (Double_t) ChamberUNIXtime.first; 
    
    ChamberUNIXtime = TimeToUnix(timestamp, 13, 25, 0, 0,0,0); // Mon Nov 22 13:25:00 2021 Cal_05mm_7.dat
    ChamberTime[15]  = (Double_t) ChamberUNIXtime.first; 
    
    ChamberUNIXtime = TimeToUnix(timestamp, 13, 30, 0, 0,0,0); // Mon Nov 22 13:30:00 2021 Cal_05mm_8.dat
    ChamberTime[16]  = (Double_t) ChamberUNIXtime.first; 
    
    ChamberUNIXtime = TimeToUnix(timestamp, 13, 37, 0, 0,0,0); // Mon Nov 22 13:37:00 2021 Cal_05mm_9.dat
    ChamberTime[17]  = (Double_t) ChamberUNIXtime.first; 
    
    ChamberUNIXtime = TimeToUnix(timestamp, 13, 44, 0, 0,0,0); // Mon Nov 22 13:44:00 2021 Cal_05mm_10.dat
    ChamberTime[18]  = (Double_t) ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 13, 50, 0, 0,0,0); // Mon Nov 22 13:50:00 2021 Cal_05mm_11.dat
    ChamberTime[19]  = (Double_t) ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 13, 57, 0, 0,0,0); // Mon Nov 22 13:57:00 2021 Cal_05mm_12.dat
    ChamberTime[20]  = (Double_t) ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 14, 04, 0, 0,0,0); // Mon Nov 22 14:04:00 2021 Cal_05mm_13.dat
    ChamberTime[21]  = (Double_t) ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 14, 10, 0, 0,0,0); // Dummy file Mon Nov 22 14:09:00 2021 Cal_05mm_14.dat
    ChamberTime[22]  = (Double_t) ChamberUNIXtime.first;
    
    ChamberUNIXtime = TimeToUnix(timestamp, 14, 14, 0, 0,0,0); // Mon Nov 22 14:14:00 2021 Cal_05mm_15.dat
    ChamberTime[23]  = (Double_t) ChamberUNIXtime.first;
    
    //cout<<endl;
    
    /*cout<<"------------Chamber is at the beam center---------"<<endl;
    
    for(Int_t i = 0; i < 24; i++){//Loop for pritting chamber time
        
        TString ChamberTime_1 = Form("%f", ChamberTime[i]);
        cout<<"This is time for Chamber center "<<ChamberTime_1<<endl;
        
    }
    
    cout<<"----------------------------------------------------"<<endl;	
    cout<<endl;*/
    
    Double_t st1[100];
    Double_t st2[100];
    Double_t st3[100];
    Double_t st4[100];
    //Vectors of pairs: beginning and end of approximation intervals
    //Here is used the function TimeToUnix. The function returns vectors of pairs of time in UNIX format.
    //TimeToUnix(timestamp, hours, minutes, seconds, hours2, minutes2, seconds2)
    //hours, minutes, seconds     - the beginning of the approximation interval
    //hours2, minutes2, seconds2  - the end of the approximation interval
   pair<Int_t, Int_t> FitRange = TimeToUnix(timestamp, 11, 17, 0, 11, 19, 0); // Iteration 0: before jump //11, 18, 20, 11, 19, 0
    st1[0] = (Double_t) FitRange.first; //the beginning of the approximation interval (beam is at the center of chamber) st1
    st2[0] = (Double_t) FitRange.second;//the end of the approximation interval                                          st2
    
   FitRange = TimeToUnix(timestamp, 11, 19, 10, 11, 19, 40);// Iteration 0: after jump
    st3[0] = (Double_t) FitRange.first; //the beginning of the approximation interval (beam is at the center of timepix) st3
    st4[0] = (Double_t) FitRange.second;//the end of the approximation interval                                          st4
    
   FitRange = TimeToUnix(timestamp, 11, 27, 30, 11, 28, 30); //Iteration 1: before jump
    st1[1] = (Double_t) FitRange.first;
    st2[1] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 11, 29, 50, 11, 30, 35); //Iteration 1: after jump
    st3[1] = (Double_t) FitRange.first;
    st4[1] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 11, 37, 40, 11, 38, 5); //Iteration 2: before jump
    st1[2] = (Double_t) FitRange.first;
    st2[2] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 11, 38, 55, 11, 39, 45); //Iteration 2: after jump
    st3[2] = (Double_t) FitRange.first;
    st4[2] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 11, 47, 5, 11, 47, 50); //Iteration 3: before jump
    st1[3] = (Double_t) FitRange.first;
    st2[3] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 11, 48, 20, 11, 49, 0); //Iteration 3: after jump
    st3[3] = (Double_t) FitRange.first;
    st4[3] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 11, 53, 20, 11, 54, 25); //Iteration 4: before jump
    st1[4] = (Double_t) FitRange.first;
    st2[4] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 11, 55, 0, 11, 55, 50); //Iteration 4: after jump
    st3[4] = (Double_t) FitRange.first;
    st4[4] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 12,0, 25, 12, 1, 5); //Iteration 5: before jump
    st1[5] = (Double_t) FitRange.first;
    st2[5] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 12, 1, 25, 12, 2,0); //Iteration 5: after jump
    st3[5] = (Double_t) FitRange.first;
    st4[5] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 12, 4, 50, 12, 5, 55); //Iteration 6: before jump
    st1[6] = (Double_t) FitRange.first;
    st2[6] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 12, 6, 40, 12, 7, 15); //Iteration 6: after jump
    st3[6] = (Double_t) FitRange.first;
    st4[6] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 12, 11, 15, 12,12, 0); //Iteration 7: before jump
    st1[7] = (Double_t) FitRange.first;
    st2[7] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 12, 12, 25, 12, 13, 0); //Iteration 7: after jump
    st3[7] = (Double_t) FitRange.first;
    st4[7] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 12, 16, 20, 12, 17, 15); //Iteration 8: before jump
    st1[8] = (Double_t) FitRange.first;
    st2[8] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 12, 17, 40, 12, 18, 0); //Iteration 8: after jump
    st3[8] = (Double_t) FitRange.first;
    st4[8] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 12, 46, 0, 12, 47, 35); //Iteration 9: before jump
    st1[9] = (Double_t) FitRange.first;
    st2[9] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 12, 47, 55, 12, 48, 30); //Iteration 9: after jump
    st3[9] = (Double_t) FitRange.first;
    st4[9] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 0, 50, 13, 1, 40); //Iteration 10: before jump
    st1[10] = (Double_t) FitRange.first;
    st2[10] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 2, 0, 13, 2, 25); //Iteration 10: after jump
    st3[10] = (Double_t) FitRange.first;
    st4[10] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 7, 0, 13, 7, 30); //Iteration 11: before jump
    st1[11] = (Double_t) FitRange.first;
    st2[11] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 7, 30, 13, 8, 0); //Iteration 11: after jump
    st3[11] = (Double_t) FitRange.first;
    st4[11] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 11, 20, 13, 12, 10); //Iteration 12: before jump
    st1[12] = (Double_t) FitRange.first;
    st2[12] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 12, 55, 13, 13, 20); //Iteration 12: after jump
    st3[12] = (Double_t) FitRange.first;
    st4[12] = (Double_t) FitRange.second;

   FitRange = TimeToUnix(timestamp, 13, 15, 0, 13, 15, 20); //Iteration 13: before jump
    st1[13] = (Double_t) FitRange.first;
    st2[13] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 15, 22, 13, 15, 50); //Iteration 13: after jump
    st3[13] = (Double_t) FitRange.first;
    st4[13] = (Double_t) FitRange.second;

   FitRange = TimeToUnix(timestamp, 13, 19, 45, 13, 20, 40); //Iteration 14: before jump
    st1[14] = (Double_t) FitRange.first;
    st2[14] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 21, 0, 13, 21,30); //Iteration 14: after jump
    st3[14] = (Double_t) FitRange.first;
    st4[14] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 25, 40, 13, 26, 20); //Iteration 15: before jump
    st1[15] = (Double_t) FitRange.first;
    st2[15] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 26, 30, 13, 27, 0); //Iteration 15: after jump
    st3[15] = (Double_t) FitRange.first;
    st4[15] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 30, 30, 13, 31, 20); //Iteration 16: before jump
    st1[16] = (Double_t) FitRange.first;
    st2[16] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 32, 0, 13, 33, 0); //Iteration 16: after jump
    st3[16] = (Double_t) FitRange.first;
    st4[16] = (Double_t) FitRange.second;

   FitRange = TimeToUnix(timestamp, 13, 37, 5, 13, 38, 10); //Iteration 17: before jump //13, 37, 45, 13, 38, 10
    st1[17] = (Double_t) FitRange.first;
    st2[17] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 38, 30, 13, 39, 0); //Iteration 17: after jump
    st3[17] = (Double_t) FitRange.first;
    st4[17] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 45, 20, 13, 46, 20); //Iteration 18: before jump
    st1[18] = (Double_t) FitRange.first;
    st2[18] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 46, 25, 13, 47, 0); //Iteration 18: after jump
    st3[18] = (Double_t) FitRange.first;
    st4[18] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 52, 45, 13, 53, 45); //Iteration 19: before jump //13, 53, 0, 13, 53, 45
    st1[19] = (Double_t) FitRange.first;
    st2[19] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 54, 0, 13, 54, 35); //Iteration 19: after jump
    st3[19] = (Double_t) FitRange.first;
    st4[19] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 13, 58, 40, 13, 59, 50); //Iteration 20: before jump '//13, 59, 0, 13, 59, 50
    st1[20] = (Double_t) FitRange.first;
    st2[20] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 14, 0, 0, 14, 0, 30); //Iteration 20: after jump
    st3[20] = (Double_t) FitRange.first;
    st4[20] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 14, 4, 0, 14, 5, 30); //Iteration 21: before jump //14, 5, 0, 14, 5, 30
    st1[21] = (Double_t) FitRange.first;
    st2[21] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 14, 5, 50, 14, 6, 15); //Iteration 21: after jump
    st3[21] = (Double_t) FitRange.first;
    st4[21] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 14, 12, 30, 14, 12, 50); //Iteration 22: before jump
    st1[22] = (Double_t) FitRange.first;
    st2[22] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 14, 13, 0, 14, 13, 20); //Iteration 22: after jump
    st3[22] = (Double_t) FitRange.first;
    st4[22] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 14, 16, 30, 14, 17, 30); //Iteration 23: before jump
    st1[23] = (Double_t) FitRange.first;
    st2[23] = (Double_t) FitRange.second;
    
   FitRange = TimeToUnix(timestamp, 14, 17, 50, 14, 18, 20); //Iteration 23: after jump
    st3[23] = (Double_t) FitRange.first;
    st4[23] = (Double_t) FitRange.second;
    
    vector<pair<Double_t, Double_t>> timeScan = TimepixCounter();//Vector pairs for time range of timepix 
    int counter = 0;
    //cout<<endl;
    //cout<<"Calibration without plate "<<endl;
 
    for(Int_t i = 0; i <= 23; i++){ //The loop may be used for printing name files and time in GMT format
        
        startMeas.push_back(ChamberTime[i]);// Start time of iteration
        startMeasTIMEPIX.push_back(timeScan[i].first);
        stopMeas.push_back(timeScan[i].second);  //It is the time from TimepixCounter() (the end of measurement) for event when timepix is at the beam center. 
        
        vector<Double_t> dummy22;
        dummy22.push_back(timeScan[i].second);
        vector<TDatime> UNIX11 = TimeToGMT(dummy22);//Time to GMT format for comfortable displaying the time into the terminal

        //UNIX11[0].Print();
        TString stra  = Form("%f", startMeas[i]);//It is necessary for displaying the data in format 12323... insted of 123e-3
        TString stra1 = Form("%f", stopMeas[i]);
        //cout<<endl;
        //cout<<"Start "<<stra << " " <<"Stop "<<stra1<<endl;//Printing the result

        counter++; //counter - number of timepix files
    }
   info g[100]; //The structure is used like storage important data

   vector<Twinpad*> dummyCanv(counter);
   vector<TGraph*>  grCalibration_0mm(counter); //The graph of ionization current
   vector<TGraph*>  grPad(counter);            //The graph of position

   npoints = -1;
   
   Double_t startIteration        = 0; //Start of scanning
   Double_t startTimepixIteration = 0; //Start of timepix measurement
   Double_t stopIteration         = 0; //Stop of timepix measurement
   Double_t stopChamber           = 0; //Stop of scanning
   
   vector<Double_t> yaxisMin(counter); //Minimum value on a graph
   vector<Double_t> yaxisMax(counter); //Maximum value on a graph
   Double_t temp  = 0;  
   Double_t temp1 = 0; 
   
   vector<Double_t> nPositionBeam;
   vector<Double_t> nTime;
    
   io = 0; //Counter of iteration
 //TFile *CurrentHist = new TFile("CerrentHist.root", "RECREATE"); //
    //Mean value of current when beam:
    Double_t mean;  //at the center of chamber
    Double_t mean1; //at the center of timepix
    Double_t mean2; //at the center of timepix (during timepix measurements)
    //Error value of current when beam:
    Double_t Errmean; //error for value 'mean'
    Double_t Errmean1; //error for value 'mean1'
    Double_t Errmean2; //error for value 'mean2'
    
    Double_t r;            //ration between current (beam is at the center chamber/timepix)
    Double_t rErr;         //ration between current (beam is at the center chamber/timepix)

    Double_t CorrCurrent;  //correspond ionization current (r*mean2)
    Double_t CorrCurrtError;//r*Errmean2
    
    Double_t fluxErrors;
    
    //int      numberPoints;
    //Double_t sumPoints;
    //Double_t meanTimepixCurrent;
    //vector<Double_t> meanTimepixIC;
   for(int k = 0; k < counter; k++){ //The main loop: filling graphs by data point, filling the structure info by calculated values
       //Here is created the empty graph
       dummyCanv[k] = new Twinpad(io,1.4,0.4);

       grCalibration_0mm[k] = new TGraph(0);
       SetGraph(grCalibration_0mm[k], " ", " ",kTRUE); 
       npoints = 0;
       
       TGraph *grCalibration_0mm_1 = new TGraph(0);
       TGraph *grPad_1             = new TGraph(0);
       
       grCalibration_0mm[k] = grCalibration_0mm_1;
       grPad[k] = grPad_1;
       
       SetGraph(grCalibration_0mm[k], "Time [sec]", "Ionization current [A] ", kTRUE);
       SetGraph(grPad[k], "Time [sec]", "Position Y [mm] ", kTRUE);
       
       temp = 0;   

    for(Int_t i = 0; i < size; i++){                                           //loop over all logfile entries
      if(startMeas[k] < timestamp[i] && timestamp[i] <= stopMeas[k]){          //select the time region of interest
            
            npoints = grCalibration_0mm[k] -> GetN();                          //check how many points we have in the graph
            grCalibration_0mm[k]->Set(npoints+1);                              //increase the number by 1
            //cout<<IC[i]<<endl;
            grCalibration_0mm[k]->SetPoint(npoints, timestamp[i]*1.0, IC[i]);  // add to the graph new value
            grPad[k]->SetPoint(npoints, timestamp[i]*1.0, y[i]*1.0);
            
            if(IC[i] > temp){ //Search max value
               temp  = IC[i];
            }
            temp1 = temp;
            if(IC[i] < temp1){ //and min
               temp1 = IC[i];
            }
            
            if(y[i] != y[0] && y[i-1] -y[i] == 10){ //The time when beam change its position
            
                nPositionBeam.push_back(y[i-3]);
                nTime.push_back(timestamp[i-3]);
                
            }
      }
    }

     yaxisMax[k] = temp; 
     yaxisMin[k] = temp1;
       
     //cout<<yaxisMax[k]<<endl; 
     //cout<<yaxisMin[k]<<endl; 

     startIteration        = startMeas[k];       //Start of scanning
     startTimepixIteration = startMeasTIMEPIX[k];//Start of timepix measurement
     stopIteration         = stopMeas[k];       //Stop of timepix measurement
     stopChamber           = nTime[k];          //Stop of scanning
    //The Graph is a class for ploting and processing data
     Graph plot {io, startIteration, stopChamber, startTimepixIteration, stopIteration, yaxisMin[k], yaxisMax[k], grCalibration_0mm[k], grPad[k], dummyCanv[k]};
       plot.FirstGraph(io, startIteration, stopChamber, startTimepixIteration, stopIteration, yaxisMin[k], yaxisMax[k]);
       plot.SecondGraph(io,startIteration, stopIteration, yaxisMin[k], yaxisMax[k]);
       
       //Double_t meanTimepix = plot.MeanTimepixCurrent(startTimepixIteration, stopIteration, grCalibration_0mm[k]);
       
       plot.Graph::FitGraph(io, st1[k], st2[k], grCalibration_0mm[k], 0); //The first graph is fitted. The current before a jump
       TString name = Form("const_form%d%i",io, 0);
       TF1* myfunc = grCalibration_0mm[k]->GetFunction(name);
         if (!myfunc) { mean = 0; Errmean = 0; } //The if-else conditions is used for checking result of fitting
         else {
          mean    = myfunc->GetParameter(0);
          Errmean = myfunc->GetParError(0);
         }
       
       plot.Graph::FitGraph(io, st3[k], st4[k], grCalibration_0mm[k], 1); //The current after a jump
       TString name1 = Form("const_form%d%i",io, 1);
       TF1* myfunc1 = grCalibration_0mm[k]->GetFunction(name1);
         if (!myfunc1) { mean1 = 0; Errmean1 = 0;}
         else {
           mean1    = myfunc1->GetParameter(0);
           Errmean1 = myfunc1->GetParError(0);
           }
      
       plot.Graph::FitGraph(io, startTimepixIteration, stopIteration, grCalibration_0mm[k], 2); //The mean value of current during timepix scan
       TString name2 = Form("const_form%d%i",io, 2);
       TF1* myfunc2 = grCalibration_0mm[k]->GetFunction(name2);
         if (!myfunc2) { mean2 = 0; Errmean2 = 0;}
         else {
           mean2    = myfunc2->GetParameter(0);
           Errmean2 = myfunc2->GetParError(0);
           }
       
       r = mean/mean1;
       rErr = r*sqrt((Errmean1/mean1)*(Errmean1/mean1)+(Errmean/mean)*(Errmean/mean));
       CorrCurrent = mean2*r;
       CorrCurrtError = CorrCurrent*sqrt((Errmean2/mean2)*(Errmean2/mean2)+(rErr/r)*(rErr/r));
       //ProtonFlux_Error[k].second      //sigma(N_area, N_max, N_energy)
       fluxErrors = ProtonFlux_Error[k].first*sqrt((Errmean2/mean2)*(Errmean2/mean2)+(ProtonFlux_Error[k].second/ProtonFlux_Error[k].first)*(ProtonFlux_Error[k].second/ProtonFlux_Error[k].first));
       
       g[k] = {startIteration, st1[k], st2[k], st3[k], st4[k], stopIteration, 
               FileTimepixNames[k], ProtonFlux_Error[k].first, fluxErrors, //ProtonFlux_Error[k].second, 
               mean, mean1, Errmean, Errmean1, mean2, Errmean2, 
               r, rErr,
               CorrCurrent, CorrCurrtError}; //The structure info
       
       plot.Graph::GLegend(mean, mean1, mean2, Errmean, Errmean1, Errmean2);
       
       
        //struct info{                  Information about structure data

          //Double_t t0;                      The start of scan
          //Double_t t1;                      The beginning of the approximation interval (Chamber)
          //Double_t t2;                      The end of the approximation interval (Chamber)
          //Double_t t3;                      The beginning of the approximation interval (Timepix)
          //Double_t t4;                      The end of the approximation interval (Timepix)
          //Double_t t5;                      The end of scan

          //string timepixName;               Name of timepix file
          //Double_t FluxProton;              The amount of proton flux
          //Double_t ErFluxProton;            Sigma for proton flux
    
          //Double_t CenterChamber;           Ionization current (beam is at the chamber center).// Before the jump
          //Double_t CenterTimepix;           Ionization current (beam is at the timepix center ).//After the jump
       
          //Double_t ErrorCenterChamber;      Error of ionization current (beam is at the chamber center)
          //Double_t ErrorCenterTimepix;      Error of ionization current (beam is at the timepix center ) 
       
          //Double_t ICCenterTimepix;         Mean value of ionization current (beam is at the timepix center ) 
          //Double_t ErrorICCenterTimepix;    Mean value of ionization current (beam is at the timepix center ) 
       
          //Double_t ratio;                   The ratio between current when beam is at center of chamber and timepix I_ch/I_timepix
          //Double_t ratioErr;                The error of ratio between current when beam is at center of chamber and timepix I_ch/I_timepix
          //Double_t CorrespondCurrent        i = ratio*ICCenterTimepix=(CenterChamber/CenterTimepix)*ICCenterTimepix
          //Double_t CorrespondCurrentError   error = ErrorCenterTimepix*CorrespondCurrent 

       //};
       
       //cout<<g[k].timepixName<<endl;
       //cout<<g[k].ICCenterTimepix<<endl;

    io++;
   }

    //for(size_t i = 0; i < nPositionBeam.size(); i++) {
        //cout<<nPositionBeam[i]<<endl;
        //cout<<nTime[i]<<endl;
    //}
    cout<<"------------------------"<<endl;
    cout<<"Size of nPositionBeam: " << nPositionBeam.size() << endl;
    cout<<"Number of graph: "       << counter              << endl;
    //cout<<"------------------------"<<endl;
    //cout<<nTime.size()<<endl;
    //CurrentHist->Close();
    

    
    TGraphErrors* CalibrationCurve  = new TGraphErrors(8);
    TGraphErrors* CalibrationCurve1 = new TGraphErrors(counter);
    
    TGraph* scaleFactor = new TGraph(0);
    TGraph* scaleFactor1 = new TGraph(0);
    
    npoints = counter;
    cout<<counter<<endl;
    /*for(int j = 0; j < counter; j++){ //The loop for filling graph by data from structute info
       
        if(j == 22 || j == 11) continue;
        if(j <= 8){
              CalibrationCurve->Set(j);
              CalibrationCurve->SetPoint(j, g[j].CorrespondCurrent, g[j].FluxProton);
              CalibrationCurve->SetPointError(j, g[j].CorrespondCurrentError, g[j].ErFluxProton);
        }
        else{
              CalibrationCurve1->Set(j);
              CalibrationCurve1->SetPoint(j, g[j].CorrespondCurrent, g[j].FluxProton);
              CalibrationCurve1->SetPointError(j, g[j].CorrespondCurrentError, g[j].ErFluxProton);
        }


    }*/
    int n =0;
    for(int j = 0; j <= 8; j++){ //The loop for filling graph by data from structute info
  
        CalibrationCurve->Set(j);
        CalibrationCurve->SetPoint(j, g[j].CorrespondCurrent, g[j].FluxProton);
        CalibrationCurve->SetPointError(j, g[j].CorrespondCurrentError, g[j].ErFluxProton);
        
        scaleFactor->Set(j);
        scaleFactor->SetPoint(j, j, g[j].ratio);
        
        
    }
    cout<<"Counter n: "<<n<<endl;
    
    for(int j = 9; j < counter; j++){ //The loop for filling graph by data from structute info
  
        if(j == 22 || j == 11) continue;
        if(g[j].timepixName == "Cal_05mm_1.dat.root") continue;
        CalibrationCurve1->Set(j);
        CalibrationCurve1->SetPoint(j, g[j].CorrespondCurrent, g[j].FluxProton);
        CalibrationCurve1->SetPointError(j, g[j].CorrespondCurrentError, g[j].ErFluxProton);
        //cout<<endl;
        //cout<<g[j].timepixName<<" "<<g[j].FluxProton<<" "<<g[j].CorrespondCurrent<<endl;
    }
    
    gStyle->SetOptFit(0111);
    TCanvas* canv = new TCanvas("canv", "Calibration curve",0,0,800,500);
    canv->cd();
    CalibrationCurve->SetTitle("Calibration curve");
    CalibrationCurve->GetXaxis()->SetTitle("Ionization current, A");
    CalibrationCurve->GetYaxis()->SetTitle("Proton flux, cm^{2} s^{-1}");
    CalibrationCurve->SetMarkerStyle(10);
    CalibrationCurve->Draw("AP");

    TF1 *func = new TF1("f1","[0]+[1]*x",0, 12E-11);
    func->FixParameter(0, 0);
    func->SetParLimits(1, 5E15, 6E15);
    CalibrationCurve->Fit("f1", "RE", 0, 12E-12);

    TCanvas* canv1 = new TCanvas("canv1", "Calibration curve with plate",0,0,800,500);
    canv1->cd();
    CalibrationCurve1->SetTitle("Calibration curve");
    CalibrationCurve1->GetXaxis()->SetTitle("Ionization current, A");
    CalibrationCurve1->GetYaxis()->SetTitle("Proton flux, cm^{2} s^{-1}");
    CalibrationCurve1->SetMarkerStyle(10);
    CalibrationCurve1->Draw("AP");
    CalibrationCurve1->Fit("f1", "RE", 0, 12E-12);
    
    TCanvas* canv2 = new TCanvas("canv2", " ",0,0,800,500);
    canv2->cd();
    scaleFactor->SetMarkerStyle(25);
    scaleFactor->Draw("AP");
    scaleFactor->Fit("pol0");
    
    
    TF1* mycurve  = CalibrationCurve->GetFunction("f1");
    TF1* mycurve1 = CalibrationCurve1->GetFunction("f1");

    Double_t calibrationScale  = mycurve->GetParameter(1);
    Double_t calibrationScale1 = mycurve1->GetParameter(1);
    
    cout<<"Calibration parameter without plate: "<<calibrationScale<<endl<<"Calibration parameter with plate: "<<calibrationScale1<<endl;

    pair<Double_t, Double_t> resultCalibration = make_pair(calibrationScale, calibrationScale1);

    return resultCalibration;
}

//----------------------------------------------//

void SetHist(TH1* h,TString titx, TString tity){

  h->GetXaxis()->SetTitle(titx.Data());
  h->GetYaxis()->SetTitle(tity.Data());
  h->GetXaxis()->SetLabelSize(0.05);
  h->GetYaxis()->SetLabelSize(0.05);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleOffset(1.3);
  h->GetXaxis()->SetTitleOffset(1.1);

}


//--------------------------------------------------------------------------------
//SELECT TIME WINDOWS OF INTEREST//
//--------------------------------------------------------------------------------

pair<Int_t, Int_t>  TimeToUnix(const vector<Int_t>& timestamp, Int_t hour, Int_t min, Int_t sec, Int_t hour2, Int_t min2, Int_t sec2){

TDatime dummy, start, stop;
Int_t year, month, day;
dummy.Set(timestamp[0]);
year  = dummy.GetYear();
month = dummy.GetMonth();
day   = dummy.GetDay();

start.Set(year, month, day, hour, min, sec);
stop.Set(year, month, day, hour2, min2, sec2);
	
Int_t startUnixTime = start.Convert(0);
Int_t stopUnixTime = stop.Convert(0);

pair<Int_t, Int_t>  StartStop = make_pair (startUnixTime,stopUnixTime);
	
return StartStop; 
	
}

//--------------------------------------------------------------------------------
void SetGraph(TGraph* gr, TString titleX, TString titleY, Bool_t btime){
   //format the graph	
   if(btime){
      gr->GetXaxis()-> SetTimeFormat("%H:%M %F1970-01-01 00:00:00");
      gr->GetXaxis()-> SetTimeDisplay(1);
   }
   gr->SetMarkerStyle(20);
   gr->SetMarkerSize(0.5);
   gr->SetMarkerColor(1);
   gr->GetXaxis()->SetTitleSize(0.05);
   gr->GetXaxis()->SetLabelSize(0.04);
   gr->GetYaxis()->SetTitleSize(0.05);
   gr->GetYaxis()->SetLabelSize(0.04);

   gr->GetXaxis()->SetTitle(titleX.Data());
   gr->GetYaxis()->SetTitle(titleY.Data());

   return;
}
//--------------------------------------------------------------------------------
void SetCanvas(TCanvas *cc){
   // set boarders of the canvas to exploit the available space
   cc->SetLeftMargin(0.17);
   cc->SetRightMargin(0.05);
   cc->SetTopMargin(0.05);
   cc->SetBottomMargin(0.15);
}
//--------------------------------------------------------------------------------
Double_t fitf(Double_t *x, Double_t *par){
   Double_t arg = 0;
   if (par[2]!=0) arg = (x[0] - par[1])/par[2];
   Double_t fun = par[0]*TMath::Exp(-0.5*arg*arg);

   return fun;
}

//--------------------------------------------------------------------------------
void SavePNGandEPS(TCanvas** arrayCan, Int_t nobr){
   //this function prints to promptline commands which are needed to save the figure
   //you should copy them by mouse to the root promptline and execute them

   //cout<<endl<<endl<<endl;
   //PNG IMAGES
   cout<<"TImage *img = TImage::Create();"<<endl;
   for(int j=0; j<nobr;j++){
      TString nameBase =arrayCan[j]->GetTitle();
      if(nameBase.BeginsWith("x")) continue;
      TString namePNG = nameBase + ".png";
      cout<<"img->FromPad("<<arrayCan[j]->GetName()<<"); img->WriteImage(\""<<namePNG.Data()<<"\");"<<endl;
   }
/*
   //EPS IMAGES
   for(int j=0; j<nobr;j++){
      TString nameBase =arrayCan[j]->GetTitle();
      if(nameBase.BeginsWith("x")) continue;
      TString nameEPS = nameBase + ".eps";
      cout<<arrayCan[j]->GetName()<<"->SaveAs(\""<<nameEPS.Data()<<"\");"<<endl;
   }
*/
}