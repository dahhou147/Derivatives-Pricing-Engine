#include <BlackScholes.hpp>
#include <gtest/gtest.h>

class BSTEST : public ::testing::Test {
protected:
    BS* pricer_call;
    BS* pricer_put;
     // est un nullptr qui sera apres instancié pour chaque test dans le SetUp();

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
    }
};

TEST_F(BSTEST, TestGetSigma) {
    double sigma = 0.24;
    EXPECT_EQ(pricer_call->GetSigma(), sigma);
}

TEST_F(BSTEST, TestGetOption) {
    Option opt_call = {100.0, 102.0, 0.0, 0.02, 0.0, OptionType::Call};
    EXPECT_EQ(pricer_call->get_option(), opt_call);
}

TEST_F(BSTEST, TestD1D2) {
    auto d1_d2 = pricer_call->d1_d2();
    EXPECT_GT(d1_d2.first, 0.0);
    EXPECT_EQ(d1_d2.first-d1_d2.second, pricer_call->GetSigma()* std::sqrt(1));
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

TEST(BS, ZeroMaturityOptionHasIntrinsicValue) {
    Option opt_call = {100.0, 105.0, 0.0, 0.0, 0.0, OptionType::Call};
    Option opt_put = {100.0, 105.0, 0.0, 0.0, 0.0, OptionType::Put};
    BS call(opt_call, 0.25);
    BS put(opt_put, 0.25);

    EXPECT_DOUBLE_EQ(call.price(), 0.0);
    EXPECT_DOUBLE_EQ(put.price(), 5.0);
}