#ifndef BLACKSCHOLES_HPP
#define BLACKSCHOLES_HPP
#include <IOptionPricer.hpp>
#include <cmath>

// la vol est une carateristique de l'option pas de pricer
class BS : public IPricer<BS> {
protected:
    double implied_vol;

public:
    BS(Option option, double sigma) : IPricer<BS>(option), implied_vol(sigma) {
    }
    std::pair<double, double> d1_d2() const;
    double price_call_impl() const;
    double price_put_impl() const;
    double get_sigma() const;
};
#endif
