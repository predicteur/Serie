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

class Compressor : public Comp_or {
protected:
	// compression parameters
	int		NBREG;					// number of linear regressions
	int		NBREG0;					// number of compressed points for level 0 (between 2 et 10)
	int		NBREG1;					// number of compressed points for level 1 (between 2 et 10)
	// coding parameters
	int		bBIT0;
	int		bBIT1;
	int		CODAGEECT0;

public:
	Compressor(const int nbreg, const int nbreg0, const int nbreg1, const float mini, const float maxi, 
		       const int bit0, const int bit1, const int bitEct, const int codageEct);
	~Compressor();
	String		check();
	String		calcul(Serie y, bool codec);	
	Serie		decompressY0(Serie payload, int taille);
};
#endif