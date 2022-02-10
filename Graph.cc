#include "TROOT.h"
#include "TStyle.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TLatex.h"
#include <vector>
#include "TGraph.h"
#include "TSystem.h"
#include "TString.h"
#include "TAxis.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TF1.h"
#include "TStyle.h"
#include "TBox.h"
#include "TH1.h"
#include "TH2D.h"
#include "TH1F.h"

#include "Twinpad.h"

class Graph
{

  public:

    Graph(Int_t _io, Double_t _startChamber, Double_t _stopChamber, Double_t _startTim, Double_t _stopTim, Double_t _yaxisMin, Double_t _yaxisMax, TGraph* gr1, TGraph* gr2, Twinpad* _c)
    {
        TString name1 = Form("graph1%d",_io);
        TString name2 = Form("graph2%d",_io);
        T1 = (TGraph*) gr1->Clone(name1);
        T2 = (TGraph*) gr2->Clone(name2);
        
        io = _io;
        startChamber = _startChamber;
        stopChamber  = _stopChamber;
        startTim     = _startTim;
        stopTim      = _stopTim;
        yaxisMin     = _yaxisMin;
        yaxisMax     = _yaxisMax;

         YMIN = yaxisMin;
         YMAX = yaxisMax;
        c = _c;  //lest initiate the Twinpad.   The numbers give the aspect ratio and size of the bottom pad
        c->SetMarginBottom(0.11);
        c->Draw();
        c->C->SetName(Form("canv%d",io));
        c->C->SetTitle(Form("Iteration%d",io));
        
    }
    ~Graph()
    {
       //delete T1;
       //delete T2;
    }

    Int_t io;

    Double_t yaxisMin;
    Double_t yaxisMax;

    Double_t YMIN;
    Double_t YMAX;

    Double_t startChamber;
    Double_t stopChamber;

    Double_t startTim;
    Double_t stopTim;

    TGraph* T1;
    TGraph* T2;
    
    TBox* box1;
    TBox* box;

    Twinpad* c;

    vector<Double_t> timestamp;

    void     FirstGraph  (Int_t io, Double_t startChamber, Double_t stopChamber, Double_t startTim, Double_t stopTim, Double_t YMIN, Double_t YMAX); //The function plot the first function (ionization current)
    void     SecondGraph(Int_t io, Double_t startChamber, Double_t stopTim, Double_t YMIN, Double_t YMAX);                                           //The function plot the second function (position)
    void     FitGraph(Int_t io, Double_t tStart, Double_t tStop, TGraph* T1, Int_t number);                                                          //Function for fitting
    void     GLegend(Double_t &mean0, Double_t &mean1, Double_t &mean2, Double_t &mean0Err, Double_t &mean1Err, Double_t &mean2Err);                 //The legend
    Double_t MeanTimepixCurrent(Double_t tStart, Double_t tStop, TGraph* T1);                                                                         //additional function. It is not used

};

void Graph::FirstGraph(Int_t io, Double_t startChamber, Double_t stopChamber, Double_t startTim, Double_t stopTim, Double_t YMIN, Double_t YMAX){
    
    YMIN = yaxisMin-yaxisMin/4;
    YMAX = yaxisMax+yaxisMax/10;
    
  TPad* p = c->GetPad(1);
    p->SetTickx();
    p->SetTicky();
      
   TH2D* hTmp = new TH2D(Form("hTop%d",io),"",200, startChamber-30, stopTim+30, 100, YMIN, YMAX);
  (TH1D*) hTmp;
    hTmp->GetXaxis()-> SetNdivisions(505);
    hTmp->GetYaxis()-> SetLabelFont(43);      //this ensures that font size in the upper and lower pad will be the same
    hTmp->GetYaxis()-> SetTitleFont(43);
    hTmp->GetYaxis()-> SetTitleSize(24);
    hTmp->GetYaxis()-> SetLabelSize(20);
    hTmp->GetYaxis()-> SetTitleOffset(2.1);
    hTmp->GetXaxis()-> SetTimeFormat("%H:%M %F1970-01-01 00:00:00");  //in our case the axis should have time format
    hTmp->GetXaxis()-> SetTimeDisplay(1);
    hTmp->GetYaxis()-> SetTitle("#it{I} [A]");
    hTmp->Draw();  //this draws this empty histogram which will be background for the color box and your graph with current
     
    box = new TBox(startTim, YMIN ,stopTim, YMAX); //Timepix
    box->SetFillColor(29);
    box->Draw(" ");

    box1 = new TBox(startChamber, YMIN, stopChamber, YMAX); // Chamber
    box1-> SetFillColor(41);
    box1-> Draw(" ");

    T1->Draw("P");  //this draws this empty histogram which will be background for the color box and your graph with current*/


}

