#ifndef TYPES_HPP
#define TYPES_HPP

#include <stdexcept>
#include <vector>

enum class OptionType { Call, Put };

struct ImpliedVol {
    double spot;
    double strike;
    double market_price;
    double maturity;
    double implied_vol;
};

struct MarketOptionData {
    double spot;
    double strike;
    double maturity;
    double price;
    double free_risk_rate;
    double dividend;
    OptionType type;
};

struct Option {
    double spot;
    double strike;
    double maturity;
    double free_risk_rate;
    double dividend;
    OptionType type;

    bool operator==(const Option& other) const {
        return spot == other.spot && strike == other.strike && maturity == other.maturity &&
               free_risk_rate == other.free_risk_rate && dividend == other.dividend &&
               type == other.type;
    }
};

struct HestonParams {
    double kappa;
    double theta;
    double vol_vol;
    double rho;
    double v0;

    bool feller_condition() const {
        return 2.0 * kappa * theta > vol_vol * vol_vol;
    }

    bool is_valid() const {
        return kappa > 0.0 && theta > 0.0 && vol_vol > 0.0 && rho >= -1.0 && rho <= 1.0 && v0 > 0.0;
    }
};
#endif
