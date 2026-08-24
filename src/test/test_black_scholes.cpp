#include <BlackScholes.hpp>
#include <gtest/gtest.h>

TEST(BS, BlackScholesClass) {
    Option opt_call = {100.0, 102.0, 0.0, 0.02, 0.0, OptionType::Call};
    Option opt_put = {100.0, 102.0, 0.0, 0.02, 0.0, OptionType::Put};
    double sigma = 0.18;
    BS black_call(opt_call, sigma);
    BS black_put(opt_put, sigma);

    EXPECT_DOUBLE_EQ(black_call.price(), 0.0);
    EXPECT_DOUBLE_EQ(black_put.price(), 2.0);
}