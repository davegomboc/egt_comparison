#ifndef EGT_COMPARISON_CUBE_BINARY_INPUTS_HPP
#define EGT_COMPARISON_CUBE_BINARY_INPUTS_HPP


#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
#include <ostream>
#include <string>
#include <vector>

#include "logic_digit_enum.hpp"
#include "bit_stuff.hpp"
#include "integer_logarithms.hpp"
#include "uint_type_selection.hpp"


template <std::size_t const required_bit_pair_count_v>
class cube_binary_inputs_tt {
public:
    static std::size_t constexpr required_bit_pair_count{
     required_bit_pair_count_v};
    static std::size_t constexpr required_bit_count{
     2 * required_bit_pair_count};
    static std::size_t constexpr index_type_required_bit_count{
     ceil_log2(required_bit_count)};

    using value_type = selected_uint_tt<required_bit_count>;
    using index_type = selected_uint_tt<index_type_required_bit_count>;

    static constexpr std::size_t value_type_bit_count{
     CHAR_BIT * sizeof(value_type)};

    static constexpr value_type stripes{
     fill_with_uint8<value_type>(UINT8_C(0x55))};
    static constexpr value_type allbits{
     fill_with_uint8<value_type>(UINT8_C(0xff))};

    cube_binary_inputs_tt & operator=(cube_binary_inputs_tt const &) = default;
    cube_binary_inputs_tt & operator=(cube_binary_inputs_tt &&) = default;
    cube_binary_inputs_tt(cube_binary_inputs_tt const & x) = default;
    cube_binary_inputs_tt(cube_binary_inputs_tt && x) = default;
    ~cube_binary_inputs_tt() = default;

    cube_binary_inputs_tt() : the_value{} {}

    explicit cube_binary_inputs_tt(value_type const & value)
      : the_value(value) {}

    cube_binary_inputs_tt(
        std::array<logic_digit_enum, required_bit_pair_count> const &
         logic_digits,
        bool const least_significant_bit_pair_first
    ) : the_value{} {
        index_type bit_pair_index{};
        while (bit_pair_index < required_bit_pair_count) {
            logic_digit_enum const bit_pair{least_significant_bit_pair_first
             ? logic_digits[bit_pair_index]
             : logic_digits[required_bit_pair_count - 1 - bit_pair_index]};
            set(bit_pair_index, bit_pair);
            ++bit_pair_index;
        }
    }
    cube_binary_inputs_tt(
        logic_digit_enum const bit_pair,
        std::size_t const repeat_count
    ) {
        assert(repeat_count <= required_bit_pair_count);
        unsigned char const two_bit_pattern{as_uchar(bit_pair)};
        std::uint8_t const fill_octet(two_bit_pattern
         | static_cast<std::uint8_t>(two_bit_pattern << 2)
         | static_cast<std::uint8_t>(two_bit_pattern << 4)
         | static_cast<std::uint8_t>(two_bit_pattern << 6));
        the_value = fill_with_uint8<value_type>(fill_octet);
        value_type const mask{allbits
         << (value_type_bit_count - 2 * repeat_count)
         >> (value_type_bit_count - 2 * repeat_count)};
        the_value &= mask;
    }

    explicit operator value_type() const {
        return the_value;
    }

    cube_binary_inputs_tt & operator=(value_type const value) {
        the_value = value;
        return *this;
    }

    logic_digit_enum operator[](index_type const i) const {
        return static_cast<logic_digit_enum>((the_value >> (2 * i)) & 0b11);
    }

    void set(index_type const i, logic_digit_enum const bit_pair) {
        the_value &= static_cast<value_type>(
         ~(static_cast<value_type>(0b11) << (2 * i)));
        the_value |= static_cast<value_type>(
         (static_cast<value_type>(bit_pair) << (2 * i)));
    }

    bool contained_by(
        cube_binary_inputs_tt<required_bit_pair_count> const & x
    ) const {
        return 0 == (~x.the_value & the_value);
    }
    bool contained_by_using_mask(
        cube_binary_inputs_tt<required_bit_pair_count> const & x,
        value_type const & mask = fill_with_uint8<value_type>(UINT8_C(0xff))
    ) const {
        return 0 == (mask & (~x.the_value & the_value));
    }

    bool contains(
        cube_binary_inputs_tt<required_bit_pair_count> const & x
    ) const {
        return x.contained_by(*this);
    }
    bool contains_using_mask(
        cube_binary_inputs_tt<required_bit_pair_count> const & x,
        value_type const & mask = fill_with_uint8<value_type>(UINT8_C(0xff))
    ) const {
        return x.contained_by_using_mask(*this, mask);
    }

    bool overlaps(
        cube_binary_inputs_tt<required_bit_pair_count> const & x
    ) const {
        value_type b{the_value ^ x.the_value};
        return 0 == (just_even_bits(b) & just_odd_bits(b));
    }
    bool overlaps_using_mask(
        cube_binary_inputs_tt<required_bit_pair_count> const & x,
        value_type const & mask = fill_with_uint8<value_type>(UINT8_C(0xff))
    ) const {
        value_type b{mask & (the_value ^ x.the_value)};
        return 0 == (just_even_bits(b) & just_odd_bits(b));
    }

