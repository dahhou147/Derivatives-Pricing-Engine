#include <BlackScholes.hpp>
#include <HestonModel.hpp>
#include <VolatilitySurfaceCalibrator.hpp>
#include <iostream>
#include <types.hpp>

int main() {

    double spot = 100.0;
    double strike = 100.0;
    double maturity = 1.0;
    double free_risk_interest_rate = 0.03;
    double dividend = 0.0;
    double kappa = 2.5;
    double theta = 0.05;
    double vol_vol = 0.4;
    double rho = -0.4;
    double v0 = 0.04;
    double implied_vol = std::sqrt(v0);

    Option base_opt = {spot, strike, maturity, free_risk_interest_rate, dividend, OptionType::Call};
    HestonParams true_params = {kappa, theta, vol_vol, rho, v0};
    Heston heston_base(base_opt, true_params);
    BS black(base_opt, implied_vol);

    std::cout << "le prix de l'option selon Heston est : " << heston_base.price() << std::endl;
    std::cout << "le prix de l'option selon black est : " << black.price() << std::endl;

    // Generate synthetic market data (grid of strikes and maturities)
    std::vector<MarketOptionData> market_quotes;
    std::vector<double> strikes = {80, 90, 100, 110, 120};
    std::vector<double> maturities = {0.25, 0.5, 1.0, 2.0};

    for (double T : maturities) {
        for (double K : strikes) {
            Option o = {spot, K, T, free_risk_interest_rate, dividend, OptionType::Call};
            Heston h(o, true_params);
            double model_price = h.price();
            // small deterministic perturbation to mimic market noise
            double noisy_price = model_price * (1.0 + 0.002 * ((K - spot) / spot));
            MarketOptionData q = {
                spot, K, T, noisy_price, free_risk_interest_rate, dividend, OptionType::Call};
            market_quotes.push_back(q);
        }
    }

    // Run Heston calibration on synthetic data
    HestonCalibrator hcal(market_quotes);
    hcal.fit();
    HestonParams calib = hcal.GetParams();

    std::cout << "True params: kappa=" << kappa << " theta=" << theta << " vol_vol=" << vol_vol
              << " rho=" << rho << " v0=" << v0 << std::endl;
    std::cout << "Calibrated params: kappa=" << calib.kappa << " theta=" << calib.theta
              << " vol_vol=" << calib.vol_vol << " rho=" << calib.rho << " v0=" << calib.v0
              << std::endl;

    return 0;
}