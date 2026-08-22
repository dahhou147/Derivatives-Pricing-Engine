#ifndef TYPES_HPP
#define TYPES_HPP

#include <stdexcept>
#include <vector>

enum class OptionType { Call, Put };

struct Option {
    double Spot;
    double strike;
    double maturity;
    double free_risk_interest;
    double devidende;
    OptionType type;
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
        return kappa > 0.0 && theta > 0.0 && vol_vol > 0.0 &&
               rho >= -1.0 && rho <= 1.0 && v0 > 0.0;
    }
};

struct MarketData {
    double spot;
    // this depends on the maturity ESTER as free risk interest rate
    //  
    double dividend;
    std::vector<double> maturities;
    std::vector<double> strikes;
    std::vector<double> market_prices;
};

struct MarketQuote{
    double strike;
    double maturity;
    double price;
    bool isCall;
};

#endif
