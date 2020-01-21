/*
Comp_or.h - Serie library v1.0.0 - 2020-01-21
Copyright (C) 2019-2020 Philippe Thomy.  All rights reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef COMP_OR
#define COMP_OR
#include "Serie.h"

/*#include <iostream>
#pragma once
#include <algorithm>
#include <string>*/

class Comp_or {
protected:
	// parametres normalistion
	float	MINI;					//plage mini et maxi des mesures prise en compte(écrétage sinon)
	float	MAXI;					//plage mini et maxi des mesures prise en compte(écrétage sinon)
	// paramètres internes
	int		BITS;
	int		BITECT;					// nb de bits pour l'écart-type ex. 8
	int		BITYP;					// nb de bits pour l'écart-type ex. 8
	int		CODAGEECT;				// prise encompte de l'écart-type dans le codage (0: non, 1: oui)
	Serie	y0;						// valeurs à compresser
	Serie	yr0;					// résultat non normalisé
	bool	calculKo;
	// paramètres codage
	Serie	paylEct;
	Serie	paylYp;
	float	precCod;
public:
	Comp_or(float mini, float maxi, int bits, int bitEct, int bitYp, int codageEct);
	~Comp_or();
	float		precisionCodage();
	int			taillePayload();
	float		tauxCompression();
	String		check();
	Serie		simul();
	Serie		ecartTypeSimul(bool codec);
	Serie		compressEct();
	Serie		compressYp();
	Serie		compress();
	Serie		decompressEct(Serie payl);
};
#endif

