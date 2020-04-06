/*
Serie.cpp - Serie library v1.0.0 - 2020-01-21
Copyright (C) 2019-2020 Philippe Thomy.  All rights reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
See file LICENSE.txt for further informations on licensing terms.
*/
#include "Serie.h"
#include <MatrixMath.h>

using namespace std;

Serie sn;	// pre-instantiate

//******************************************************************************
//* Constructors - destructor
//******************************************************************************
Serie::Serie() {
	LEN = 0;
	NOM = "vide";
	SERIE = new float[1];
	SERIE[0] = 0.0;
}
Serie::Serie(const int len) {
	LEN = len;	NOM = "vide";
	SERIE = new float[LEN];
	for (int i = 0; i < LEN; i++) SERIE[i] = 0.0;
}
Serie::Serie(const int len, String nom) {
	LEN = len;	NOM = nom;
	SERIE = new float[LEN];
	for (int i = 0; i < LEN; i++) SERIE[i] = 0.0;
}
Serie::Serie(const int len, String nom, float deb) {
	LEN = len;	NOM = nom;
	SERIE = new float[LEN];
	for (int i = 0; i < LEN; i++) SERIE[i] = deb;
}
Serie::Serie(const int len, String nom, float deb, float fin) {
	LEN = len;	NOM = nom;
	SERIE = new float[LEN];
	if (LEN > 0) for (int i = 0; i < LEN; i++) SERIE[i] = deb + i * (fin - deb) / float(LEN - 1);
}
Serie::Serie(const Serie & serie) {
	LEN = serie.LEN; NOM = serie.NOM;
	SERIE = new float[LEN];
	for (int i = 0; i<LEN; i++) SERIE[i] = serie.SERIE[i];
}
Serie::~Serie() { delete [] SERIE; }
Serie & Serie::operator=(const Serie & z) {
	LEN = z.LEN; NOM = z.NOM;
	delete [] SERIE;
	SERIE = new float[LEN];
	for (int i = 0; i<LEN; i++) SERIE[i] = z.SERIE[i];
	return *this;
}
//******************************************************************************
//* Operators
//******************************************************************************
Serie	operator+(const Serie & serie1, const Serie & serie2) {
	Serie res(min(serie1.LEN, serie2.LEN), serie1.NOM + " + " + serie2.NOM);
	for (int i = 0; i < res.LEN; i++) res.SERIE[i] = serie1.SERIE[i] + serie2.SERIE[i];
	return res;
}
Serie	operator|(const Serie & serie1, const Serie & serie2) {
	Serie sserie(serie1.LEN + serie2.LEN, serie1.NOM + " | " + serie2.NOM);
	for (int i = 0; i < serie1.LEN; i++) sserie[i] = serie1[i];
	for (int i = 0; i < serie2.LEN; i++) sserie[i + serie1.LEN] = serie2[i];
	return sserie;
}
Serie operator|(const Serie & serie1, float valeur) {
	Serie sserie(serie1.LEN + 1, serie1.NOM);
	for (int i = 0; i < serie1.LEN; i++) sserie[i] = serie1[i];
	sserie[serie1.LEN] = valeur;
	return sserie;
}
Serie	operator-(const Serie & serie1, const Serie & serie2) {
	Serie res(min(serie1.LEN, serie2.LEN), serie1.NOM + " - " + serie2.NOM);
	for (int i = 0; i < res.LEN; i++) res.SERIE[i] = serie1.SERIE[i] - serie2.SERIE[i];
	return res;
}
Serie	operator*(const Serie & serie1, const Serie & serie2) {
	Serie res(min(serie1.LEN, serie2.LEN), serie1.NOM + " * " + serie2.NOM);
	for (int i = 0; i < res.LEN; i++) res.SERIE[i] = serie1.SERIE[i] * serie2.SERIE[i];
	return res;
}
Serie	operator*(float coef, const Serie & serie) {
	Serie res(serie.LEN, "c * " + serie.NOM);
	for (int i = 0; i < res.LEN; i++) res.SERIE[i] = coef * serie.SERIE[i];
	return res;
}
void	operator+=(Serie & serie1, const Serie & serie2) { serie1 = serie1 + serie2; }
void	operator|=(Serie & serie1, const Serie & serie2) { serie1 = serie1 | serie2; }
void 	operator|=(Serie & serie1, float valeur) { serie1 = serie1 | valeur; }
float&	Serie::operator[](int i) { float *nul = 0;  if (i < LEN) return SERIE[i]; else return *nul; }
float	Serie::operator[](int i) const { float nul = 0;  if (i < LEN) return SERIE[i]; else return nul; }

