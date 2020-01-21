/*
Compactor.h - Serie library v1.0.0 - 2020-01-21
Copyright (C) 2019-2020 Philippe Thomy.  All rights reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
See file LICENSE.txt for further informations on licensing terms.
*/
#ifndef COMPACTOR
#define COMPACTOR
#include "Comp_or.h"

class Compactor : public Comp_or {
protected:
	int		NBREG0;				// nombre de points de la regression
	int		BIT;				// nombre de bits de codage de chaque point de régression
	Serie	yp0;				// liste des points de régression (normalisé)
public:
	 Compactor(const int nbreg0, const float mini, const float maxi, const int bit, const int bitEct, const int codageEct);
	~Compactor();
	String		check();
	String		calcul(Serie y, bool codec);
	Serie		param();

	// fonctions à utiliser uniquement en mode récepteur
	Serie		decompressYp(Serie payload);
	Serie		decompressY0(Serie payload, int taille);
};
#endif