    std::vector<cube_binary_inputs_tt<required_bit_pair_count>>
    complement() const {
        std::vector<cube_binary_inputs_tt<required_bit_pair_count>> result;
        auto const even{just_even_bits(the_value)};
        auto const odd{just_odd_bits(the_value)};
        using half_type = decltype(even); // &*$!# integral promotion
        auto const fixed_bits{static_cast<half_type>(
         ~static_cast<half_type>(even & odd))};
        for (auto const b: setbits(fixed_bits)) {
            result.emplace_back(cube_binary_inputs_tt<required_bit_pair_count>(
             static_cast<value_type>(interleave(
             static_cast<half_type>(static_cast<half_type>(~b) | odd),
             static_cast<half_type>(static_cast<half_type>(~b) | even)))));
        }
        return result;
    }
    std::vector<cube_binary_inputs_tt<required_bit_pair_count>>
    complement_using_mask(value_type const & mask) const {
        std::vector<cube_binary_inputs_tt<required_bit_pair_count>> result;
        auto const even{just_even_bits(the_value | ~mask)};
        auto const odd{just_odd_bits(the_value | ~mask)};
        using half_type = decltype(even); // &*$!# integral promotion
        auto const fixed_bits{
         static_cast<half_type>(~static_cast<half_type>(even & odd))};
        for (auto const b: setbits(fixed_bits)) {
            result.emplace_back(cube_binary_inputs_tt<required_bit_pair_count>(
             static_cast<value_type>(interleave(
              static_cast<half_type>(static_cast<half_type>(~b) | odd),
              static_cast<half_type>(static_cast<half_type>(~b) | even)))
             & mask));
        }
        return result;
    }

    cube_binary_inputs_tt<required_bit_pair_count> expand(
        cube_binary_inputs_tt<required_bit_pair_count> const & x
    ) const {
        return cube_binary_inputs_tt<required_bit_pair_count>{
         x.the_value | the_value};
    }
    cube_binary_inputs_tt<required_bit_pair_count> & expand_in_place(
        cube_binary_inputs_tt<required_bit_pair_count> const & x
    ) {
        the_value |= x.the_value;
        return *this;
    }

    template <typename pseudorandom_number_generator_type>
    std::vector<index_type> generate_raise_attempt_ordering(
        pseudorandom_number_generator_type & generator
    ) const {
        std::array<index_type, required_bit_pair_count> indices;
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), generator);
        std::vector<index_type> result;
        for (index_type const index: indices) {
            logic_digit_enum const bit_pair{(*this)[index]};
            if ((lde_off == bit_pair) or (lde_on == bit_pair)) {
                result.push_back(index);
            }
        }
        return result;
    }

    void raise(index_type const index) {
        assert((lde_off == (*this)[index]) or (lde_on == (*this)[index]));
        set(index, lde_dc);
    }

private:
    value_type the_value;
};

template <std::size_t const required_bit_pair_count>
bool operator==(
    cube_binary_inputs_tt<required_bit_pair_count> const & lhs,
    cube_binary_inputs_tt<required_bit_pair_count> const & rhs
) {
    using value_type
     = typename cube_binary_inputs_tt<required_bit_pair_count>::value_type;
    return static_cast<value_type>(lhs) == static_cast<value_type>(rhs);
}

template <std::size_t const required_bit_pair_count>
bool operator!=(
    cube_binary_inputs_tt<required_bit_pair_count> const & lhs,
    cube_binary_inputs_tt<required_bit_pair_count> const & rhs
) {
    return not (lhs == rhs);
}

template <std::size_t const required_bit_pair_count>
bool operator<(
    cube_binary_inputs_tt<required_bit_pair_count> const & lhs,
    cube_binary_inputs_tt<required_bit_pair_count> const & rhs
) {
    using value_type
     = typename cube_binary_inputs_tt<required_bit_pair_count>::value_type;
    return static_cast<value_type>(lhs) < static_cast<value_type>(rhs);
}

template <std::size_t const required_bit_pair_count>
bool operator>(
    cube_binary_inputs_tt<required_bit_pair_count> const & lhs,
    cube_binary_inputs_tt<required_bit_pair_count> const & rhs
) {
    return rhs < lhs;
}

template <std::size_t const required_bit_pair_count>
bool operator<=(
    cube_binary_inputs_tt<required_bit_pair_count> const & lhs,
    cube_binary_inputs_tt<required_bit_pair_count> const & rhs
) {
    return not (lhs > rhs);
}

template <std::size_t const required_bit_pair_count>
bool operator>=(
    cube_binary_inputs_tt<required_bit_pair_count> const & lhs,
    cube_binary_inputs_tt<required_bit_pair_count> const & rhs
) {
    return not (lhs < rhs);
}

// The least significant bit pair is emitted on the left.
template <std::size_t const required_bit_pair_count>
std::ostream & operator<<(
    std::ostream & out,
    cube_binary_inputs_tt<required_bit_pair_count> const & binary_inputs
) {
    using index_type
     = typename cube_binary_inputs_tt<required_bit_pair_count>::index_type;
    for (index_type nth_bit_pair{}; nth_bit_pair != required_bit_pair_count;
     ++nth_bit_pair) {
        out.put(as_char(binary_inputs[nth_bit_pair]));
        if (0 == nth_bit_pair % 6) {
            if (required_bit_pair_count != 1 + nth_bit_pair) {
                out.put(' ');
            }
        }
    }
    return out;
}


#endif // EGT_COMPARISON_CUBE_BINARY_INPUTS_HPP
