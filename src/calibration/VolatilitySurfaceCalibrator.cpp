#include <BlackScholes.hpp>
#include <HestonModel.hpp>
#include <VolatilitySurfaceCalibrator.hpp>
#include <ceres/ceres.h>
#include <iostream>
#include <math.hpp>
#include <nlopt.hpp>
#include <types.hpp>

constexpr int max_iter = 5000;

void BSCalibrator::fit() {
    for (auto& option : market_data) {
        auto func = [&option](double sigma) {
            Option opt = {option.spot,           option.strike,   option.maturity,
                          option.free_risk_rate, option.dividend, option.type};
            BS black(opt, sigma);
            return black.price() - option.price;
        };

        constexpr double sigma_min = 1e-6;
        constexpr double sigma_max = 5.0;

        auto result = brent_solve(func, sigma_min, sigma_max);

        if (result.converged) {
            ImpliedVol iv = {option.spot, option.strike, option.price, option.maturity,
                             result.root};
            results.push_back(iv);
        } else {
            std::cout << "ERROR of calibration (strike=" << option.strike
                      << ", maturity=" << option.maturity << ")" << std::endl;
        }
    }
}

std::vector<ImpliedVol> BSCalibrator::get_ivs() const {
    return results;
}

/*
en fait on doit exprimer la vol en fonction des paramatres de de Heston;

* le modele de Heston se calibre sur tout les cotation de marché pour differente maturité et
differente strikes
*
*/

double readsmile(double price, Option option) {
    auto func = [&option, &price](double sigma) {
        BS black(option, sigma);
        double diff = black.price() - price;
        return diff;
    };
    constexpr double sigma_min = 1e-6;
    constexpr double sigma_max = 5.0;

    auto result = brent_solve(func, sigma_min, sigma_max);
    if (result.converged) {
        return result.root;
    } else {
        std::cout << "ERROR of reading the smile" << std::endl;
        return -1;
    }
}

struct HestonCostFunction {
private:
    std::vector<MarketOptionData> data;

public:
    HestonCostFunction(std::vector<MarketOptionData> & data_) : data(data_) {
    }

    bool operator()(const double* const params, double* residuals) const {
        double kappa = params[0];
        double theta = params[1];
        double vol_vol = params[2];
        double rho = params[3];
        double v0 = params[4];

        HestonParams hparams = {kappa, theta, vol_vol, rho, v0};
        size_t i = 0;
        for (auto& option : data) {
            Option opt = {option.spot,           option.strike,   option.maturity,
                          option.free_risk_rate, option.dividend, option.type};
            Heston heston(opt, hparams);
            double heston_price = heston.price();
            double iv_heston = readsmile(heston_price, opt);
            double iv_market = readsmile(option.price, opt);
            residuals[i] = iv_heston - iv_market;
            i++;
        }
        return true;
    }
};
struct FellerCondition {
    template <typename T> bool operator()(const T* const params, T* residuals) const {
        T kappa = params[0];
        T theta = params[1];
        T sigma = params[2];
        T constraint = T(2.0) * kappa * theta - sigma * sigma;
        residuals[0] = (constraint < T(0)) ? -constraint * T(100.0) : T(0.0);
        return true;
    }
};

HestonParams HestonCalibrator::GetParams() const {
    return params;
}
void HestonCalibrator::SetParams(const HestonParams& calibrated_params) {
    params = calibrated_params;
}

void HestonCalibrator::fit() {
    double hparams[5] = {1.5, 0.04, 0.3, -0.7, 0.04};
    ceres::Problem problem;

    ceres::CostFunction* cost_function =
        new ceres::NumericDiffCostFunction<HestonCostFunction, ceres::CENTRAL, ceres::DYNAMIC, 5>(
            new HestonCostFunction(market_data), ceres::TAKE_OWNERSHIP,
            static_cast<int>(market_data.size()));
    problem.AddResidualBlock(cost_function, nullptr, hparams);
    problem.SetParameterLowerBound(hparams, 0, 0.01);
    problem.SetParameterUpperBound(hparams, 0, 15.0);
    problem.SetParameterLowerBound(hparams, 1, 0.0001);
    problem.SetParameterUpperBound(hparams, 1, 2.0);
    problem.SetParameterLowerBound(hparams, 2, 0.01);
    problem.SetParameterUpperBound(hparams, 2, 2.5);
    problem.SetParameterLowerBound(hparams, 3, -0.95);
    problem.SetParameterUpperBound(hparams, 3, 0.95);
    problem.SetParameterLowerBound(hparams, 4, 0.0001);
    problem.SetParameterUpperBound(hparams, 4, 2.0);
    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;
    options.max_num_iterations = max_iter;
    options.function_tolerance = 1e-4;
    options.parameter_tolerance = 1e-4;
    options.minimizer_progress_to_stdout = true;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    if (summary.IsSolutionUsable()) {
        std::cout << "Optimization succeeded." << std::endl;
        HestonParams calibrated_params;

        calibrated_params.kappa = hparams[0];
        calibrated_params.theta = hparams[1];
        calibrated_params.vol_vol = hparams[2];
        calibrated_params.rho = hparams[3];
        calibrated_params.v0 = hparams[4];
        this->SetParams(calibrated_params);
    } else {
        std::cout << "Optimization failed: " << summary.FullReport() << std::endl;
    }
}