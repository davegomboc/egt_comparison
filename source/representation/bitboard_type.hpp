#if !defined(EGT_COMPARISON_REPRESENTATION_BITBOARD_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_BITBOARD_TYPE_HPP


#include <bitset>
#include <cassert>
#include <iosfwd>
#include <string>
#include <vector>

#include "representation/coordinate_type.hpp"
#include "representation/logic_digits_type.hpp"


namespace representation {


class bitboard_type {
public:
    bitboard_type() {}
    explicit bitboard_type(coordinate_type coordinate) {
        the_bitboard.set(static_cast<unsigned char>(coordinate.as_enum()));
    }
    explicit bitboard_type(std::bitset<64> bitboard) : the_bitboard(bitboard) {}

    bool all() const { return the_bitboard.all(); }
    bool any() const { return the_bitboard.any(); }
    bool none() const { return the_bitboard.none(); }

    explicit operator bool() const { return any(); }

    std::string as_string() const;
    logic_digits_type as_logic_digits() const;
    std::uint64_t as_syzygy_bitboard_value() const;

    bitboard_type & operator|=(bitboard_type other) {
        the_bitboard |= other.the_bitboard;
        return *this;
    }
    bitboard_type & operator&=(bitboard_type other) {
        the_bitboard &= other.the_bitboard;
        return *this;
    }
    bitboard_type & operator^=(bitboard_type other) {
        the_bitboard ^= other.the_bitboard;
        return *this;
    }
    bitboard_type operator~() {
        bitboard_type result;
        result.the_bitboard = this->the_bitboard;
        result.the_bitboard.flip();
        return result;
    }

private:
    std::bitset<64> the_bitboard;
};


bitboard_type operator|(
    bitboard_type const lhs,
    bitboard_type const rhs);


bitboard_type operator&(
    bitboard_type const lhs,
    bitboard_type const rhs);


bitboard_type operator^(
    bitboard_type const lhs,
    bitboard_type const rhs);


std::ostream & operator<<(
    std::ostream & out,
    bitboard_type bitboard);


bool operator==(bitboard_type lhs, bitboard_type rhs);
bool operator!=(bitboard_type lhs, bitboard_type rhs);


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_BITBOARD_TYPE_HPP
