    //#include <SigFox.h>

//#include <ArduinoLowPower.h>
#include "Serie.h"
#include "Compressor.h"
#include "Compactor.h"
#include <ESP8266WiFi.h>

using namespace std;

void setup() {
  Serial.begin(115200);                                       
  delay(10); Serial.println('\n');

  // données d'exemple  
  Serial.println("données exemple : ");
  const int n = 16;
  const int p = 6;
  const int l = 2;
  float y0i[n] = { 2, 3.5, 5, 15, 20, 16, 18, 6, 8, 3.5, 5, 10, 12, 10, 12, 18 };
  Serie sn; // pour appel des fonctions statiques
  Serie y0 = Serie(n, "y0"); y0.setSerie(y0i, n); Serial.println(y0.pr());
  Serie x0 = Serie(n, "x0", 1, 16); Serial.println(x0.pr());
  Serie yt0 = y0.sousSerie(0, 6) | y0.sousSerie(9, 7); yt0.setNom("yt0"); Serial.println(yt0.pr());
  Serie xt0 = x0.sousSerie(0, 6) | x0.sousSerie(9, 7); xt0.setNom("xt0"); Serial.println(xt0.pr());
  Serie xtr0 = x0.sousSerie(6, 3); xtr0.setNom("xtr0"); Serial.println(xtr0.pr());
  float xpi[p] = { 1, 4, 7, 10, 13, 16 };
  Serie xp = Serie(p, "xp"); xp.setSerie(xpi, p); Serial.println(xp.pr());
  float ypi[p] = { 1, 12, 20, 5, 10, 17 };
  Serie yp = Serie(p, "yp"); yp.setSerie(ypi, p); Serial.println(yp.pr());
  Serie xl = Serie(l, "xl"); xl[0] = 1; xl[1] = 16; Serial.println(xl.pr()+'\n');
  
  
  // exemple information 
  float moy = y0.moyenne();  // 10, 25
  float ect = y0.ecartType(); //  5.67
  Serial.println("Exemple informations : moyenne : " + String(moy ) + " écart-type : " + String(ect));

  // exemple création
  Serie x(16, "x", 0, 15); // [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
  Serie y(16, "y", 1, 20); // y : [1.00   2.27   3.53   4.80   6.07   7.33   8.60   9.87  11.13  12.40  13.67  14.93  16.20  17.47  18.73  20.00]
  Serie ym(16, "ym", y.moyenne()); //ym : [10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50  10.50]
  Serie ye = y.ecretage(5, 15); // ye : [5.00   5.00   5.00   5.00   6.07   7.33   8.60   9.87  11.13  12.40  13.67  14.93  15.00  15.00  15.00  15.00]
  Serie yc = ye + 0.4f * ym; // yc : [9.20   9.20   9.20   9.20  10.27  11.53  12.80  14.07  15.33  16.60  17.87  19.13  19.20  19.20  19.20  19.20]
  Serial.println(x.pr());
  Serial.println(y.pr());
  Serial.println(ym.pr());
  Serial.println(ye.pr());
  Serial.println(yc.pr()+'\n');

  // exemple complément de valeurs
  float yp0 = y0[5] - y0[4];
  float yp1 = y0[10] - y0[9];
  Serie ytr0 = sn.intCont(xt0[5], yt0[5], yp0, x0[9], y0[9], yp1, xtr0); // ytr0 : [11.52  7.0  3.86];
  Serie y1 = y0.sousSerie(0, 6) | ytr0 | y0.sousSerie(9, 7); // y1 : [2, 3.5, 5, 15, 20, 16, 11.51, 7.0, 3.86, 3.5, 5, 10, 12, 10, 12, 18];
  Serie y2 = sn.intLin(xt0, yt0, x0); // yn : [2.0 3.5 5.0 15.0 20.0 16.0 12.9 9.8 6.6 3.5 5.0 10.0 12.0 10.0 12.0 18.0]
  Serie yx3 = y2.intLin(xt0, yt0, x0);
  Serial.println("exemple complément de valeurs :" + ytr0.pr());
  Serial.println(y1.pr());
  Serial.println(y2.pr());
  Serial.println(yx3.pr()+'\n');

  // exemple compression simple
  Serie yr = sn.regPol(x0, y0, xp); // yr : [0.81  13.49  14.63   5.89   8.62  15.69]
  Serie yre = sn.intPol(xp, yr, x0); // [0.81   4.15   9.13  13.49  16.00  16.30  14.63  11.71   8.46   5.89   4.83   5.76   8.62  12.60  15.92  15.69]
  float etr = sn.etDiff(y0, yre); // 2.999
  float ecr = sn.ecDiff(y0, yre); // 2.520
  Serie yl = sn.regPol(x0, y0, xl); // yl : [7.91  12.59]
  Serie yle = sn.intPol(xl, yl, x0); // yle : [7.91   8.22   8.54   8.85   9.16   9.47   9.78  10.09  10.41  10.72  11.03  11.34  11.65  11.96  12.28  12.59]
  float etl = sn.etDiff(y0, yle); // 2.999
  float ecl = sn.ecDiff(y0, yle); //2.520
  Serial.println("exemple compression simple : ");
  Serial.println("ecart compression : " + String(etr) + " " + String(ecr));
  Serial.println(yre.pr());
  Serial.println(yr.pr());
  Serial.println(yle.pr());
  Serial.println(yl.pr()+'\n');

  // exemple interpolation
  y1 = sn.intPol(xp, yp, x0); // y1 : [1.00 - 2.57   3.48  12.00  18.64  21.39  20.00  15.54   9.84   5.00   2.89   4.61  10.00  17.14  21.81  17.00]
  y2 = sn.intSpline(xp, yp, x0, 0, 0); // y2 : [1.00   4.31   7.88  12.00  16.55  19.89  20.00  15.76   9.63   5.00   4.36   6.66  10.00  12.82  15.06  17.00]
  Serie y3 = sn.intLin(xp, yp, x0); // y3 : [1.00   4.67   8.33  12.00  14.67  17.33  20.00  15.00  10.00   5.00   6.67   8.33  10.00  12.33  14.67  17.00]
  float lamb = 5;
  Serie yp2 = sn.lisSpline(xp, yp, lamb); // yp2: [2.84  10.99  14.93   9.52  11.00  15.71]
  Serie y4 = sn.intSpline(xp, yp2, x0, 0, 0); // y4 : [2.84   5.66   8.40  10.99  13.28  14.78  14.93  13.48  11.29   9.52   9.07   9.73  11.00  12.50  14.08  15.71]
  Serial.println("exemple interpolation : " + y1.pr());
  Serial.println(y2.pr());
  Serial.println(y3.pr());
  Serial.println(yp2.pr());
  Serial.println(y4.pr()+'\n');

  // exemple codage
  int bit = 3;
  Serie payl = yp.normalisation(0, 25).codage(p, bit); // [0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1]
  Serie ypc = payl.decodage(p, bit).denormalisation(0, 25); // ypc : [1.56  10.94  20.31   4.69  10.94  17.19]
  float ecart1 = sn.etDiff(yp, ypc); // 0.653
  float ecart2 = sn.ecDiff(yp, ypc); // 0.563
  Serial.println("exemple codage : " + payl.pr());
  Serial.println(ypc.pr());
  Serial.println("ecart codage : " + String(ecart1) + " " + String(ecart2));

  // exemples compression (compactor, compressor)

  // parametres normalistion
  float mini = 0.0; // plage mini et maxi des mesures prise en compte(écrétage sinon)
  float maxi = 25.0; // plage mini et maxi des mesures prise en compte(écrétage sinon)
  // parametres compression
  int nbreg = 8; // nombre de régression de niveau 1 ex. 8, 0 si aucune
  int nbreg0 = 2; // nombre de points de la regression de niveau 0 (entre 2 et 10)
  int nbreg1 = 2; // nombre de points de la regression de niveau 1 (entre 2 et 10)
  // parametres codage
  int bit0 = 4; // nb de bits pour les coeff de niveau 0 ex. 8
  int bit1 = 4; // nb de bits pour les coeff de niveau 1 ex. 4
  int bitEct = 8; // nb de bits pour l'écart-type ex. 8
  int codageEct = 0; // codage de l'écart-type réultant (0: non, 1: oui)

  //compression 1 : estimation unique, un seul point (moyenne)
  {
    nbreg = 0;
    nbreg0 = 1;
    bit0 = 3;
    Compactor comp(nbreg0, mini, maxi, bit0, bitEct, codageEct);
    String res = comp.calcul(y0, true);
    y1 = comp.simul(); // [10.94 ... 10.94]
    Serie y11 = comp.decompressY0(comp.compress(), y0.len()); // valeurs idem;
    Serie ectc1 = comp.ecartTypeSimul(false); // 5.71
    // résultats identiques avec Compressor;
    /*Compressor combo(nbreg, nbreg0, nbreg1, mini, maxi, bit0, bit1, bitEct, codageEct);
    res = combo.calcul(y0, true);
    y2 = combo.simul();
    y21 = combo.decompressY0(combo.compress());
    float ectc2 = combo.ecartTypeSimul(false);*/
    Serial.println("1 comp : bits : " + String(comp.taillePayload()) + " taux : " + String(comp.tauxCompression())); // bits :  3  taux : 0.012
    Serial.println("ectc1 : " + String(ectc1[0]) + String(comp.decompressEct(comp.compress())[0])); // ectc1 : 5.71 0.0
    Serial.println(y1.pr()); 
    Serial.println(y11.pr()+'\n'); 
  }
  //compression 2 : estimation unique, deux point (linéaire)
  {
    nbreg = 0;
    nbreg0 = 2;
    bit0 = 3;
    Compactor comp(nbreg0, mini, maxi, bit0, bitEct, codageEct);
    String res = comp.calcul(y0, true);
    y1 = comp.simul(); // [7.81, 8.23, 8.65, 9.06, 9.48, 9.90, 10.31, 10.73, 11.15, 11.56, 11.98, 12.40, 12.81, 13.23, 13.64, 14.06]
    Serie ectc1 = comp.ecartTypeSimul(false); // 5.55
    // résultats identiques avec Compressor
    Serial.println("2 comp : bits : " + String(comp.taillePayload()) + " taux : " + String(comp.tauxCompression())); //bits :  6  taux : 0.023
    Serial.println("ectc1 : " + String(ectc1[0]) + String(comp.decompressEct(comp.compress())[0])); // ectc1 : 5.55 0.0
    Serial.println(y1.pr()+'\n');
  }
  //compression 3 : estimation unique, six points (polynomiale)
  {
    nbreg = 0;
    nbreg0 = 6;
    bit0 = 3;
    Compactor comp(nbreg0, mini, maxi, bit0, bitEct, codageEct);
    String res = comp.calcul(y0, true);
    y1 = comp.simul(); // [1.56   5.12  10.00  14.06  16.20  16.09  14.06  10.83   7.38   4.69   3.62   4.67   7.81  12.26  16.33  17.19]
    Serie ectc1 = comp.ecartTypeSimul(false); // 3.12
    // résultats identiques avec Compressor
    Serial.println("3 comp : bits : " + String(comp.taillePayload()) + " taux : " + String(comp.tauxCompression())); //bits : 18  taux :  0.070
    Serial.println("ectc1 : " + String(ectc1[0]) + String(comp.decompressEct(comp.compress())[0])); // ectc1 : 3.12 0.0
    Serial.println(y1.pr()+'\n');
  }
  //compression 4 : estimation unique, seize points (codage de chaque point)
  {
    nbreg = 16;
    nbreg0 = 0;
    nbreg1 = 1;
    bit1 = 3;
    Compressor combo(nbreg, nbreg0, nbreg1, mini, maxi, bit0, bit1, bitEct, codageEct);
    String res = combo.calcul(y0, true);
    y2 = combo.simul(); // [1.56   4.69   4.69  14.06  20.31  17.19  17.19   4.69   7.81   4.69   4.69  10.94  10.94  10.94  10.94  17.19]
    Compressor combo2(nbreg, nbreg0, nbreg1, mini, maxi, bit0, bit1, bitEct, codageEct);
    Serie y21 = combo2.decompressY0(combo.compress(), n);
    Serie ectc2 = combo.ecartTypeSimul(false);
    float precision = combo.precisionCodage();

    Serial.println("4 comp : bits : " + String(combo.taillePayload()) + " taux : " + String(combo.tauxCompression())); // bits :  48  taux : 0.188
    Serial.println("ectc2, precision : " + String(ectc2[0]) + String(combo.decompressEct(combo.compress())[0]) + " " + String(precision)); // ectc2 : 0.89 0.0 3.57
    Serial.println(y2.pr());
    Serial.println(y21.pr());
    Serial.println(combo.compress().pr()+'\n');
  }
  //compression 5 : estimation mixte, un point (moyenne) puis quatre interpolations linéaires
  {
    nbreg = 4;
    nbreg0 = 1;
    nbreg1 = 2;
    bit1 = 2;
    bit0 = 3;
    Compressor combo(nbreg, nbreg0, nbreg1, mini, maxi, bit0, bit1, bitEct, codageEct);
    String res = combo.calcul(y0, true);
    y2 = combo.simul(); // [2.37   6.18   9.99  13.79  19.51  15.70  11.89   8.08   8.08   8.08   8.08   8.08   8.08   9.99  11.89  13.79]
    Compressor combo2(nbreg, nbreg0, nbreg1, mini, maxi, bit0, bit1, bitEct, codageEct);
    Serie y21 = combo2.decompressY0(combo.compress(), n);
    Serie ectc2 = combo.ecartTypeSimul(false);
    float precision = combo.precisionCodage();

    Serial.println("5 comp : bits : " + String(combo.taillePayload()) + " taux : " + String(combo.tauxCompression())); // bits :  27  taux : 0.11
    Serial.println("ectc2, precision : " + String(ectc2[0]) + String(combo.decompressEct(combo.compress())[0]) + " " + String(precision)); // ectc2 : 2.94 0.0 7.62
    Serial.println(y2.pr());
    Serial.println(y21.pr());
    Serial.println(combo.compress().pr()+'\n');
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
