#ifndef BLACKSCHOLESGREEKS_HPP
#define BLACKSCHOLESGREEKS_HPP
#include <BlackScholes.hpp>
class BSGreeks {
private:
    BS pricer;
    Option option;
    double d1, d2;

public:
    BSGreeks(BS pricer_) : pricer(pricer_), option(pricer_.get_option()) {
        auto [_d1, _d2] = pricer_.d1_d2();
        d1 = _d1;
        d2 = _d2;
    }
    BS get_pricer() const;
    double delta() const;
    double gamma() const;
    double vega() const;
    double theta() const;
    double rho() const;
};
#endif
