# Pricer

A C++ quantitative finance project focused on equity derivatives pricing, Greeks, and model calibration.

This repository explores the core building blocks of a pricing engine: Black-Scholes valuation, Heston stochastic volatility, sensitivity analysis, and calibration workflows. The goal is to implement financial models in a clean, extensible, and testable C++ architecture while reinforcing the numerical and software engineering skills required in quantitative finance.

## Why this project

Pricing is one of the foundations of modern quantitative finance. A robust pricing engine must combine:

- mathematical modeling
- numerical stability
- performance-aware implementation
- validation through tests
- extensibility for additional models and risk measures

This project is built around that challenge and focuses on the mechanics behind equity derivatives valuation.

## Core components

### Equity option pricing

- European call and put valuation
- closed-form Black-Scholes implementation
- d1 and d2 term computation
- option structure modeling for pricing workflows

### Stochastic volatility

- Heston model implementation
- characteristic function formulation
- numerical integration for price evaluation
- parameter management for model calibration

### Risk and sensitivity

- delta
- gamma
- vega
- theta
- rho

### Calibration

- implied volatility extraction from synthetic market quotes
- Heston parameter fitting experiments
- optimization-based calibration workflows

## Technology stack

- C++20
- CMake
- GoogleTest
- Boost
- Eigen3
- Ceres

## Project structure

```text
Pricer/
├── CMakeLists.txt
├── README.md
├── run.sh
├── src/
│   ├── calibration/
│   │   ├── VolatilitySurfaceCalibrator.cpp
│   │   └── VolatilitySurfaceCalibrator.hpp
│   ├── greeks/
│   │   ├── BlackScholesGreeks.cpp
│   │   ├── BlackScholesGreeks.hpp
│   │   ├── HestonGreeks.cpp
│   │   └── HestonGreeks.hpp
│   ├── models/
│   │   ├── BlackScholes.cpp
│   │   ├── BlackScholes.hpp
│   │   ├── HestonModel.cpp
│   │   ├── HestonModel.hpp
│   │   └── IOptionPricer.hpp
│   ├── test/
│   │   └── test_black_scholes.cpp
│   ├── types/
│   │   └── types.hpp
│   ├── utils/
│   │   └── math.hpp
│   ├── main.cpp
│   └── ...
└── build/
```

## Build and run

```bash
./run.sh
```

or:

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
./bin/pricer
```

## Tests

```bash
ctest --test-dir build --output-on-failure
```

The test suite validates the core pricing logic, including:

- option and sigma accessors
- d1/d2 computation
- Black-Scholes pricing behavior
- put-call parity
- intrinsic value checks at zero maturity

## Outcome

This project demonstrates a practical interest in:

- equity derivatives
- quantitative finance
- numerical pricing methods
- C++-based financial engineering

It is a strong example of a personal project centered on pricing models, model validation, and computational finance in C++.
