# Serie
Ce repository regroupe un ensemble de fonctions de traitement de séries temporelles. Elles sont écrites en C++ pour être utilisées sur des équipements d'acquisition de données (ex. ESP / Arduino).
Les séries sont des objets composés d'une suite de valeurs de longueur dynamique. Elles disposent également d'un nom.

# Fonctions de création et de combinaison
* **constructeur :** Plusieurs modes de création sont possibles
    * Série vide
    * Série d'une longueur donnée avec les valeurs à 0
    * Série d'une longueur donnée avec les valeurs initialisées à une valeur donnée
    * Série d'une longueur donnée avec les valeurs initialisées par répartition entre un point de départ et un point d'arrivée
    * Série créée par recopie d'une autre série
* **opérateurs :** Les opérations +, -, += et * sont surchargées pour combiner les valeurs de séries de même longueur. Une opération supplémentaire | permet de regrouper deux séries. L'opérteur d'affectation = est également surchargé pour copier une série dans une autre. Les valeurs des séries sont accessibles directement par l'opérateur []
* **sousSerie :** Génération d'une série constituée d'un extrait d'une série

# Fonctions d'analyse d'une série
## Valeurs statistiques d'une série
* **moyenne :** Valeur moyenne de la série
* **ecartType :** écart-type de la série
## Caractérisation de deux séries
* **ecDiff :** valeur moyenne des écarts (valeur absolue) entre les deux séries 
* **etDiff :** valeur moyenne des carrés des écarts entre les deux séries  
## Autre
* **pr :** Affichage de la série
* **autoCorrélation :** à venir 


# Fonctions de transformation d'une série
## Fonctions de traitement
* **ecretage :** Nouvelle série avec application d'un seuil mini, maxi pour chque valeur
* **regPol :** Génération d'une série réduite par régression polynomiale de la série
* **intPol :** Génération d'une série étendue par interpolation polynomiale de la série
* **intLin :** Génération d'une série étendue par interpolation linéaire de la série
* **intCont :** Génération de termes manquants d'une série par interpolation cubique de la série
* **intSpline :** Génération d'une série étendue par spline cubique d'interpolation de la série
* **lisSpline :** Génération d'une série lissée par spline cubique de la série
## Fonctions de conversion
* **codage :** Transformation de la série en une suite de bits par codage de chaque valeur de la série
* **decodage :** Transformation d'une suite de bits en une série de valeurs par decodage de chaque valeur de la série
* **normalisation :** Génération d'une série normalisée (valeurs comprises entre -0,5 et +0,5) 
* **denormalisation :** Génération d'une série denormalisée (fonction inverse de la normalisation) 
