#if !defined(EGT_COMPARISON_HEX_DISPLAY_HPP)
#define EGT_COMPARISON_HEX_DISPLAY_HPP


#include <array>
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>
#include <string_view>


#include "bit_indexing_and_sizing.hpp"


template <typename uint_type>
void print_hex(
    std::ostream & out,
    uint_type const & value
) {
    std::ios_base::fmtflags const old_format_flags{out.flags()};
    out << "0x" << std::noshowbase << std::hex << std::internal
     << std::setfill('0') << std::setw(2 * sizeof(uint_type))
     << static_cast<std::uint64_t>(value);
    out.flags(old_format_flags);
}


template <typename uint_type>
void print_hex(
    std::ostream & out,
    uint_type const * const start_address,
    uint_type const * const past_end_address
) {
    assert(start_address <= past_end_address);
    if (nullptr == start_address) {
        out << "nullptr";
        return;
    }

    for (uint_type const * current_address{start_address};
     current_address < past_end_address; ++current_address) {
        if (current_address != start_address) {
            out << ' ';
        }
        print_hex(out, *current_address);
    }
}


#endif // EGT_COMPARISON_HEX_DISPLAY_HPP
