#ifndef Heston_greeks_HPP
#define Heston_greeks_HPP
#include <Heston.hpp>
constexpr double EPSILON = 1e-5;
class greeks_Heston {
private:
    Heston pricer;
    Option opt;
    HestonParams params;
public:
    greeks_Heston(Heston pricer_) : pricer(pricer_) {
        params = pricer.get_params();
        opt = pricer.get_option();
    }
    double delta() const;
    double gamma() const;
    double vega() const;
    double theta() const;
    double rho() const;
};
#endif
