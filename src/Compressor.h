/*
Compressor.h - Serie library v1.0.0 - 2020-01-21
Copyright (C) 2019-2020 Philippe Thomy.  All rights reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
See file LICENSE.txt for further informations on licensing terms.
*/
#ifndef COMPRESSOR
#define COMPRESSOR

#include "Comp_or.h"
//#include "Serie.h"
//#include <iostream>
//#include <algorithm>

struct SigfoxMessage {
	uint32_t msg1;
	uint32_t msg2;
	uint32_t msg3;
};
class Compressor : public Comp_or {
protected:
	// parametres compression
	int		NBREG;					// nombre de régression de niveau 1 ex. 8, 0 si aucune
	int		NBREG0;					// nombre de points de la regression de niveau 0 (entre 2 et 10)
	int		NBREG1;					// nombre de points de la regression de niveau 1 (entre 2 et 10)
	// parametres codage
	int		bBIT0;
	int		bBIT1;
	int		CODAGEECT0;
public:
	Compressor(const int nbreg, const int nbreg0, const int nbreg1, const float mini, const float maxi, 
		       const int bit0, const int bit1, const int bitEct, const int codageEct);
	~Compressor();
	String		check();
	String		calcul(Serie y, bool codec);
	
	// fonctions à utiliser uniquement en mode récepteur
	Serie		decompressY0(Serie payload, int taille);
};
#endif