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
Serie::~Serie() { delete SERIE; }
Serie & Serie::operator=(const Serie & z) {
	LEN = z.LEN; NOM = z.NOM;
	delete SERIE;
	SERIE = new float[LEN];
	for (int i = 0; i<LEN; i++) SERIE[i] = z.SERIE[i];
	return *this;
}
Serie operator+(const Serie & serie1, const Serie & serie2) {
	Serie res(min(serie1.LEN, serie2.LEN), serie1.NOM + " + " + serie2.NOM);
	for (int i = 0; i < res.LEN; i++) res.SERIE[i] = serie1.SERIE[i] + serie2.SERIE[i];
	return res;
}
Serie operator|(const Serie & serie1, const Serie & serie2) {
	Serie sserie(serie1.LEN + serie2.LEN, serie1.NOM + " | " + serie2.NOM);
	for (int i = 0; i < serie1.LEN; i++) sserie[i] = serie1[i];
	for (int i = 0; i < serie2.LEN; i++) sserie[i + serie1.LEN] = serie2[i];
	return sserie;
}
Serie operator-(const Serie & serie1, const Serie & serie2) {
	Serie res(min(serie1.LEN, serie2.LEN), serie1.NOM + " - " + serie2.NOM);
	for (int i = 0; i < res.LEN; i++) res.SERIE[i] = serie1.SERIE[i] - serie2.SERIE[i];
	return res;
}
Serie operator*(const Serie & serie1, const Serie & serie2) {
	Serie res(min(serie1.LEN, serie2.LEN), serie1.NOM + " * " + serie2.NOM);
	for (int i = 0; i < res.LEN; i++) res.SERIE[i] = serie1.SERIE[i] * serie2.SERIE[i];
	return res;
}
Serie operator*(float coef, const Serie & serie) {
	Serie res(serie.LEN, "c * " + serie.NOM);
	for (int i = 0; i < res.LEN; i++) res.SERIE[i] = coef * serie.SERIE[i];
	return res;
}
void operator+=(Serie & serie1, const Serie & serie2) { serie1 = serie1 + serie2; }
void operator|=(Serie & serie1, const Serie & serie2) { serie1 = serie1 | serie2; }
float& Serie::operator[](int i) { return SERIE[i]; }
float  Serie::operator[](int i) const { return SERIE[i]; }
//-----------------------------------------------------------------------------------------------------------------------------
void Serie::init(float dep, float fin) {
	if (LEN > 0) for (int i = 0; i < LEN; i++) SERIE[i] = dep + i * (fin - dep) / float(LEN - 1);
}
//-----------------------------------------------------------------------------------------------------------------------------
String Serie::pr() {
	String valeur = NOM + " :[ ";
	for (int i = 0; i < LEN; i++) valeur += String(SERIE[i]) + String(", ");
	valeur += "] ";
	return valeur;
}
//-----------------------------------------------------------------------------------------------------------------------------
void  Serie::setNom(String nom) { NOM = nom; }
String  Serie::lenom() { return NOM; }
int  Serie::len() { return LEN; }
float* Serie::serie() { return SERIE; }
//-----------------------------------------------------------------------------------------------------------------------------
void Serie::setSerie(float* serie, int len) {
	LEN = len;
	delete SERIE;
	SERIE = new float[LEN];
	for (int i = 0; i < LEN; i++) SERIE[i] = serie[i];
}
//-----------------------------------------------------------------------------------------------------------------------------
Serie  Serie::copie() {
	Serie z(LEN, NOM);
	for (int i = 0; i<LEN; i++) z.SERIE[i] = SERIE[i];
	return z;
}
//-----------------------------------------------------------------------------------------------------------------------------
float Serie::ecDiff(Serie y) {
	float ecart = 0;
	for (int i = 0; i<LEN; i++) ecart += fabs(SERIE[i] - y[i]) / float(LEN);
	return ecart;
}
//-----------------------------------------------------------------------------------------------------------------------------
float Serie::etDiff(Serie y) {
	float ecart = 0;
	for (int i = 0; i<LEN; i++) ecart += pow(SERIE[i] - y[i], 2) / float(LEN);
	return float(sqrt(ecart));
}
//-----------------------------------------------------------------------------------------------------------------------------
float Serie::moyenne() {
	if (LEN < 1) return -1.0;
	float moyenne = 0;
	for (int i = 0; i<LEN; i++) moyenne += SERIE[i] / LEN;
	return moyenne;
}
//-----------------------------------------------------------------------------------------------------------------------------
float Serie::ecartType() {
	if (LEN < 1) return -1.0;
	Serie moyen(LEN, "", moyenne());
	return etDiff(moyen);
}
//-----------------------------------------------------------------------------------------------------------------------------
Serie Serie::ecretage(float mini, float maxi) {
	Serie serie(LEN, NOM);
	for (int i = 0; i<LEN; i++) serie[i] = min(maxi, max(mini, SERIE[i]));
	return serie;
}
//-----------------------------------------------------------------------------------------------------------------------------
Serie Serie::sousSerie(int indice, int len2) {
	Serie sous(len2, "ss-" + NOM);
	for (int i = 0; i<len2; i++) sous[i] = SERIE[indice + i];
	return sous;
}
//-----------------------------------------------------------------------------------------------------------------------------
Serie Serie::normalisation(float mini, float maxi) {
	Serie serie(LEN, NOM + "(n)");
	for (int i = 0; i < LEN; i++) serie[i] = ((min(maxi, max(mini, SERIE[i]))) - mini) / (maxi - mini) - 0.5f;
	return serie;
}
//-----------------------------------------------------------------------------------------------------------------------------
Serie Serie::denormalisation(float mini, float maxi) {
	Serie serie(LEN, NOM + "(d)");
	for (int i = 0; i < LEN; i++)  serie[i] = min(maxi, max(mini, (mini + (maxi - mini)*(SERIE[i] + 0.5f))));
	return serie;
}
//-----------------------------------------------------------------------------------------------------------------------------
Serie Serie::codage(int nbreg0, int bit) {
	Serie payl;
	for (int i = 0; i < nbreg0; i++) payl |= codbin((long)conversion(SERIE[i], -0.5f, 0.5f, bit), bit);
	payl.NOM = "payl";
	return payl;
}
//-----------------------------------------------------------------------------------------------------------------------------
Serie Serie::decodage(int nbreg0, int bit) {
	Serie yp0(nbreg0, "yp0");
	for (int i = 0; i < nbreg0; i++) yp0[i] = conversionb(decbin(this->sousSerie(i * bit, bit), bit), -0.5, 0.5, bit);
	return yp0;
}
//-----------------------------------------------------------------------------------------------------------------------------
float Serie::etDiff(Serie serie1, Serie serie2) { return serie1.etDiff(serie2); }
float Serie::ecDiff(Serie serie1, Serie serie2) { return serie1.ecDiff(serie2); }
//-----------------------------------------------------------------------------------------------------------------------------
int Serie::conversion(float valeur, float mini, float maxi, int bits) {
	int val = int(floor(pow(2, bits) * (valeur - mini) / (maxi - mini)));
	return max(0, min(int(pow(2, bits) - 1), val));
}
//-----------------------------------------------------------------------------------------------------------------------------
float Serie::conversionb(int valeurb, float mini, float maxi, int bits) {
	return float(mini + (maxi - mini) * (float(valeurb) + 0.5) / pow(2, bits));
}
//-----------------------------------------------------------------------------------------------------------------------------
Serie Serie::codbin(long param, int lon) {
	//for (int i = 0; i<lon; i++) payl[rang + i] = bitRead(param, i);
	Serie payl(lon, "payl");
	for (int i = 0; i < lon; i++) {
		payl[i] = float(param % 2);
		param /= 2;
	}
	return payl;
}
//-----------------------------------------------------------------------------------------------------------------------------
long Serie::decbin(Serie payl, int lon) {
	long param = 0;
	//for (int i = 0; i<lon; i++) bitWrite(param, i, payl[rang + i]);
	for (int i = 0; i < lon; i++) param += (long)(payl[i] * pow(2, i));
	return param;
}
//-----------------------------------------------------------------------------------------------------------------------------
Serie Serie::intCont(float x0, float y0, float yp0, float x1, float y1, float yp1, Serie xi) {
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
	delete[] Y01, Xi, Yi, matP, Mat;
	return yi;
}
//-----------------------------------------------------------------------------------------------------------------------------
Serie Serie::intLin(Serie xp, Serie yp, Serie xn) {
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
Serie Serie::regPol(Serie xn, Serie yn, Serie xp) {
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
	delete[] s, matXp, matS, matP, matT, matY;
	return yp;
}
//-----------------------------------------------------------------------------------------------------------------------------
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
	delete[] Xn, Xp, matyp, matP, matyn;
	return yn;
}
//-----------------------------------------------------------------------------------------------------------------------------
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
	delete[] h, r, f, m, c, cp;
	return yn;
}
//-----------------------------------------------------------------------------------------------------------------------------
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
	delete[] h, q, qt, r, rqt, k, matyp, matype;
	return ype;
}