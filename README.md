# Derivatives Pricing Engine

This repository is a C++20 project for option pricing and sensitivity analysis using simple financial models.

It currently includes Black-Scholes and Heston pricing components, along with basic structures for Greeks and calibration.

## What is actually implemented

### 1. Market data structures

In [src/types/types.hpp](src/types/types.hpp):

- `Option`: spot, strike, maturity, risk-free rate, dividend, option type
- `OptionType`: `Call` / `Put`
- `HestonParams`: parameters of the Heston model
- `MarketData` and `MarketQuote`: market data structures

### 2. Black-Scholes model

In [src/models/BS.hpp](src/models/BS.hpp) and [src/models/BS.cpp](src/models/BS.cpp):

- `BS` class inheriting from the generic `IPricer` template
- computation of `d1` and `d2`
- pricing for call / put options
- access to the implied-volatility parameter

### 3. Heston model

In [src/models/Heston.hpp](src/models/Heston.hpp) and [src/models/Heston.cpp](src/models/Heston.cpp):

- `Heston` class
- parameters: `kappa`, `theta`, `vol_vol`, `rho`, `v0`
- Heston characteristic function
- numerical integration for pricing
- call / put option pricing
- access to model parameters through `get_params()`

### 4. Generic pricing template

In [src/models/IPricer.hpp](src/models/IPricer.hpp):

- `IPricer<Derived>` provides a common interface
- `price()` method dispatches based on option type (`Call` / `Put`)

### 5. Greeks

In [src/greeks/BS_greeks.hpp](src/greeks/BS_greeks.hpp), [src/greeks/BS_greeks.cpp](src/greeks/BS_greeks.cpp), [src/greeks/Heston_greeks.hpp](src/greeks/Heston_greeks.hpp), and [src/greeks/Heston_greeks.cpp](src/greeks/Heston_greeks.cpp):

- calculation of `delta`, `gamma`, `vega`, `theta`, and `rho`
- implementation for both Black-Scholes and Heston models

### 6. Calibration

In [src/calibration/BSCalib.hpp](src/calibration/BSCalib.hpp):

- `ImpliedVol` and `MarketData` structures
- `BSCalibrator` class in progress
- concept for calibrating the volatility surface from market prices

Calibration is not fully finalized in the current code; it is more of a working prototype/base implementation.

## Current entry point

The main program in [src/main.cpp](src/main.cpp) builds a simple example with:

- spot = 100
- strike = 100
- maturity = 1.0
- risk-free rate = 3%
- dividend = 0
- Heston and Black-Scholes models

It then prints the price computed by each model.

## Project structure

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

## Requirements

- CMake 3.20+
- C++20 compiler (g++, clang, MSVC)
- optionally, system dependencies required by the project environment

## Build

### Option 1: use the script

```bash
./run.sh
```

### Option 2: manual build

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Execution

```bash
./build/bin/pricer
```

or, if the script was used:

```bash
./run.sh
```

## Important notes

- The project is still a pricing engine in development, not a finished product.
- Some parts are calibration / Greeks prototypes.
- Naming conventions for some option fields are still uneven across the codebase.
- The core pricing logic is present, but some parts still need cleanup and unification before industrial use.

## Project goal

The goal is to provide a derivatives pricing engine with:

- market option pricing via reference models,
- Greeks calculation,
- preparation for implied-volatility calibration,
- extensible structure for adding more pricing models.

## Current status

The project is functional as a C++20 prototype for pricing and exploring financial models, with concrete implementations of:

- Black-Scholes
- Heston
- generic pricing interface
- basic Greeks
- basic volatility-surface calibration foundation
