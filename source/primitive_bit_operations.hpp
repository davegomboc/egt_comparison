#if !defined(EGT_COMPARISON_PRIMITIVE_BIT_OPERATIONS_HPP)
#define EGT_COMPARISON_PRIMITIVE_BIT_OPERATIONS_HPP


#include <climits>
#include <limits>
#include <type_traits>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"


// TODO: replace with std::has_single_bit() when upgrading to C++20.
template <typename integral_type>
constexpr bool has_single_bit(integral_type const value) {
    return value and not (value & (value - 1));
}

// TODO: replace with std::countr_zero() when upgrading to C++20.
template <typename integral_type>
constexpr integral_type count_trailing_zeroes(unsigned char const value) {
    return value ? static_cast<integral_type>(__builtin_ctz(value))
     : static_cast<integral_type>(sizeof(unsigned int) * CHAR_BIT);
}

// TODO: replace with std::countr_zero() when upgrading to C++20.
template <typename integral_type>
constexpr integral_type count_trailing_zeroes(unsigned short const value) {
    return value ? static_cast<integral_type>(__builtin_ctz(value))
     : static_cast<integral_type>(sizeof(unsigned int) * CHAR_BIT);
}

// TODO: replace with std::countr_zero() when upgrading to C++20.
template <typename integral_type>
constexpr integral_type count_trailing_zeroes(unsigned int const value) {
    return value ? static_cast<integral_type>(__builtin_ctz(value))
     : static_cast<integral_type>(sizeof(unsigned int) * CHAR_BIT);
}

// TODO: replace with std::countr_zero() when upgrading to C++20.
template <typename integral_type>
constexpr integral_type count_trailing_zeroes(unsigned long const value) {
    return value ? static_cast<integral_type>(__builtin_ctzl(value))
     : static_cast<integral_type>(sizeof(unsigned long) * CHAR_BIT);
}

// TODO: replace with std::countr_zero() when upgrading to C++20.
template <typename integral_type>
constexpr integral_type count_trailing_zeroes(unsigned long long const value) {
    return value ? static_cast<integral_type>(__builtin_ctzll(value))
     : static_cast<integral_type>(sizeof(unsigned long long) * CHAR_BIT);
}

template <typename integral_type>
constexpr integral_type find_first_set_bit(unsigned char const value) {
    return static_cast<integral_type>(__builtin_ffs(value));
}

template <typename integral_type>
constexpr integral_type find_first_set_bit(unsigned short const value) {
    return static_cast<integral_type>(__builtin_ffs(value));
}

template <typename integral_type>
constexpr integral_type find_first_set_bit(unsigned int const value) {
    return static_cast<integral_type>(__builtin_ffs(value));
}

template <typename integral_type>
constexpr integral_type find_first_set_bit(unsigned long const value) {
    return static_cast<integral_type>(__builtin_ffsl(value));
}

template <typename integral_type>
constexpr integral_type find_first_set_bit(unsigned long long const value) {
    return static_cast<integral_type>(__builtin_ffsll(value));
}

// For when the integral promotion done by operator~ is undesirable.
template <typename integral_type>
constexpr integral_type invert(integral_type const value) {
    return static_cast<integral_type>(~value);
}

// TODO: replace with std::popcount() when upgrading to C++20.
template <typename integral_type>
constexpr integral_type popcount(unsigned char const value) {
    return __builtin_popcount(value);
}

// TODO: replace with std::popcount() when upgrading to C++20.
template <typename integral_type>
constexpr integral_type popcount(unsigned short const value) {
    return __builtin_popcount(value);
}

// TODO: replace with std::popcount() when upgrading to C++20.
template <typename integral_type>
constexpr integral_type popcount(unsigned int const value) {
    return __builtin_popcount(value);
}

// TODO: replace with std::popcount() when upgrading to C++20.
template <typename integral_type>
constexpr integral_type popcount(unsigned long const value) {
    return __builtin_popcountl(value);
}

// TODO: replace with std::popcount() when upgrading to C++20.
template <typename integral_type>
constexpr integral_type popcount(unsigned long long const value) {
    return __builtin_popcountll(value);
}

// TODO: replace with std::rotl() when upgrading to C++20.
template <typename integral_type>
constexpr integral_type rotate_left(
    integral_type const value,
    unsigned int bit_places_to_rotate_left
) {
    unsigned int const bit_count{CHAR_BIT * sizeof(integral_type)};
    unsigned int const mask{bit_count - 1};
    bit_places_to_rotate_left &= mask;
    return static_cast<integral_type>((value << bit_places_to_rotate_left)
     | (value >> (-bit_places_to_rotate_left & mask)));
}

// TODO: replace with std::rotr() when upgrading to C++20.
template <typename integral_type>
constexpr integral_type rotate_right(
    integral_type const value,
    unsigned int bit_places_to_rotate_right
) {
    unsigned int const bit_count{CHAR_BIT * sizeof(integral_type)};
    unsigned int const mask{bit_count - 1};
    bit_places_to_rotate_right &= mask;
    return static_cast<integral_type>((value >> bit_places_to_rotate_right)
     | (value << (-bit_places_to_rotate_right & mask)));
}

template <typename integral_type>
static integral_type constexpr all_bits_set = static_cast<integral_type const>(
 std::numeric_limits<std::make_unsigned_t<integral_type>>::max());

template <typename integral_type>
static integral_type constexpr no_bits_set = static_cast<integral_type const>(
 std::numeric_limits<std::make_unsigned_t<integral_type>>::min());

template <typename integral_type>
static integral_type constexpr even_bits_set = static_cast<integral_type const>(
 0x5555555555555555ull);

template <typename integral_type>
static integral_type constexpr odd_bits_set =
 invert<integral_type>(even_bits_set<integral_type>);

static_assert(popcount<unsigned long long>(even_bits_set<unsigned long long>)
 == popcount<unsigned long long>(odd_bits_set<unsigned long long>));


#pragma GCC diagnostic pop


#endif // EGT_COMPARISON_PRIMITIVE_BIT_OPERATIONS_HPP
