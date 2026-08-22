# Derivatives Pricing Engine

Ce dépôt est un projet C++20 pour le pricing d’options et l’analyse de sensibilités par des modèles financiers simples.

Il contient actuellement des éléments de pricing Black-Scholes et Heston, ainsi que des structures de base pour les greeks et la calibration.

## Ce qui est réellement implémenté

### 1. Structures de données de marché

Dans [src/types/types.hpp](src/types/types.hpp) :

- `Option` : spot, strike, maturity, taux sans risque, dividende, type d’option
- `OptionType` : `Call` / `Put`
- `HestonParams` : paramètres du modèle Heston
- `MarketData` et `MarketQuote` : structures de données de marché

### 2. Modèle Black-Scholes

Dans [src/models/BS.hpp](src/models/BS.hpp) et [src/models/BS.cpp](src/models/BS.cpp) :

- classe `BS` héritant d’un template générique `IPricer`
- calcul de `d1` et `d2`
- pricing d’option call / put
- accès au paramètre de volatilité implicite

### 3. Modèle de Heston

Dans [src/models/Heston.hpp](src/models/Heston.hpp) et [src/models/Heston.cpp](src/models/Heston.cpp) :

- classe `Heston`
- paramétrage avec `kappa`, `theta`, `vol_vol`, `rho`, `v0`
- fonction caractéristique Heston
- intégration numérique pour le pricing
- pricing d’option call / put
- accès aux paramètres via `get_params()`

### 4. Template générique de pricing

Dans [src/models/IPricer.hpp](src/models/IPricer.hpp) :

- `IPricer<Derived>` fournit une interface commune
- méthode `price()` selon le type d’option (`Call` / `Put`)

### 5. Greeks

Dans [src/greeks/BS_greeks.hpp](src/greeks/BS_greeks.hpp), [src/greeks/BS_greeks.cpp](src/greeks/BS_greeks.cpp), [src/greeks/Heston_greeks.hpp](src/greeks/Heston_greeks.hpp), [src/greeks/Heston_greeks.cpp](src/greeks/Heston_greeks.cpp) :

- calcul de `delta`, `gamma`, `vega`, `theta`, `rho`
- implémentation pour le modèle Black-Scholes et Heston

### 6. Calibration

Dans [src/calibration/BSCalib.hpp](src/calibration/BSCalib.hpp) :

- structures `ImpliedVol` et `MarketData`
- classe `BSCalibrator` en préparation
- idée de calibration de la surface de vol sur les prix de marché

La calibration n’est pas complétement finale dans le code actuel ; c’est plutôt une base de travail / prototype.

## Point d’entrée actuel

Le programme principal dans [src/main.cpp](src/main.cpp) construit un exemple simple avec :

- spot = 100
- strike = 100
- maturity = 1.0
- taux sans risque = 3%
- dividende = 0
- modèle Heston et modèle Black-Scholes

Il affiche ensuite le prix calculé par chacun des deux modèles.

## Structure du projet

```text
Pricer/
├── CMakeLists.txt
├── README.md
├── run.sh
├── build/
├── src/
│   ├── main.cpp
│   ├── calibration/
│   │   ├── BSCalib.cpp
│   │   └── BSCalib.hpp
│   ├── greeks/
│   │   ├── BS_greeks.cpp
│   │   ├── BS_greeks.hpp
│   │   ├── Heston_greeks.cpp
│   │   └── Heston_greeks.hpp
│   ├── models/
│   │   ├── BS.cpp
│   │   ├── BS.hpp
│   │   ├── Heston.cpp
│   │   ├── Heston.hpp
│   │   ├── IPricer.hpp
│   │   └── ...
│   ├── types/
│   │   └── types.hpp
│   └── utils/
│       └── math.hpp
└── tests/
```

## Prérequis

- CMake 3.20+
- compilateur C++20 (g++, clang, MSVC)
- éventuellement dépendances système utiles au projet selon l’environnement

## Build

### Option 1 : utiliser le script

```bash
./run.sh
```

### Option 2 : build manuel

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Exécution

```bash
./build/bin/pricer
```

ou, si le script a été utilisé :

```bash
./run.sh
```

## Remarques importantes

- Le projet est encore un moteur de pricing en développement, pas un produit fini.
- Certaines parties sont des prototypes de calibration / greeks.
- Les conventions de nommage de certains champs d’option peuvent encore être inégales dans le code.
- Le cœur de la logique de pricing est présent, mais il faut encore nettoyer et unifier certaines parties avant un usage industriel.

## Objectif du projet

Le but est de fournir un moteur de pricing de dérivés avec :

- prix de marché d’options via modèles de référence,
- calcul de sensibilités (greeks),
- préparation de calibrations de volatilités implicites,
- structure extensible pour ajouter des modèles de pricing supplémentaires.

## État actuel

Le projet est fonctionnel comme prototype de pricing et d’exploration de modèles financiers en C++20, avec une implémentation concrète de :

- Black-Scholes
- Heston
- interface générique de pricing
- greeks de base
- base de calibration de surface de vol
