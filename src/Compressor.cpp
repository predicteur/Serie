/*
Compressor.cpp - Serie library v1.0.0 - 2020-01-21
Copyright (C) 2019-2020 Philippe Thomy.  All rights reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
See file LICENSE.txt for further informations on licensing terms.
*/
#include "Compressor.h"
#include "Compactor.h"

using namespace std;

Compressor::Compressor(const int nbreg, const int nbreg0, const int nbreg1, const float mini, const float maxi,
					   const int bit0, const int bit1, const int bitEct, const int codageEct) : Comp_or(mini, maxi, bitEct, bitEct, bitEct, codageEct) {
	// parametres compression
	NBREG  = nbreg;
	NBREG0 = nbreg0;
	NBREG1 = nbreg1;
	// parametres codage
	bBIT0   = bit0;
	bBIT1   = bit1;
	if (nbreg0 < 1) {
		CODAGEECT0 = 0;
		BITS = nbreg * nbreg1 * bit1 + codageEct * bitEct;
	}
	else if (nbreg < 1) {
		CODAGEECT0 = codageEct;
		BITS = nbreg0 * bit0 + CODAGEECT0 * bitEct;
	}
	else {
		CODAGEECT0 = 1;
		BITS = nbreg0 * bit0 + CODAGEECT0 * bitEct + nbreg * nbreg1 * bit1 + codageEct * bitEct;
	}
	BITYP = BITS - codageEct * bitEct;
	paylYp = Serie(BITYP, "paylYp");
}
Compressor::~Compressor(){}
//-----------------------------------------------------------------------------------------------------------------------------
String Compressor::check(){
	String resultat = Comp_or::check();
	if (NBREG0 > y0.len())							resultat = "nombre de points de la compression supérieur à celui de l'échantillon";
	if (NBREG > 0) if (NBREG1 > y0.len() / NBREG)	resultat = "nombre de points de la compression supérieur à celui de l'échantillon";
	if (bBIT0 < 1 or bBIT1 < 1)						resultat = "nombre de bits insuffisant";
	return resultat;
}
String Compressor::calcul(Serie y, bool codec) {
	int indPay  = 0;
	Serie y0fo(y.len()), y1fo;
	float mini1 = MINI, maxi1 = MAXI;
	y0 = y;
	paylYp = Serie();
	if (check() != "ok")  return check();

	// compression initiale
	if (NBREG0 > 0) {
		Compactor comp0(NBREG0, MINI, MAXI, bBIT0, BITECT, CODAGEECT0);
		String res = comp0.calcul(y0, codec);
		if (res != "ok") Serial.println(res);
		y0fo = comp0.simul();
		Serie ect0 = comp0.ecartTypeSimul(true);
		paylYp = comp0.compressYp();
		Serie paylEct0 = ect0.normalisation(MINI, MAXI).codage(1, BITECT);
		if (CODAGEECT0 > 0) paylYp |= paylEct0;
		precCod = comp0.precisionCodage();
		mini1 = -2.0f * ect0[0];
		maxi1 =  2.0f * ect0[0];
		indPay = NBREG0 * bBIT0 + BITECT * CODAGEECT0;
	}
	// compression complémentaire
	Serie y1 = y0 - y0fo; 
	if (NBREG > 0) {
		Compactor comp1(NBREG1, mini1, maxi1, bBIT1, BITECT, CODAGEECT);
		for (int i = 0; i < NBREG; i++) {
			String res = comp1.calcul(y1.sousSerie(y1.len() * i / NBREG, y1.len() / NBREG), codec);
			if (res != "ok") Serial.println(res);
			y1fo |= comp1.simul();
			paylYp |= comp1.compressYp();
		}
		indPay += NBREG1 * bBIT1 * NBREG;
		precCod = comp1.precisionCodage();
	}
	// donnees de sortie
	if (NBREG > 0) yr0 = (y0fo + y1fo).ecretage(MINI, MAXI);
	else yr0 = y0fo;
	yr0.setNom("yr0");
	calculKo = false;
	paylEct = ecartTypeSimul(true).normalisation(MINI, MAXI).codage(1, BITECT);
	return "ok";
}

// fonctions à utiliser uniquement en mode récepteur
Serie Compressor::decompressY0(Serie payl, int taille) {
	int indYp1 = 0;
	Serie yr1, yr, yr0(taille);
	float mini1 = MINI, maxi1 = MAXI;
	if (NBREG0 > 0) {
		indYp1 = NBREG0 * bBIT0 + BITECT * CODAGEECT0;
		Compactor comp0(NBREG0, MINI, MAXI, bBIT0, BITECT, CODAGEECT0);
		yr0 = comp0.decompressY0(payl, taille);
		Serie ect0 = comp0.decompressEct(payl);
		mini1 = -2 * ect0[0];
		maxi1 =  2 * ect0[0];
	}
	if (NBREG > 0) {
		Compactor comp1(NBREG1, mini1, maxi1, bBIT1, BITECT, CODAGEECT);
		for (int i = 0; i < NBREG; i++) yr1 |= comp1.decompressY0(payl.sousSerie(indYp1 + i * NBREG1 * bBIT1, NBREG1 * bBIT1), taille/NBREG);
		yr = yr0 + yr1;
	}
	else yr = yr0;
	yr.setNom("y0r");
	return yr;
}