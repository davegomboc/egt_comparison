#if !defined(EGT_COMPARISON_BIT_ARRAY_DISPLAY_HPP)
#define EGT_COMPARISON_BIT_ARRAY_DISPLAY_HPP


#include <array>
#include <cassert>
#include <cstdint>
#include <ostream>
#include <string_view>


#include "bit_indexing_and_sizing.hpp"


enum class bit_grouping_enum : unsigned int {
    normal = 0,
    highlight = 1,
    padding = 2,
};


inline std::array<std::string_view const, 3> constexpr grouping_colours{
 "\x1b[37;40m", "\x1b[43;30m", "\x1b[40;31m"};
inline std::array<std::string_view, 3> constexpr chars{"01", "01", "_!"};
std::string_view constexpr chars_for_bit_pairs{"?01-"};

template <typename uint_type>
void debug_print_bits(
    std::ostream & out,
    uint_type const data_ptr[],
    std::size_t const start_bit,
    std::size_t const past_end_bit,
    bit_grouping_enum const grouping
) {
    assert(start_bit <= past_end_bit);
    if (start_bit == past_end_bit) return;
    if (nullptr == data_ptr) {
        out << "nullptr";
        return;
    }

    if (0 == (start_bit & 0b111)) {
        out << ' ';
    }
    out << grouping_colours[static_cast<unsigned int>(grouping)];
    assert(start_bit <= past_end_bit);
    for (std::size_t bit_n{start_bit}; bit_n != past_end_bit; ++bit_n) {
        if ((start_bit != bit_n) and (past_end_bit != bit_n)
         and (0 == (bit_n & 0b111))) {
            out << ' ';
        }
        bool const value{0 != (data_ptr[which<uint_type>(bit_n)]
         & one_hot_of<uint_type>(bit_n))};
        out << chars[static_cast<unsigned int>(grouping)][value ? 1 : 0];
    }
    if (bit_grouping_enum::normal != grouping) {
        out << grouping_colours[static_cast<unsigned int>(
         bit_grouping_enum::normal)];
    }
}


template <typename uint_type>
void debug_print_bit_pairs(
    std::ostream & out,
    uint_type const data_ptr[],
    std::size_t const start_bit,
    std::size_t const past_end_bit,
    bit_grouping_enum const grouping
) {
    assert(start_bit <= past_end_bit);
    assert(0 == (start_bit & 1));
    assert(0 == (past_end_bit & 1));
    if (start_bit == past_end_bit) return;
    if (nullptr == data_ptr) {
        out << "nullptr";
        return;
    }

    if (2 == (start_bit % 12)) {
        out << ' ';
    }
    out << grouping_colours[static_cast<unsigned int>(grouping)];
    assert(start_bit <= past_end_bit);
    for (std::size_t bit_n{start_bit}; bit_n != past_end_bit; bit_n += 2) {
        if ((start_bit != bit_n) and (past_end_bit != bit_n)
         and (2 == (bit_n % 12))) {
            out << ' ';
        }
        bool const low_bit_value{0 != (data_ptr[which<uint_type>(bit_n)]
         & one_hot_of<uint_type>(bit_n))};
        bool const high_bit_value{0 != (data_ptr[which<uint_type>(1 + bit_n)]
         & one_hot_of<uint_type>(1 + bit_n))};
        unsigned long bit_pair_value{
         (high_bit_value ? 2u : 0u) + (low_bit_value ? 1u : 0u)};
        out << chars_for_bit_pairs[bit_pair_value];
    }
    if (bit_grouping_enum::normal != grouping) {
        out << grouping_colours[static_cast<unsigned int>(
         bit_grouping_enum::normal)];
    }
}


#endif // EGT_COMPARISON_BIT_ARRAY_DISPLAY_HPP
