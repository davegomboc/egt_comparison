#ifndef EGT_COMPARISON_BIT_STUFF_HPP
#define EGT_COMPARISON_BIT_STUFF_HPP


#include <bitset>
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <string>
#include <type_traits>

#include <immintrin.h>


// Based on https://stackoverflow.com/questions/45225925/ answer by "Justin"
template <
    typename uint_type,
    typename = std::enable_if<
        std::is_unsigned_v<uint_type> and std::is_integral_v<uint_type>
    >
> constexpr uint_type fill_with_uint8(std::uint8_t const fill_uint8) {
    uint_type result{};
    for (std::size_t i{}; i != sizeof(uint_type); ++i) {
        result |= static_cast<uint_type>(fill_uint8) << (8 * i);
    }
    return result;
}

constexpr __uint128_t fill_with_uint64(std::uint64_t const fill_uint64) {
    return (static_cast<__uint128_t>(fill_uint64) << 64) | fill_uint64;
}

template <
    typename uint_type,
    typename = std::enable_if<
        std::is_unsigned_v<uint_type> and std::is_integral_v<uint_type>
    >
> constexpr uint_type just_msbs_set(std::size_t const set_bit_count) {
    assert(set_bit_count <= CHAR_BIT * sizeof(uint_type));
    uint_type result{std::numeric_limits<uint_type>::max()};
    result >>= CHAR_BIT * sizeof(uint_type) - set_bit_count;
    result <<= CHAR_BIT * sizeof(uint_type) - set_bit_count;
    return result;
}

template <
    typename uint_type,
    typename = std::enable_if<
        std::is_unsigned_v<uint_type> and std::is_integral_v<uint_type>
    >
> constexpr uint_type just_lsbs_set(std::size_t const set_bit_count) {
    assert(set_bit_count <= CHAR_BIT * sizeof(uint_type));
    uint_type result{std::numeric_limits<uint_type>::max()};
    result <<= CHAR_BIT * sizeof(uint_type) - set_bit_count;
    result >>= CHAR_BIT * sizeof(uint_type) - set_bit_count;
    return result;
}

template <
    typename uint_type,
    typename = std::enable_if<
        std::is_unsigned_v<uint_type> and std::is_integral_v<uint_type>
    >
> constexpr uint_type first_permutation(std::size_t const set_bit_count) {
    return just_lsbs_set<uint_type>(set_bit_count);
}

template <
    typename uint_type,
    typename = std::enable_if<
        std::is_unsigned_v<uint_type> and std::is_integral_v<uint_type>
    >
> constexpr uint_type past_last_permutation(std::size_t const set_bit_count) {
    return first_permutation<uint_type>(set_bit_count) >> 1;
}

template <
    typename uint_type,
    typename = std::enable_if<
        std::is_unsigned_v<uint_type> and std::is_integral_v<uint_type>
    >
> constexpr uint_type next_permutation(
    uint_type const current_permutation,
    std::size_t const uint_used_bit_count
) {
    assert(uint_used_bit_count <= CHAR_BIT * sizeof(uint_type));
    // see
    // https://graphics.stanford.edu/~seander/bithacks.html#NextBitPermutation
    uint_type const & v{current_permutation};
    uint_type t{v | (v - 1)};
    // Is the final bitand actually necessary?  Could it be asserted instead?
    return ((t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(v) + 1)))
     & just_lsbs_set<uint_type>(uint_used_bit_count);
}

// The generated string will have the most significant bit on the left.
template <
    typename uint_type,
    typename = std::enable_if<
        std::is_unsigned_v<uint_type> and std::is_integral_v<uint_type>
    >
> std::string to_binary_string(
    uint_type const value,
    std::size_t const uint_used_bit_count = CHAR_BIT * sizeof(uint_type)
) {
    std::size_t offset{((uint_used_bit_count - 1) / 64) * 64};
    std::size_t msbs_to_skip{64 - (uint_used_bit_count - offset)};
    std::string result;
    while (offset) {
        result += (std::bitset<64>(static_cast<unsigned long long int>(
         value >> offset))).to_string().substr(msbs_to_skip);
        offset -= 64;
        msbs_to_skip = 0;
    }
    result += std::bitset<64>(static_cast<unsigned long long int>(value))
     .to_string().substr(msbs_to_skip);
    return result;
}

struct uint128hash {
    std::size_t operator()(__uint128_t const x) const {
        return std::hash<std::uint64_t>{}(
         static_cast<std::uint64_t>(x) ^ static_cast<std::uint64_t>(x >> 64));
    }
};

template <typename T>
struct bithasher {
    using bithash = std::hash<T>;
};

template <>
struct bithasher<__uint128_t> {
    using bithash = uint128hash;
};

template <typename T>
using bithash = typename bithasher<T>::bithash;

