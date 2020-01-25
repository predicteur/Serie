# Presentation
This repository groups together a set of time series functions. They are written in C ++ to be used on data acquisition equipment (eg ESP / Arduino).

Two types of objects are used:
* Serie : object representing time series,
* Compactor / Compressor : object representing the compression of a time series,

# Object Serie
Time series include a series of numerical values.

## Principle of representation
The Serie object is composed of the following arguments :
* **Name :** Character string
* **Length :** Number of values in the series (integer)
* **List of values :** The successive numerical values of the series (dynamic array of real)

## Creation and update functions
* **constructor :** Several creation modes are possible
    * Empty series
    * Series of a given length with values at 0
    * Series of a given length with the values initialized to a given value
    * Series of a given length with the values initialized by distribution between a starting point and an ending point
    * Series created by copying from another series
* **operators :** The operators +, -, += and * are overloaded to combine the values of series of the same length. An additional operation (operator | overloaded) makes it possible to group two series. The assignment (operator = overloaded) is used to copy one series to another. The values of the series are directly accessible (operator []).

* **update :** The functions are as follows:
    * **sousSerie()**: Generation of a series consisting of an extract from a series
    * **init()**: same as constructor
    * **copie()**: same as copy constructor
    * **setName()**: name setter
    * **setSerie()**: setter of the list of values
* **getters :** The functions are as follows:
    * **len()**: number of values in the series
    * **lenom()**: name of the series
    * **serie()**: list of values in the series    

## Functions for analyzing a series
### Statistical values of a series
* **moyenne()**: average value of the series
* **ecartType()**: standard deviation of the series
### Characterization of two series
* **ecDiff()**: average value of the differences (absolute value) between the two series
* **etDiff()**: square root of the mean of the squares of the differences between the two series
### Other
* **pr()**: display of the series
* **autoCorrelation()**: coming soon

## Transformation functions of a series
### Processing functions
* **ecretage()**: New series with application of a minimum, maximum threshold for each value
* **egPol()**: Generation of a reduced series by polynomial regression of the series
* **intPol()**: Generation of an extended series by polynomial interpolation of the series
* **intLin()**: Generation of an extended series by linear interpolation of the series
* **intCont()**: Generation of missing terms of a series by cubic interpolation of the series
* **intSpline()**: Generation of an extended series by cubic spline of interpolation of the series
* **lisSpline()**: Generation of a smoothed series by cubic spline of the series
### Conversion functions
* **codage()**: Transformation of the series into a series of bits by coding each value of the series
* **decodage()**: Transformation of a series of bits into a series of values by decoding each value of the series
* **normalisation()**: Generation of a standardized series (values between -0.5 and +0.5)
* **denormalisation()**: Generation of a denormalized series (inverse function of normalisation)

## Use
See the examples given on the use of the Serie object.

# Compactor / Compressor objects

These two objects represent the compression of a time series (eg to minimize the volumes of data sent over an LPWAN network).

## Compression goals
1 - Replacement of a set of points coded on 16 bits (eg integer) or 32 bits (eg real) into a reduced set of points and coded on a reduced number of bits.

Example: 16 real points (64 bytes) compressed into 12 bytes

2 - Use of easily implementable algorithms on microcontrollers.

Example: use of Sigfox (limitation to 140 messages per day and 12 bytes per message).
-> with a measurement every 15 seconds, you can send the 32 values measured every 8 minutes on 12 bytes

## Compression principles
Compression is carried out by sending parameters allowing a sequence of several values to be reconstructed (sequence of 32 values in the example above).

