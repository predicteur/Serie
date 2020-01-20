#ifndef SERIE_
#define SERIE_
//#pragma once
//#include <iostream>
//#include <string>
// std::string -> String
#include <Arduino.h>

class Serie {
protected:
	int		LEN;				// taille de la série
	String	NOM;				// nom de la série
	float*	SERIE;				// liste des points
public:
	Serie(const int len, String nom, float deb, float fin);
	Serie(const int len, String nom, float deb);
	Serie(const int len, String nom);
	Serie(const int len);
	Serie(const Serie & serie);
	Serie();
	~Serie();
	
	Serie&	operator=(const Serie & serie);
	friend	Serie operator+(const Serie & serie1, const Serie & serie2);
	friend	Serie operator-(const Serie & serie1, const Serie & serie2);
	friend	Serie operator*(const Serie & serie1, const Serie & serie2);
	friend	Serie operator*(float coef, const Serie & serie2);
	friend	Serie operator|(const Serie & serie1, const Serie & serie2);
	friend  void  operator+=(Serie & serie1, const Serie & serie2);
	friend  void  operator|=(Serie & serie1, const Serie & serie2);
	float&	operator[](int i);
	float	operator[](int i) const;
	
	void	setNom(String nom);
	void	setSerie(float* serie, int len);
	String  lenom();
	int		len();
	float*  serie();

	void	init(float dep, float fin);
	String pr();
	Serie	copie();
	float   ecDiff(Serie y);
	float   etDiff(Serie y);
	float	moyenne();
	float	ecartType();
	Serie	ecretage(float mini, float maxi);
	Serie	sousSerie(int indice, int len2);
	Serie   normalisation(float mini, float maxi);
	Serie	denormalisation(float mini, float maxi);
	Serie   codage(int nbreg0, int bit);
	Serie   decodage(int nbreg0, int bit);

	static float	etDiff(Serie x, Serie y);
	static float	ecDiff(Serie x, Serie y);
	static float	conversionb(int valeurb, float mini, float maxi, int bits);
	static int		conversion(float valeur, float mini, float maxi, int bits);
	static Serie	codbin(long param, int lon);
	static long		decbin(Serie payl, int lon);
	static Serie	intCont(float x0, float y0, float yp0, float x1, float y1, float yp1, Serie xi);
	static Serie	intLin(Serie xp, Serie yp, Serie xn);
	static Serie	regPol(Serie xn, Serie yn, Serie xp);
	static Serie	intPol(Serie xp, Serie yp, Serie xn);
	static Serie	intSpline(Serie xp, Serie yp, Serie xn, float prem, float der);
	static Serie	lisSpline(Serie xp, Serie yp, float lamb);
};

//extern MatrixMath Matrix;

#endif