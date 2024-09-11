#ifndef EGT_COMPARISON_UINT_TYPE_SELECTION_HPP
#define EGT_COMPARISON_UINT_TYPE_SELECTION_HPP


#include <cstddef>
#include <cstdint>
#include <type_traits>


template <std::size_t, typename>
struct uint_type_selection_tt{};

template <std::size_t const required_bit_count>
struct uint_type_selection_tt<
    required_bit_count,
    std::enable_if_t<(required_bit_count > 64) and (required_bit_count <= 128)>
> {
    using type = __uint128_t;
};

// TODO: Consider whether it's actually even worth it to sometimes use less than
// 64 bits.
template <std::size_t const required_bit_count>
struct uint_type_selection_tt<
    required_bit_count,
    std::enable_if_t<(required_bit_count > 32) and (required_bit_count <= 64)>
> {
    using type = std::uint_least64_t;
};

template <std::size_t const required_bit_count>
struct uint_type_selection_tt<
    required_bit_count,
    std::enable_if_t<(required_bit_count > 16) and (required_bit_count <= 32)>
> {
    using type = std::uint_least32_t;
};

template <std::size_t const required_bit_count>
struct uint_type_selection_tt<
    required_bit_count,
    std::enable_if_t<(required_bit_count > 8) and (required_bit_count <= 16)>
> {
    using type = std::uint_least16_t;
};

template <std::size_t required_bit_count>
struct uint_type_selection_tt<
    required_bit_count,
    std::enable_if_t<(required_bit_count <= 8)>
> {
    using type = std::uint_least8_t;
};

template <std::size_t const required_bit_count>
using selected_uint_tt
 = typename uint_type_selection_tt<required_bit_count, void>::type;


#endif // EGT_COMPARISON_UINT_TYPE_SELECTION_HPP
