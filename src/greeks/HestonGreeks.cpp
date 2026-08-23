#include <HestonGreeks.hpp>

double HestonGreeks::delta() const {
    Option up = option, down = option;
    up.spot += EPSILON;
    down.spot -= EPSILON;
    double p_up = Heston(up, params).price();
    double p_down = Heston(down, params).price();
    return (p_up - p_down) / (2.0 * EPSILON);
}
double HestonGreeks::gamma() const {
    const double eps_gamma = 1.0;
    Option up = option, down = option;
    up.spot += eps_gamma;
    down.spot -= eps_gamma;
    double p_up = Heston(up, params).price();
    double p_mid = pricer.price();
    double p_down = Heston(down, params).price();
    return (p_up - 2.0 * p_mid + p_down) / (eps_gamma * eps_gamma);
}
double HestonGreeks::vega() const {
    double v_up = params.v0 + EPSILON, v_down = params.v0 - EPSILON;
    HestonParams p_up_params = params;
    p_up_params.v0 = v_up;
    HestonParams p_down_params = params;
    p_down_params.v0 = v_down;
    double p_up = Heston(option, p_up_params).price();
    double p_down = Heston(option, p_down_params).price();
    return (p_up - p_down) / (2.0 * EPSILON);
}
double HestonGreeks::theta() const {
    Option up = option, down = option;
    up.maturity += EPSILON;
    down.maturity -= EPSILON;
    double p_up = Heston(up, params).price();
    double p_down = Heston(down, params).price();
    return -(p_up - p_down) / (2.0 * EPSILON);
}
double HestonGreeks::rho() const {
    Option up = option, down = option;
    up.free_risk_rate += EPSILON;
    down.free_risk_rate -= EPSILON;
    double p_up = Heston(up, params).price();
    double p_down = Heston(down, params).price();
    return (p_up - p_down) / (2.0 * EPSILON);
}