The parameters come from several estimations by polynomial regression and are then coded on a defined number of bits ([see curve compression](https://fr.wikipedia.org/wiki/Compression_de_courbe)).

The compression error (standard deviation between the initial values and the reconstructed values) is also integrated into the compression.

## Implementation
Two types of algorithms are implemented (class: Compactor and class: Compressor). They are based on the Serie object and have the following functions:
* independent functions of a dataset:
    * **check()**: check the input parameters
    * **taillePayload()**: number of bits of compressed data
    * **precisionCodage()**: precision of the coding used
* functions linked to the dataset:
    * **calcul()**: generation of the main outputs
    * **simul()**: simulated values after compression / decompression (usable after calcul)
    * **ecartTypeSimul()**: standard deviation of simulated values / original values (usable after calcul)
    * **compress()**: compressed values (usable after calcul)
* functions related to decompressing a dataset:
    * **decompressY0()**: values reconstituted by decompression
    * **decompressYp()**: parameter values from the regression (Compactor only)
    * **decompressEcartType()**: standard deviation of simulated values / original values

## Principle of simple compression (class: Compactor)
### Step 1: Standardization
Scaling of \[- 0.5 0.5 \] of the values of the sequence from the minimum, maximum thresholds.
### Step 2: Regression
Calculation of the p parameters of the polynomial representing the sequence.

An additional parameter is calculated to represent the performance of the compression (including the deviations linked to the coding): the standard deviation between the initial points and the estimated points.
### Step 3: Coding of parameters
The parameters (points + standard deviation) are coded on the number of bits defined.

The result is a bit array (which can then be converted to variables of given length).

## Principle of advanced compression (class: Compressor)
A second method combining two regression levels is implemented. It allows in particular to get rid of the minimum / maximum limits which penalize the coding:

1 - Realization of a first regression on the supplied sequence.

2 - Realization of a second series of regressions on the sub-sequences made up of the differences between the initial values and those resulting from the first regression. The points of the second regression are coded in the envelope \[- 2 * type deviation, 2 * type deviation \] which makes it possible to reduce the number of coding bits necessary.

*Example of a sequence of 32 points*: We carry out a first regression with one point (average). For the 32 deviations from the average, perform 4 regressions (polynomials on 3 points) on the 4 sub-sequences of 8 points. On a therefore represented our 32 initial points by 1 + 4 * 3 points. The coding of the 4 * 3 points can be done on a low number of bits, for example 3 or 4 bits while the coding of the average point or the initial standard deviation must be done according to the minimum-maximum range defined.

This advanced compression is based on the simple compression class (Compactor).

## Principle of decompression implementation
Decompression consists in calculating the values from the parameters coded with the opposite steps to those of compression:
### Step 1: Decoding the parameters
Reconstruction of actual values from the coded value.
### Step 2: Decompression
Reconstitution of the estimated values from the parameters of the polynomial.
### Step 3: Denormalization
Scaling of estimates based on defined minimum / maximum thresholds
### Step 4: Standard deviation
Reconstruction of the compression indicator (standard deviation).

## Algorithms used
[see curve compression](https://fr.wikipedia.org/wiki/Compression_de_courbe)

## Use
See the examples given on the two types of regressions.

----



# Présentation
Ce repository regroupe un ensemble de fonctions de traitement de séries temporelles. Elles sont écrites en C++ pour être utilisées sur des équipements d'acquisition de données (ex. ESP / Arduino).

Deux types d'objets sont utilisés :
* Serie : objet représentant les série temporelles,
* Compactor / Compressor : objet représentant la compression d'une série temporelle,

# Objet Serie
Les séries temporelles comprennent une suite de valeurs numériques.
## Principe de représentation
L'objet Serie est composé des attributs suivants :
* **Nom :** Chaîne de caractères
* **Longueur :** Nombre de valeurs de la série (entier)
* **Liste de valeurs :** Les valeurs numériques successives de la série (tableau dynamique de valeurs réelles)

## Fonctions de création et de lecture
* **constructeur :** Plusieurs modes de création sont possibles
    * Série vide
    * Série d'une longueur donnée avec les valeurs à 0
    * Série d'une longueur donnée avec les valeurs initialisées à une valeur donnée
    * Série d'une longueur donnée avec les valeurs initialisées par répartition entre un point de départ et un point d'arrivée
    * Série créée par recopie d'une autre série
* **opérateurs :** Les opérateurs +, -, += et * sont surchargées pour combiner les valeurs de séries de même longueur. 
Une opération supplémentaire (opérateur | surchargé) permet de regrouper deux séries. 
L'affectation (opérateur = surchargé) est utilisée pour copier une série dans une autre. Les valeurs des séries sont accessibles directement (opérateur [] ).
* **mise à jour :** Les fonctions sont les suivantes :
    * **sousSerie()** : Génération d'une série constituée d'un extrait d'une série
    * **init()** : idem constructeur
    * **copie()** : idem constructeur de recopie
    * **setNom()** : modification du nom
    * **setSerie()** : modification de la liste des valeurs
* **consultation :** Les fonctions sont les suivantes :
    * **len()** : nombre de valeurs de la série
    * **lenom()** : nom de la série
    * **serie()** : liste des valeurs de la série
    
## Fonctions d'analyse d'une série
### Valeurs statistiques d'une série
* **moyenne()** : valeur moyenne de la série
* **ecartType()** : écart-type de la série
### Caractérisation de deux séries
* **ecDiff()** : valeur moyenne des écarts (valeur absolue) entre les deux séries 
* **etDiff()** : racine carrée de la moyenne des carrés des écarts entre les deux séries  
### Autre
* **pr()** : affichage de la série
* **autoCorrelation()** : à venir 

## Fonctions de transformation d'une série
### Fonctions de traitement
* **ecretage()** : Nouvelle série avec application d'un seuil mini, maxi pour chaque valeur
* **regPol()** : Génération d'une série réduite par régression polynomiale de la série
* **intPol()** : Génération d'une série étendue par interpolation polynomiale de la série
* **intLin()** : Génération d'une série étendue par interpolation linéaire de la série
* **intCont()** : Génération de termes manquants d'une série par interpolation cubique de la série
* **intSpline()** : Génération d'une série étendue par spline cubique d'interpolation de la série
* **lisSpline()** : Génération d'une série lissée par spline cubique de la série
### Fonctions de conversion
* **codage()** : Transformation de la série en une suite de bits par codage de chaque valeur de la série
* **decodage()** : Transformation d'une suite de bits en une série de valeurs par decodage de chaque valeur de la série
* **normalisation()** : Génération d'une série normalisée (valeurs comprises entre -0,5 et +0,5) 
* **denormalisation()** : Génération d'une série denormalisée (fonction inverse de la normalisation) 

## Utilisation
Voir les exemples donnés sur l'utilisation de l'objet Serie.

# Objets Compactor / Compressor 
Ces deux objets représentent la compression d'une série temporelle (ex pour minimiser les volumes de données envoyés sur un réseau LPWAN).

## Objectifs de la compression

1 - Remplacement d'un ensemble de points codés sur 16 bits (ex. entier) ou 32 bits (ex.réel) en un ensemble réduit de points et codés sur un nombre de bits réduits.

Exemple : 16 points réels (64 octets) compressés en 12 octets

2 - Utilisation d'algorithmes facilement implémentables sur des micro-controleurs.

Exemple : utilisation de Sigfox (limitation à 140 messages par jour et 12 octets par message).
-> avec une mesure toutes les 15 secondes, on peut envoyer les 32 valeurs mesurées toutes les 8 minutes sur 12 octets
## Principes de compression
La compression s'effectue par un envoi de paramètres permettant de reconstruire une séquence de plusieurs valeurs (séquence de 32 valeurs dans l'exemple ci-dessus). 

Les paramètres sont issus de plusieurs estimations par régression polynomiale et sont ensuite codés sur un nombre de bits défini ([voir compression de courbe](https://fr.wikipedia.org/wiki/Compression_de_courbe)).

L'erreur de compression (écart-type entre les valeurs de départ et les valeurs reconstruites) est également intégrée à la compression.
## Mise en oeuvre
Deux types d'algorithmes sont mis en place (classe : Compactor et classe : Compressor). Ils s'appuient sur l'objet Serie et présentent les fonctions suivantes :
* fonctions indépendantes d'un jeu de données : 
    * **check()**               : vérification des paramètres d'entrée 
    * **taillePayload()**       : nombre de bits des données compressées
    * **precisionCodage()**     : précision du codage utilisé
* fonctions liées au jeu de données : 
    * **calcul()**              : génération des principales sorties
    * **simul()**               : valeurs simulées après compression / décompression (utilisable après calcul)
    * **ecartTypeSimul()**      : écart-type des valeurs simulées / valeurs d'origine - fonction (utilisable après calcul)
    * **compress()**            : valeurs compressées (utilisable après calcul)
* fonctions liées à la décompression d'un jeu de données :
    * **decompressY0()**        : valeurs reconstituées par la décompression
    * **decompressYp()**        : valeurs des paramètres issus de la régression (Compactor uniquement)
    * **decompressEcartType()** : écart-type des valeurs simulées / valeurs d'origine
## Principe de la compression simple (classe : Compactor)
### Etape 1 : Normalisation
Mise à une échelle de \[-0,5  0,5\] des valeurs de la séquence à partir des seuils mini, maxi imposés ou libres.
### Etape 2 : Régression
Calcul des p paramètres du polynome représentant la séquence.

Un paramètre supplémentaire est calculé pour représenter la performance de la compression (y compris les écarts liés au codage) : l'écart-type entre les points initiaux et les points estimés.
### Étape 3 : Codage des paramètres 
Les paramètres ( points + écart-type) sont codés sur le nombre de bits défini.

Le résultat est un tableau de bit (qui peut ensuite être converti en variables de longueur donnée).
## Principe de la compression avancée (classe : Compressor)
Une deuxième méthode combinant deux niveaux de régression est implémentée. Elle permet notamment de s'affranchir des limites mini/maxi qui pénalisent le codage :

1 - Réalisation d'une première régression sur la séquence fournie.

2 - Réalisation d'une deuxième série de régressions sur des sous-séquences constituée des écarts entre les valeurs initiales et celles issues de la première régression. Les points de la deuxième régression sont codés dans l'enveloppe \[-2 * écart-type, 2 * écart-type\] ce qui permet de réduire le nombre de bits de codage nécessaire.

*Exemple d'une séquence de 32 points* : On effectue une première régression avec un point (moyenne). Pour les 32 écarts à la moyenne, on effectue 4 régressions (polynomiales sur 3 points) sur les 4 sous-séquences de 8 points. On a donc représenté nos 32 points initiaux par 1 + 4 * 3 points. Le codage des 4 * 3 points peut se faire sur un nombre de bits faible (codage sur la plage de 4 écart-type), par exemple 3 ou 4 bits alors que le codage du point de moyenne ou de l'écart-type initial doit se faire en fonction de la plage mini-maxi définie.

Cette compression avancée s'appuie sur la classe de compression simple.
## Principe de mise en oeuvre de la décompression
La décompression consiste à calculer les valeurs à partir des paramètres codés avec les étapes inverses à celles de la compression :
### Etape 1 : Décodage des paramètres
Reconstitution des valeurs réelles à partir de la valeur codée.
### Etape 2 : Décompression
Reconstitution des valeurs estimées à partir des paramètres du polynome.
### Etape 3 : Dénormalisation
Mise à l'échelle des estimations à partir des seuils mini/maxi définis
### Etape 4 : Ecart-type
Reconstitution de l'indicateur de compression (écart-type).
## Algorithmes utilisés
[voir compression de courbe](https://fr.wikipedia.org/wiki/Compression_de_courbe)
## Utilisation
Voir les exemples donnés sur les deux types de régressions.
