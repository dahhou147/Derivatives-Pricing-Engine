# Derivatives Pricing Engine

**C++20 quantitative finance engine for pricing and analyzing equity derivatives.**

A modular derivatives pricing engine implementing **Black-Scholes**, **Heston stochastic volatility**, **Fourier/Carr-Madan pricing**, **Greeks**, **implied volatility**, and **volatility calibration**.

Built with modern C++20, CMake, and GoogleTest, with a focus on numerical methods, model validation, and extensibility.

---

## ✨ Features

### Pricing Models

* **Black-Scholes-Merton**

  * European call and put pricing
  * Analytical pricing
  * Put-call parity
  * Intrinsic value checks

* **Heston Stochastic Volatility**

  * Stochastic volatility dynamics
  * Characteristic function
  * Fourier-based pricing
  * Numerical integration

### Risk & Volatility

* Delta
* Gamma
* Vega
* Theta
* Rho
* Implied volatility
* Volatility surface calibration

### Numerical Methods

* Carr-Madan Fourier pricing
* Numerical quadrature
* Gauss-Legendre integration
* Characteristic-function based pricing
* Numerical validation against analytical results

### Engineering

* Modern **C++20**
* **CMake** build system
* **GoogleTest** test suite
* Modular architecture
* Designed for extensibility

---

## 📐 Mathematical Models

### Black-Scholes-Merton

For a European call option:

$$
C = S_0 N(d_1) - K e^{-rT}N(d_2)
$$

where

$$
d_1 =
\frac{
\ln(S_0/K) + (r + \frac{1}{2}\sigma^2)T
}{
\sigma\sqrt{T}
}
$$

and

$$
d_2 = d_1 - \sigma\sqrt{T}
$$

The corresponding European put is obtained using:

$$
P = K e^{-rT}N(-d_2) - S_0N(-d_1)
$$

The implementation also provides analytical Greeks and implied volatility calculations.

---

### Heston Model

The Heston model introduces stochastic volatility:

$$
dS_t = \mu S_t\,dt + \sqrt{v_t}S_t\,dW_t^S
$$

$$
dv_t =
\kappa(\theta-v_t)\,dt
+
\sigma\sqrt{v_t}\,dW_t^v
$$

with

$$
dW_t^S dW_t^v = \rho\,dt
$$

where:

| Parameter | Description             |
| --------- | ----------------------- |
| $S_t$     | Underlying price        |
| $v_t$     | Instantaneous variance  |
| $\kappa$  | Mean-reversion speed    |
| $\theta$  | Long-term variance      |
| $\sigma$  | Volatility of variance  |
| $\rho$    | Correlation             |
| $r$       | Risk-free interest rate |

The Heston implementation uses the model's **characteristic function** together with numerical/Fourier integration to obtain option prices.

---

## 🔢 Carr-Madan Fourier Pricing

For models with a tractable characteristic function, option prices can be obtained using Fourier methods.

The engine implements the **Carr-Madan approach**, transforming the option pricing problem into a numerical integration problem.

This provides an efficient alternative to direct numerical integration and is particularly useful for stochastic-volatility models such as Heston.

---

## 📊 Greeks

The engine provides standard first- and second-order risk sensitivities:

| Greek     | Description                                  |
| --------- | -------------------------------------------- |
| **Delta** | Sensitivity to the underlying price          |
| **Gamma** | Sensitivity of Delta to the underlying price |
| **Vega**  | Sensitivity to volatility                    |
| **Theta** | Sensitivity to time decay                    |
| **Rho**   | Sensitivity to interest rates                |

These quantities are essential for understanding the risk profile of derivative portfolios.

---

## 🎯 Implied Volatility

Given a market option price, the engine can recover the corresponding Black-Scholes implied volatility.

Conceptually:

$$
C_{market} = C_{BS}(S_0,K,T,r,\sigma_{imp})
$$

The engine solves numerically for:

$$
\sigma_{imp}
$$

This can then be used to construct and analyze implied volatility smiles and surfaces.

---

## 🧪 Calibration

The project includes functionality for calibrating model parameters against option market data.

The calibration workflow is:

```text
Market Option Prices
        │
        ▼
   Calibration
        │
        ▼
Heston Parameters
        │
        ▼
   Model Prices
        │
        ▼
 Pricing Error
```

The objective is to find model parameters that minimize the difference between observed market prices and model prices.

This provides a foundation for constructing calibrated stochastic-volatility models.

---

