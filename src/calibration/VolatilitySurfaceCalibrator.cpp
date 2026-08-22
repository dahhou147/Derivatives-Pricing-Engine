#include <BlackScholes.hpp>
#include <VolatilitySurfaceCalibrator.hpp>
#include <iostream>
#include <nlopt.hpp>
#include <math.hpp>
#include <types.hpp>


void BSCalibrator::fit() {
    for (auto& option : market_data) {
        auto func = [&option](double sigma) {
            Option opt = {option.spot, option.strike, option.maturity,
                          option.free_risk_rate, option.dividend, option.type};
            BS black(opt, sigma);
            return black.price() - option.price;
        };

        constexpr double sigma_min = 1e-6;
        constexpr double sigma_max = 5.0;

        auto result = brent_solve(func, sigma_min, sigma_max);

        if (result.converged) {
            ImpliedVol iv = {option.spot, option.strike, option.price,
                              option.maturity, result.root};
            results.push_back(iv);
        } else {
            std::cout << "ERROR of calibration (strike=" << option.strike
                       << ", maturity=" << option.maturity << ")" << std::endl;
        }
    }
}

std::vector<ImpliedVol> BSCalibrator::get_ivs() const{
    return results;
}