//******************************************************************************
//* Class Functions
//******************************************************************************

//-----------------------------------------------------------------------------------------------------------------------------
/* Initialize Serie (like constructor)
*/
void	Serie::init(float dep, float fin) {
	if (LEN > 0) for (int i = 0; i < LEN; i++) SERIE[i] = dep + i * (fin - dep) / float(LEN - 1);
}
void Serie::init(float dep) { init(dep, dep); }
//-----------------------------------------------------------------------------------------------------------------------------
/* Print Serie : generate a string with name and values
*/
String	Serie::pr() {
	String valeur = NOM + " :[ ";
	for (int i = 0; i < LEN; i++) valeur += String(SERIE[i]) + String(", ");
	valeur += "] ";
	return valeur;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Json Serie : generate a string with Json structure
*/
String	Serie::json(int lenserie) {
	String json = "{ \"serie\" : \"" + NOM + "\" , \"value\" : ";
	if (lenserie == 1) json += String(SERIE[0]);
	else {
		json += " [ ";
		for (int i = 0; i < min(LEN, lenserie); i++) {
			if (i > 0) json += " , ";
			json += String(SERIE[i]);
		}
		json += " ] ";
	}
	json += " }";
	return json;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* getters - setters
*/
void	Serie::setNom(String nom) { NOM = nom; }
String  Serie::getNom() { return NOM; }
int		Serie::len() { return LEN; }
float*	Serie::serie() { return SERIE; }
void	Serie::setSerie(float* serie, int len) {
	LEN = len;
	delete [] SERIE;
	SERIE = new float[LEN];
	for (int i = 0; i < LEN; i++) SERIE[i] = serie[i];
}
//-----------------------------------------------------------------------------------------------------------------------------
/* duplication of a Serie (like constructor)
*/
Serie	Serie::copie() {
	Serie z(LEN, NOM);
	for (int i = 0; i<LEN; i++) z.SERIE[i] = SERIE[i];
	return z;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Add a new value in the list and delete the oldest
*/
void	Serie::refresh(float valeur) {
	for (int i = LEN - 2; i > -1; i--) SERIE[i + 1] = SERIE[i];
	SERIE[0] = valeur;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Add a new value in the list
*/
void	Serie::complete(float valeur) {
	float* seri = new float[LEN + 1];
	for (int i = 0; i < LEN; i++) seri[i + 1] = SERIE[i];
	seri[0] = valeur;
	setSerie(seri, LEN + 1);
	delete [] seri;
}

//-----------------------------------------------------------------------------------------------------------------------------
/* average value of the differences (absolute value) between the two series
*/
float	Serie::ecDiff(Serie y) {
	float ecart = 0;
	for (int i = 0; i<LEN; i++) ecart += fabs(SERIE[i] - y[i]) / float(LEN);
	return ecart;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* square root of the mean of the squares of the differences between the two series (standard deviation)
*/
float	Serie::etDiff(Serie y) {
	float ecart = 0;
	for (int i = 0; i<LEN; i++) ecart += pow(SERIE[i] - y[i], 2) / float(LEN);
	return float(sqrt(ecart));
}
//-----------------------------------------------------------------------------------------------------------------------------
/* average value of the series
*/
float	Serie::moyenne() {
	if (LEN < 1) return -1.0;
	float moyenne = 0;
	for (int i = 0; i<LEN; i++) moyenne += SERIE[i] / LEN;
	return moyenne;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* standard deviation of the series
*/
float	Serie::ecartType() {
	if (LEN < 1) return -1.0;
	Serie moyen(LEN, "", moyenne());
	return etDiff(moyen);
}
//-----------------------------------------------------------------------------------------------------------------------------
/* New series with application of a minimum, maximum threshold for each value
*/
Serie	Serie::ecretage(float mini, float maxi) {
	Serie serie(LEN, NOM);
	for (int i = 0; i<LEN; i++) serie[i] = min(maxi, max(mini, SERIE[i]));
	return serie;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of a series consisting of an extract from a series
*/
Serie	Serie::sousSerie(int indice, int len2) {
	Serie sous(len2, "ss-" + NOM);
	for (int i = 0; i<len2; i++) sous[i] = SERIE[indice + i];
	return sous;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of a standardized series (values between -0.5 and +0.5) : -0.5 -> mini, +0.5 -> maxi
*/
Serie	Serie::normalisation(float mini, float maxi) {
	Serie serie(LEN, NOM + "(n)");
	for (int i = 0; i < LEN; i++) serie[i] = ((min(maxi, max(mini, SERIE[i]))) - mini) / (maxi - mini) - 0.5f;
	return serie;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of a denormalized series (inverse function of normalisation)
*/
Serie	Serie::denormalisation(float mini, float maxi) {
	Serie serie(LEN, NOM + "(d)");
	for (int i = 0; i < LEN; i++)  serie[i] = min(maxi, max(mini, (mini + (maxi - mini)*(SERIE[i] + 0.5f))));
	return serie;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Transformation of the series into a series of bits by coding each value of the series 
@ arg : nbreg0 : number of values, bit : number of bit for each value
*/
Serie	Serie::codage(int nbreg0, int bit) {
	Serie payl;
	for (int i = 0; i < nbreg0; i++) payl |= codbin(conversion(SERIE[i], -0.5f, 0.5f, bit), bit);
	payl.NOM = "payl";
	return payl;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Transformation of a series of bits into a series of values by decoding each value of the series
@arg nbreg0 : number of values
@arg bit : number of bit for each value
*/
Serie	Serie::decodage(int nbreg0, int bit) {
	Serie yp0(nbreg0, "yp0");
	for (int i = 0; i < nbreg0; i++) yp0[i] = conversionb(decbin(this->sousSerie(i * bit, bit), bit), -0.5, 0.5, bit);
	return yp0;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* eg static functions
@arg-new dernier :  to change only the 0 value (true)
*/
void  Serie::lissSA(Serie yp, int larg, bool causal, bool dernier) {
	if (dernier) SERIE[0] = lisSA(yp, larg, causal)[0];
	else setSerie(lisSA(yp, larg, causal).serie(), yp.len());
}
void  Serie::lissSG(Serie yp, int larg, int degre, bool causal, bool dernier) {
	if (dernier) SERIE[0] = lisSG(yp, larg, degre, causal)[0];
	else setSerie(lisSG(yp, larg, degre, causal).serie(), yp.len());
}
void  Serie::lissGA(Serie yp, int larg, bool causal, bool dernier) {
	if (dernier) SERIE[0] = lisGA(yp, larg, causal)[0];
	else setSerie(lisGA(yp, larg, causal).serie(), yp.len());
}
void  Serie::lissWA(Serie yp, int larg, bool causal, bool dernier) {
	if (dernier) SERIE[0] = lisWA(yp, larg, causal)[0];
	else setSerie(lisWA(yp, larg, causal).serie(), yp.len());
}
void  Serie::lissES(Serie yp, float alpha, bool doub, bool dernier) {
	if (dernier) SERIE[0] = lisES(yp, alpha, doub)[0];
	else setSerie(lisES(yp, alpha, doub).serie(), yp.len());
}

//******************************************************************************
//* Static Functions
//******************************************************************************

//-----------------------------------------------------------------------------------------------------------------------------
/* eg class functions 
*/
float	Serie::etDiff(Serie serie1, Serie serie2) { return serie1.etDiff(serie2); }
float	Serie::ecDiff(Serie serie1, Serie serie2) { return serie1.ecDiff(serie2); }
//-----------------------------------------------------------------------------------------------------------------------------
/* conversion of a value(valeur) between mini and maxi in an integer between 0 and 2**bits 
*/
uint32_t	Serie::conversion(float valeur, float mini, float maxi, int bits) {
	int val = int(floor(pow(2, bits) * (valeur - mini) / (maxi - mini)));
	return max(0, min(int(pow(2, bits) - 1), val));
}
//-----------------------------------------------------------------------------------------------------------------------------
/* conversion of an integer(valeurb) between 0 and 2**bits to a value between mini and maxi
*/
float	Serie::conversionb(uint32_t valeurb, float mini, float maxi, int bits) {
	return float(mini + (maxi - mini) * (float(valeurb) + 0.5) / pow(2, bits));
}
//-----------------------------------------------------------------------------------------------------------------------------
/* conversion of an integer (param) between 0 and 2**lon in a Serie of values 0 or 1
*/
Serie	Serie::codbin(uint32_t param, int lon) {
	//for (int i = 0; i<lon; i++) payl[rang + i] = bitRead(param, i);
	Serie payl(lon, "payl");
	for (int i = 0; i < lon; i++) {
		payl[i] = float(param % 2);
		param /= 2;
	}
	return payl;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* conversion of a Serie of values 0 or 1 in an integer between 0 and 2**lon
*/
uint32_t Serie::decbin(Serie payl, int lon) {
	uint32_t param = 0;
	//for (int i = 0; i<lon; i++) bitWrite(param, i, payl[rang + i]);
	for (int i = 0; i < lon; i++) param += (uint32_t)round(payl[i] * pow(2, i));
	return param;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of missing terms of a series by cubic polynomial interpolation of the series
@arg x0, y0, yp0 : first point of the interpolation (value and derivative (slope of the tangent))
@arg x1, y1, yp1 : first point of the interpolation (value and derivative (slope of the tangent))
@arg xi : x-coordinate of the point to be define
@result : y-coordinate of the point to be define
*/
Serie	Serie::intCont(float x0, float y0, float yp0, float x1, float y1, float yp1, Serie xi) {
	int ni = xi.len();
	double* Y01 = new double[4], *matP = new double[4], *Xi = new double[ni * 4];
	double* Yi = new double[ni * 4], *Mat = new double[4 * 4];
	Serie  yi(ni, "yi");
	Y01[0] = y0;
	Y01[1] = y1;
	Y01[2] = yp0;
	Y01[3] = yp1;
	for (int i = 0; i < ni; i++) for (int j = 0; j < 4; j++) Xi[i * 4 + j] = pow(xi[i], j);
	for (int j = 0; j < 4; j++) {
		Mat[0 * 4 + j] = pow(x0, j);
		Mat[1 * 4 + j] = pow(x1, j);
	}
	Mat[2 * 4 + 0] = double(0);
	Mat[3 * 4 + 0] = double(0);
	for (int j = 0; j < 3; j++) {
		Mat[2 * 4 + j + 1] = (j + 1) * pow(x0, j);
		Mat[3 * 4 + j + 1] = (j + 1) * pow(x1, j);
	}
	int resOk = Matrix.Invert(Mat, 4); if (resOk == 0) return yi;
	Matrix.Multiply(Mat, Y01, 4, 4, 1, matP);
	Matrix.Multiply(Xi, matP, ni, 4, 1, Yi);
	for (int i = 0; i < ni; i++) yi[i] = (float)Yi[i];
	delete[] Y01; delete[] Xi; delete[] Yi; delete[] matP; delete[] Mat;
	return yi;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of an extended series by linear interpolation of the series
@arg xp, yp : coordinate of the points
@arg xn : x-coordinate of the point to be define
@result : y-coordinate of the point to be define
*/
Serie	Serie::intLin(Serie xp, Serie yp, Serie xn) {
	int n = xn.len(), p = xp.len();
	Serie yn(n, "yn");
	if (p == 1) for (int i = 0; i < n; i++) yn[i] = yp[0];
	else for (int i = 0; i < n; i++) {
		int deb = 0;
		int j = 0;
		while (j < p) {
			if (xn[i] < xp[j]) {
				deb = max(j - 1, 0);
				j = p + 1;
			}
			else j += 1;
		}
		if (j == p) deb = p - 2;
		yn[i] = yp[deb] + (yp[deb + 1] - yp[deb]) / (xp[deb + 1] - xp[deb]) * (xn[i] - xp[deb]);
	}
	return yn;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of a reduced series by polynomial regression of the series
@arg xn, yn : coordinate of the points
@arg xp : x-coordinate of the point to be define
@result : y-coordinate of the point to be define
*/
Serie	Serie::regPol(Serie xn, Serie yn, Serie xp) {
	int n = xn.len(), p = xp.len();
	double* s = new double[2 * p], *matXp = new double[p*p], *matS = new double[p*p];
	double* matP = new double[p*p], *matT = new double[p], *matY = new double[p];
	Serie  yp(p, "yp");
	float   normalisation = max(xn[n - 1], 0.00001f);
	for (int j = 0; j < 2 * p; j++) {
		s[j] = 0.0;
		for (int i = 0; i < n; i++) s[j] += pow(xn[i] / normalisation, j);
	}
	for (int j = 0; j < p; j++) {
		matT[j] = 0.0;
		for (int i = 0; i < n; i++) matT[j] += yn[i] * pow(xn[i] / normalisation, j);
	}
	for (int i = 0; i < p; i++) for (int j = 0; j < p; j++) matS[i * p + j] = s[i + j];
	for (int i = 0; i < p; i++) for (int j = 0; j < p; j++) matXp[i * p + j] = pow(xp[i] / normalisation, j);
	int resOk = Matrix.Invert(matS, p);
	if (resOk != 0) {
		Matrix.Multiply(matS, matT, p, p, 1, matP);
		Matrix.Multiply(matXp, matP, p, p, 1, matY);
		for (int i = 0; i < p; i++) yp[i] = (float)matY[i];
	}
	delete [] s; delete[] matXp; delete[] matS; delete[] matP; delete[] matT; delete[] matY;
	return yp;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of an extended series by polynomial interpolation of the series
@arg xp, yp : coordinate of the points
@arg xn : x-coordinate of the point to be define
@result : y-coordinate of the point to be define
*/
Serie	Serie::intPol(Serie xp, Serie yp, Serie xn) {
	int n = xn.len(), p = xp.len();
	double* Xn = new double[n*p], *Xp = new double[p*p], *matP = new double[p];
	double* matyp = new double[p], *matyn = new double[n];
	Serie  yn(n, "yn");
	for (int i = 0; i < p; i++) matyp[i] = (double)yp[i];
	float normalisation = max(xp[p - 1], 0.00001f);
	if (p > 0) {
		for (int i = 0; i < p; i++) for (int j = 0; j < p; j++) {
			if (p == 1) Xp[i * p + j] = float(1);
			else		Xp[i * p + j] = pow(xp[i] / normalisation, j);
		}
		if (p == 1) matP[0] = yp[0] / Xp[0];
		else {
			int resOk = Matrix.Invert(Xp, p); if (resOk == 0) return yn;
			Matrix.Multiply(Xp, matyp, p, p, 1, matP);
		}
		for (int i = 0; i < n; i++) for (int j = 0; j < p; j++)	Xn[i * p + j] = pow(xn[i] / normalisation, j);
		Matrix.Multiply(Xn, matP, n, p, 1, matyn);
		for (int i = 0; i < n; i++) yn[i] = (float)matyn[i];
	}
	delete [] Xn; delete[] Xp; delete[] matyp; delete[] matP; delete[] matyn;
	return yn;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of an extended series by cubic spline of interpolation of the series
@arg xp, yp : coordinate of the points
@arg xn : x-coordinate of the point to be define
@arg prem, der : second derivative at the beginning and the end of the spline (default : 0) -> see french wikipedia Spline
@result : y-coordinate of the point to be define
*/
Serie	Serie::intSpline(Serie xp, Serie yp, Serie xn, float prem, float der) {
	int n = xn.len(), p = xp.len();
	double* h = new double[p - 1], *f = new double[p], *r = new double[p*p], *m = new double[p*p];
	double* c = new double[p - 1], *cp = new double[p - 1];
	Serie yn(n, "yn");
	for (int i = 0; i < p - 1; i++) h[i] = double(xp[i + 1] - xp[i]);
	for (int i = 1; i < p - 1; i++)	f[i] = double(yp[i + 1] - yp[i]) / h[i] - double(yp[i] - yp[i - 1]) / h[i - 1];
	f[0] = double(prem);
	f[p - 1] = double(der);
	for (int i = 0; i < p*p; i++)  r[i] = 0;
	r[0] = 1.0;
	r[p * p - 1] = 1.0;
	for (int i = 1; i < p - 1; i++) {
		r[i * p + i] = (h[i - 1] + h[i]) / 3.0;
		r[i * p + i + 1] = h[i] / 6.0;
		r[i * p + i - 1] = h[i - 1] / 6.0;
	}
	int resOk = Matrix.Invert(r, p);
	if (resOk == 0) return yn;
	Matrix.Multiply(r, f, p, p, 1, m);
	for (int i = 0; i < p - 1; i++) {
		c[i] = (yp[i + 1] - yp[i]) / h[i] - h[i] / 6.0 * (m[i + 1] - m[i]);
		cp[i] = yp[i] - m[i] / 6.0 * pow(h[i], 2);
	}
	for (int i = 0; i < n; i++) {
		int k = 0;
		while (k < p - 1) {
			if (xn[i] > xp[k + 1]) k += 1;
			else {
				yn[i] = float(m[k] / 6.0 / h[k] * pow(xp[k + 1] - xn[i], 3) + m[k + 1] / 6.0 / h[k] * pow(-xp[k] + xn[i], 3) + c[k] * (-xp[k] + xn[i]) + cp[k]);
				k = p;
			}
		}
	}
	delete [] h; delete[] r; delete[] f; delete[] m; delete[] c; delete[] cp;
	return yn;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of a smoothed series by moving-average filter
@arg yp : coordinate of the points
@arg coef : coefficients of the filter
@arg name : filter's name
@arg causal : True if the coefficients are not applied to the future values, False if they are applied either to past and future values 
@result : y-coordinate of the new filtered points
*/
Serie	Serie::lisMA(Serie yp, Serie coef, String name, bool causal) {
	Serie ypf(yp.len(), name);
	int   lon = coef.len();
	if (causal) lon = (coef.len() - 1) / 2 + 1;
	for (int i = 0; i < yp.len(); i++) {
		float totcoef = 0;
		for (int j = 0; j < lon; j++) {
			ypf[i] += yp[max(0, min(yp.len() - 1, i - j + (coef.len() - 1) / 2))] * coef[j];
			totcoef += coef[j];
		}
		ypf[i] /= totcoef;
	}
	return ypf;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of a smoothed series by simple moving - average filter
@arg yp : coordinate of the points
@arg largeur : number of coefficients
@arg causal : True if the coefficients are not applied to the future values, False if they are applied either to past and future values
@result : y-coordinate of the new filtered points
*/
Serie	Serie::lisSA(Serie yp, int largeur, bool causal) {
	int demi = (largeur - 1) % 2;
	Serie coef(largeur + demi, "cSA", 1);
	if (demi) {
		coef[0] = 0.5;
		coef[coef.len() - 1] = 0.5;
	}
	return lisMA(yp, coef, "ySA", causal);
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of a smoothed series by moving-average Savitzky-Golay filter -> see wikipedia : Algorithme_de_Savitzky-Golay
@arg yp : coordinate of the points
@arg largeur : number of coefficients (5, 7, 9)
@arg degre : level of the polynome (2 or 4)
@arg causal : True if the coefficients are not applied to the future values, False if they are applied either to past and future values
@result : y-coordinate of the new filtered points
*/
Serie	Serie::lisSG(Serie yp, int largeur, int degre, bool causal) {
	float coefSG[2][3][5] = { { { 17, 12, -3, 0, 0 },{ 7, 6, 3, -2, 0 },{ 59, 54, 39, 14, -21 } },
	{ { 0, 0, 0, 0, 0 },{ 131, 75, -30, 5, 0 },{ 179, 135, 30, -55, 15 } } };
	int deg = degre / 2 - 1;
	int	lar = (largeur - 1) / 2 - 2;
	Serie coef(largeur, "cSG");
	for (int i = 0; i < largeur; i++) coef[i] = coefSG[deg][lar][abs((largeur - 1) / 2 - i)];
	return lisMA(yp, coef, "ySG", causal);
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of a smoothed series by moving-average gaussian filter
@arg yp : coordinate of the points
@arg largeur : number of coefficients (odd)
@arg causal : True if the coefficients are not applied to the future values, False if they are applied either to past and future values
@result : y-coordinate of the new filtered points
*/
Serie	Serie::lisGA(Serie yp, int largeur, bool causal) {
	float coefGA[4][9] = { { 0, 0, 0, 1, 2, 1, 0, 0, 0 },{ 0, 0, 1, 4, 6, 4, 1, 0, 0 },
	{ 0, 1, 6, 15, 20, 15, 6, 1, 0 },{ 1, 8, 28, 56, 70, 56, 28, 8, 1 } };
	int lar = (largeur - 1) / 2 - 1;
	Serie coef(9, "cSG");
	for (int i = 0; i < 9; i++) coef[i] = coefGA[lar][i];
	return lisMA(yp, coef, "yGA", causal);
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of a smoothed series by weighted-moving-average filter
@arg yp : coordinate of the points
@arg largeur : number of coefficients (odd)
@arg causal : True if the coefficients are not applied to the future values, False if they are applied either to past and future values
@result : y-coordinate of the new filtered points
*/
Serie	Serie::lisWA(Serie yp, int largeur, bool causal) {
	int demi = (largeur - 1) / 2;
	Serie coef(largeur, "cWA");
	for (int i = 0; i < demi + 1; i++) {
		coef[i + demi] = float(demi + 1 - i);
		coef[-i + demi] = float(demi + 1 - i);
	}
	return lisMA(yp, coef, "yWA", causal);
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of a smoothed series by exponential smoothing filter
@arg yp : coordinate of the points
@arg alpha : degree of weighting decrease, a constant smoothing factor between 0 and 1
@arg doub : True for Double exponential smoothing, False for Basic exponential smoothing
@result : y-coordinate of the new filtered points
*/
Serie	Serie::lisES(Serie yp, float alpha, bool doub) {
	Serie ypf1(yp.len(), "ypf1", yp[0]), ypf2(yp.len(), "ypf2", yp[0]), ypf;
	for (int i = yp.len() - 2; i >= 0; i--) {
		ypf1[i] = alpha * yp[i] + (1.0f - alpha)*ypf1[i + 1];
		ypf2[i] = alpha * ypf1[i] + (1.0f - alpha)*ypf2[i + 1];
	}
	if (doub) ypf = 2.0 * ypf1 - ypf2;
	else ypf = ypf1;
	ypf.setNom("yES");
	return ypf;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Generation of a smoothed series by cubic spline of the series
@arg xp, yp : coordinate of the points
@arg lamb : smoothing coefficient (0 : return the same points, infinite : return a regression line)
@result : y-coordinate of the smoothing points
*/
Serie	Serie::lisSpline(Serie xp, Serie yp, float lamb) {
	String valeur;
	int p = xp.len();
	double* h = new double[p - 1], *q = new double[p * (p - 2)], *qt = new double[(p - 2) * p];
	double* r = new double[(p - 2) * (p - 2)], *rqt = new double[(p - 2) * p], *k = new double[p*p];
	double* matype = new double[p], *matyp = new double[p];	for (int i = 0; i < p; i++) matyp[i] = (double)yp[i];
	Serie ype(p, "ype");
	for (int i = 0; i < p - 1; i++)	h[i] = xp[i + 1] - xp[i];
	for (int i = 0; i < p * (p - 2); i++)  q[i] = 0;
	for (int i = 1; i < p - 1; i++)	q[i * (p - 2) + i - 1] = 1.0 / h[i - 1] + 1.0 / h[i];
	for (int i = 0; i < p - 2; i++)	q[i * (p - 2) + i] = -1.0 / h[i];
	for (int i = 2; i < p; i++)		q[i * (p - 2) + i - 2] = -1.0 / h[i - 1];
	for (int i = 0; i < (p - 2) * (p - 2); i++)  r[i] = 0;
	for (int i = 0; i < p - 2; i++) r[i * (p - 2) + i] = (h[i] + h[i + 1]) / 3.0;
	for (int i = 0; i < p - 3; i++)	r[i * (p - 2) + i + 1] = -h[i] / 6.0;
	for (int i = 1; i < p - 2; i++)	r[i * (p - 2) + i - 1] = -h[i - 1] / 6.0;
	int resOk = Matrix.Invert(r, p - 2);
	if (resOk == 0) return ype;
	Matrix.Transpose(q, p, p - 2, qt);
	Matrix.Multiply(r, qt, p - 2, p - 2, p, rqt);
	Matrix.Multiply(q, rqt, p, p - 2, p, k);
	for (int i = 0; i < p*p; i++) k[i] *= lamb;
	for (int i = 0; i < p; i++) k[i * p + i] += 1.0;
	resOk = Matrix.Invert(k, p);
	if (resOk == 0) return ype;
	Matrix.Multiply(k, matyp, p, p, 1, matype);
	for (int i = 0; i < p; i++) ype[i] = (float)matype[i];
	delete [] h; delete[] q; delete[] qt; delete[] r; delete[] rqt; delete[] k; delete[] matyp; delete[] matype;
	return ype;
}