/*
Compactor.cpp - Serie library v1.0.0 - 2020-01-21
Copyright (C) 2019-2020 Philippe Thomy.  All rights reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
See file LICENSE.txt for further informations on licensing terms.
*/
#include "Compactor.h"

using namespace std;

Compactor::Compactor(const int nbreg0, const float mini, const float maxi, const int bit, const int bitEct, const int codageEct) : Comp_or(mini, maxi, bitEct, bitEct, bitEct, codageEct) {
	NBREG0	= nbreg0;
	BIT		= bit;
	BITS	= nbreg0 * bit + codageEct * bitEct;
	BITYP	= nbreg0 * bit;
	paylYp	= Serie(BITYP, "paylYp");
	yp0		= Serie(nbreg0, "yp0");					// paramètres non normalisé
}
Compactor::~Compactor() {}

String Compactor::check(){
	String resultat = Comp_or::check();
	if (NBREG0 > y0.len())	resultat = "nombre de points de la compression supérieur à celui de l'échantillon";
	if (NBREG0 < 1)				resultat = "nombre de points doit être supérieur à 1";
	if (BIT < 1)				resultat = "nombre de bits insuffisant";
	return resultat;
}
String Compactor::calcul(Serie y, bool codec){
	y0 = y;
	if (check() != "ok")  return check();
	int p = NBREG0;
	yp0 = Serie(NBREG0, "yp0");
	precCod = float((MAXI - MINI) / (pow(2, BIT) - 1));
	Serie xn(y0.len(), "xn", 0.0f, y0.len() - 1), xp(p, "xp");
	if (p > 1) for (int i = 0; i < p; i++) xp[i] = float((y0.len() - 1) * i / (p - 1));
	Serie yp = Serie::regPol(xn, y0.normalisation(MINI, MAXI), xp);
	if (codec) for (int i = 0; i < NBREG0; i++) yp0[i] = Serie::conversionb(Serie::conversion(yp[i], -0.5, 0.5, BIT), -0.5, 0.5, BIT);
	else yp0 = yp;
	yp0.setNom("yp0");
	yr0 = Serie::intPol(xp, yp0, xn).denormalisation(MINI, MAXI); yr0.setNom("yr0");
	paylYp = yp0.codage(NBREG0, BIT);
	calculKo = false;
	paylEct = ecartTypeSimul(true).normalisation(MINI, MAXI).codage(1, BITECT);
	return "ok";
}
Serie Compactor::param(){
	Serie yp;
	if (calculKo) return yp;
	return yp0.denormalisation(MINI, MAXI);
}
// fonctions à utiliser uniquement en mode récepteur

Serie Compactor::decompressYp(Serie payload){ return payload.sousSerie(0, BITS - CODAGEECT * BITECT).decodage(NBREG0, BIT); }

Serie Compactor::decompressY0(Serie payload, int taille){
	Serie xn(taille, "xn", 0, taille - 1);
	Serie xp(NBREG0, "xp", 1.0f);
	if (NBREG0 > 1) {
		float *xpSerie = new float[NBREG0];
		for (int i = 0; i < NBREG0; i++) xpSerie[i] = float((taille - 1) * i / (NBREG0 - 1));
		if (NBREG0 > 1) xp.setSerie(xpSerie, NBREG0);
		delete xpSerie;
	}
	return Serie::intPol(xp, decompressYp(payload), xn).denormalisation(MINI, MAXI);
}

