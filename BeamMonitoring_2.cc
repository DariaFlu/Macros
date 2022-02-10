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

#include "TimepixCounter.cc"
#include "Twinpad.h"
#include "Graph.cc"


using namespace std;

//HERE DECLARE FUNCTION HEADERS
Double_t fitf(Double_t *x, Double_t *par);
void SetGraph(TGraph* gr, TString titleX, TString titleY, Bool_t btime);
void SetCanvas(TCanvas *cc);
void SavePNGandEPS(TCanvas** arrayCan, Int_t nobr);
pair<Int_t, Int_t>  TimeToUnix(const vector<Int_t>& timestamp, Int_t hour, Int_t min, Int_t sec, Int_t hour2, Int_t min2, Int_t sec2);


//void Draw(const vector<Int_t>& timestamp, Double_t startIteration, Double_t startTimepixIteration, Double_t stopIteration, TGraph* grCalibration_0mm, TGraph* grPad, Twinpad* c, int k, Double_t  yaxisMax,Double_t  yaxisMin, Double_t stopChamber  );//void Draw(const vector<Int_t>& timestamp, Int_t hour, Int_t min, Int_t sec, Int_t hour2, Int_t min2, Int_t sec2, Double_t startDummy, Double_t stopDummy);
void SetGraph(TGraph* gr, TString titleX, TString titleY, Bool_t btime);

