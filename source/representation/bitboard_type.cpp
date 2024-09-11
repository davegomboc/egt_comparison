#include "representation/bitboard_type.hpp"


#include <algorithm>
#include <cassert>
#include <ostream>

#include "fathom.hpp"


namespace representation {


std::string bitboard_type::as_string() const {
    std::string result{the_bitboard.to_string()};
    std::reverse(result.begin(), result.end());
    for (unsigned int index{56}; 0 != index; index -= 8) {
        result.insert(index, 1, '\'');
    }
    return result;
}


logic_digits_type bitboard_type::as_logic_digits() const {
    logic_digits_type result;
    result.reserve(64);
    for (std::size_t nth_bit{}; nth_bit != 64; ++nth_bit) {
        result.push_back(the_bitboard[nth_bit] ? on : off);
    }
    return result;
}


std::uint64_t bitboard_type::as_syzygy_bitboard_value() const {
    return __builtin_bswap64(the_bitboard.to_ulong());
}


bitboard_type operator|(
    bitboard_type const lhs,
    bitboard_type const rhs
) {
    bitboard_type result{lhs};
    result |= rhs;
    return result;
}


bitboard_type operator&(
    bitboard_type const lhs,
    bitboard_type const rhs
) {
    bitboard_type result{lhs};
    result &= rhs;
    return result;
}


bitboard_type operator^(
    bitboard_type const lhs,
    bitboard_type const rhs
) {
    bitboard_type result{lhs};
    result ^= rhs;
    return result;
}


std::ostream & operator<<(
    std::ostream & out,
    bitboard_type const bitboard
) {
    out << bitboard.as_string();
    return out;
}


bool operator==(bitboard_type const lhs, bitboard_type const rhs) {
    return (lhs ^ rhs).none();
}


bool operator!=(bitboard_type const lhs, bitboard_type const rhs) {
    return not (lhs == rhs);
}


} // namespace representation
