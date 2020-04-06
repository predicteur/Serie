
//#include <SigFox.h>
//#include <ArduinoLowPower.h>
#include <ESP8266WiFi.h>

#include <Mesure.h>
#include <Compressor.h>

using namespace std;

typedef struct __attribute__ ((packed)) sigfox_message {
	uint32_t  msg1;
	uint32_t  msg2;
	uint32_t  msg3;
} SigfoxMessage;

// example data
int histo = 16, compteur = 0;
float mini = -20.0f, maxi = 50.0f, alpha = 0.5f;
String liste[6] = { "valeurMin", "valeurMax", "capteur", "type", "valeur", "valeurFiltree" };
String listenot[4] = { "valeurMin", "valeurMax", "type", "valeur" };
String res;
SigfoxMessage sigMes;

// init Mesure
Mesure mes = Mesure(2, 2, 2, histo, "Temperature", liste);

// init Compressor
int nbreg = 8, nbreg0 = 1, nbreg1 = 2, bit1 = 4, bit0 = 3, bitEct = 8, codageEct = 0;
Compressor combo(nbreg, nbreg0, nbreg1, mini, maxi, bit0, bit1, bitEct, codageEct);

void setup() {
	Serial.begin(115200);                                       
	delay(10); Serial.println(); 
  
	mes.setVal("capteur", "CT212");
	mes.setVal("type", "Temp");
	mes.setVal("valeurMin", mini);
	mes.setVal("valeurMax", maxi);

	Serial.println(mes.pr());
	Serial.println(mes.json());Serial.println(); 

	Serial.println(" bits : " + String(combo.taillePayload()) + " codage : " + String(combo.tauxCompression())); Serial.println();  // bits :  27  taux : 0.11
}

void loop() {
  // one step more
  compteur++;
	mes.refresh(0);

	//data acquisition simulation
	float temp = 60.0f * float(cos(3.14159 * float(random(100)) / 100.0));

	//Mesure update
	mes["valeur"][0] = temp;
	mes["valeur"] = mes["valeur"].ecretage(mes.getFloat("valeurMin"), mes.getFloat("valeurMax"));
	mes["valeurFiltree"].lissES(mes["valeur"], alpha, false);

	//send "valeur filtree" with sigfox
	if (compteur >= histo) {
		compteur = 0;

		res = combo.calcul(mes["valeurFiltree"], true);
		Serie valComp = combo.compress();

		Serial.println("ecart-type, precision codage : " + String(combo.ecartTypeSimul(false)[0]) + " " + String(combo.precisionCodage()));
		Serial.println("valeurs avant decompression : " + mes["valeurFiltree"].pr());
		Serial.println("valeurs apres decompression : " + combo.simul().pr());
		Serial.println("valeurs compressees envoyees :" + valComp.pr());

		sigMes.msg1 = sn.decbin(valComp.sousSerie( 0, 32), 32);
		sigMes.msg2 = sn.decbin(valComp.sousSerie(32, 32), 32);
		sigMes.msg3 = sn.decbin(valComp.sousSerie(64, 32), 32);

		// export sigMes with sigfox

		// decompress example
		Serie valCompRec = sn.codbin(sigMes.msg1, 32) | sn.codbin(sigMes.msg2, 32) | sn.codbin(sigMes.msg3, 11);
		valCompRec.setNom("payr");
		Serial.println("valeur compressees decodees : " + valCompRec.pr() + '\n'); 
    	Serial.println("valeurs reconstituées :   " + combo.decompressY0(combo.compress(), histo).pr()); Serial.println();
   }
} 
