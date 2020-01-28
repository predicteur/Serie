/*
Comp_or.cpp - Serie library v1.0.0 - 2020-01-21
Copyright (C) 2019-2020 Philippe Thomy.  All rights reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
See file LICENSE.txt for further informations on licensing terms.
*/
#include "Comp_or.h"
#include "Serie.h"

using namespace std;

//******************************************************************************
//* Constructors - destructor
//******************************************************************************

Comp_or::Comp_or(float mini, float maxi, int bits, int bitEct, int bitYp, int codageEct) {
	// normalization data
	MINI		= mini;	
	MAXI		= maxi;
	// internal parameters
	BITS		= bits;
	BITECT		= bitEct;
	BITYP		= bitYp;
	calculKo	= true;
	// coding parameters
	CODAGEECT	= codageEct;
}
Comp_or::~Comp_or()	{ }

//******************************************************************************
//* Class Functions
//******************************************************************************

//-----------------------------------------------------------------------------------------------------------------------------
/* precision of the coding used (maximal eror) */
float	Comp_or::precisionCodage()	{ return precCod; }
//-----------------------------------------------------------------------------------------------------------------------------
/* number of bits of compressed data */
int		Comp_or::taillePayload()	{ return BITS; }
//-----------------------------------------------------------------------------------------------------------------------------
/* ratio between the number of bits after compression and the number of bits before compression (2 bytes per value) */
float	Comp_or::tauxCompression()	{ return float(BITS) / float(y0.len()) / 16.0f; }
//-----------------------------------------------------------------------------------------------------------------------------
/* simulated values after compression / decompression (usable after calcul) */
Serie	Comp_or::simul()			{ return yr0; }
//-----------------------------------------------------------------------------------------------------------------------------
/* compressed value of standard deviation (usable after calcul) */
Serie	Comp_or::compressEct()		{ return paylEct; }
//-----------------------------------------------------------------------------------------------------------------------------
/* compressed value whithout standard deviation (usable after calcul) */
Serie	Comp_or::compressYp()		{ return paylYp; }
//-----------------------------------------------------------------------------------------------------------------------------
/* check the input parameters */
String	Comp_or::check() {
	String resultat = "ok";
	if (MINI > MAXI)						resultat = " seuils mini - maxi incohérents";
	if (BITECT < 1)							resultat = " nombre de bits insuffisant";
	if (CODAGEECT == 1 and BITS < BITECT)	resultat = " nombre de bits incohérents";
	return resultat;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* standard deviation
@arg codec : True -> calculation with coding, False -> whithout coding
@result : Serie with only one value */
Serie	Comp_or::ecartTypeSimul(bool codec) {
	if (calculKo) return 0;
	Serie ect(1, "ect", Serie::etDiff(y0, yr0));
	Serie ects(ect);
	if (codec) ects = ect.normalisation(MINI, MAXI).codage(1, BITECT).decodage(1, BITECT).denormalisation(MINI, MAXI);
	return ects;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* compressed value (usable after calcul) */
Serie	Comp_or::compress() {
	Serie payl(paylYp);
	if (CODAGEECT == 1) payl |= paylEct;
	payl.setNom("payl");
	return payl;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* decompressed value for standard deviation (Serie with only one value) */
Serie	Comp_or::decompressEct(Serie payl) {
	Serie serieEct(1, "ect", 0.0f);
	if (CODAGEECT == 0) return serieEct;
	serieEct = payl.sousSerie(BITS - BITECT, BITECT).decodage(1, BITECT).denormalisation(MINI, MAXI);
	return serieEct;
}

