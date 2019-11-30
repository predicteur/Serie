# Series
Ce repository regroupe un ensemble de fonctions de traitement de séries temporelles. Elles sont écrites en C++ pour être utilisées sur des équipements d'acquisition de données (ex. ESP / Arduino).

# Fonctions d'analyse d'une série
## Valeurs statistiques
init : initialisation d'une série à partir du point de départ et du point d'arrivée
moyenne : Valeur moyenne de la série
ecartType : écrt-type de la série


# Fonctions de transformation d'une série
## 
ecretage : Nouvelle série avec application d'un seuil mini, maxi pour chque valeur
regPol : Génération d'une série réduite par régression polynomiale de la série
intPol : Génération d'une série étendue par interpolation polynomiale de la série
intLin : Génération d'une série étendue par interpolation linéaire de la série
intSpline : Génération d'une série étendue par spline cubique d'interpolation de la série


# Fonctions de transformation de deux séries
## 
ecDiff : valeur moyenne des écarts (valeur absolue) entre les deux séries 
etDiff : valeur moyenne des carés des écarts entre les deux séries  

# Fonctions de transformation de deux séries
## 
ecDiff : valeur moyenne des écarts (valeur absolue) entre les deux séries 
