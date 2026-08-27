#include <BlackScholes.hpp>
#include <BlackScholesGreeks.hpp>
#include <HestonGreeks.hpp>
#include <HestonModel.hpp>
#include <gtest/gtest.h>

class BSTEST : public ::testing::Test {
protected:
    BS* pricer_call;
    BS* pricer_put;

    void SetUp() override {
        Option opt_call = {100.0, 102.0, 1.0, 0.02, 0.0, OptionType::Call};
        Option opt_put = {100.0, 102.0, 1.0, 0.02, 0.0, OptionType::Put};

        double sigma = 0.24;
        pricer_call = new BS(opt_call, sigma);
        pricer_put = new BS(opt_put, sigma);
    }
    void TearDown() override {
        delete pricer_call;
        pricer_call = nullptr;
        delete pricer_put;
        pricer_put = nullptr;
    }
};

TEST_F(BSTEST, TestGetSigma) {
    double sigma = 0.24;
    EXPECT_EQ(pricer_call->GetSigma(), sigma);
}

TEST_F(BSTEST, TestGetOption) {
    Option opt_call = {100.0, 102.0, 1.0, 0.02, 0.0, OptionType::Call};
    EXPECT_EQ(pricer_call->get_option(), opt_call);
}

TEST_F(BSTEST, TestD1D2) {
    auto d1_d2 = pricer_call->d1_d2();
    EXPECT_GT(d1_d2.first, 0.0);
    EXPECT_EQ(d1_d2.first - d1_d2.second, pricer_call->GetSigma() * std::sqrt(1));
}

TEST(BS, BlackScholesClass) {
    Option opt_call = {100.0, 102.0, 0.0, 0.02, 0.0, OptionType::Call};
    Option opt_put = {100.0, 102.0, 0.0, 0.02, 0.0, OptionType::Put};
    double sigma = 0.18;
    BS black_call(opt_call, sigma);
    BS black_put(opt_put, sigma);

    EXPECT_DOUBLE_EQ(black_call.price(), 0.0);
    EXPECT_DOUBLE_EQ(black_put.price(), 2.0);
}

TEST_F(BSTEST, PriceForPositiveMaturity) {
    EXPECT_GT(pricer_call->price(), 0.0);
    EXPECT_LT(pricer_call->price(), pricer_call->get_option().spot);
}

TEST_F(BSTEST, PutCallParity) {
    double lhs = pricer_call->price() - pricer_put->price();
    Option opt_call = pricer_call->get_option();
    double rhs = opt_call.spot * std::exp(-opt_call.dividend * opt_call.maturity) -
                 opt_call.strike * std::exp(-opt_call.free_risk_rate * opt_call.maturity);
    EXPECT_NEAR(lhs, rhs, 1e-8);
}

TEST_F(BSTEST, CallPriceIncreasesWithSpot) {
    Option option_with_higher_spot = pricer_call->get_option();
    option_with_higher_spot.spot = 110.0;
    BS higher_spot(option_with_higher_spot, pricer_call->GetSigma());

    EXPECT_GT(higher_spot.price(), pricer_call->price());
}

TEST_F(BSTEST, PutPriceDecreasesWithSpot) {
    Option option_with_higher_spot = pricer_put->get_option();
    option_with_higher_spot.spot = 110.0;
    BS higher_spot(option_with_higher_spot, pricer_put->GetSigma());

    EXPECT_LT(higher_spot.price(), pricer_put->price());
}

TEST_F(BSTEST, PriceIncreasesWithVolatility) {
    Option opt = pricer_call->get_option();
    BS low_vol(opt, 0.10);
    BS high_vol(opt, 0.50);

    EXPECT_GT(high_vol.price(), low_vol.price());
}

TEST_F(BSTEST, LowerStrikeIncreasesCallPrice) {
    Option opt = pricer_call->get_option();
    opt.strike = 90.0;
    BS lower_strike(opt, pricer_call->GetSigma());

    EXPECT_GT(lower_strike.price(), pricer_call->price());
}

