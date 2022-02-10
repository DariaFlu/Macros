#include "Twinpad.h"
#include "TBox.h"

#include "TROOT.h"
#include "TStyle.h"
#include <iostream>
#include "TCanvas.h"
#include "TString.h"
#include "TAxis.h"
#include "TH1.h"
#include "TH2D.h"
#include "TDatime.h"
#include "TPad.h"
#include "TLegend.h"
#include <vector>

using std::cout;
using std::endl;


pair<Int_t, Int_t>  TimeToUnix(const vector<Int_t>& timestamp, Int_t hour, Int_t min, Int_t sec, Int_t hour2, Int_t min2, Int_t sec2);
void SetHist(TH1* h,TString titx, TString tity);
void SavePNGandEPS(Int_t iobr,TString name);



Int_t Draw(){

   //This is and example of drawing macro which allows to have two pads above each other with the same y axis
   //such representation is convenient when you would like to present what is happeing with the 
   //ionization cahmber current and the y coordinate at given time 

   //First les us do some dummy definition of the time window we want to display.  You need to change the values.
   vector<Int_t> timestamp;  timestamp.push_back(1637590057);	
   pair<Int_t, Int_t> UnixTime = TimeToUnix(timestamp,12,24,0,12,40,0);
   Int_t startUnixTime   = UnixTime.first;    //start of the window
   Int_t stopUnixTime    = UnixTime.second;   //end of the window


   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);

   cout<<endl<<endl<<endl<<endl;  
   cout<<"TImage *img = TImage::Create();"<<endl; //this is for later printing od the figure to eps/png


   Twinpad *c; // twinpad is  a  canvas which has  upper nad lower pad with the same X axis
   TPad *p;   // this will be pointer to upper/lower pad in the canvas
   Int_t io = 0; //counter of figures  (although there is at the moment just one figure, i imagine you will use this for multiple plots
   TH2D *hTmp;   // dummy histogram to set titles and scales on X and Y axes in figures
   TBox *box;   // this will be the color box showing the range of the timepix measurement
    
   c = new Twinpad(io,1.4,0.4);  //lest initiate the Twinpad.   The numbers give the aspect ratio and size of the bottom pad
   c->SetMarginBottom(0.11);
   c->Draw();

   //upper pad with the ionizaton chambe current verus time
   p = c->GetPad(1);
   p->SetTickx();
   p->SetTicky();

   Double_t  yaxisMin = -4e-11;   //you need to define minim of the y axis range in the upper pad
   Double_t  yaxisMax = 1e-9;     //you need to define maximum of the y axis range in the lower pad

   //let us now initiate the dummy histogram which we will use to set X and Y axis titles and ranges
   hTmp = new TH2D(Form("hTop%d",io),"",20, startUnixTime, stopUnixTime, 10, yaxisMin, yaxisMax);   //here the last two numbers set the range of Y axis. Change them

   SetHist((TH1*) hTmp,"time", "#it{I}_{ch} (A)"); //define axis titles
   hTmp->GetXaxis()->SetNdivisions(505);
   hTmp->GetYaxis()->SetLabelFont(43);      //this ensures that font size in the upper and lower pad will be the same
   hTmp->GetYaxis()->SetTitleFont(43);
   hTmp->GetYaxis()->SetTitleSize(24);
   hTmp->GetYaxis()->SetLabelSize(20);
   hTmp->GetYaxis()->SetTitleOffset(2.1);
   hTmp->GetXaxis()-> SetTimeFormat("%H:%M %F1970-01-01 00:00:00");  //in our case the axis should have time format
   hTmp->GetXaxis()-> SetTimeDisplay(1);
   hTmp->Draw();  //this draws this empty histogram which will be background for the color box and your graph with current

   //now we initiate the color box that will be showing timepix measurement. Here you need to change the start and stop time
   box = new TBox( stopUnixTime-200, yaxisMin, stopUnixTime-60,  yaxisMax);
   box->SetFillColor(5);
   box->Draw("same");

   //here you would be plotting your graph:
   // graph_with_ionization_current_versus_time->Draw("psame");     

   gPad->RedrawAxis();  //this command draws once again ticks on X and Y axes  which would be hidden by the box

   //Now lets define the bottom pad  that will be showing  the y axis coordinate versus time
   p = c->GetPad(2); 
   p->SetTicky();
   p->SetTickx();
   Double_t yMin = 230; //  Y axis range max in the upper pad 
   Double_t yMax = 265;  //  Y axis range max in the lower pad 

   hTmp = new TH2D(Form("hBottomPad%d",io),"", 20, startUnixTime, stopUnixTime, 10, yMin, yMax); 
   hTmp->GetXaxis()->SetNdivisions(505);
   hTmp->GetYaxis()->SetNdivisions(505);
   hTmp->GetXaxis()->SetLabelFont(43);
   hTmp->GetXaxis()->SetTitleFont(43);
   hTmp->GetYaxis()->SetLabelFont(43);
   hTmp->GetYaxis()->SetTitleFont(43);
   hTmp->GetXaxis()->SetTitleSize(24);
   hTmp->GetXaxis()->SetLabelSize(20);
   hTmp->GetYaxis()->SetTitleSize(24);
   hTmp->GetYaxis()->SetLabelSize(20);
   hTmp->GetXaxis()->SetTitleOffset(2.8);
   hTmp->GetYaxis()->SetTitleOffset(2.1);
   hTmp->GetXaxis()->SetLabelOffset(0.01);
   hTmp->GetYaxis()->SetLabelOffset(0.01);
   hTmp->GetYaxis()->SetTitle("#it{y} [mm]");  // here you define tile of the y axis 
   hTmp->GetYaxis()->CenterTitle(1);
   hTmp->GetXaxis()->SetTitle("time"); //here you define title of the x axis
   hTmp->GetXaxis()-> SetTimeFormat("%H:%M %F1970-01-01 00:00:00");
   hTmp->GetXaxis()-> SetTimeDisplay(1);
   hTmp->SetMarkerStyle(20);
   hTmp->SetMarkerColor(0);
   hTmp->Draw();  //here you draw this dummy  histogram which will be background for your y axis trend

   //here draw y versus time:
   // y->Draw("samep");

   //save figure to png and eps
   SavePNGandEPS(io, Form("figure%d",io)); // printout commands that when copied to the root prompt  creat the figure
   io++; // increase figure counter.   


  return 1;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void SavePNGandEPS(Int_t iobr,TString name){

   //EPS IMAGES
   TString nameEPS = name + ".eps";
   cout<<Form("c%d",iobr)<<"->SaveAs(\""<<nameEPS.Data()<<"\");"<<endl;

   //PNG IMAGES
   TString namePNG = name + ".png";
           cout<<"img->FromPad(c"<<iobr<<"); img->WriteImage(\""<<namePNG.Data()<<"\");"<<endl;


}



