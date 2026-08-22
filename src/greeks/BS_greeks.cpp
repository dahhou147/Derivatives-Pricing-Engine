#include <BS_greeks.hpp>
#include <math.hpp>

BS greeks_bs::get_pricer() const {
    return pricer;
}

double greeks_bs::delta() const {
    switch (option.type) {
    case OptionType::Call:
        return exp(-option.devidende * option.maturity) * norm_cdf(d1);
    case OptionType::Put:
        return exp(-option.devidende * option.maturity) * (norm_cdf(d1) - 1.0);
    default:
        throw std::invalid_argument("Unknown option type");
    }
}
double greeks_bs::gamma() const {
    return exp(-option.devidende * option.maturity) * norm_density(d1) /
           (option.Spot * pricer.get_sigma() * std::sqrt(option.maturity));
}
double greeks_bs::vega() const {
    return option.Spot * exp(-option.devidende * option.maturity) * norm_density(d1) *
           std::sqrt(option.maturity);
}
double greeks_bs::theta() const {
    double common = -option.Spot * exp(-option.devidende * option.maturity) * norm_density(d1) *
                    pricer.get_sigma() / (2.0 * std::sqrt(option.maturity));
    switch (option.type) {
    case OptionType::Call:
        return common +
               option.devidende * option.Spot * exp(-option.devidende * option.maturity) *
                   norm_cdf(d1) -
               option.free_risk_interest * option.strike *
                   exp(-option.free_risk_interest * option.maturity) * norm_cdf(d2);
    case OptionType::Put:
        return common -
               option.devidende * option.Spot * exp(-option.devidende * option.maturity) *
                   norm_cdf(-d1) +
               option.free_risk_interest * option.strike *
                   exp(-option.free_risk_interest * option.maturity) * norm_cdf(-d2);
    default:
        throw std::invalid_argument("Unknown option type");
    }
}
double greeks_bs::rho() const {
    switch (option.type) {
    case OptionType::Call:
        return option.strike * option.maturity * exp(-option.free_risk_interest * option.maturity) *
               norm_cdf(d2);
    case OptionType::Put:
        return -option.strike * option.maturity *
               exp(-option.free_risk_interest * option.maturity) * norm_cdf(-d2);
    default:
        throw std::invalid_argument("Unknown option type");
    }
}
// mfbm