#include <HestonModel.hpp>
#include <math.hpp>

Heston::Heston(Option options, double kappa, double theta, double vol_vol, double rho, double v0) :
    IPricer<Heston>(options), kappa(kappa), theta(theta), vol_vol(vol_vol), rho(rho), v0(v0) {
}

std::complex<double> Heston::characteristic_function(std::complex<double> u) const {
    const std::complex<double> i(0.0, 1.0);
    const double x = log(option.spot);
    std::complex<double> d =
        sqrt(pow(kappa - rho * vol_vol * i * u, 2.0) + vol_vol * vol_vol * u * (u + i));
    std::complex<double> A_minus = kappa - rho * vol_vol * i * u - d;
    std::complex<double> A_plus = kappa - rho * vol_vol * i * u + d;
    std::complex<double> g = A_minus / A_plus;
    std::complex<double> exp_dt = exp(-d * option.maturity);
    std::complex<double> C =
        option.free_risk_rate * i * u * option.maturity +
        (kappa * theta) / (vol_vol * vol_vol) *
            (A_minus * option.maturity - 2.0 * log((1.0 - g * exp_dt) / (1.0 - g)));
    std::complex<double> D =
        (A_minus / (vol_vol * vol_vol)) * ((1.0 - exp_dt) / (1.0 - g * exp_dt));
    return exp(C + D * v0 + i * u * x);
}

/*
 * Heston Semi-Analytical Pricing Formula
 *
 * European call price under the Heston stochastic volatility model:
 *
 *   C = S * P1 - K * e^{-rT} * P2
 *
 * where P1 and P2 are risk-adjusted probabilities obtained via Fourier inversion.
 *
 * Key idea:
 * ---------
 * The characteristic function of log(S_T) under the Heston model admits a
 * closed-form expression. Heston (1993) shows that the solution has the affine form:
 *
 *   φ_j(u) = exp( C_j(τ, u) + D_j(τ, u) * v0 + i*u * log(S) )
 *
 * where C_j and D_j satisfy a system of Riccati ODEs derived by substituting
 * this ansatz into the Kolmogorov PDE associated with the Heston dynamics.
 *
 * The two probabilities differ by their measure:
 *   - P2 : risk-neutral measure Q      (numeraire: money market account B(t))
 *   - P1 : T-forward measure Q^T       (numeraire: zero-coupon bond P(t,T))
 *
 * Recovery via Fourier inversion (Gil-Pelaez):
 *
 *   P_j = 1/2 + (1/π) * ∫_0^∞ Re[ e^{-i*u*log(K)} * φ_j(u) / (i*u) ] du
 *
 * Numerical integration is performed via Gauss-Legendre quadrature
 * on adaptive subintervals to handle the oscillatory integrand.
 *
 * for the first iteration the length of the inverval is 0.1 Why ? because we ingrating by 1 \phi
 * near zero this terme explose
 *
 */

double Heston::integrate(int j) const {
    const std::complex<double> ci(0.0, 1.0);
    const double logK = log(option.strike);
    const std::complex<double> phi_minus_i =
        (j == 1) ? characteristic_function(std::complex<double>(0.0, -1.0))
                 : std::complex<double>(1.0, 0.0);
    auto integrand = [&](double phi) -> double {
        const std::complex<double> u(phi, 0.0);
        std::complex<double> numer;
        if (j == 1)
            numer = exp(-ci * u * logK) * characteristic_function(u - ci) / phi_minus_i;
        else
            numer = exp(-ci * u * logK) * characteristic_function(u);
        return real(numer / (ci * u));
    };
    constexpr double eps = 1e-8;
    double sum = 0.0;
    for (int k = 0; k < 10; ++k)
        sum += gauss_legendre(integrand, eps + k * 0.1, eps + (k + 1) * 0.1);
    for (int k = 0; k < 49; ++k)
        sum += gauss_legendre(integrand, 1.0 + k * 1.0, 1.0 + (k + 1) * 1.0);
    for (int k = 0; k < 15; ++k)
        sum += gauss_legendre(integrand, 50.0 + k * 10.0, 50.0 + (k + 1) * 10.0);
    return sum;
}

double Heston::price_call_impl() const {
    const double P1 = 0.5 + (1.0 / M_PI) * integrate(1);
    const double P2 = 0.5 + (1.0 / M_PI) * integrate(2);
    return option.spot * exp(-option.dividend * option.maturity) * P1 -
           option.strike * exp(-option.free_risk_rate * option.maturity) * P2;
}

double Heston::price_put_impl() const {
    const double P1 = 0.5 + (1.0 / M_PI) * integrate(1);
    const double P2 = 0.5 + (1.0 / M_PI) * integrate(2);
    return option.strike * exp(-option.free_risk_rate * option.maturity) * (1.0 - P2) -
           option.spot * exp(-option.dividend * option.maturity) * (1.0 - P1);
}

HestonParams Heston::get_params() const {
    return {kappa, theta, vol_vol, rho, v0};
}