## 🏗️ Project Structure

```text
Derivatives-Pricing-Engine/
│
├── include/
│   ├── ...
│
├── src/
│   ├── ...
│
├── tests/
│   ├── ...
│
├── CMakeLists.txt
├── README.md
└── ...
```

The codebase is organized around independent pricing components so that additional models and numerical methods can be added without redesigning the entire engine.

---

## ⚡ Build

### Requirements

* C++20 compatible compiler
* CMake
* GoogleTest

### Clone

```bash
git clone https://github.com/dahhou147/Derivatives-Pricing-Engine.git

cd Derivatives-Pricing-Engine
```

### Build

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

---

## 🧪 Tests

The project uses **GoogleTest** for automated testing.

Run the test suite with:

```bash
ctest --output-on-failure
```

The tests cover core pricing functionality and numerical properties such as:

* Black-Scholes pricing
* Call/put consistency
* Put-call parity
* Intrinsic value
* Model calculations
* Numerical components

The goal is to validate both the mathematical implementation and the numerical behavior of the engine.

---

## 🔬 Numerical Validation

A pricing engine is only useful if its numerical results can be trusted.

The project therefore emphasizes validation through mathematical identities and cross-method comparisons.

Examples include:

```text
Analytical Black-Scholes
          │
          ├──────────────┐
          ▼              ▼
      Call Price      Put Price
          │              │
          └──────┬───────┘
                 ▼
          Put-Call Parity
```

For stochastic-volatility models, numerical results can be compared across different pricing approaches to identify integration or convergence issues.

---

## 📈 Example Use Case

A typical workflow is:

```text
Market Data
    │
    ▼
Option Parameters
    │
    ▼
┌──────────────────────┐
│ Derivatives Engine   │
├──────────────────────┤
│ Black-Scholes        │
│ Heston               │
│ Fourier Pricing      │
│ Greeks               │
│ Implied Volatility   │
│ Calibration          │
└──────────────────────┘
    │
    ▼
Price / Greeks / Volatility
```

For example, an option can be characterized by:

```text
Spot       : 100
Strike     : 105
Maturity   : 1.0 year
Rate       : 5%
Volatility : 20%
```

The engine can then compute:

```text
Call Price
Put Price
Delta
Gamma
Vega
Theta
Rho
Implied Volatility
```

---

## 🛠️ Roadmap

The project is actively designed to be extended toward a more complete quantitative-finance library.

### Pricing

* [x] Black-Scholes
* [x] Heston
* [x] Fourier / Carr-Madan
* [ ] Monte Carlo pricing
* [ ] American options
* [ ] Barrier options
* [ ] Asian options
* [ ] Basket options

### Volatility

* [x] Implied volatility
* [x] Heston calibration
* [ ] Volatility smile visualization
* [ ] Volatility surface generation
* [ ] Local volatility
* [ ] SABR

### Risk

* [x] Delta
* [x] Gamma
* [x] Vega
* [x] Theta
* [x] Rho
* [ ] Portfolio-level Greeks
* [ ] Scenario analysis

### Engineering

* [x] C++20
* [x] CMake
* [x] GoogleTest
* [ ] Continuous Integration
* [ ] Benchmarks
* [ ] API documentation
* [ ] Python bindings

---

## 🤝 Contributing

Contributions are welcome.

If you would like to improve the project:

1. Fork the repository.
2. Create a feature branch.

```bash
git checkout -b feature/my-feature
```

3. Implement your changes.
4. Add or update tests.
5. Commit your changes.

```bash
git commit -m "Add my feature"
```

6. Push the branch.

```bash
git push origin feature/my-feature
```

7. Open a Pull Request.

For larger changes, opening an issue first is recommended so that the implementation can be discussed before development begins.

---

## 📚 References

The implementation is inspired by standard quantitative-finance literature, including:

* Black, F. & Scholes, M. — *The Pricing of Options and Corporate Liabilities*
* Merton, R. — *Theory of Rational Option Pricing*
* Heston, S. — *A Closed-Form Solution for Options with Stochastic Volatility*
* Carr, P. & Madan, D. — *Option Valuation Using the Fast Fourier Transform*
* Gatheral, J. — *The Volatility Surface*

---

## 👤 Author

**dahhou147**

Quantitative finance • C++ • Derivatives Pricing • Stochastic Volatility

---

### ⭐ If you're interested in quantitative finance, derivatives pricing, or C++ financial engineering, feel free to explore the project and contribute.
