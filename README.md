# Pricer - C++20 Project

Environnement de développement pour C++20.

## Prérequis

- CMake 3.20+
- Compilateur supportant C++20 (g++ 10+, clang 13+, ou MSVC)

## Build

```bash
# Créer le répertoire de build
mkdir build
cd build

# Générer les fichiers de build
cmake ..

# Compiler
cmake --build .
# ou
make
```

## Exécution

```bash
./bin/pricer
```

## Structure du projet

```
├── src/           # Code source
│   └── main.cpp
├── tests/         # Tests (optionnel)
├── CMakeLists.txt # Configuration CMake
└── README.md
```

## Utilisation de C++20

Ce projet est configuré pour utiliser C++20 avec les dernières fonctionnalités :

- Modules (si supportés)
- Concepts
- Coroutines
- Ranges
- Et plus...
