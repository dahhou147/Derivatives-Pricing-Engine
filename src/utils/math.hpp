#pragma once
#include <array>
#include <cmath>
#include <complex>

inline double norm_cdf(double x) {
    return 0.5 * (1.0 - std::erf(-x / std::sqrt(2.0)));
}

inline double norm_density(double x) {
    return (1.0 / std::sqrt(2.0 * M_PI)) * std::exp(-0.5 * x * x);
}

// ── Gauss-Legendre quadrature — 32 points ──────────────────────────────────
// Nodes on [-1, 1]: 16 positive nodes, with the 16 negative nodes obtained
// by symmetry.
//
// Legendre polynomials form an orthogonal basis on the interval [-1, 1].
// Gauss-Legendre quadrature chooses the evaluation points as the roots of
// the Legendre polynomial P_n and associates a weight with each point,
// corresponding to the integral of the associated Lagrange basis polynomial.
//
// Here, the positive nodes are stored in GL_X and their corresponding
// weights are stored in GL_W.
//
// The integral is approximated by:
//
// \f[
// \int_{-1}^{1} f(x)\,dx
// \approx
// \sum_{i=1}^{n} w_i f(x_i)
// \f]
//
// The symmetry of the nodes and weights allows us to store only the 16
// positive values for the 32-point quadrature.

static constexpr int GL_N = 32;

// les point d'evaluation
static constexpr std::array<double, GL_N / 2> GL_X = {{
    0.04830766568773831623, 0.14447196158279649349,
    0.23928736225213707454, 0.33186860228212764978,
    0.42135127613063534536, 0.50689990893222939002,
    0.58771575724076232904, 0.66304426693021520098,
    0.73218211874028968039, 0.79448379596794240696,
    0.84936761373256997013, 0.89632115576605212397,
    0.93490607593773968917, 0.96476225558750643077,
    0.98561151154526833540, 0.99726386184948156354
}};
// les poids
static constexpr std::array<double, GL_N / 2> GL_W = {{
    0.09654008851472780057, 0.09563872007927485942,
    0.09384439908080456564, 0.09117387869576388471,
    0.08765209300440381114, 0.08331192422694675522,
    0.07819389578707030647, 0.07234579410884850625,
    0.06582222277636184684, 0.05868409347853554715,
    0.05099805926237617620, 0.04283589802222668066,
    0.03427386291302143310, 0.02539206530926205946,
    0.01627439473090567061, 0.00701861000947009660
}};

// Intègre f sur [a, b] par Gauss-Legendre 32 points
template <typename F>
inline double gauss_legendre(F&& f, double a, double b) {
    const double mid  = 0.5 * (a + b);
    const double half = 0.5 * (b - a);
    double sum = 0.0;
    for (int i = 0; i < GL_N / 2; ++i)
        sum += GL_W[i] * (f(mid + half * GL_X[i]) + f(mid - half * GL_X[i]));
    return half * sum;
}
