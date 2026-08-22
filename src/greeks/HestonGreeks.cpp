#include <HestonGreeks.hpp>

double HestonGreeks::delta() const {
    Option up = option, down = option;
    up.spot += EPSILON;
    down.spot -= EPSILON;
    double p_up =
        Heston(up, params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    double p_down =
        Heston(down, params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    return (p_up - p_down) / (2.0 * EPSILON);
}
double HestonGreeks::gamma() const {
    const double eps_gamma = 1.0;
    Option up = option, down = option;
    up.spot += eps_gamma;
    down.spot -= eps_gamma;
    double p_up =
        Heston(up, params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    double p_mid = pricer.price();
    double p_down =
        Heston(down, params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    return (p_up - 2.0 * p_mid + p_down) / (eps_gamma * eps_gamma);
}
double HestonGreeks::vega() const {
    double v_up = params.v0 + EPSILON, v_down = params.v0 - EPSILON;
    double p_up =
        Heston(option, params.kappa, params.theta, params.vol_vol, params.rho, v_up).price();
    double p_down =
        Heston(option, params.kappa, params.theta, params.vol_vol, params.rho, v_down).price();
    return (p_up - p_down) / (2.0 * EPSILON);
}
double HestonGreeks::theta() const {
    Option up = option, down = option;
    up.maturity += EPSILON;
    down.maturity -= EPSILON;
    double p_up =
        Heston(up, params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    double p_down =
        Heston(down, params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    return -(p_up - p_down) / (2.0 * EPSILON);
}
double HestonGreeks::rho() const {
    Option up = option, down = option;
    up.free_risk_rate += EPSILON;
    down.free_risk_rate -= EPSILON;
    double p_up =
        Heston(up, params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    double p_down =
        Heston(down, params.kappa, params.theta, params.vol_vol, params.rho, params.v0).price();
    return (p_up - p_down) / (2.0 * EPSILON);
}