// "_cx" suffix refers to constexpr (to distingush from C++20 consteval)
constexpr std::uint64_t just_even_bits_cx(__uint128_t x) {
    x &= fill_with_uint64(UINT64_C(0x5555555555555555));
    x = (x | (x >>  1)) & fill_with_uint64(UINT64_C(0x3333333333333333));
    x = (x | (x >>  2)) & fill_with_uint64(UINT64_C(0x0f0f0f0f0f0f0f0f));
    x = (x | (x >>  4)) & fill_with_uint64(UINT64_C(0x00ff00ff00ff00ff));
    x = (x | (x >>  8)) & fill_with_uint64(UINT64_C(0x0000ffff0000ffff));
    x = (x | (x >> 16)) & fill_with_uint64(UINT64_C(0x00000000ffffffff));
    x = (x | (x >> 32)) & static_cast<__uint128_t>(
     UINT64_C(0xffffffffffffffff));
    return static_cast<std::uint64_t>(x);
}

constexpr std::uint32_t just_even_bits_cx(std::uint64_t x) {
    x &= UINT64_C(0x5555555555555555);
    x = (x | (x >>  1)) & UINT64_C(0x3333333333333333);
    x = (x | (x >>  2)) & UINT64_C(0x0f0f0f0f0f0f0f0f);
    x = (x | (x >>  4)) & UINT64_C(0x00ff00ff00ff00ff);
    x = (x | (x >>  8)) & UINT64_C(0x0000ffff0000ffff);
    x = (x | (x >> 16)) & UINT64_C(0x00000000ffffffff);
    return static_cast<std::uint32_t>(x);
}

constexpr std::uint16_t just_even_bits_cx(std::uint32_t x) {
    x &= UINT32_C(0x55555555);
    x = (x | (x >> 1)) & UINT32_C(0x33333333);
    x = (x | (x >> 2)) & UINT32_C(0x0f0f0f0f);
    x = (x | (x >> 4)) & UINT32_C(0x00ff00ff);
    x = (x | (x >> 8)) & UINT32_C(0x0000ffff);
    return static_cast<std::uint16_t>(x);
}

constexpr std::uint8_t just_even_bits_cx(std::uint16_t x) {
    x &= UINT16_C(0x5555);
    x = (x | (x >> 1)) & UINT16_C(0x3333);
    x = (x | (x >> 2)) & UINT16_C(0x0f0f);
    x = (x | (x >> 4)) & UINT16_C(0x00ff);
    return static_cast<std::uint8_t>(x);
}

constexpr std::uint8_t just_even_bits_cx(std::uint8_t x) {
    x &= UINT8_C(0x55);
    x = (x | (x >> 1)) & UINT8_C(0x33);
    x = (x | (x >> 2)) & UINT8_C(0x0f);
    return x;
}

template <
    typename uint_type,
    typename = std::enable_if<
        std::is_unsigned_v<uint_type> and std::is_integral_v<uint_type>
    >
> constexpr auto just_even_bits(uint_type const x) {
    return just_even_bits_cx(x);
}

// When available, these runtime-only function definitions will be better
// matches than the above function template.
#ifdef __BMI2__
inline std::uint64_t just_even_bits(__uint128_t const x) {
    return (_pext_u64(static_cast<std::uint64_t>(x >> 64),
     UINT64_C(0x5555555555555555)) << 32)
     | _pext_u64(static_cast<std::uint64_t>(x), UINT64_C(0x5555555555555555));
}

inline std::uint32_t just_even_bits(std::uint64_t const x) {
    return static_cast<std::uint32_t>(
     _pext_u64(x, UINT64_C(0x5555555555555555)));
}

inline std::uint16_t just_even_bits(std::uint32_t const x) {
    return static_cast<std::uint16_t>(_pext_u32(x, UINT32_C(0x55555555)));
}

inline std::uint8_t just_even_bits(std::uint16_t const x) {
    return static_cast<std::uint8_t>(_pext_u32(x, UINT16_C(0x5555)));
}
#endif

template <
    typename uint_type,
    typename = std::enable_if<
        std::is_unsigned_v<uint_type> and std::is_integral_v<uint_type>
    >
> constexpr auto just_odd_bits(uint_type const x) {
    return just_even_bits(static_cast<uint_type>(x >> 1));
}

// interleave is extended from https://lemire.me/blog/2018/01/08/

// "_cx" suffix refers to constexpr (to distingush from C++20 consteval)
constexpr __uint128_t interleave_cx(std::uint64_t const value) {
    __uint128_t x(value);
    x = (x ^ (x << 32)) & fill_with_uint64(UINT64_C(0x00000000ffffffff));
    x = (x ^ (x << 16)) & fill_with_uint64(UINT64_C(0x0000ffff0000ffff));
    x = (x ^ (x <<  8)) & fill_with_uint64(UINT64_C(0x00ff00ff00ff00ff));
    x = (x ^ (x <<  4)) & fill_with_uint64(UINT64_C(0x0f0f0f0f0f0f0f0f));
    x = (x ^ (x <<  2)) & fill_with_uint64(UINT64_C(0x3333333333333333));
    x = (x ^ (x <<  1)) & fill_with_uint64(UINT64_C(0x5555555555555555));
    return x;
}

