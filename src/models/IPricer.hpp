#ifndef IPRICER_HPP
#define IPRICER_HPP

#include <types.hpp>
#include <stdexcept>

template <typename Derived> class IPricer {
protected:
    Option option;
public:
    IPricer(Option o) : option(o) {}
    Option get_option() const { return option; }
    double price() const {
        switch (option.type) {
        case OptionType::Call:
            return static_cast<const Derived*>(this)->price_call_impl();
        case OptionType::Put:
            return static_cast<const Derived*>(this)->price_put_impl();
        default:
            throw std::invalid_argument("Unknown option type");
        }
    }
};

#endif
