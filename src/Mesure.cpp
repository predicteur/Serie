/*
Mesure.cpp - Serie library v1.5.0 - 2020-03-31
Copyright (C) 2019-2020 Philippe Thomy.  All rights reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
See file LICENSE.txt for further informations on licensing terms.
*/
#include <Mesure.h>
#include <MatrixMath.h>

using namespace std;

//******************************************************************************
//* Constructors - destructor
//******************************************************************************
Mesure::Mesure(const int nbatt, int nbstr, int nbser, int lenser, String nom, String* liste) {
	NBATT = nbatt; NBSER = nbser; NBSTR = nbstr; NOM = nom; LENSER = lenser;
	SERIES = new Serie[max(NBSER, 1)];
	ATTS = new float[max(NBATT, 1)];
	NAMEATTS = new String[max(NBATT, 1)];
	STRS = new String[max(NBSTR, 1)];
	NAMESTRS = new String[max(NBSTR, 1)];
	init(liste);
}
Mesure::Mesure(const int nbatt, int nbstr, int nbser, int lenser) {
	String* liste = new String[nbatt + nbstr + nbser];
	NBATT = nbatt; NBSER = nbser; NBSTR = nbstr; NOM = "vide"; LENSER = lenser;
	SERIES = new Serie[max(NBSER, 1)];
	ATTS = new float[max(NBATT, 1)];
	NAMEATTS = new String[max(NBATT, 1)];
	STRS = new String[max(NBSTR, 1)];
	NAMESTRS = new String[max(NBSTR, 1)];
	for (int i = 0; i < nbatt; i++) liste[i] = "att" + String(i);
	for (int i = 0; i < nbstr; i++) liste[nbatt + i] = "str" + String(i);
	for (int i = 0; i < nbser; i++) liste[nbatt + nbstr + i] = "ser" + String(i);
	init(liste);
	delete[] liste;
}
Mesure::Mesure() {
	NBATT = 0; NBSER = 0; NBSTR = 0; NOM = "vide"; LENSER = 1;
	SERIES = new Serie[1];
	ATTS = new float[1];
	NAMEATTS = new String[1];
	STRS = new String[1];
	NAMESTRS = new String[1];
}
Mesure::~Mesure() { delete[] SERIES; delete[] ATTS; delete[] NAMEATTS; delete[] STRS; delete[] NAMESTRS; }

//******************************************************************************
//* Operators
//******************************************************************************
Serie&	Mesure::operator[](String nom) {
	for (int i = 0; i < NBSER; i++) if (SERIES[i].getNom() == nom) return SERIES[i];
	return SERIES[0];
}
Serie	Mesure::operator[](String nom) const {
	for (int i = 0; i < NBSER; i++) if (SERIES[i].getNom() == nom) return SERIES[i];
	return SERIES[0];
}
Serie&	Mesure::operator[](int num) { return SERIES[num]; }
Serie	Mesure::operator[](int num) const { return SERIES[num]; }

//******************************************************************************
//* Class Functions
//******************************************************************************

