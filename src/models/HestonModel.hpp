#ifndef HESTONMODEL_HPP
#define HESTONMODEL_HPP
#include <IOptionPricer.hpp>
#include <complex>

class Heston : public IPricer<Heston> {
private:
    double kappa;
    double theta;
    double vol_vol;
    double rho;
    double v0;

public:
    Heston(Option options, double kappa, double theta, double vol_vol, double rho, double v0);
    std::complex<double> characteristic_function(std::complex<double> u) const;
    double integrate(int j) const;
    double price_call_impl() const;
    double price_put_impl() const;
    HestonParams get_params() const;
};
#endif
