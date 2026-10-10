#include <concepts>
#include <iostream>


template< typename T> concept BasicArithmetic = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
    { a - b } -> std::same_as<T>;
    { a * b } -> std::same_as<T>;
    { a / b } -> std::same_as<T>;
};

template <typename T> concept Numbre = requires(T a){
    {T.size()} -> std::convertible_to<size_t>;
} && BasicArithmetic<T>;