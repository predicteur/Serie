
//#include <SigFox.h>
//#include <ArduinoLowPower.h>
#include <ESP8266WiFi.h>

#include "Serie.h"
#include "Compressor.h"
#include "Compactor.h"

using namespace std;

void setup() {
  Serial.begin(115200);                                       
  delay(10); Serial.println(); Serial.println();

  // données d'exemple  
  Serial.println("données exemple : ");
  const int n = 16;
  float y0i[n] = { 2, 3.5, 5, 15, 20, 16, 18, 6, 8, 3.5, 5, 10, 12, 10, 12, 18 };
  Serie y0 = Serie(n, "y0"); y0.setSerie(y0i, n); 						                              Serial.println(y0.pr());
  Serie x0 = Serie(n, "x0", 1, 16); 								                                        Serial.println(x0.pr() +'\n');
  
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
    Serie y1 = comp.simul(); // [10.94 ... 10.94]
    Serie y11 = comp.decompressY0(comp.compress(), y0.len()); // valeurs idem;
    Serie ectc1 = comp.ecartTypeSimul(false); // 5.71
    // résultats identiques avec Compressor;
    /*Compressor combo(nbreg, nbreg0, nbreg1, mini, maxi, bit0, bit1, bitEct, codageEct);
    res = combo.calcul(y0, true);
    y2 = combo.simul();
    y21 = combo.decompressY0(combo.compress());
    float ectc2 = combo.ecartTypeSimul(false);*/
    Serial.println("compression 1 :"); Serial.println();
    Serial.println(" bits : " + String(comp.taillePayload()) + " taux : " + String(comp.tauxCompression())); // bits :  3  taux : 0.012
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
    Serie y1 = comp.simul(); // [7.81, 8.23, 8.65, 9.06, 9.48, 9.90, 10.31, 10.73, 11.15, 11.56, 11.98, 12.40, 12.81, 13.23, 13.64, 14.06]
    Serie ectc1 = comp.ecartTypeSimul(false); // 5.55
    // résultats identiques avec Compressor
    Serial.println("compression 2 :");Serial.println();
    Serial.println("bits : " + String(comp.taillePayload()) + " taux : " + String(comp.tauxCompression())); //bits :  6  taux : 0.023
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
    Serie y1 = comp.simul(); // [1.56   5.12  10.00  14.06  16.20  16.09  14.06  10.83   7.38   4.69   3.62   4.67   7.81  12.26  16.33  17.19]
    Serie ectc1 = comp.ecartTypeSimul(false); // 3.12
    // résultats identiques avec Compressor
    Serial.println("compression 3 : "); Serial.println();
    Serial.println("bits : " + String(comp.taillePayload()) + " taux : " + String(comp.tauxCompression())); //bits : 18  taux :  0.070
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
    Serie y2 = combo.simul(); // [1.56   4.69   4.69  14.06  20.31  17.19  17.19   4.69   7.81   4.69   4.69  10.94  10.94  10.94  10.94  17.19]
    Compressor combo2(nbreg, nbreg0, nbreg1, mini, maxi, bit0, bit1, bitEct, codageEct);
    Serie y21 = combo2.decompressY0(combo.compress(), n);
    Serie ectc2 = combo.ecartTypeSimul(false);
    float precision = combo.precisionCodage();

    Serial.println("compression 4 : "); Serial.println();
    Serial.println("bits : " + String(combo.taillePayload()) + " taux : " + String(combo.tauxCompression())); // bits :  48  taux : 0.188
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
    Serie y2 = combo.simul(); // [2.37   6.18   9.99  13.79  19.51  15.70  11.89   8.08   8.08   8.08   8.08   8.08   8.08   9.99  11.89  13.79]
    Compressor combo2(nbreg, nbreg0, nbreg1, mini, maxi, bit0, bit1, bitEct, codageEct);
    Serie y21 = combo2.decompressY0(combo.compress(), n);
    Serie ectc2 = combo.ecartTypeSimul(false);
    float precision = combo.precisionCodage();

    Serial.println("compression 5 : ");Serial.println();
    Serial.println("bits : " + String(combo.taillePayload()) + " taux : " + String(combo.tauxCompression())); // bits :  27  taux : 0.11
    Serial.println("ectc2, precision : " + String(ectc2[0]) + String(combo.decompressEct(combo.compress())[0]) + " " + String(precision)); // ectc2 : 2.94 0.0 7.62
    Serial.println(y2.pr());
    Serial.println(y21.pr());
    Serial.println(combo.compress().pr()+'\n');
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
