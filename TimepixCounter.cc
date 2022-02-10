#include <string>
#include <fstream>
//#include <ofstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include "TDatime.h"
#include "TROOT.h"
#include "TSystem.h"
#include <algorithm>
#include <map>
using namespace std;

//HERE DECLARE FUNCTION HEADERS
int CounterLines = 0;

vector<TDatime> TimeToGMT(const vector<Double_t>& time1);
pair<string, string> TimepixTime(string nameFile);
vector<pair<Double_t, Double_t>> TimepixCounter();
bool operator<(const pair<string, string>& l, const pair<string, string>& r); 
bool operator==(const pair<string, string>& l, const pair<string, string>& r);

//HERE STARTS THE MAIN FUNCTION
vector<pair<Double_t, Double_t>> TimepixCounter(){


   vector<pair<Double_t, Double_t>> timeScan; //Scan time for the center Timepix
   gSystem->cd("/home/dnn1488/Desktop/Root/DariaCalib2022Nov22/TimePixCalib");
   gROOT->ProcessLine(".!ls>TimeMeasurement.txt"); //List of files
   
   //+++++++++++++++++++++++++++++++++++++
   //OPEN FILE AND READ DATA   
   //+++++++++++++++++++++++++++++++++++++

   ifstream fileDir; //Open the TimeMeasurement.txt
   fileDir.open("TimeMeasurement.txt", ifstream::in); //Stream for reading the file
   string c; 
   vector<string> filenames; // List of files
   string mask = "dsc"; //Mask for finding a  'smt.dat.dsc' file
   int i = 0;

   pair<Double_t, Double_t> UnixTimeFormat; //Time in Unix format
   map<Double_t, string> Map_Of_File_Names;

    while(getline(fileDir,c)){
      
      string check (c.end()-3, c.end());//Searching the line <"Start time" ("Acquisition start time")>:
      if(check == mask){ //Checking the string
        
          // cout<<"_________________________________________________"<<endl;
          filenames.push_back(c);
          //cout<<filenames[i]<<endl;
          UnixTimeFormat = make_pair(stod(TimepixTime(c).first), stod(TimepixTime(c).second)); //Pair of time of timepix scan
          timeScan.push_back(UnixTimeFormat);
          Map_Of_File_Names[timeScan[i].first] = c;//Map of files. It will be used for printing the time into terminal
          
          i++;
      }
     }

   //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //SORT DATA (Initionally the files list was sorted alphabetically)  
   //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
    sort(timeScan.begin(), timeScan.end());//Sort the file using the first field (the first time)
    //cout<<filenames[i]<<endl;
    ofstream ofs;
    ofs.open("RootFile.txt", ofstream::out | ofstream::trunc);
    
    for(size_t i = 0; i < timeScan.size(); i++){
        
        string rn1 = Map_Of_File_Names[timeScan[i].first];
        string rootName = rn1.replace(
            (rn1.length()-3), 4, "root");
        ofs<<rootName<<endl;//Printig the name of the file
        cout<<"---Timepix Calibration. Start and stop of the scan---"<<endl;
        cout<<Map_Of_File_Names[timeScan[i].first]<<endl;//Printig the name of the file
        TString stra11 = Form("%f", timeScan[i].first);//Displaying the time in 11122 format instead of 121e-1
        TString stra111 = Form("%f", timeScan[i].second);
        //cout<< timeScan[i].first<<endl;
        //cout<< timeScan[i].second<<endl;
        cout<<"UNIX time start: "<<stra11<<endl;//Printing the time from 70 line
        cout<<"UNIX time stop: "<<stra111<<endl;
        
        vector<Double_t> storage;//The vector will be used for convertation time to GMT format
        storage.push_back(timeScan[i].first);
        storage.push_back(timeScan[i].second);
        //Here is printing the time in GMT format
        vector<TDatime> dummyTime1 = TimeToGMT(storage);
        cout<<"Start: ";
        dummyTime1[0].Print();
        cout<<"Stop: ";
        dummyTime1[1].Print();
    cout<<"------------------------------------------------------"<<endl;
    }
    ofs.close();
    fileDir.close();
    	
    return timeScan;
}

 //Sort data by operator
bool operator<(const pair<string, string>& l, const pair<string, string>& r) 
{
    return (l.first < r.first);
}

bool operator==(const pair<string, string>& l, const pair<string, string>& r) 
{
    return (l.first == r.first);
}

   //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //UNIX time to GMT format 
   //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
vector<TDatime> TimeToGMT(const vector<Double_t>& time1){

vector<TDatime> time_GMT(time1.size());
TDatime dummy, timeNorm;
Int_t year, month, day, hour, min, sec;

	for(size_t i = 0; i < time1.size(); i++){

		dummy.Set(time1[i]);
		year  = dummy.GetYear();
		month = dummy.GetMonth();
		day   = dummy.GetDay();

		hour  = dummy.GetHour();
        min   = dummy.GetMinute();
        sec   = dummy.GetSecond();
		timeNorm.Set(year, month, day, hour, min, sec);
		timeNorm = timeNorm.Convert(0);
		
		time_GMT[i] = timeNorm;
	}

	
return time_GMT; 
	
}

   //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //READING THE FILE
   //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
pair<string, string> TimepixTime(string nameFile){
	
   // file.open("DariaCalib2022Nov22/TimepixCalib/Cal_0mm_1.dat.dsc", ifstream::in);
   ifstream file;
   size_t offset;
   string line;
   string search = "\"Start time\" (\"Acquisition start time\"):"; //Searching for the required string
   
   string dummy0, dummy1, timeDummy;
   vector<string> time;
	   
   //file.open("Cal_0mm_2.dat.dsc", ifstream::in);
   file.open(nameFile, ifstream::in);

   int i =0;
   int count=0;
   if(file.is_open()){
      while(!file.eof()){
         getline(file, line);
         CounterLines++;
		  
            if((offset = line.find(search, 0)) != string::npos){ //Searching the line by mask "\"Start time\" (\"Acquisition start time\"):"

                getline(file, line);
                getline(file, line);//Line 3 in "Cal_....dat.dsc" file (measurement time) is obtained here
                 count++; //Conter of elements
                istringstream ss;
                ss.str(line);

                ss>>timeDummy;
				//cout<<timeDummy<<endl;

                time.push_back(timeDummy); //Time in Unix format

            }
            i++;
         }
   }

//cout<<"Count "<<count<<endl;

    file.close();

    vector<Double_t> time1(time.size());
//Conversion to double type
   for(size_t i = 0; i < time.size(); i++){
       time1[i] = stold(time[i]);

}
//Unix time to GMT format
Int_t num = -1;
    vector<TDatime> FunTime = TimeToGMT(time1);
    for(size_t i = 0; i < time.size(); i++){
       //FunTime[i].Print();
       //cout<<endl;
		num++;
	}
//Printing the start and end time of the measurement
/*cout<<"--------------------------------------------------"<<endl;
	FunTime[1].Print();
	FunTime[num].Print();

cout<<"_________________________________________________"<<endl;*/

	
//pair<TDatime, TDatime>  StartStopTimepixMeasurement = make_pair (FunTime[1],FunTime[num]); //The pair of GMT start/end time
pair<string, string>  StartStopTimepixMeasurement = make_pair (time[1],time[num]); //The pair of Unix start/end time
	
return StartStopTimepixMeasurement; 	

}