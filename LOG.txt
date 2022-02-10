   ------------------------------------------------------------------
  | Welcome to ROOT 6.23/01                        https://root.cern |
  | (c) 1995-2020, The ROOT Team; conception: R. Brun, F. Rademakers |
  | Built for linuxx8664gcc on Jan 03 2021, 23:52:00                 |
  | From heads/master@v6-23-01-RF-binSampling-359-gfbe6b2bf34        |
  | With                                                             |
  | Try '.help', '.demo', '.license', '.credits', '.quit'/'.q'       |
   ------------------------------------------------------------------


Processing BeamMonitoring_2.cc+...

****************************************
Minimizer is Minuit / Migrad
Chi2                      =  1.50672e-24
NDf                       =           14
Edm                       =  2.50491e-24
NCalls                    =           39
p0                        =   1.5715e-11   +/-   3.24065e-13 
p1                        =      197.531   +/-   0.179883    
p2                        =      11.3971   +/-   0.272121    

****************************************
Minimizer is Minuit / Migrad
Chi2                      =  2.54843e-24
NDf                       =           10
Edm                       =  3.23415e-25
NCalls                    =           40
p0                        =  1.58882e-11   +/-   5.77406e-13 
p1                        =      259.385   +/-   0.2552      
p2                        =      9.85615   +/-   0.420187    
__________________________________________________
I_t/I_c for Y Scan = 0.574473
I_t/I_c for Y Scan, where center of chamber is mean value of gauss function = 0.574035
__________________________________________________
Chamber = 259
Timepix = 249
Chamber center by fitting = 259.385
__________________________________________________

****************************************
Minimizer is Minuit / Migrad
Chi2                      =   1.8445e-25
NDf                       =           25
Edm                       =  1.91814e-25
NCalls                    =           39
p0                        =  4.18582e-12   +/-   5.19668e-14 
p1                        =      196.087   +/-   0.231744    
p2                        =       22.765   +/-   0.328891    

****************************************
Minimizer is Minuit / Migrad
Chi2                      =  1.80084e-25
NDf                       =           17
Edm                       =  2.09563e-26
NCalls                    =           40
p0                        =   4.9625e-12   +/-   6.45088e-14 
p1                        =      259.772   +/-   0.227716    
p2                        =      20.6271   +/-   0.316171    
__________________________________________________
I_t/I_c for Y Scan = 0.873139
I_t/I_c for Y Scan, where center of chamber is mean value of gauss function = 0.872528
__________________________________________________
Chamber = 4.95903e-12
Timepix = 4.32992e-12
Chamber center by fitting = 259.772
__________________________________________________



TImage *img = TImage::Create();
img->FromPad(can0); img->WriteImage("ALL_SCANS_VS_TIME.png");
img->FromPad(can1); img->WriteImage("SCAN_X1.png");
img->FromPad(can2); img->WriteImage("SCAN_Y1.png");
img->FromPad(can3); img->WriteImage("SCAN_X2.png");
img->FromPad(can4); img->WriteImage("SCAN_Y2.png");

############-Chamber is at the beam center-########
This is time for Chamber center 1637576220.000000
This is time for Chamber center 1637576760.000000
This is time for Chamber center 1637577420.000000
This is time for Chamber center 1637577900.000000
This is time for Chamber center 1637578380.000000
This is time for Chamber center 1637578740.000000
This is time for Chamber center 1637579040.000000
This is time for Chamber center 1637579400.000000
This is time for Chamber center 1637579700.000000
###################################################