TEST_F(BSTEST, HigherStrikeIncreasesPutPrice) {
    Option opt = pricer_put->get_option();
    opt.strike = 120.0;
    BS higher_strike(opt, pricer_put->GetSigma());

    EXPECT_GT(higher_strike.price(), pricer_put->price());
}

TEST(BS, ZeroMaturityOptionHasIntrinsicValue) {
    Option opt_call = {100.0, 105.0, 0.0, 0.0, 0.0, OptionType::Call};
    Option opt_put = {100.0, 105.0, 0.0, 0.0, 0.0, OptionType::Put};
    BS call(opt_call, 0.25);
    BS put(opt_put, 0.25);

    EXPECT_DOUBLE_EQ(call.price(), 0.0);
    EXPECT_DOUBLE_EQ(put.price(), 5.0);
}

TEST(BS, GreeksHaveExpectedSigns) {
    Option opt_call = {100.0, 100.0, 1.0, 0.03, 0.0, OptionType::Call};
    BS bs(opt_call, 0.20);
    BSGreeks greeks(bs);

    EXPECT_GT(greeks.delta(), 0.0);
    EXPECT_GT(greeks.gamma(), 0.0);
    EXPECT_GT(greeks.vega(), 0.0);
    EXPECT_LT(greeks.theta(), 0.0);
    EXPECT_GT(greeks.rho(), 0.0);
}

TEST(Heston, PriceIsPositiveAndFinite) {
    Option opt = {100.0, 100.0, 1.0, 0.03, 0.0, OptionType::Call};
    HestonParams params = {2.5, 0.05, 0.4, -0.4, 0.04};
    Heston heston(opt, params);

    EXPECT_GT(heston.price(), 0.0);
    EXPECT_TRUE(std::isfinite(heston.price()));
}

TEST(Heston, PutCallParityHoldsApproximately) {
    Option opt_call = {100.0, 100.0, 1.0, 0.03, 0.0, OptionType::Call};
    Option opt_put = {100.0, 100.0, 1.0, 0.03, 0.0, OptionType::Put};
    HestonParams params = {2.5, 0.05, 0.4, -0.4, 0.04};
    Heston call(opt_call, params);
    Heston put(opt_put, params);

    double lhs = call.price() - put.price();
    double rhs = opt_call.spot * std::exp(-opt_call.dividend * opt_call.maturity) -
                 opt_call.strike * std::exp(-opt_call.free_risk_rate * opt_call.maturity);
    EXPECT_NEAR(lhs, rhs, 1e-4);
}

TEST(Heston, GreeksAreFinite) {
    Option opt = {100.0, 100.0, 1.0, 0.03, 0.0, OptionType::Call};
    HestonParams params = {2.5, 0.05, 0.4, -0.4, 0.04};
    HestonGreeks greeks(Heston(opt, params));

    EXPECT_TRUE(std::isfinite(greeks.delta()));
    EXPECT_TRUE(std::isfinite(greeks.gamma()));
    EXPECT_TRUE(std::isfinite(greeks.vega()));
    EXPECT_TRUE(std::isfinite(greeks.theta()));
    EXPECT_TRUE(std::isfinite(greeks.rho()));
}

TEST_F(BSTEST, VeryLowVolatilityProducesNearIntrinsicValue) {
    Option opt = pricer_call->get_option();
    BS low_vol(opt, 1e-6);
    EXPECT_GT(low_vol.price(), 0.0);
}

TEST_F(BSTEST, VeryHighVolatilityDoesNotProduceNegativePrice) {
    Option opt = pricer_call->get_option();
    BS high_vol(opt, 5.0);
    EXPECT_GT(high_vol.price(), 0.0);
}

TEST_F(BSTEST, CallPriceAtVeryLongMaturityStaysFinite) {
    Option opt = pricer_call->get_option();
    opt.maturity = 10.0;
    BS long_maturity(opt, pricer_call->GetSigma());
    EXPECT_TRUE(std::isfinite(long_maturity.price()));
}