//HERE STARTS THE MAIN FUNCTION
int BeamMonitoring_2(){
	
   gSystem->cd("/home/dnn1488/Desktop/Root/DariaCalib2022Nov22/BeamMonitoring");
	
   vector<Int_t> timestamp; //timestamp
   vector<Int_t> x;    //position x [mm]
   vector<Int_t> y;    //position y [mm]
	
   vector<Double_t> IC; //ionization chamber current [A]

   //+++++++++++++++++++++++++++++++++++++
   //OPEN FILE AND READ DATA   
   //+++++++++++++++++++++++++++++++++++++
   ifstream file;
  // file.open("DariaCalib2022Nov22/BeamMonitoring/LOG_1637575860.txt", ifstream::in);
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

   pair<Int_t, Int_t> UnixTime = TimeToUnix(timestamp,12,24,0,12,40,0);
   Int_t startUnixTime_SCANS   = UnixTime.first;
   Int_t stopUnixTime_SCANS    = UnixTime.second;
	
   UnixTime = TimeToUnix(timestamp,12,25,12,12,28,0);
   Int_t startUnixTime_SCANX1  = UnixTime.first;
   Int_t stopUnixTime_SCANX1   = UnixTime.second;
	
   UnixTime = TimeToUnix(timestamp,12,29,58,12,31,46);
   Int_t startUnixTime_SCANY1  = UnixTime.first;
   Int_t stopUnixTime_SCANY1   = UnixTime.second;
	
   UnixTime = TimeToUnix(timestamp,12,32,52,12,35,50);
   Int_t startUnixTime_SCANX2  = UnixTime.first;
   Int_t stopUnixTime_SCANX2   = UnixTime.second;
	
   UnixTime = TimeToUnix(timestamp,12,37,30,12,39,8);
   Int_t startUnixTime_SCANY2  = UnixTime.first;
   Int_t stopUnixTime_SCANY2   = UnixTime.second;

	// Time for the range 11:17 - 12:17
   UnixTime = TimeToUnix(timestamp,11,17,0,12,17,0);
   Int_t startCalibration  = UnixTime.first;
   Int_t stopCalibration   = UnixTime.second;
	
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   int size = timestamp.size();  //how many entries in total are in the input logfile
   Int_t npoints;                //the number of points in given  graph

   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   // Fill graphs that show position variation during longer period  
   // where one can idenfies the regions of interest 

   npoints = 0; //at the beginnng there are no points in the graph

   TGraph *grXvsTime = new TGraph(0); //this will show x versus time
   SetGraph(grXvsTime, "Time", "Position [mm]",kTRUE); 

   TGraph *grYvsTime = new TGraph(0); //this will show y versus time
   SetGraph(grYvsTime, "Time", "Position [mm]",kTRUE); 
   grYvsTime->SetMarkerColor(2);

   for(Int_t i=0; i<size; i++){ //loop over all inputs from the log file and selct just those which are in time range of interest
      if(startUnixTime_SCANS < timestamp[i] && timestamp[i] <= stopUnixTime_SCANS){
          npoints = grXvsTime->GetN(); //current number of points in the graph 
          grXvsTime->Set(npoints+1);   // this will be new number of points 
          grXvsTime->SetPoint(npoints, timestamp[i]*1.0, x[i]*1.0); //set the last point

	  //for y we will do analogous thing:
          grYvsTime->Set(npoints+1);  //increase number of points in the graph
          grYvsTime->SetPoint(npoints, timestamp[i]*1.0, y[i]*1.0); //set the last point
      }
   }
 	                       
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   // Fill graphs that show position variation during individual scans

   // Second X scan which is seen in the previous graph 
   TGraph *grScanX2 = new TGraph(0);
   SetGraph(grScanX2, "Position x [mm]", "Ionization chamber current [A]",kFALSE); 

   npoints = 0;

   //we will draw the profile using the second data point for given position,  this requires:
   // check whether the positon has changes
   // check whether this is the second data point
   Int_t  oldpos  = -1;    // old position
   Bool_t fillNextValue = kFALSE;  // boolean that will tell use whether we want to store this value

   for(Int_t i=0; i<size; i++){ //loop over all logfile entries
      if(startUnixTime_SCANX2 < timestamp[i] && timestamp[i] <= stopUnixTime_SCANX2){ //select the time region of interest

         if(oldpos!=x[i]){  //if the postion wrt previous data point changed do:
	     oldpos = x[i];  // this is new position
             fillNextValue = kTRUE;  //we would like to fill new data point
             continue;   //we can continue to read the second data point
	 }

	 if(fillNextValue){  // this will be valid when we are on the second data point for given position
            fillNextValue = kFALSE;  //we do not want to read the next data point at the same position
            oldpos = x[i];   //update old position
            
            npoints = grScanX2->GetN();    //check how many points we have in the graph
            grScanX2->Set(npoints+1);      //increase the number by 1
            
            grScanX2->SetPoint(npoints, x[i]*1.0, IC[i]);  // add to the graph new value
	 }
      }
   }
//+++++++++++++
	
   TGraph *grScanY2   = new TGraph(0);
   SetGraph(grScanY2, "Position y [mm]", "Ionization chamber current [A]",kFALSE); 

   npoints = 0;

   oldpos  = -1;    // old position
   fillNextValue = kFALSE;  // boolean that will tell use whether we want to store this value
	
	for(Int_t i=0; i<size; i++){ //loop over all logfile entries
      if(startUnixTime_SCANY2 < timestamp[i] && timestamp[i] <= stopUnixTime_SCANY2){ //select the time region of interest

         if(oldpos!=y[i]){  //if the postion wrt previous data point changed do:
	     oldpos = y[i];  // this is new position
             fillNextValue = kTRUE;  //we would like to fill new data point
             continue;   //we can continue to read the second data point
	 }

	 if(fillNextValue){  // this will be valid when we are on the second data point for given position
        fillNextValue = kFALSE;  //we do not want to read the next data point at the same position
	    oldpos = y[i];   //update old position
            
            npoints = grScanY2->GetN();    //check how many points we have in the graph
            grScanY2->Set(npoints+1);      //increase the number by 1
            
            grScanY2->SetPoint(npoints, y[i]*1.0, IC[i]);  // add to the graph new value
	 }
      }
   }
   //+++++++++++++Scan Y1

   TGraph *grScanY1   = new TGraph(0);
   SetGraph(grScanY1, "Position y [mm]", "Ionization chamber current [A]",kFALSE); 

   npoints = 0;

   oldpos  = -1;    // old position
   fillNextValue = kFALSE;  // boolean that will tell use whether we want to store this value

    for(Int_t i=0; i<size; i++){ //loop over all logfile entries
      if(startUnixTime_SCANY1 < timestamp[i] && timestamp[i] <= stopUnixTime_SCANY1){ //select the time region of interest

         if(oldpos!=y[i]){  //if the postion wrt previous data point changed do:
             oldpos = y[i];  // this is new position
             fillNextValue = kTRUE;  //we would like to fill new data point
             continue;   //we can continue to read the second data point
     }
    
     if(fillNextValue){  // this will be valid when we are on the second data point for given position
        fillNextValue = kFALSE;  //we do not want to read the next data point at the same position
        oldpos = y[i];   //update old position
            
            npoints = grScanY1->GetN();    //check how many points we have in the graph
            grScanY1->Set(npoints+1);      //increase the number by 1
            
            grScanY1->SetPoint(npoints, y[i]*1.0, IC[i]);  // add to the graph new value
     }
    }
   }

   //+++++++++++++Scan X1+++++++++++++//

   TGraph *grScanX1   = new TGraph(0);
   SetGraph(grScanX1, "Position x [mm]", "Ionization chamber current [A]",kFALSE); 

   npoints = 0;
   oldpos  = -1;    // old position

   fillNextValue = kFALSE;  // boolean that will tell use whether we want to store this value

    for(Int_t i=0; i<size; i++){ //loop over all logfile entries
      if(startUnixTime_SCANX1 < timestamp[i] && timestamp[i] <= stopUnixTime_SCANX1){ //select the time region of interest

         if(oldpos!=x[i]){  //if the postion wrt previous data point changed do:
         oldpos = x[i];  // this is new position
             fillNextValue = kTRUE;  //we would like to fill new data point
             continue;   //we can continue to read the second data point
     }

     if(fillNextValue){  // this will be valid when we are on the second data point for given position
        fillNextValue = kFALSE;  //we do not want to read the next data point at the same position
        oldpos = x[i];   //update old position
            
            npoints = grScanX1->GetN();    //check how many points we have in the graph
            grScanX1->Set(npoints+1);      //increase the number by 1
            
            grScanX1->SetPoint(npoints, x[i]*1.0, IC[i]);  // add to the graph new value
     }
      }
   }

   //++++++++++++++++++++
   //DRAWING 
   //++++++++++++++++++++

   TCanvas *can[100];
   TLegend  *leg;
   Int_t io = 0;  //counter of figures
   gStyle->SetOptTitle(0);

   //DRAW X AND Y POSITIONS DURING LONGER TIME INTERVAL
   can[io] = new TCanvas(Form("can%d",io), "ALL_SCANS_VS_TIME",0,0,800,500);
   SetCanvas((TCanvas*) can[io]);
   can[io]->cd();
   grXvsTime->SetMinimum(100);
   grXvsTime->SetMaximum(330);
   grXvsTime->Draw("AP");
   grYvsTime->Draw("Psame");

   leg = new TLegend(0.2,0.8,0.4,0.95," ","brNDC");
   leg->SetFillStyle(0); 
   leg->SetBorderSize(0); 
   leg->SetTextSize(0.05);
   leg->AddEntry((TObject*) grXvsTime, "x position","p");
   leg->AddEntry((TObject*) grYvsTime, "y position","p");
   leg->Draw();

   io++;

	
//Timepix center X = 181 mm  Y = 250 mm
/*********************************************************/
//Center of the ionization chamber X = 181 mm  Y = 260 mm

    //DRAW X POSITIONS DURING THE FIRT X SCAN 
   TF1 * fitGaus2[100];
       
   can[io] = new TCanvas(Form("can%d",io), "SCAN_X1",0,0,800,500);
   SetCanvas((TCanvas*) can[io]);
   can[io]->cd();
   grScanX1->Draw("AP");

   //fit the distribution with a gaussian
   fitGaus2[io] = new TF1("fitX1",fitf,155., 235., 3);
   fitGaus2[io]->SetParameters(14.e-12,199.,10.); 
   grScanX1->Fit((TF1*) fitGaus2[io],"RN");
   fitGaus2[io]->Draw("same");

   leg = new TLegend(0.2,0.7,0.6,0.9," ","brNDC");
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.045);
   leg->AddEntry((TObject*) fitGaus2[io], "Gaussian fit","l");
   leg->AddEntry((TObject*) 0, Form("#mu = %.1f #pm %.1f mm",    fitGaus2[io]->GetParameter(1), fitGaus2[io]->GetParError(1))," ");
   leg->AddEntry((TObject*) 0, Form("#sigma = %.1f #pm %.1f mm", fitGaus2[io]->GetParameter(2), fitGaus2[io]->GetParError(2))," ");  
   leg->Draw();

	
   io++;
	
   //DRAW Y POSITIONS DURING THE FIRT Y SCAN 
   TF1 * fitGaus4[100];
	   
   can[io] = new TCanvas(Form("can%d",io), "SCAN_Y1",0,0,800,500);
   SetCanvas((TCanvas*) can[io]);
   can[io]->cd();
   grScanY1->Draw("AP");

   //fit the distribution with a gaussian
   fitGaus4[io] = new TF1("fitY1",fitf,230., 290., 3);
   fitGaus4[io]->SetParameters(16.e-12,260.,10.); 
   grScanY1->Fit((TF1*) fitGaus4[io],"RN");
   fitGaus4[io]->Draw("same");

   leg = new TLegend(0.2,0.7,0.6,0.9," ","brNDC");
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.045);
   leg->AddEntry((TObject*) fitGaus4[io], "Gaussian fit","l");
   leg->AddEntry((TObject*) 0, Form("#mu = %.1f #pm %.1f mm",    fitGaus4[io]->GetParameter(1), fitGaus4[io]->GetParError(1))," ");
   leg->AddEntry((TObject*) 0, Form("#sigma = %.1f #pm %.1f mm", fitGaus4[io]->GetParameter(2), fitGaus4[io]->GetParError(2))," ");  
   leg->Draw();
    
   Double_t TimepixCenter_Y = 249;
   Double_t ChamberCenter_Y = 259;
	
   Double_t meanY_ChamberCenter1 = fitGaus4[io]->GetParameter(1);

   Double_t TimepixCenter_IC1 = fitGaus4[io]->Eval(TimepixCenter_Y);
   Double_t ChamberCenter_IC1 = fitGaus4[io]->Eval(ChamberCenter_Y);
	
   Double_t ChamberCenter_IC11 = fitGaus4[io]->Eval(meanY_ChamberCenter1);
	
   Double_t ratioChamberTimepix11 = TimepixCenter_IC1/ChamberCenter_IC1;
   Double_t ratioChamberTimepix12 = TimepixCenter_IC1/ChamberCenter_IC11;
    
   Double_t r = ChamberCenter_IC1/TimepixCenter_IC1;

   cout<<"__________________________________________________"<<endl;
   cout<<"I_t/I_c for Y Scan = "<<ratioChamberTimepix11<<endl;
   cout<<"I_t/I_c for Y Scan, where center of chamber is mean value of gauss function = "<<ratioChamberTimepix12<<endl;
   cout<<"SCALE FACTOR: "<<r<<endl;
   cout<<"__________________________________________________"<<endl;
		
   cout<<"Chamber = "<<ChamberCenter_Y<<endl;
   cout<<"Timepix = "<<TimepixCenter_Y<<endl;
   cout<<"Chamber center by fitting = "<<meanY_ChamberCenter1<<endl;
   cout<<"__________________________________________________"<<endl;
	
   io++;
	
   //DRAW X POSITIONS DURING THE SECOND X SCAN 
   TF1 * fitGaus[100];
	   
   can[io] = new TCanvas(Form("can%d",io), "SCAN_X2",0,0,800,500);
   SetCanvas((TCanvas*) can[io]);
   can[io]->cd();
   grScanX2->Draw("AP");

   //fit the distribution with a gaussian
   fitGaus[io] = new TF1("fitX",fitf,120., 255., 3);
   fitGaus[io]->SetParameters(4.e-12,195.,22.); 
   grScanX2->Fit((TF1*) fitGaus[io],"RN");
   fitGaus[io]->Draw("same");

   leg = new TLegend(0.2,0.7,0.6,0.9," ","brNDC");
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.045);
   leg->AddEntry((TObject*) fitGaus[io], "Gaussian fit","l");
   leg->AddEntry((TObject*) 0, Form("#mu = %.1f #pm %.1f mm",    fitGaus[io]->GetParameter(1), fitGaus[io]->GetParError(1))," ");
   leg->AddEntry((TObject*) 0, Form("#sigma = %.1f #pm %.1f mm", fitGaus[io]->GetParameter(2), fitGaus[io]->GetParError(2))," ");  
   leg->Draw();

	
   io++;

   //DRAW Y POSITIONS DURING THE SECOND Y SCAN 
   TF1 * fitGaus1[100];
	   
   can[io] = new TCanvas(Form("can%d",io), "SCAN_Y2",0,0,800,500);
   SetCanvas((TCanvas*) can[io]);
   can[io]->cd();
   grScanY2->Draw("AP");

   //fit the distribution with a gaussian
   fitGaus1[io] = new TF1("fitY",fitf,210.,310., 3);
   fitGaus1[io]->SetParameters(5.e-12,261.5,20.5); 
   grScanY2->Fit((TF1*) fitGaus1[io],"RN");
   fitGaus1[io]->Draw("same");

   leg = new TLegend(0.6,0.7,1.1,0.9," ","brNDC");
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.045);
   leg->AddEntry((TObject*) fitGaus1[io], "Gaussian fit","l");
   leg->AddEntry((TObject*) 0, Form("#mu = %.1f #pm %.1f mm",    fitGaus1[io]->GetParameter(1), fitGaus1[io]->GetParError(1))," ");
   leg->AddEntry((TObject*) 0, Form("#sigma = %.1f #pm %.1f mm", fitGaus1[io]->GetParameter(2), fitGaus1[io]->GetParError(2))," ");  
   leg->Draw();


   Double_t TimepixCenter_IC = fitGaus1[io]->Eval(TimepixCenter_Y);
   Double_t ChamberCenter_IC = fitGaus1[io]->Eval(ChamberCenter_Y);
	
   Double_t meanY_ChamberCenter = fitGaus1[io]->GetParameter(1);
   Double_t ChamberCenter_IC3 = fitGaus1[io]->Eval(meanY_ChamberCenter);
	
   Double_t ratioChamberTimepix1 = TimepixCenter_IC/ChamberCenter_IC;
   Double_t ratioChamberTimepix2 = TimepixCenter_IC/ChamberCenter_IC3 ;
	
   cout<<"__________________________________________________"<<endl;
   cout<<"I_t/I_c for Y Scan = "<<ratioChamberTimepix1<<endl;
   cout<<"I_t/I_c for Y Scan, where center of chamber is mean value of gauss function = "<<ratioChamberTimepix2<<endl;
   cout<<"__________________________________________________"<<endl;
   cout<<"Chamber = "<<ChamberCenter_IC<<endl;
   cout<<"Timepix = "<<TimepixCenter_IC<<endl;
   cout<<"Chamber center by fitting = "<<meanY_ChamberCenter <<endl;
   cout<<"__________________________________________________"<<endl;
   io++;

   //MAKE PICTURES
   SavePNGandEPS((TCanvas**) can, io);
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//CALIBRATION WHEN THERE IS no degrader plate in the beam trajectory 11:17-12:17
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   
	
   TGraph *grCalib   = new TGraph(0);
   SetGraph(grCalib, " ", " ",kTRUE); 
   npoints = 0;

    for(Int_t i=0; i<size; i++){ //loop over all logfile entries
      if(startCalibration < timestamp[i] && timestamp[i] <= stopCalibration){ //select the time region of interest
   
            npoints = grCalib->GetN();    //check how many points we have in the graph
            grCalib->Set(npoints+1);      //increase the number by 1
            //cout<<IC[i]<<endl;
            grCalib->SetPoint(npoints, timestamp[i]*1.0, IC[i]);  // add to the graph new value
	 }
      }

   //gStyle->SetOptTitle(0);

   //DRAW
   can[io] = new TCanvas(Form("can%d",io), "ALL_CALIBRATION_VS_TIME",0,0,800,500);
   SetCanvas((TCanvas*) can[io]);
   can[io]->cd();

   grCalib->Draw("AP");
 
	io++;
	
   //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //PLOTTING GRAPH USING TIME RANGE OF TIMEPIX DETECTOR (TimepixCounter.cc)
   //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	/*Double_t ChamberTime[100];
	
	vector<Double_t> startMeas;//Start of the iteration
	vector<Double_t> stopMeas;//End of the iteration
	vector<Double_t> startMeasTIMEPIX;
	
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
	
	//The end of chamber measurement 
	
	//Double_t stopChamber [] = {};
	
	cout<<endl;
	cout<<"############-Chamber is at the beam center-########"<<endl;
	
	for(Int_t i = 0; i < 24; i++){//Loop for pritting chamber time
		
		TString ChamberTime_1 = Form("%f", ChamberTime[i]);
        cout<<"This is time for Chamber center "<<ChamberTime_1<<endl;
		
	}
	
	cout<<"###################################################"<<endl;	
	cout<<endl;
	vector<pair<Double_t, Double_t>> timeScan = TimepixCounter();//Vector pair for range for timepix time
	int counter = 0;
	cout<<endl;
	cout<<"Calibration without plate "<<endl;
	//for(Int_t i = 0; i <= 8; i++)
	for(Int_t i = 0; i <= 23; i++){

		
		startMeas.push_back(ChamberTime[i]);// Start time of iteration
	  	startMeasTIMEPIX.push_back(timeScan[i].first);
		stopMeas.push_back(timeScan[i].second);  //It is the time from TimepixCounter() (the end of measurement) for event when timepix is at the beam center. 
		
		vector<Double_t> dummy22;
		dummy22.push_back(timeScan[i].second);
		vector<TDatime> UNIX11 = TimeToGMT(dummy22);//Time to GMT format for comfortable displaying the time into the terminal
		
		//UNIX11[0].Print();
		TString stra  = Form("%f", startMeas[i]);//It is necessary for displaying the data in format 12323... insted of 123e-3
		TString stra1 = Form("%f", stopMeas[i]);
		cout<<endl;
		cout<<"Start "<<stra << " " <<"Stop "<<stra1<<endl;//Printing the result
		
		counter++;
	}
	
   vector<Twinpad*> dummyCanv(counter);
   vector<TGraph*>  grCalibration_0mm(counter);
   vector<TGraph*>  grPad(counter);
   //vector<TBox*> box(counter);
   //vector<TPad*> tpad(counter);
   //TCanvas* canv[100];
   npoints = -1;
   
   Double_t startIteration        = 0;
   Double_t startTimepixIteration = 0;
   Double_t stopIteration         = 0;
   Double_t stopChamber           = 0;
  
   vector<Double_t> yaxisMin(counter);
   vector<Double_t> yaxisMax(counter);
   Double_t temp = 0;  
   Double_t temp1 = 0; 
	
   vector<Double_t> nPositionBeam;
   vector<Double_t> nTime;
    
   io = 0;
 TFile *CurrentHist = new TFile("CerrentHist.root", "RECREATE");
    
   for(int k = 0; k < counter; k++){
       
       dummyCanv[k] = new Twinpad(io,1.4,0.4);
       //tpad[k] = new TPad (Form("tpad%d", k), Form("tpad%d", k), 0.2, 0.2, 0.5, 0.5, -1, -1, -2);
       grCalibration_0mm[k] = new TGraph(0);
       SetGraph(grCalibration_0mm[k], " ", " ",kTRUE); 
       npoints = 0;
       
       TGraph *grCalibration_0mm_1 = new TGraph(0);
       TGraph *grPad_1 = new TGraph(0);
       
       grCalibration_0mm[k] = grCalibration_0mm_1;
       grPad[k] = grPad_1;
       
       SetGraph(grCalibration_0mm[k], "Time [sec]", "Ionization current [A] ", kTRUE);
       SetGraph(grPad[k], "Time [sec]", "Position Y [mm] ", kTRUE);
       
       temp = 0;   
    for(Int_t i = 0; i < size; i++){ //loop over all logfile entries
      if(startMeas[k] < timestamp[i] && timestamp[i] <= stopMeas[k]){ //select the time region of interest
   
            
            npoints = grCalibration_0mm[k] -> GetN();    //check how many points we have in the graph
            grCalibration_0mm[k]->Set(npoints+1);      //increase the number by 1
            //cout<<IC[i]<<endl;
            grCalibration_0mm[k]->SetPoint(npoints, timestamp[i]*1.0, IC[i]);  // add to the graph new value
            grPad[k]->SetPoint(npoints, timestamp[i]*1.0, y[i]*1.0);
            
            if(IC[i] > temp){
               temp = IC[i];
            }
            temp1 = temp;
            if(IC[i] < temp1){
              temp1 = IC[i];
            }
            
            if(y[i] != y[0] && y[i-1] -y[i] == 10){
            
                nPositionBeam.push_back(y[i-3]);
                nTime.push_back(timestamp[i-3]);
                
            }
      }
    }
    
     yaxisMax[k] = temp; 
     yaxisMin[k] = temp1;
       
     cout<<yaxisMax[k]<<endl; 
     cout<<yaxisMin[k]<<endl; 

     startIteration        = startMeas[k];
     startTimepixIteration = startMeasTIMEPIX[k];
     stopIteration         = stopMeas[k];
     stopChamber           = nTime[k];
    
     Graph plot {io, startIteration, stopChamber, startTimepixIteration, stopIteration, yaxisMin[k], yaxisMax[k], grCalibration_0mm[k], grPad[k], dummyCanv[k]};
	   plot.FirstGraph(io, startIteration, stopChamber, startTimepixIteration, stopIteration, yaxisMin[k], yaxisMax[k]);
	   plot.SecondGraph(io);
 
    io++;
   }

	for(size_t i = 0; i < nPositionBeam.size(); i++) {
        cout<<nPositionBeam[i]<<endl;
        //cout<<nTime[i]<<endl;
    }
    cout<<"------------------------"<<endl;
    cout<<"Size of nPositionBeam: "<<nPositionBeam.size()<<endl;
    cout<<"Number of graph: "<<counter<<endl;
    //cout<<"------------------------"<<endl;
    //cout<<nTime.size()<<endl;
    //CurrentHist->Close();
    */
   return 0;
}


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





//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//SELECT TIME WINDOWS OF INTEREST//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void SetCanvas(TCanvas *cc){
   // set boarders of the canvas to exploit the available space
   cc->SetLeftMargin(0.17);
   cc->SetRightMargin(0.05);
   cc->SetTopMargin(0.05);
   cc->SetBottomMargin(0.15);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Double_t fitf(Double_t *x, Double_t *par){
   Double_t arg = 0;
   if (par[2]!=0) arg = (x[0] - par[1])/par[2];
   Double_t fun = par[0]*TMath::Exp(-0.5*arg*arg);

   return fun;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void SavePNGandEPS(TCanvas** arrayCan, Int_t nobr){
   //this function prints to promptline commands which are needed to save the figure
   //you should copy them by mouse to the root promptline and execute them

   cout<<endl<<endl<<endl;
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


