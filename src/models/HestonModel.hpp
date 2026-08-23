#ifndef HESTONMODEL_HPP
#define HESTONMODEL_HPP
#include <IOptionPricer.hpp>
#include <complex>
#include <types.hpp>

class Heston : public IPricer<Heston> {
private:

    HestonParams params;

public:
    Heston(Option options, HestonParams params_);
    // NOTE: use the constructor taking `HestonParams` to construct Heston.
    std::complex<double> characteristic_function(std::complex<double> u) const;
    double integrate(int j) const;
    double price_call_impl() const;
    double price_put_impl() const;
    HestonParams get_params() const;
};
#endif