void Graph::SecondGraph(Int_t io, Double_t startChamber, Double_t stopTim, Double_t YMIN, Double_t YMAX){

    YMIN = yaxisMin-yaxisMin/4;
    YMAX = yaxisMax+yaxisMax/10;
    TPad* p = c->GetPad(2);
    p->SetTickx();
    p->SetTicky();

    TH2D* hTmp2 = new TH2D(Form("hTop%d",io),"",200, startChamber-30, stopTim+30, 100, YMIN, YMAX); 
    (TH1D*) hTmp2;
     hTmp2->GetXaxis()-> SetNdivisions(505);
     hTmp2->GetYaxis()-> SetNdivisions(505);
     hTmp2->GetXaxis()-> SetLabelFont(43);
     hTmp2->GetXaxis()-> SetTitleFont(43);
     hTmp2->GetYaxis()-> SetLabelFont(43);
     hTmp2->GetYaxis()-> SetTitleFont(43);
     hTmp2->GetXaxis()-> SetTitleSize(24);
     hTmp2->GetXaxis()-> SetLabelSize(20);
     hTmp2->GetYaxis()-> SetTitleSize(24);
     hTmp2->GetYaxis()-> SetLabelSize(20);
     hTmp2->GetXaxis()-> SetTitleOffset(2.8);
     hTmp2->GetYaxis()-> SetTitleOffset(2.1);
     hTmp2->GetXaxis()-> SetLabelOffset(0.01);
     hTmp2->GetYaxis()-> SetLabelOffset(0.01);
     hTmp2->GetYaxis()-> SetTitle("#it{y} [mm]");  // here you define tile of the y axis 
     hTmp2->GetYaxis()-> CenterTitle(1);
     hTmp2->GetXaxis()-> SetTitle("time"); //here you define title of the x axis
     hTmp2->GetXaxis()-> SetTimeFormat("%H:%M %F1970-01-01 00:00:00");
     hTmp2->GetXaxis()-> SetTimeDisplay(1);
     hTmp2->SetMarkerStyle(20);
     hTmp2->SetMarkerColor(0);
     hTmp2->Draw();
 
    T2->Draw("AP"); 

}

void Graph::FitGraph(Int_t io, Double_t tStart, Double_t tStop, TGraph* T1, Int_t number){
  TPad* p = c->GetPad(1);
  p->cd();
  TString name1 = Form("const_form%d%i",io, number);
    TF1* const_form1 = new TF1(name1,"[0]",tStart,tStop);
    const_form1 -> SetParName(0,"Constant Ionization current");
    T1->Fit(name1,"0R", "", tStart,tStop);
    if(number == 0) { const_form1-> SetLineColor(3); }
    if(number == 1) { const_form1-> SetLineColor(4); }
    if(number == 2) { const_form1-> SetLineColor(2); }
    const_form1->Draw("same");

  TLegend* leg;
    if (number == 0) {
       leg = new TLegend(0.1, 0.5, 0.5, 0.6," ","brNDC");
     }
    if (number == 1) {
      leg = new TLegend(0.5, 0.5, 0.9, 0.6," ","brNDC");
     }
    if (number == 2) {
      leg = new TLegend(0.6, 0.5, 0.9, 0.6," ","brNDC");
    }
     leg->SetFillStyle(0);
     leg->SetBorderSize(0);
     leg->SetTextSize(0.045);

     Double_t ConstCurrent11 = (const_form1->GetParameter(0))*1E+15;
     Double_t ConstError1    = (const_form1->GetParError(0))*1E+15;
     //leg->AddEntry((TObject*) 0, Form("const = %.1f #pm %.1f fA", ConstCurrent11 , ConstError1)," "); 
    
     leg->Draw();

}

Double_t Graph::MeanTimepixCurrent(Double_t tStart, Double_t tStop, TGraph* T1){

  Int_t points                   = T1->GetN();
  Double_t* pointerTimeWindow    = T1->GetX();
  Double_t* pointerCurrentWindow = T1->GetY();
  
  Double_t iMean;
  Double_t deltaX;
  Double_t sum = 0;
  Int_t    npoint = 0;

  vector<Double_t> yIC;
  for(Int_t i = 0; i < points; i++) {
  
    Double_t x = pointerTimeWindow[i];
    Double_t y = pointerCurrentWindow[i];
    
    if(i == 0){
     deltaX = pointerTimeWindow[1] - pointerTimeWindow[0];   
      }
    if(tStart < x && x <= tStop){ 
      sum += y*deltaX;
      yIC.push_back(y); 
      npoint++;
    }
  }
  
  Double_t meanCurrent = sum/(npoint*(tStop - tStart));

  return meanCurrent;
}

void Graph::GLegend(Double_t &mean0, Double_t &mean1, Double_t &mean2, Double_t &mean0Err, Double_t &mean1Err, Double_t &mean2Err){

   TPad* p =  c-> GetPad(1);
   auto* legend = new TLegend(0.5, 0.7, 0.97, 0.97);
   auto h = new TH1F("", "", 1, 0, 1);

   TF1* line0 = new TF1("line0","[0]",0,1);
   TF1* line1 = new TF1("line1","[0]",0,1);
   TF1* line2 = new TF1("line2","[0]",0,1);
   
   line0->SetLineColor(3);
   line1->SetLineColor(4);
   line2->SetLineColor(2);
  
   mean0    = mean0*1E+15;
   mean1    = mean1*1E+15;
   mean2    = mean2*1E+15;
   mean0Err = mean0Err*1E+15;
   mean1Err = mean1Err*1E+15;
   mean2Err = mean2Err*1E+15;
  
   legend->AddEntry((TObject*) line0, Form("const = %.1f #pm %.1f fA", mean0 , mean0Err),"l");
   legend->AddEntry((TObject*) line1, Form("const = %.1f #pm %.1f fA", mean1 , mean1Err),"l");
   legend->AddEntry((TObject*) line2, Form("const = %.1f #pm %.1f fA", mean2 , mean2Err),"l");

 
   legend->Draw();
}