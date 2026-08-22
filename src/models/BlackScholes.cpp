#include <BlackScholes.hpp>
#include <cmath>
#include <math.hpp>

std::pair<double, double> BS::d1_d2() const {
    auto d1 = (std::log(option.spot / option.strike) +
               (option.risk_free_rate - option.dividend + 0.5 * std::pow(get_sigma(), 2.0)) *
                   option.maturity) /
              (get_sigma() * std::sqrt(option.maturity));
    auto d2 = d1 - get_sigma() * std::sqrt(option.maturity);
    return std::make_pair(d1, d2);
}
double BS::price_call_impl() const {
    auto [d1, d2] = d1_d2();
    return option.spot * exp(-option.dividend * option.maturity) * norm_cdf(d1) -
           option.strike * exp(-option.risk_free_rate * option.maturity) * norm_cdf(d2);
}
double BS::price_put_impl() const {
    auto [d1, d2] = d1_d2();
    return option.strike * exp(-option.risk_free_rate * option.maturity) * norm_cdf(-d2) -
           option.spot * exp(-option.dividend * option.maturity) * norm_cdf(-d1);
}
double BS::get_sigma() const {
    return implied_vol;
}