//-----------------------------------------------------------------------------------------------------------------------------
/* Initialize Mesure (like constructor)
*/
void	Mesure::init(String* liste) {
	for (int i = 0; i < NBATT; i++) { NAMEATTS[i] = liste[i]; ATTS[i] = 0; }
	for (int i = 0; i < NBSTR; i++) { NAMESTRS[i] = liste[NBATT + i]; STRS[i] = "chaine" + String(i); }
	for (int i = 0; i < NBSER; i++) SERIES[i] = Serie(LENSER, liste[NBATT + NBSTR + i]);
}
void	Mesure::initNoms(String* liste) {
	for (int i = 0; i < NBATT; i++) NAMEATTS[i] = liste[i];
	for (int i = 0; i < NBSTR; i++) NAMESTRS[i] = liste[NBATT + i];
	for (int i = 0; i < NBSER; i++) SERIES[i].setNom(liste[NBATT + NBSTR + i]);
}
//-----------------------------------------------------------------------------------------------------------------------------
/* getters - setters
*/
void	Mesure::setVal(String name, float val) { for (int i = 0; i < NBATT; i++) if (NAMEATTS[i] == name) ATTS[i] = val; }
void	Mesure::setVal(String name, String val) { for (int i = 0; i < NBSTR; i++) if (NAMESTRS[i] == name) STRS[i] = val; }
String  Mesure::getValS(String name) { for (int i = 0; i < NBSTR; i++) if (NAMESTRS[i] == name) return STRS[i]; return ""; }
float   Mesure::getValF(String name) { for (int i = 0; i < NBATT; i++) if (NAMEATTS[i] == name) return ATTS[i]; return 0; }
void	Mesure::initSeries(float deb, float fin) { for (int i = 0; i < NBSER; i++) SERIES[i].init(deb, fin); }
//-----------------------------------------------------------------------------------------------------------------------------
/* Add a new value in all the Serie and delete the oldest
*/
void	Mesure::refresh(float valeur) { for (int i = 0; i < NBSER; i++) SERIES[i].refresh(valeur); }
//-----------------------------------------------------------------------------------------------------------------------------
/* Print Mesure : generate a String with name and values
*/
String  Mesure::pr() {
	String stri = "MESURE : " + NOM + "\n";
	for (int i = 0; i < NBATT; i++)	stri += NAMEATTS[i] + " : " + String(ATTS[i]) + "\n";
	for (int i = 0; i < NBSTR; i++) stri += NAMESTRS[i] + " : " + STRS[i] + "\n";
	for (int i = 0; i < NBSER; i++) stri += SERIES[i].pr() + "\n";
	return stri;
}
//-----------------------------------------------------------------------------------------------------------------------------
/* Json Mesure : generate a String with Json structure
*/
String jsonatt(char carac, String att1, String att2) {
	String json = " ";
	if (carac != '{' and carac != '[') json += ", ";
	return json + att1 + " : " + att2;
}
String Mesure::json(bool titre, int lenserie, String* liste, int nbliste) {
	String json = "{ ";
	String jsonSerie = "[ ";
	if (titre) json += jsonatt('{', "MESURE", '"' + NOM + '"');
	bool trouve = false;
	for (int i = 0; i < NBATT; i++) {
		for (int j = 0; j < nbliste; j++) trouve |= NAMEATTS[i] == liste[j];
		if (!trouve) json += jsonatt(json.charAt(json.length() - 2), '"' + NAMEATTS[i] + '"', String(ATTS[i]));
		trouve = false;
	}
	for (int i = 0; i < NBSTR; i++) {
		for (int j = 0; j < nbliste; j++) trouve |= NAMESTRS[i] == liste[j];
		if (!trouve) json += jsonatt(json.charAt(json.length() - 2), '"' + NAMESTRS[i] + '"', '"' + STRS[i] + '"');
		trouve = false;
	}
	if (NBSER > 0) {
		if (lenserie == 1) 	for (int i = 0; i < NBSER; i++) {
			for (int j = 0; j < nbliste; j++) trouve |= SERIES[i].getNom() == liste[j];
			if (!trouve) json += jsonatt(json.charAt(json.length() - 2), '"' + SERIES[i].getNom() + '"', String(SERIES[i][0]));
			trouve = false;
		}
		else {
			for (int i = 0; i < NBSER; i++) {
				for (int j = 0; j < nbliste; j++) trouve |= SERIES[i].getNom() == liste[j];
				if (!trouve) {
					if (jsonSerie.charAt(jsonSerie.length() - 2) != '[') jsonSerie += " , ";
					jsonSerie += SERIES[i].json(lenserie);
				}
				trouve = false;
			}
			jsonSerie += " ]";
			if (jsonSerie != "[  ]") json += jsonatt(json.charAt(json.length() - 2), "SERIE", jsonSerie);
		}
	}
	json += " }";
	return json;
}
String Mesure::json(int lenserie) { return json(false, lenserie, {}, 0); }
String Mesure::json() { return json(false, 1, {}, 0); }
