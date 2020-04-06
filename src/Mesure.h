/*
Mesure.h - Serie library v1.5.0 - 2020-03-31
Copyright (C) 2020 Philippe Thomy.  All rights reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
See file LICENSE.txt for further informations on licensing terms.
*/
#ifndef MESURE_
#define MESURE_
#include <Arduino.h>
#include <Serie.h>

class Mesure {
protected:
	String			NOM;				// Mesure's name
	int				NBSER;				// Number of Series
	int				NBATT;				// Number of attributes (float)
	int				NBSTR;				// Number of attributes (string)
	int				LENSER;				// Lenght of the Series
	Serie*			SERIES;				// list of Series
	String*			STRS;				// list of string values
	String*			NAMESTRS;			// list of string names
	float*			ATTS;				// list of float values
	String*			NAMEATTS;			// list of float names
public:
	// constructor - destructor
	Mesure	(const int nbatt, int nbstr, int nbser, int lenser, String nom, String* liste);
	Mesure	(const int nbatt, int nbstr, int nbser, int lenser);
	Mesure	();
	~Mesure	();

	// operator overloaded
	Serie&	operator[](String nom);
	Serie	operator[](String nom) const;

	// update
	void		init(String* liste);
	void		initNoms(String* liste);
	void		initSeries(float deb, float fin);
	void		setVal(String name, float val);
	void		setVal(String name, String val);
	void		refresh(float valeur);

	// getters
	String		getString(String name);
	float		getFloat(String name);

	// analysing and transformation functions
	String pr();
	String json(bool titre, int lenserie, String* liste, int nbliste);
	String json(int lenserie);
	String json();
};
#endif