---Timepix Calibration. Start and stop of the scan---
Cal_0mm_1c.dat.dsc
UNIX time start: 1637576452.409342
UNIX time stop: 1637576502.364970
Start: Date/Time = Mon Nov 22 11:20:52 2021
Stop: Date/Time = Mon Nov 22 11:21:42 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_0mm_1d.dat.dsc
UNIX time start: 1637576984.688364
UNIX time stop: 1637577301.082006
Start: Date/Time = Mon Nov 22 11:29:44 2021
Stop: Date/Time = Mon Nov 22 11:35:01 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_0mm_2.dat.dsc
UNIX time start: 1637577528.946195
UNIX time stop: 1637577845.298937
Start: Date/Time = Mon Nov 22 11:38:48 2021
Stop: Date/Time = Mon Nov 22 11:44:05 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_0mm_3.dat.dsc
UNIX time start: 1637578118.418985
UNIX time stop: 1637578276.030356
Start: Date/Time = Mon Nov 22 11:48:38 2021
Stop: Date/Time = Mon Nov 22 11:51:16 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_0mm_4.dat.dsc
UNIX time start: 1637578492.615822
UNIX time stop: 1637578650.346949
Start: Date/Time = Mon Nov 22 11:54:52 2021
Stop: Date/Time = Mon Nov 22 11:57:30 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_0mm_5.dat.dsc
UNIX time start: 1637578891.120059
UNIX time stop: 1637579049.603113
Start: Date/Time = Mon Nov 22 12:01:31 2021
Stop: Date/Time = Mon Nov 22 12:04:09 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_0mm_6.dat.dsc
UNIX time start: 1637579202.729887
UNIX time stop: 1637579359.867764
Start: Date/Time = Mon Nov 22 12:06:42 2021
Stop: Date/Time = Mon Nov 22 12:09:19 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_0mm_7.dat.dsc
UNIX time start: 1637579552.298449
UNIX time stop: 1637579710.741333
Start: Date/Time = Mon Nov 22 12:12:32 2021
Stop: Date/Time = Mon Nov 22 12:15:10 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_0mm_8.dat.dsc
UNIX time start: 1637579852.450879
UNIX time stop: 1637580009.805125
Start: Date/Time = Mon Nov 22 12:17:32 2021
Stop: Date/Time = Mon Nov 22 12:20:09 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_1.dat.dsc
UNIX time start: 1637581676.626000
UNIX time stop: 1637581834.173070
Start: Date/Time = Mon Nov 22 12:47:56 2021
Stop: Date/Time = Mon Nov 22 12:50:34 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_2.dat.dsc
UNIX time start: 1637582518.879005
UNIX time stop: 1637582675.672860
Start: Date/Time = Mon Nov 22 13:01:58 2021
Stop: Date/Time = Mon Nov 22 13:04:35 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_3.dat.dsc
UNIX time start: 1637582847.819806
UNIX time stop: 1637583005.813597
Start: Date/Time = Mon Nov 22 13:07:27 2021
Stop: Date/Time = Mon Nov 22 13:10:05 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_4.dat.dsc
UNIX time start: 1637583169.261993
UNIX time stop: 1637583214.375903
Start: Date/Time = Mon Nov 22 13:12:49 2021
Stop: Date/Time = Mon Nov 22 13:13:34 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_5.dat.dsc
UNIX time start: 1637583346.377144
UNIX time stop: 1637583504.164281
Start: Date/Time = Mon Nov 22 13:15:46 2021
Stop: Date/Time = Mon Nov 22 13:18:24 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_6.dat.dsc
UNIX time start: 1637583667.404726
UNIX time stop: 1637583825.833211
Start: Date/Time = Mon Nov 22 13:21:07 2021
Stop: Date/Time = Mon Nov 22 13:23:45 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_7.dat.dsc
UNIX time start: 1637583998.217691
UNIX time stop: 1637584156.362880
Start: Date/Time = Mon Nov 22 13:26:38 2021
Stop: Date/Time = Mon Nov 22 13:29:16 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_8.dat.dsc
UNIX time start: 1637584343.006866
UNIX time stop: 1637584501.958445
Start: Date/Time = Mon Nov 22 13:32:23 2021
Stop: Date/Time = Mon Nov 22 13:35:01 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_9.dat.dsc
UNIX time start: 1637584725.753465
UNIX time stop: 1637584962.782050
Start: Date/Time = Mon Nov 22 13:38:45 2021
Stop: Date/Time = Mon Nov 22 13:42:42 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_10.dat.dsc
UNIX time start: 1637585196.239936
UNIX time stop: 1637585354.313798
Start: Date/Time = Mon Nov 22 13:46:36 2021
Stop: Date/Time = Mon Nov 22 13:49:14 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_11.dat.dsc
UNIX time start: 1637585645.974078
UNIX time stop: 1637585804.806539
Start: Date/Time = Mon Nov 22 13:54:05 2021
Stop: Date/Time = Mon Nov 22 13:56:44 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_12.dat.dsc
UNIX time start: 1637586006.311067
UNIX time stop: 1637586164.839969
Start: Date/Time = Mon Nov 22 14:00:06 2021
Stop: Date/Time = Mon Nov 22 14:02:44 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_13.dat.dsc
UNIX time start: 1637586355.370350
UNIX time stop: 1637586512.984054
Start: Date/Time = Mon Nov 22 14:05:55 2021
Stop: Date/Time = Mon Nov 22 14:08:32 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_14.dat.dsc
UNIX time start: 1637586804.863227
UNIX time stop: 1637586961.706610
Start: Date/Time = Mon Nov 22 14:13:24 2021
Stop: Date/Time = Mon Nov 22 14:16:01 2021
------------------------------------------------------
---Timepix Calibration. Start and stop of the scan---
Cal_05mm_15.dat.dsc
UNIX time start: 1637587067.365771
UNIX time stop: 1637587226.237139
Start: Date/Time = Mon Nov 22 14:17:47 2021
Stop: Date/Time = Mon Nov 22 14:20:26 2021
------------------------------------------------------

