#include <BlackScholesGreeks.hpp>
#include <math.hpp>

BS BSGreeks::get_pricer() const {
    return pricer;
}

double BSGreeks::delta() const {
    switch (option.type) {
    case OptionType::Call:
        return exp(-option.dividend * option.maturity) * norm_cdf(d1);
    case OptionType::Put:
        return exp(-option.dividend * option.maturity) * (norm_cdf(d1) - 1.0);
    default:
        throw std::invalid_argument("Unknown option type");
    }
}
double BSGreeks::gamma() const {
    return exp(-option.dividend * option.maturity) * norm_density(d1) /
           (option.spot * pricer.get_sigma() * std::sqrt(option.maturity));
}
double BSGreeks::vega() const {
    return option.spot * exp(-option.dividend * option.maturity) * norm_density(d1) *
           std::sqrt(option.maturity);
}
double BSGreeks::theta() const {
    double common = -option.spot * exp(-option.dividend * option.maturity) * norm_density(d1) *
                    pricer.get_sigma() / (2.0 * std::sqrt(option.maturity));
    switch (option.type) {
    case OptionType::Call:
        return common +
               option.dividend * option.spot * exp(-option.dividend * option.maturity) *
                   norm_cdf(d1) -
               option.risk_free_rate * option.strike *
                   exp(-option.risk_free_rate * option.maturity) * norm_cdf(d2);
    case OptionType::Put:
        return common -
               option.dividend * option.spot * exp(-option.dividend * option.maturity) *
                   norm_cdf(-d1) +
               option.risk_free_rate * option.strike *
                   exp(-option.risk_free_rate * option.maturity) * norm_cdf(-d2);
    default:
        throw std::invalid_argument("Unknown option type");
    }
}
double BSGreeks::rho() const {
    switch (option.type) {
    case OptionType::Call:
        return option.strike * option.maturity * exp(-option.risk_free_rate * option.maturity) *
               norm_cdf(d2);
    case OptionType::Put:
        return -option.strike * option.maturity * exp(-option.risk_free_rate * option.maturity) *
               norm_cdf(-d2);
    default:
        throw std::invalid_argument("Unknown option type");
    }
}
// mfbm