
//#include <SigFox.h>
//#include <ArduinoLowPower.h>
#include <ESP8266WiFi.h>

#include <Serie.h>
#include <Compressor.h>
#include <Compactor.h>

using namespace std;

void setup() {
  Serial.begin(115200);                                       
  delay(10); Serial.println();

  // example data  
  Serial.println(); Serial.println("example data : "); Serial.println();
  const int n = 16, p = 6, l = 2;
  float y0i[n] = { 2, 3.5, 5, 15, 20, 16, 18, 6, 8, 3.5, 5, 10, 12, 10, 12, 18 };
  Serie y0 = Serie(n, "y0"); y0.setSerie(y0i, n); 						                              Serial.println(y0.pr());
  Serie x0 = Serie(n, "x0", 1, 16); 								                                        Serial.println(x0.pr());
  Serie yt0 = y0.sousSerie(0, 6) | y0.sousSerie(9, 7); yt0.setNom("yt0"); 			            Serial.println(yt0.pr());
  Serie xt0 = x0.sousSerie(0, 6) | x0.sousSerie(9, 7); xt0.setNom("xt0"); 			            Serial.println(xt0.pr());
  Serie xtr0 = x0.sousSerie(6, 3); xtr0.setNom("xtr0"); 					                          Serial.println(xtr0.pr());
  Serie xp = Serie(p, "xp"); float xpi[p] = { 1,  4, 7, 10, 13, 16 }; xp.setSerie(xpi, p); 	Serial.println(xp.pr());
  Serie yp = Serie(p, "yp"); float ypi[p] = { 1, 12, 20, 5, 10, 17 }; yp.setSerie(ypi, p); 	Serial.println(yp.pr());
  Serie xl = Serie(l, "xl"); xl[0] = 1; xl[1] = 16; 						                            Serial.println(xl.pr()); Serial.println();
  
  
  // example information 
  float moy = y0.moyenne();  // 10, 25
  float ect = y0.ecartType(); //  5.67
  Serial.println("Example informations : "); Serial.println();
  Serial.println("average : " + String(moy ) + " standard deviation : " + String(ect)); Serial.println();

  // example creation
  Serie x(16, "x", 0, 15); // [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
  Serie y(16, "y", 1, 20); // y : [1.00   2.27   3.53   4.80   6.07   7.33   8.60   9.87  11.13  12.40  13.67  14.93  16.20  17.47  18.73  20.00]
  Serie ym(16, "ym", y.moyenne()); //ym : [10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50]
  Serie ye = y.ecretage(5, 15); // ye : [5.00   5.00   5.00   5.00   6.07   7.33   8.60   9.87  11.13  12.40  13.67  14.93  15.00  15.00  15.00  15.00]
  Serie yc = ye + 0.4f * ym; // yc : [9.20   9.20   9.20   9.20  10.27  11.53  12.80  14.07  15.33  16.60  17.87  19.13  19.20  19.20  19.20  19.20]
  yc.refresh(10); // yc : [10.00  9.20   9.20   9.20   9.20  10.27  11.53  12.80  14.07  15.33  16.60  17.87  19.13  19.20  19.20  19.20]
  Serial.println("Example creation : "); Serial.println();
  Serial.println(x.pr());
  Serial.println(y.pr());
  Serial.println(ym.pr());
  Serial.println(ye.pr());
  Serial.println(yc.pr()); Serial.println();

  // example smoothing
  bool causal = false;
  Serie yli0 = sn.lisSA(y0, 4, causal); // ySA : [2.75   4.75   8.62  12.44  15.62  16.12  13.50  10.44   7.25   6.12   7.12   8.44  10.12  12.00  13.75  15.50]
  Serie yli1 = sn.lisSG(y0, 5, 2, true);// ySG : [2.00   2.98   4.65  11.71  19.42  17.96  16.85  10.38   5.92   5.29   3.96   8.44  11.88  10.92  11.08  16.15]
  Serie yli2 = sn.lisGA(y0, 5, causal); // yGA : [2.56   4.12   7.88  13.09  16.69  16.81  14.00   9.97   6.81   5.56   6.50   8.84  10.56  11.50  13.38  16.00]
  Serie yli3 = sn.lisWA(y0, 5, causal); // yWA : [2.67   4.61   8.22  12.72  16.11  16.11  14.00   9.94   7.33   5.83   6.89   8.61  10.33  11.78  13.56  15.78]
  Serie yli4 = sn.lisES(y0, 0.4f, true); // yES : [2.00   2.96   4.35  11.36  17.66  17.75  18.78  11.34   9.09   5.23   4.52   7.54  10.30  10.29  11.52  15.88]
  Serial.println("Example smoothing : "); Serial.println();
  Serial.println(yli0.pr());
  Serial.println(yli1.pr());
  Serial.println(yli2.pr());
  Serial.println(yli3.pr());
  Serial.println(yli4.pr()); Serial.println();

  // exemple complément de valeurs
  float yp0 = y0[5] - y0[4];
  float yp1 = y0[10] - y0[9];
  Serie ytr0 = sn.intCont(xt0[5], yt0[5], yp0, x0[9], y0[9], yp1, xtr0); // ytr0 : [11.52  7.0  3.86];
  Serie y1 = y0.sousSerie(0, 6) | ytr0 | y0.sousSerie(9, 7); // y1 : [2, 3.5, 5, 15, 20, 16, 11.51, 7.0, 3.86, 3.5, 5, 10, 12, 10, 12, 18];
  Serie y2 = sn.intLin(xt0, yt0, x0); // yn : [2.0 3.5 5.0 15.0 20.0 16.0 12.9 9.8 6.6 3.5 5.0 10.0 12.0 10.0 12.0 18.0]
  Serial.println("exemple complément de valeurs :"); Serial.println();
  Serial.println(ytr0.pr());
  Serial.println(y1.pr());
  Serial.println(y2.pr()); Serial.println();

  // exemple compression simple
  Serie yr = sn.regPol(x0, y0, xp); // yr : [0.81  13.49  14.63   5.89   8.62  15.69]
  Serie yre = sn.intPol(xp, yr, x0); // [0.81   4.15   9.13  13.49  16.00  16.30  14.63  11.71   8.46   5.89   4.83   5.76   8.62  12.60  15.92  15.69]
  float etr = sn.etDiff(y0, yre); // 2.999
  float ecr = sn.ecDiff(y0, yre); // 2.520
  Serie yl = sn.regPol(x0, y0, xl); // yl : [7.91  12.59]
  Serie yle = sn.intPol(xl, yl, x0); // yle : [7.91   8.22   8.54   8.85   9.16   9.47   9.78  10.09  10.41  10.72  11.03  11.34  11.65  11.96  12.28  12.59]
  float etl = sn.etDiff(y0, yle); // 2.999
  float ecl = sn.ecDiff(y0, yle); //2.520
  Serial.println("exemple compression simple : "); Serial.println();
  Serial.println("ecart compression : " + String(etr) + " " + String(ecr));
  Serial.println(yre.pr());
  Serial.println(yr.pr());
  Serial.println(yle.pr());
  Serial.println(yl.pr()); Serial.println();

  // exemple interpolation
  y1 = sn.intPol(xp, yp, x0); // y1 : [1.00 - 2.57   3.48  12.00  18.64  21.39  20.00  15.54   9.84   5.00   2.89   4.61  10.00  17.14  21.81  17.00]
  y2 = sn.intSpline(xp, yp, x0, 0, 0); // y2 : [1.00   4.31   7.88  12.00  16.55  19.89  20.00  15.76   9.63   5.00   4.36   6.66  10.00  12.82  15.06  17.00]
  Serie y3 = sn.intLin(xp, yp, x0); // y3 : [1.00   4.67   8.33  12.00  14.67  17.33  20.00  15.00  10.00   5.00   6.67   8.33  10.00  12.33  14.67  17.00]
  float lamb = 5;
  Serie yp2 = sn.lisSpline(xp, yp, lamb); // yp2: [2.84  10.99  14.93   9.52  11.00  15.71]
  Serie y4 = sn.intSpline(xp, yp2, x0, 0, 0); // y4 : [2.84   5.66   8.40  10.99  13.28  14.78  14.93  13.48  11.29   9.52   9.07   9.73  11.00  12.50  14.08  15.71]
  Serial.println("exemple interpolation : "); Serial.println();
  Serial.println(y1.pr());
  Serial.println(y2.pr());
  Serial.println(y3.pr());
  Serial.println(yp2.pr());
  Serial.println(y4.pr()); Serial.println();

  // exemple codage
  int bit = 3;
  Serie payl = yp.normalisation(0, 25).codage(p, bit); // [0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1]
  Serie ypc = payl.decodage(p, bit).denormalisation(0, 25); // ypc : [1.56  10.94  20.31   4.69  10.94  17.19]
  float ecart1 = sn.etDiff(yp, ypc); // 0.653
  float ecart2 = sn.ecDiff(yp, ypc); // 0.563
  Serial.println("exemple codage : "); Serial.println();
  Serial.println(payl.pr());
  Serial.println(ypc.pr());
  Serial.println("ecart codage : " + String(ecart1) + " " + String(ecart2)); Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:

}