Calibration without plate 

Start 1637576220.000000 Stop 1637576502.364970

Start 1637576760.000000 Stop 1637577301.082006

Start 1637577420.000000 Stop 1637577845.298937

Start 1637577900.000000 Stop 1637578276.030356

Start 1637578380.000000 Stop 1637578650.346949

Start 1637578740.000000 Stop 1637579049.603113

Start 1637579040.000000 Stop 1637579359.867764

Start 1637579400.000000 Stop 1637579710.741333

Start 1637579700.000000 Stop 1637580009.805125
3.935e-12
1.859e-12




TImage *img = TImage::Create();

****************************************
Minimizer is Minuit / Migrad
Chi2                      =  3.31761e-25
NDf                       =           24
Edm                       =  2.90573e-39
NCalls                    =           13
Constant Ionization current =  1.97884e-12   +/-   2.35146e-14 
3.745e-12
1.998e-12




TImage *img = TImage::Create();

****************************************
Minimizer is Minuit / Migrad
Chi2                      =  2.99731e-24
NDf                       =          155
Edm                       =  1.72654e-37
NCalls                    =           15
Constant Ionization current =  2.01862e-12   +/-   1.11337e-14 
3.435e-12
2.147e-12




TImage *img = TImage::Create();

****************************************
Minimizer is Minuit / Migrad
Chi2                      =   4.2167e-24
NDf                       =          157
Edm                       =  1.16552e-37
NCalls                    =           13
Constant Ionization current =  1.88615e-12   +/-   1.30379e-14 
5.154e-12
3.191e-12




TImage *img = TImage::Create();

****************************************
Minimizer is Minuit / Migrad
Chi2                      =  1.25345e-24
NDf                       =           78
Edm                       =  6.63296e-38
NCalls                    =           13
Constant Ionization current =  2.88604e-12   +/-   1.42624e-14 
6.731e-12
3.872e-12




TImage *img = TImage::Create();

****************************************
Minimizer is Minuit / Migrad
Chi2                      =  1.70444e-24
NDf                       =           78
Edm                       =  1.43493e-40
NCalls                    =           13
Constant Ionization current =  3.75668e-12   +/-   1.66314e-14 
1.163e-11
4.758e-12




TImage *img = TImage::Create();

****************************************
Minimizer is Minuit / Migrad
Chi2                      =  2.61409e-24
NDf                       =           78
Edm                       =  2.23885e-37
NCalls                    =           13
Constant Ionization current =  4.65477e-12   +/-   2.05968e-14 
9.817e-12
5.616e-12




TImage *img = TImage::Create();

****************************************
Minimizer is Minuit / Migrad
Chi2                      =  7.15521e-24
NDf                       =           77
Edm                       =  2.90573e-39
NCalls                    =           13
Constant Ionization current =   5.2911e-12   +/-   3.45158e-14 
1.139e-11
6.1e-12




TImage *img = TImage::Create();

****************************************
Minimizer is Minuit / Migrad
Chi2                      =  2.64482e-24
NDf                       =           78
Edm                       =  2.42503e-38
NCalls                    =           13
Constant Ionization current =   6.0869e-12   +/-   2.07175e-14 
1.207e-11
7.164e-12




TImage *img = TImage::Create();

****************************************
Minimizer is Minuit / Migrad
Chi2                      =  2.94629e-24
NDf                       =           77
Edm                       =  8.96831e-38
NCalls                    =           13
Constant Ionization current =  6.66959e-12   +/-   2.21485e-14 
(int) 0
