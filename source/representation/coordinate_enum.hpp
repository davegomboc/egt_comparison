#if !defined(EGT_COMPARISON_REPRESENTATION_COORDINATE_ENUM_HPP)
#define EGT_COMPARISON_REPRESENTATION_COORDINATE_ENUM_HPP


#include <cassert>
#include <cstddef>
#include <iosfwd>
#include <string>
#include <type_traits>
#include <vector>

#include "representation/logic_digits_type.hpp"


namespace representation {


enum class file_enum: unsigned char {
    a_file = 0b000'000,
    b_file = 0b000'001,
    c_file = 0b000'010,
    d_file = 0b000'011,
    e_file = 0b000'100,
    f_file = 0b000'101,
    g_file = 0b000'110,
    h_file = 0b000'111,
    file_mask = 0b000'111,
};


bool next(file_enum & file);


bool prev(file_enum & file);


file_enum & operator++(file_enum & file);


file_enum & operator--(file_enum & file);


constexpr bool valid(file_enum const file) {
    unsigned char const value{static_cast<unsigned char>(file)};
    return (value & static_cast<unsigned char>(file_enum::file_mask))
     == value;
}


constexpr char as_char(file_enum const file) {
    assert(valid(file));
    return static_cast<char>(static_cast<unsigned char>('a')
     + static_cast<unsigned char>(file));
}


enum class rank_enum: unsigned char {
    rank_8 = 0b000'000,
    rank_7 = 0b001'000,
    rank_6 = 0b010'000,
    rank_5 = 0b011'000,
    rank_4 = 0b100'000,
    rank_3 = 0b101'000,
    rank_2 = 0b110'000,
    rank_1 = 0b111'000,
    rank_mask = 0b111'000,
};


bool next(rank_enum & file);


bool prev(rank_enum & file);


rank_enum & operator++(rank_enum & file);


rank_enum & operator--(rank_enum & file);


constexpr bool valid(rank_enum const rank) {
    unsigned char const value{static_cast<unsigned char>(rank)};
    return (value & static_cast<unsigned char>(rank_enum::rank_mask))
     == value;
}


constexpr char as_char(rank_enum const rank) {
    assert(valid(rank));
    return static_cast<char>(static_cast<unsigned char>('8')
     - (static_cast<unsigned char>(rank) >> 3));
}


enum class coordinate_enum : unsigned char {
    square_mask = 0x3f,
};


constexpr bool valid(coordinate_enum const coordinate) {
    unsigned char const value{static_cast<unsigned char>(coordinate)};
    return (value & static_cast<unsigned char>(coordinate_enum::square_mask))
     == value;
}


bool next(coordinate_enum & coordinate);


bool prev(coordinate_enum & coordinate);


coordinate_enum & operator++(coordinate_enum & coordinate);


coordinate_enum & operator--(coordinate_enum & coordinate);


constexpr coordinate_enum operator|(
    file_enum const file,
    rank_enum const rank
) {
    assert(valid(file));
    assert(valid(rank));
    return static_cast<coordinate_enum>(
     static_cast<unsigned char>(file) | static_cast<unsigned char>(rank));
}


constexpr file_enum just_file(coordinate_enum const coordinate) {
    return static_cast<file_enum>(
     static_cast<unsigned char>(coordinate) &
     static_cast<unsigned char>(file_enum::file_mask));
}


constexpr rank_enum just_rank(coordinate_enum const coordinate) {
    return static_cast<rank_enum>(
     static_cast<unsigned char>(coordinate) &
     static_cast<unsigned char>(rank_enum::rank_mask));
}


constexpr coordinate_enum generate(
    coordinate_enum const file,
    coordinate_enum const rank
) {
    assert(valid(just_file(file)));
    assert(valid(just_rank(rank)));
    return just_file(file) | just_rank(rank);
}


// Shifts all bits left by six, then sets bits 5-0 (rrr fff) to hold coordinate.
inline void append_coordinate(
    std::size_t & position_id,
    coordinate_enum coordinate
) {
    position_id <<= 6;
    position_id |= static_cast<unsigned char>(coordinate);
}

// The format is rrr fff.  (See the values of rank_enum and file_enum.)
template <typename uint_type>
void append_coordinate(
    std::vector<uint_type> & vector_id,
    coordinate_enum const coordinate
) {
    static_assert(std::is_integral_v<uint_type>,
     "Template argument must be integral.");
    static_assert(std::is_unsigned_v<uint_type>,
     "Template argument must be unsigned.");
    unsigned char const value{static_cast<unsigned char>(coordinate)};
    vector_id.push_back(0 != (0b100000 & value));
    vector_id.push_back(0 != (0b010000 & value));
    vector_id.push_back(0 != (0b001000 & value));
    vector_id.push_back(0 != (0b000100 & value));
    vector_id.push_back(0 != (0b000010 & value));
    vector_id.push_back(0 != (0b000001 & value));
}


// Extracts bits 5-0 (rrr fff) as coordinate, then shifts all bits right by six.
inline coordinate_enum remove_coordinate(std::size_t & position_id) {
    coordinate_enum const result{
     static_cast<unsigned char>(position_id & 0b111111)};
    position_id >>= 6;
    return result;
}


// The format is rrr fff.  (See the values of rank_enum and file_enum.)
template <typename uint_type>
coordinate_enum remove_coordinate(std::vector<uint_type> & vector_id) {
    static_assert(std::is_integral_v<uint_type>,
     "Template argument must be integral.");
    static_assert(std::is_unsigned_v<uint_type>,
     "Template argument must be unsigned.");
    std::size_t const size{vector_id.size()};
    assert(6 <= size);
    unsigned char value{};
    if (0 != vector_id[size - 1]) value |= 0b000001;
    if (0 != vector_id[size - 2]) value |= 0b000010;
    if (0 != vector_id[size - 3]) value |= 0b000100;
    if (0 != vector_id[size - 4]) value |= 0b001000;
    if (0 != vector_id[size - 5]) value |= 0b010000;
    if (0 != vector_id[size - 6]) value |= 0b100000;
    for (unsigned int i{}; i != 6; ++i) {
        vector_id.pop_back();
    }
    return static_cast<coordinate_enum>(value);
}


logic_digits_type as_logic_digits(coordinate_enum coordinate);


std::string as_string(coordinate_enum coordinate);


std::ostream & operator<<(
    std::ostream & out,
    coordinate_enum coordinate);


constexpr bool operator==(
    coordinate_enum const lhs,
    coordinate_enum const rhs
) {
    return static_cast<unsigned char>(lhs) == static_cast<unsigned char>(rhs);
}


constexpr bool operator!=(
    coordinate_enum const lhs,
    coordinate_enum const rhs
) {
    return not (lhs == rhs);
}


constexpr bool operator<(
    coordinate_enum const lhs,
    coordinate_enum const rhs
) {
    return static_cast<unsigned char>(lhs) < static_cast<unsigned char>(rhs);
}


constexpr bool operator>(
    coordinate_enum const lhs,
    coordinate_enum const rhs
) {
    return rhs < lhs;
}


constexpr bool operator<=(
    coordinate_enum const lhs,
    coordinate_enum const rhs
) {
    return not (rhs < lhs);
}


constexpr bool operator>=(
    coordinate_enum const lhs,
    coordinate_enum const rhs
) {
    return not (lhs < rhs);
}


constexpr coordinate_enum flipped_file(coordinate_enum const coordinate) {
    return static_cast<coordinate_enum>(
     static_cast<unsigned char>(just_rank(coordinate)) |
     (static_cast<unsigned char>(file_enum::file_mask)
     ^ (static_cast<unsigned char>(just_file(coordinate)))));
}


constexpr coordinate_enum flipped_rank(coordinate_enum const coordinate) {
    return static_cast<coordinate_enum>(
     static_cast<unsigned char>(just_file(coordinate)) |
     (static_cast<unsigned char>(rank_enum::rank_mask)
     ^ (static_cast<unsigned char>(just_rank(coordinate)))));
}


constexpr coordinate_enum flipped_a8h1(coordinate_enum const coordinate) {
    return static_cast<coordinate_enum>(
     (static_cast<unsigned char>(just_file(coordinate)) << 3) |
     (static_cast<unsigned char>(just_rank(coordinate)) >> 3));
}


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_COORDINATE_ENUM_HPP
