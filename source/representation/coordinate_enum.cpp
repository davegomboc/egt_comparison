#include "representation/coordinate_enum.hpp"


#include <array>
#include <cassert>
#include <ostream>
#include <type_traits>


namespace representation {


// Returns false iff the file wrapped around the board (h-file in, a-file out).
bool next(file_enum & file) {
    unsigned char value{static_cast<unsigned char>(file)};
    ++value;
    value &= static_cast<unsigned char>(file_enum::file_mask);
    file = static_cast<file_enum>(value);
    return (static_cast<unsigned char>(file_enum::a_file) != value);
}


// Returns false iff the file wrapped around the board (a-file in, h-file out).
bool prev(file_enum & file) {
    unsigned char value{static_cast<unsigned char>(file)};
    --value;
    value &= static_cast<unsigned char>(file_enum::file_mask);
    file = static_cast<file_enum>(value);
    return (static_cast<unsigned char>(file_enum::h_file) != value);
}


file_enum & operator++(file_enum & file) {
    next(file);
    return file;
}


file_enum & operator--(file_enum & file) {
    prev(file);
    return file;
}


// Returns false iff the rank wrapped around the board (rank_1 in, rank_8 out).
bool next(rank_enum & rank) {
    unsigned char value{static_cast<unsigned char>(rank)};
    value -= 8;
    value &= static_cast<unsigned char>(rank_enum::rank_mask);
    rank = static_cast<rank_enum>(value);
    return (static_cast<unsigned char>(rank_enum::rank_8) != value);
}


// Returns false iff the rank wrapped around the board (rank_8 in, rank_1 out).
bool prev(rank_enum & rank) {
    unsigned char value{static_cast<unsigned char>(rank)};
    value += 8;
    value &= static_cast<unsigned char>(rank_enum::rank_mask);
    rank = static_cast<rank_enum>(value);
    return (static_cast<unsigned char>(rank_enum::rank_1) != value);
}


rank_enum & operator++(rank_enum & rank) {
    next(rank);
    return rank;
}


rank_enum & operator--(rank_enum & rank) {
    prev(rank);
    return rank;
}


// Returns false iff the coordinate wrapped around the board (h1 in, a8 out).
bool next(coordinate_enum & coordinate) {
    unsigned char value{static_cast<unsigned char>(coordinate)};
    ++value;
    value &= static_cast<unsigned char>(coordinate_enum::square_mask);
    coordinate = static_cast<coordinate_enum>(value);
    return (static_cast<unsigned char>(file_enum::a_file) |
     static_cast<unsigned char>(rank_enum::rank_8)) != value;
}


// Returns false iff the coordinate wrapped around the board (a8 in, h1 out).
bool prev(coordinate_enum & coordinate) {
    unsigned char value{static_cast<unsigned char>(coordinate)};
    value = static_cast<unsigned char>((value - 1)
     & static_cast<unsigned char>(coordinate_enum::square_mask));
    coordinate = static_cast<coordinate_enum>(value);
    return (static_cast<unsigned char>(file_enum::h_file) |
     static_cast<unsigned char>(rank_enum::rank_1)) != value;
}


coordinate_enum & operator++(coordinate_enum & coordinate) {
    next(coordinate);
    return coordinate;
}


coordinate_enum & operator--(coordinate_enum & coordinate) {
    prev(coordinate);
    return coordinate;
}


// The format is rrr fff.  (See the values of rank_enum and file_enum.)
logic_digits_type as_logic_digits(coordinate_enum const coordinate) {
    assert(valid(coordinate));
    unsigned char value{static_cast<unsigned char>(coordinate)};
    logic_digits_type result;
    result.push_back(value & 0b100000 ? on : off);
    result.push_back(value & 0b010000 ? on : off);
    result.push_back(value & 0b001000 ? on : off);
    result.push_back(value & 0b000100 ? on : off);
    result.push_back(value & 0b000010 ? on : off);
    result.push_back(value & 0b000001 ? on : off);
    return result;
}


std::string as_string(coordinate_enum const coordinate) {
    return std::string{as_char(just_file(coordinate)),
     as_char(just_rank(coordinate))};
}


std::ostream & operator<<(
    std::ostream & out,
    coordinate_enum const coordinate
) {
    return out << as_string(coordinate);
}


template void append_coordinate(
    std::vector<bool> & vector_id,
    coordinate_enum coordinate);
template void append_coordinate(
    std::vector<unsigned char> & vector_id,
    coordinate_enum coordinate);
template void append_coordinate(
    std::vector<unsigned short> & vector_id,
    coordinate_enum coordinate);
template void append_coordinate(
    std::vector<unsigned int> & vector_id,
    coordinate_enum coordinate);
template void append_coordinate(
    std::vector<unsigned long> & vector_id,
    coordinate_enum coordinate);
template void append_coordinate(
    std::vector<unsigned long long> & vector_id,
    coordinate_enum coordinate);

template coordinate_enum remove_coordinate(std::vector<bool> & vector_id);
template coordinate_enum remove_coordinate(
    std::vector<unsigned char> & vector_id);
template coordinate_enum remove_coordinate(
    std::vector<unsigned short> & vector_id);
template coordinate_enum remove_coordinate(
    std::vector<unsigned int> & vector_id);
template coordinate_enum remove_coordinate(
    std::vector<unsigned long> & vector_id);
template coordinate_enum remove_coordinate(
    std::vector<unsigned long long> & vector_id);


} // namespace representation
