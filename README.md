# Presentation
This repository groups together a set of time series functions. They are written in C ++ to be used on data acquisition equipment (eg ESP / Arduino).

Three types of objects are used:
* Mesure : objet representing a set of attributes or time series to be managed,
* Serie : object representing time series,
* Compactor / Compressor : object representing the compression of a time series,


----
----
                                       version francaise

----
----

# Présentation
Ce repository regroupe un ensemble de fonctions de traitement de séries temporelles. Elles sont écrites en C++ pour être utilisées sur des équipements d'acquisition de données (ex. ESP / Arduino).

Trois types d'objets sont utilisés :
* Mesure : objet représentant l'ensemble des attributs et des séries temporelles qui composent une mesure
* Serie : objet représentant une série temporelle,
* Compactor / Compressor : objet représentant la compression d'une série temporelle,

# Objet Mesure
L'objet Mesure se compose de la liste des données qui définissent une mesure.

## Principe de representation
L'objet Mesure est composé des attributs suivants :
* **Nom :** Chaine de caractères (nom de la mesure)
* liste d'objets Serie definie par
   * **Series :** liste de Serie
   * **NbSer :** Nombre de Serie de la liste
   * **LenSer :** Nombre de valeurs qui composent les Serie de la liste
* liste des attributs (String) définis par
   * **Strs :** liste de String
   * **NameStrs :** liste de String (nom de chaque String)
   * **NbStr :** Nombre des attributs de type String
* liste des attributs (Float) définis par
   * **Atts :** liste de Float
   * **NameAtts :** liste de String (nom de chaque Float)
   * **NbAtt :** Nombre des attributs de type Float

## Fonctions de création et de lecture
* **constructeur :** Plusieurs modes de création sont possibles
    * Mesure vide
    * Mesure avec le nombre et longueur des données mais sans les noms
    * Mesure complète (avec nombre, longueur et noms)

* **operateurs :** The operators [] is used to access to the values of the series.

* **mise à jour :** Les fonctions sont les suivantes :
    * **refresh()**: Ajoute une nouvelle valeur à toutes les Serie et supprime la dernière
    * **init()** : initialisation des noms et valeurs
    * **initNoms()**: initialisation des noms 
    * **initSeries()** : initialisation de la liste des valeurs des séries
    * **setVal()**: initialisation des valeurs des attributs string et float
    
* **consultation :** Les fonctions sont les suivantes :
    * **getString()**: valeur de l'attribut string
    * **getFloat()**: valeur de l'attribut float
    * **getNom()**: nom des series

## Fonctions d'analyse d'une Mesure
* **pr()**: affichage de la Mesure
* **json()**: affichage de la Mesure au format Json

## Utilisation
Voir les exemples donnés sur l'utilisation de l'objet Mesure.

# Objet Serie
Les séries temporelles comprennent une suite de valeurs numériques.
## Principe de représentation
L'objet Serie est composé des attributs suivants :
* **Nom :** Chaîne de caractères
* **Longueur :** Nombre de valeurs de la série (entier)
* **Liste de valeurs :** Les valeurs numériques successives de la série (tableau dynamique de valeurs réelles).La plus récente a l'index 0.

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
    * **complete()**: Ajoute une nouvelle valeur à la liste
    * **refresh()**: Ajoute une nouvelle valeur à la liste et supprime la dernière
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
* **json()**: affichage de la série au format Json
* **autoCorrelation()** : à venir 

## Fonctions de transformation d'une série
### Fonctions de filtrage
* **ecretage()** : Nouvelle série avec application d'un seuil mini, maxi pour chaque valeur
* **lisSpline()** : Génération d'une série lissée par spline cubique de la série
* **lisSA()**: Génération d'une série lissée par un filtre moyenne mobile simple
* **lisSG()**: Génération d'une série lissée par un filtre moyenne mobile Savitzky-Golay
* **lisGA()**: Génération d'une série lissée par un filtre moyenne mobile gaussien
* **lisWA()**: Génération d'une série lissée par un filtre moyenne mobile pondérée
* **lisES()**: Génération d'une série lissée par un filtre moyenne mobile exponentielle
### Fonctions de réduction de séries
* **regPol()** : Génération d'une série réduite par régression polynomiale de la série
### Fonctions d'extension de séries
* **intPol()** : Génération d'une série étendue par interpolation polynomiale de la série
* **intLin()** : Génération d'une série étendue par interpolation linéaire de la série
* **intCont()** : Génération de termes manquants d'une série par interpolation polynomiale cubique de la série
* **intSpline()** : Génération d'une série étendue par spline cubique d'interpolation de la série
### Fonctions de conversion
* **codage()** : Transformation de la série en une suite de bits par codage de chaque valeur de la série
* **decodage()** : Transformation d'une suite de bits en une série de valeurs par decodage de chaque valeur de la série
* **normalisation()** : Génération d'une série normalisée (valeurs comprises entre -0,5 et +0,5) 
* **denormalisation()** : Génération d'une série denormalisée (fonction inverse de la normalisation) 
* **conversion()**: Conversion d'une valeur comprise entre mini et maxi en un entier compris entre 0 et 2\*\*bits
* **conversionb()**: Conversion d'un entier compris entre 0 et 2\*\*bits en une valeur comprise entre mini et maxi
* **codbin()**: Conversion d'un entier compris entre 0 et 2\*\*bits en une Serie de valeurs 0 or 1 (longueur : bits)
* **decbin()**: Conversion d'une Serie de valeurs 0 or 1 en un entier

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
    * **check()**               : vérification des paramètres du constructeur 
    * **taillePayload()**       : nombre de bits des données compressées
    * **precisionCodage()**     : précision du codage utilisé
    * **tauxCompression()**     : ratio entre le nombre de bits après compression et le nombre de bit avant compression (2 octets par valeur)
* fonctions liées au jeu de données : 
    * **calcul()**              : génération des principales sorties
    * **simul()**               : valeurs simulées après compression / décompression (utilisable après calcul)
    * **ecartTypeSimul()**      : écart-type des valeurs simulées / valeurs d'origine - fonction (utilisable après calcul)
    * **param()**               : valeurs des paramètres (utilisable après calcul) (Compactor uniquement)
    * **compress()**            : valeurs compressées (utilisable après calcul)
    * **compressEct()**         : valeur de l'écart-type compressé (utilisable après calcul)
    * **compressYp()**          : valeurs compressées hors écart-type (utilisable après calcul)
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
