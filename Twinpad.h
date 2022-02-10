#pragma once

#include "TROOT.h"
#include "TStyle.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TLatex.h"

class Twinpad {

    public:

        // ---- Costructor ------------
        Twinpad(int inID=0, float inRelSize=1.0, float inR = 0.2, int inXOffset = 10, int inYOffset=10, float inAspect=0.7 );
        // ---- Destructor ------------
        ~Twinpad(){
            if(C)      delete C;
            if(toppad) delete toppad;
        }


        TPad* GetPad(int padID){ return (TPad*) toppad->cd(padID);}//coordinates 0,0 = upper left 

        //-------------------------------------------------------------------------------------
        void Draw();

        //-------------------------------------------------------------------------------------
        /*void Hset(TH1* hid, TString xtit="", TString ytit="",
                double titoffx = 2.5, double titoffy = 1.5,
                double titsizex = 20, double titsizey = 20,
                double labeloffx = 0.01, double labeloffy = 0.001,
                double labelsizex = 16, double labelsizey = 16,
                int divx = 505, int divy=505);
        */

        void SetMarginLeft(float x){ MarginLeft = x;}
        void SetMarginRight(float x){ MarginRight = x;}
        void SetMarginTop(float x){ MarginTop = x;}
        void SetMarginBottom(float x){ MarginBottom = x;}

        //  M A I N     C A N V A S 
        int   ID;
        float aspektCanvas;      //size and positioning of the main canvas   
        int   sizeCanvas;
        int   sdxCanvas; 
        int   sdyCanvas;

        float MarginLeft; //Margins around the latice of nx times ny pads 
        float MarginBottom;
        float MarginRight;
        float MarginTop;

        float space;
        float ratio;
        TString mcpad;
        TCanvas *C;
        TVirtualPad *toppad;
};

//___________________________________________________________________
 Twinpad::Twinpad(int inID, float inRelSize, float inR, int inXOffset, int inYOffset, float inAspect){

            aspektCanvas = inAspect;
            sizeCanvas   = 600*inRelSize;

            MarginLeft   = 0.17;
            MarginBottom = 0.1; 
            MarginRight  = 0.05;
            MarginTop    = 0.05;
            ID           = inID;
            mcpad        = Form("c%d",ID);
            ratio = inR;
            sdxCanvas    = inXOffset;
            sdyCanvas    = inYOffset;
            space        = 0;
}


//_________________________________________________________________________

void Twinpad::Draw(){ 

            char name[200];
            // cout<<"Draw"<<endl;
            C = new TCanvas(mcpad, mcpad, sdxCanvas, sdyCanvas, sizeCanvas*aspektCanvas, sizeCanvas);//the main canvas
            C->SetFillStyle(4000); C->SetFillColor(10);
            gStyle->SetOptStat(0);    gStyle->SetOptTitle(0);
            C->SetTopMargin(0.); C->SetBottomMargin(0.);//our pads will have no margin
            C->SetLeftMargin(0.);C->SetRightMargin(0.);
            //C->cd();
            C->Draw();

            toppad = gPad;
            toppad->Clear();

            TPad   *pp      = NULL;
            sprintf(name, "UPad%d",this->ID ); //some dummy name
            pp = new TPad(name,name, 0, 0 + ratio + space  , 1, 1, 0); //create pad
        
            pp->SetNumber(1);   //assign a number to it. Possible to access it via :  toppad->cd(ih);  
            pp->SetTopMargin(MarginTop/(1-ratio)); pp->SetBottomMargin(0.0015);//our pads will have no margin
            pp->SetLeftMargin(MarginLeft);pp->SetRightMargin(MarginRight);
            pp->Draw();

            sprintf(name, "LPad%d",this->ID ); //some dummy name
            pp = new TPad(name,name,0, 0, 1, ratio ,0); //create pad
            pp->SetNumber(2);   //assign a number to it. Possible to access it via :  toppad->cd(ih); 
            pp->SetTopMargin(0.0015); pp->SetBottomMargin(MarginBottom/ratio);//our pads will have no margin
            pp->SetLeftMargin(MarginLeft);pp->SetRightMargin(MarginRight);
            pp->Draw();
        }
//___________________________________________________________________
/*
void Twinpad::Hset(TH1* hid, TString xtit="", TString ytit="",
                double titoffx, double titoffy,
                double titsizex, double titsizey,
                double labeloffx, double labeloffy,
                double labelsizex, double labelsizey,
                int divx, int divy){

            //hid.GetXaxis()->CenterTitle(1);
            //hid.GetYaxis()->CenterTitle(1);

            hid->GetXaxis()->SetTitleOffset(titoffx);
            hid->GetYaxis()->SetTitleOffset(titoffy);

            hid->GetXaxis()->SetTitleFont(43);
            hid->GetYaxis()->SetTitleFont(43);
            hid->GetXaxis()->SetTitleSize(titsizex);
            hid->GetYaxis()->SetTitleSize(titsizey);

            hid->GetXaxis()->SetLabelOffset(labeloffx);
            hid->GetYaxis()->SetLabelOffset(labeloffy);

            hid->GetXaxis()->SetLabelFont(43);
            hid->GetYaxis()->SetLabelFont(43);
            hid->GetXaxis()->SetLabelSize(labelsizex);
            hid->GetYaxis()->SetLabelSize(labelsizey);

            hid->GetXaxis()->SetNdivisions(divx);
            hid->GetYaxis()->SetNdivisions(divy);

            hid->GetXaxis()->SetTitle(xtit);
            hid->GetYaxis()->SetTitle(ytit);

}*/
//___________________________________________________________________
