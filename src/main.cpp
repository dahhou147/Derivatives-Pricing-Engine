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

    Option opt = {spot, strike, maturity, free_risk_interest_rate, dividend, OptionType::Call};
    Heston heston(opt, kappa, theta, vol_vol, rho, v0);
    BS black(opt, implied_vol);

    auto price_BS = black.price();
    auto price_heston = heston.price();

    std::cout << "le prix de l'option selon Heston est : " << price_heston << std::endl;
    std::cout << "le prix de l'option selon black est : " << price_BS << std::endl;

    MarketOptionData market_quote = {spot, strike, maturity, 9.0, free_risk_interest_rate, 0.0, OptionType::Call};

    BSCalibrator calibrator({market_quote});
    calibrator.fit();

    auto ivs= calibrator.get_ivs();
    for(auto & iv:ivs){
        std::cout<< "la iv est  : "<<iv.implied_vol<<std::endl;
    }
    double iv  = 0.189305;
    BS black_test(opt, iv);
    double price_test = black_test.price();

    std::cout << "this norly should be :"<< price_test << std:: endl;

    return 0;
}