constexpr std::uint64_t interleave_cx(std::uint32_t const value) {
    std::uint64_t x(value);
    x = (x ^ (x << 16)) & UINT64_C(0x0000ffff0000ffff);
    x = (x ^ (x <<  8)) & UINT64_C(0x00ff00ff00ff00ff);
    x = (x ^ (x <<  4)) & UINT64_C(0x0f0f0f0f0f0f0f0f);
    x = (x ^ (x <<  2)) & UINT64_C(0x3333333333333333);
    x = (x ^ (x <<  1)) & UINT64_C(0x5555555555555555);
    return x;
}

constexpr std::uint32_t interleave_cx(std::uint16_t const value) {
    std::uint32_t x(value);
    x = (x ^ (x << 8)) & UINT32_C(0x00ff00ff);
    x = (x ^ (x << 4)) & UINT32_C(0x0f0f0f0f);
    x = (x ^ (x << 2)) & UINT32_C(0x33333333);
    x = (x ^ (x << 1)) & UINT32_C(0x55555555);
    return x;
}

constexpr std::uint16_t interleave_cx(std::uint8_t const value) {
    std::uint16_t x(value);
    x = (x ^ (x << 4)) & UINT16_C(0x0f0f);
    x = (x ^ (x << 2)) & UINT16_C(0x3333);
    x = (x ^ (x << 1)) & UINT16_C(0x5555);
    return x;
}

template <
    typename uint_type,
    typename = std::enable_if<
        std::is_unsigned_v<uint_type> and std::is_integral_v<uint_type>
    >
> constexpr auto interleave(uint_type const even, uint_type const odd) {
    static_assert(not std::is_same_v<std::remove_cv_t<uint_type>, __uint128_t>);
    return interleave_cx(even) | (interleave_cx(odd) << 1);
}

// When available, these runtime-only function definitions will be better
// matches than the above function template.
#ifdef __BMI2__
inline std::uint64_t interleave(
    std::uint32_t const even,
    std::uint32_t const odd
) {
    return _pdep_u64(even, UINT64_C(0x5555555555555555))
     | _pdep_u64(odd, UINT64_C(0xaaaaaaaaaaaaaaaa));
}

inline std::uint32_t interleave(
    std::uint16_t const even,
    std::uint16_t const odd
) {
    return _pdep_u32(even, UINT32_C(0x55555555))
     | _pdep_u32(odd, UINT32_C(0xaaaaaaaa));
}

inline std::uint16_t interleave(
    std::uint8_t const even,
    std::uint8_t const odd
) {
    return static_cast<std::uint16_t>(
     _pdep_u32(even, UINT16_C(0x5555)) | _pdep_u32(odd, UINT16_C(0xaaaa)));
}
#endif

// TODO: Perhaps could be made constexpr?
struct temporary_hack_to_avoid_ctad_maybe_unsupported_diagnostic_t;
template <typename I>
class setbits {
    static_assert(
     ((static_cast<I>(0)) - 1) == (~(static_cast<I>(0))),
     "not two's complement");
    class iterator {
    public:
        // TODO: Consider improving these.
        using iterator_category = std::input_iterator_tag;
        using value_type = I;
        using difference_type = I;
        using pointer = I const *;
        using reference = I;

        explicit iterator(const I & i) : x(i) {}
        explicit iterator(I && i) : x(std::move(i)) {}

        // returns bits with only the next set bit set
        I operator*() const {
            return x & -x;
        }
        iterator & operator++() {
            x ^= (x & -x);
            return *this;
        }
        iterator operator++(int) {
            auto ret = *this;
            ++(*this);
            return ret;
        }
        bool operator==(const iterator & i) const {
            return x == i.x;
        }
        bool operator!=(const iterator & i) const {
            return !(*this == i);
        }

    private:
        I x;
    };

    I x;

public:
    setbits(I const & i) : x(i) {}
    setbits(I && i) : x(std::move(i)) {}
    iterator begin() const {
        return iterator(x);
    }
    iterator end() const {
        return iterator(0);
    }
};
setbits(temporary_hack_to_avoid_ctad_maybe_unsupported_diagnostic_t) -> setbits<void>;

// there are faster ways with special instructions,
// but we'll use this for now
template <typename I>
std::size_t nbitsset(I const & x) {
    std::size_t ret{};
    for ([[maybe_unused]] auto b : setbits<I>(x)) { ++ret; }
    return ret;
}


#endif // EGT_COMPARISON_BIT_STUFF_HPP
