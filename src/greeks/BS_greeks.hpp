#ifndef BS_greeks_HPP
#define BS_greeks_HPP
#include <BS.hpp>
class greeks_bs {
private:
    BS pricer;
    Option option;
    double d1, d2;
public:
    greeks_bs(BS pricer_) : pricer(pricer_), option(pricer_.get_option()) {
        auto [_d1, _d2] = pricer_.d1_d2();
        d1 = _d1; d2 = _d2;
    }
    BS get_pricer() const;
    double delta() const;
    double gamma() const;
    double vega() const;
    double theta() const;
    double rho() const;
};
#endif
