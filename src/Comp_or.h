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

class Comp_or {
protected:
	// normalization data
	float	MINI;					//minimum, maximum thresholds
	float	MAXI;					//minimum, maximum thresholds
	// internal parameters
	int		BITS;					// total bits number
	int		BITECT;					// standard deviation bits number eg 8
	int		BITYP;					// parameters bits number eg 8
	int		CODAGEECT;				// standard deviation coding (0: no, 1: yes)
	Serie	y0;						// values to compress
	Serie	yr0;					// values after compress/decompress cycle
	bool	calculKo;				// result of calcul function
	// coding parameters
	Serie	paylEct;				// standard deviation compressed
	Serie	paylYp;					// parameters compressed
	float	precCod;				// precision of the coding
public:
	Comp_or(float mini, float maxi, int bits, int bitEct, int bitYp, int codageEct);
	~Comp_or();
	int			taillePayload();
	float		tauxCompression();
	float		tauxCompression(int taille);

	float		precisionCodage();
	String		check();
	Serie		simul();
	Serie		ecartTypeSimul(bool codec);
	Serie		compressEct();
	Serie		compressYp();
	Serie		compress();
	Serie		decompressEct(Serie payl);
};
#endif

