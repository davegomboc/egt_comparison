#ifndef EGT_COMPARISON_HASH_COMBINE_HPP
#define EGT_COMPARISON_HASH_COMBINE_HPP


#include <cstddef>
#include <functional>


// Based on https://open-std.org/jtc1/sc22/wg21/docs/papers/20217/p0814r0.pdf
template <typename T>
void hash_combine(std::size_t & seed, T const & value) {
    seed ^= std::hash<T>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}


#endif // EGT_COMPARISON_HASH_COMBINE_HPP
