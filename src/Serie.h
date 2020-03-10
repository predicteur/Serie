/*
Serie.h - Serie library v1.0.0 - 2020-01-21
Copyright (C) 2019-2020 Philippe Thomy.  All rights reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
See file LICENSE.txt for further informations on licensing terms.
*/
#ifndef SERIE_
#define SERIE_
#include <Arduino.h>

class Serie {
protected:
	int		LEN;				// lenght of the series
	String	NOM;				// name of the series
	float*	SERIE;				// set of values
public:
	// constructor - destructor
	Serie(const int len, String nom, float deb, float fin);
	Serie(const int len, String nom, float deb);
	Serie(const int len, String nom);
	Serie(const int len);
	Serie(const Serie & serie);
	Serie();
	~Serie();
	
	// operator overloaded
	Serie&	operator=(const Serie & serie);
	friend	Serie operator+(const Serie & serie1, const Serie & serie2);
	friend	Serie operator-(const Serie & serie1, const Serie & serie2);
	friend	Serie operator*(const Serie & serie1, const Serie & serie2);
	friend	Serie operator*(float coef, const Serie & serie2);
	friend	Serie operator|(const Serie & serie1, const Serie & serie2);
	friend  Serie operator|(const Serie & serie1, float valeur);
	friend  void  operator+=(Serie & serie1, const Serie & serie2);
	friend  void  operator|=(Serie & serie1, const Serie & serie2);
	friend  void  operator|=(Serie & serie1, float valeur);
	float&	operator[](int i);
	float	operator[](int i) const;
	
	// update
	void	init(float dep, float fin);
	void	refresh(float valeur);
	void	complete(float valeur);
	Serie	copie();
	void	setNom(String nom);
	void	setSerie(float* serie, int len);
	Serie	sousSerie(int indice, int len2);

	// getters
	String  lenom();
	int		len();
	float*  serie();

	// analysing and transformation functions
	String pr();
	float   ecDiff(Serie y);
	float   etDiff(Serie y);
	float	moyenne();
	float	ecartType();
	Serie   normalisation(float mini, float maxi);
	Serie	denormalisation(float mini, float maxi);
	Serie   codage(int nbreg0, int bit);
	Serie   decodage(int nbreg0, int bit);
	Serie	ecretage(float mini, float maxi);

	static Serie	lisSpline(Serie xp, Serie yp, float lamb);
	static Serie	lisMA(Serie yp, Serie coef, String name, bool causal);
	static Serie	lisSA(Serie yp, int largeur, bool causal);
	static Serie	lisSG(Serie yp, int largeur, int degre, bool causal);
	static Serie	lisGA(Serie yp, int largeur, bool causal);
	static Serie	lisWA(Serie yp, int largeur, bool causal);
	static Serie	lisES(Serie yp, float alpha, bool doub);

	static float	etDiff(Serie x, Serie y);
	static float	ecDiff(Serie x, Serie y);
	static float	conversionb(long valeurb, float mini, float maxi, int bits);
	static long		conversion(float valeur, float mini, float maxi, int bits);
	static Serie	codbin(long param, int lon);
	static long		decbin(Serie payl, int lon);

	static Serie	regPol(Serie xn, Serie yn, Serie xp);

	static Serie	intCont(float x0, float y0, float yp0, float x1, float y1, float yp1, Serie xi);
	static Serie	intLin(Serie xp, Serie yp, Serie xn);
	static Serie	intPol(Serie xp, Serie yp, Serie xn);
	static Serie	intSpline(Serie xp, Serie yp, Serie xn, float prem, float der);
};

extern Serie sn;

#endif