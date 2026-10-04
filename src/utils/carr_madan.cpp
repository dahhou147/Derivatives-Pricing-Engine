#include <cmath>
#include <iostream>
#include <types.hpp>
#include <vector>

class CarrMadan {
private:
    HestonParams params;
    Option option;
    double alpha;
    int N;
    double eta;
    double lambda;

public:
    CarrMadan(const Option& option_, const HestonParams& params_, double alpha_, int N_,
              double eta_) : option(option_), params(params_), alpha(alpha_), N(N_), eta(eta_) {
        lambda = (2.0 * M_PI) / (N * eta);
    }
};