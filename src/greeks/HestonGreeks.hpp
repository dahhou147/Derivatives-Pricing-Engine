#ifndef HESTONGREEKS_HPP
#define HESTONGREEKS_HPP
#include <HestonModel.hpp>
constexpr double EPSILON = 1e-5;
class HestonGreeks {
private:
    Heston pricer;
    Option option;
    HestonParams params;

public:
    HestonGreeks(Heston pricer_) : pricer(pricer_) {
        params = pricer.get_params();
        option = pricer.get_option();
    }
    double delta() const;
    double gamma() const;
    double vega() const;
    double theta() const;
    double rho() const;
};
#endif
