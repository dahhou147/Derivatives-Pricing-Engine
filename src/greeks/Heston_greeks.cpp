#include <Heston_greeks.hpp>

double greeks_Heston::delta() const {
    Option up = opt, down = opt;
    up.Spot += EPSILON; down.Spot -= EPSILON;
    double p_up   = Heston(up,   params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    double p_down = Heston(down, params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    return (p_up - p_down) / (2.0 * EPSILON);
}
double greeks_Heston::gamma() const {
    const double eps_gamma = 1.0;
    Option up = opt, down = opt;
    up.Spot += eps_gamma; down.Spot -= eps_gamma;
    double p_up   = Heston(up,   params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    double p_mid  = pricer.price();
    double p_down = Heston(down, params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    return (p_up - 2.0 * p_mid + p_down) / (eps_gamma * eps_gamma);
}
double greeks_Heston::vega() const {
    double v_up = params.v0 + EPSILON, v_down = params.v0 - EPSILON;
    double p_up   = Heston(opt, params.kappa, params.theta, params.vol_vol, params.rho, v_up).price();
    double p_down = Heston(opt, params.kappa, params.theta, params.vol_vol, params.rho, v_down).price();
    return (p_up - p_down) / (2.0 * EPSILON);
}
double greeks_Heston::theta() const {
    Option up = opt, down = opt;
    up.maturity += EPSILON; down.maturity -= EPSILON;
    double p_up   = Heston(up,   params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    double p_down = Heston(down, params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    return -(p_up - p_down) / (2.0 * EPSILON);
}
double greeks_Heston::rho() const {
    Option up = opt, down = opt;
    up.free_risk_interest += EPSILON; down.free_risk_interest -= EPSILON;
    double p_up   = Heston(up,   params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    double p_down = Heston(down, params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    return (p_up - p_down) / (2.0 * EPSILON);
}
